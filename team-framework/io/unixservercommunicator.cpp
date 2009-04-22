/************************************************************************
    $Id: unixservercommunicator.cpp,v 1.7 2005/10/06 18:55:53 jonico Exp $
    
    RTB - Team Framework: Framework for RealTime Battle robots to communicate efficiently in a team
    Copyright (C) 2004 The RTB- Team Framework Group: http://rtb-team.sourceforge.net

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    $Log: unixservercommunicator.cpp,v $
    Revision 1.7  2005/10/06 18:55:53  jonico
    Added verbose messages about the name of the unix domain sockets in order to facilitate error tracking for the user.

    Revision 1.6  2005/08/10 10:36:26  jonico
    Fixed accept bug that only occured without optimization

    Revision 1.5  2005/07/11 17:17:30  jonico
    removed some fcntl includes, improved socket filename deletion, introduced blocking parameter for security infrastructure

    Revision 1.4  2005/01/27 18:08:03  jonico
    Fixed busy waiting select bug in the server communicators, fixed NullLogger problem in createrobotconf

    Revision 1.3  2005/01/06 17:59:28  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#include <common.h>

#include "unixservercommunicator.h"
#include "unixremoteclientconnection.h"
#include "../exceptions/keynotfoundexception.h"
#include "../exceptions/confignotloadedexception.h"
#include "../rtbglobal/masterresourcecontrol.h"

#include <ios>
// #include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

/**
 * Namespace
 */
namespace IO {

	using Exceptions::KeyNotFoundException;
	using Exceptions::ConfigNotLoadedException;
	using RTBGlobal::MasterResourceControl;
	using std::ios_base;

	/**
	 * Constructors
	 */
	UnixServerCommunicator::UnixServerCommunicator() throw(IOException, ServerIsPresentException, bad_exception) 
	: _serverSocketBound(false),_mrc(MasterResourceControl::Instance()) {

		//obtain Logger
		try {
			_logger=_mrc->createLogger("IO");
		}
		catch (RTBException& e) {
			throw IOException("Could not obtain logger in UnixServerCommunicator::UnixServerCommunicator(), nested exception is: "+e.getMessage());
		}
		
		_logger->logMessage(10, "Try to get UnixServerCommunicator.");

			
		//open Socket
		_serverSocket = socket(AF_UNIX, SOCK_STREAM, 0);
		if( _serverSocket < 0 )
			throw IOException("Could not open socket. socket() failed.");
			
		//socket is in blocking-mode per default
		// _logger->logMessage(2, "Switch socket to blocking-mode.");
		//if( fcntl(_serverSocket, F_SETFL, fcntl(_serverSocket, F_GETFL, 0) & (~O_NONBLOCK)) != 0 ) {
		//	close(_serverSocket);
		//	throw IOException("Could not switch to blocking-mode. fcntl() failed.");
		//}

		//try to get the name of unix-domain-socket form configfile
		string domainName;
		try {
			domainName = _mrc->getConfigurationProperty("IO","DomainSocketName");
		}
		catch(KeyNotFoundException& e) {
			close(_serverSocket);
			throw IOException("Could not get DomainSocketName from section IO. KeyNotFoundException occured: " + e.getMessage());
		}
		catch(ConfigNotLoadedException& e) {
			close(_serverSocket);
			throw IOException("Could not get DomainSocketName from section IO. ConfigNotLoadedException occured: " + e.getMessage());
		}
		_domainName = domainName;

		//initializing data for socket
		sockaddr_un socketData;
		socketData.sun_family = AF_UNIX;
		domainName.copy(socketData.sun_path, domainName.size());
		socketData.sun_path[domainName.size()] = 0;
		
		//try to create a unix-domain-socket
		_logger->logMessage(2, "Try to create a unix-domain-socket.");
		if( bind(_serverSocket, reinterpret_cast< struct sockaddr* >(&socketData), sizeof(socketData)) < 0 ) {
			close(_serverSocket);
			throw ServerIsPresentException("Could not create ServerSocket. bind() failed.");
		}
		_serverSocketBound=true;
		_logger->logMessage(10, "Bound unix-domain-socket "+domainName);

		//switch to listen-mode
		if( listen(_serverSocket, 3) < 0 ) {
			close(_serverSocket);
			unlink(_domainName.c_str());
			_serverSocketBound=false;
			throw IOException("Could not create ServerSocket. listen() failed.");
		}				
	}
	/**
	 * Destructor
	 */
	UnixServerCommunicator::~UnixServerCommunicator () throw(){
		close(_serverSocket);
		//delete unix-domain-socket		
		if (_serverSocketBound)
			unlink(_domainName.c_str());
	}
	/**
	 * Methods
	 */
	auto_ptr <ClientConnection> UnixServerCommunicator::createRemoteClientConnection () throw (ServerIsPresentException, IOException, bad_exception) {
		
		//wait until RTB-Server is sending data (=> Exception) or a client wants to connect 
		_logger->logMessage(2, "Waiting for remoteclients.");
			
		fd_set listenSocketSet;
		FD_ZERO(&listenSocketSet);
		FD_SET(0, &listenSocketSet);
		FD_SET(_serverSocket, &listenSocketSet);

		if( select(_serverSocket+1, &listenSocketSet, NULL, NULL, NULL) < 0 ) 
			throw IOException("Could not create remote connection. select() failed.");
			
		//nothing from RTB-Server received?
		if(FD_ISSET(0, &listenSocketSet) ) {
			//received data from RTB-Server => time to connect is over, remove visible file name
			unlink(_domainName.c_str());
			_serverSocketBound=false;
			throw ServerIsPresentException("Master server answered, no pending connection requests will be accepted.");
		}


		if( FD_ISSET(_serverSocket, &listenSocketSet) ) {
		
			//establish client connection
			_logger->logMessage(5, "Establish connection to new remoteclient.");
			sockaddr clientAddress;
			socklen_t addressLen=sizeof(clientAddress);
			int clientSocket = accept(_serverSocket, &clientAddress, &addressLen);
				
			if( clientSocket >= 0 ) {
				
				//clientSocket already in blocking-mode
				//if( fcntl(clientSocket, F_SETFL, fcntl(clientSocket, F_GETFL, 0) & (~O_NONBLOCK)) != 0 )
				//	throw IOException("Could not create remote connection. fcntl() failed.");
	
				return auto_ptr <ClientConnection>(new UnixRemoteClientConnection(clientSocket));
			}
			else
				throw IOException("accept() for client connection failed");
				

		}
		
		throw IOException("Select returns but no file descriptor was ready to read from.");
		
	}
}
