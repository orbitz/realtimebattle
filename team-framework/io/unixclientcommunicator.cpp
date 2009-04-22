/************************************************************************
    $Id: unixclientcommunicator.cpp,v 1.5 2005/10/06 18:55:52 jonico Exp $
    
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

    $Log: unixclientcommunicator.cpp,v $
    Revision 1.5  2005/10/06 18:55:52  jonico
    Added verbose messages about the name of the unix domain sockets in order to facilitate error tracking for the user.

    Revision 1.4  2005/07/11 17:17:30  jonico
    removed some fcntl includes, improved socket filename deletion, introduced blocking parameter for security infrastructure

    Revision 1.3  2005/01/06 17:59:28  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#include <common.h>

#include "unixclientcommunicator.h"
#include "unixserverconnection.h"
#include "../rtbglobal/masterresourcecontrol.h"
#include "../log/logger.h"
#include "../exceptions/rtbexception.h"
#include "../exceptions/keynotfoundexception.h"
#include "../exceptions/confignotloadedexception.h"


// #include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

/**
 * Namespace
 */
namespace IO {

	using Log::Logger;
	using RTBGlobal::MasterResourceControl;
	using Exceptions::RTBException;
	using Exceptions::KeyNotFoundException;
	using Exceptions::ConfigNotLoadedException;
	
	/**
	 * Constructors
	 */
	UnixClientCommunicator::UnixClientCommunicator () throw(IOException, bad_exception) 
	: _serverConnection(0), _serverConnectionInv(false), _mrc(MasterResourceControl::Instance()) {
		
		//obtain logger		
		try {
			_logger = _mrc->createLogger("IO");
		}
		catch (RTBException& e) {
			throw IOException("Could not obtain logger in UnixIOFactory::getSocket, nested exception is: "+e.getMessage());
		}
			
		// log Message
		_logger->logMessage(2, "Initializing UnixClientCommunicator.");
	}
	/**
	 * Destructor
	 */
	/**
	 * Has to delete serverconnection
	 */
	UnixClientCommunicator::~UnixClientCommunicator () throw() {
	}
	/**
	 * Methods
	 */
	void UnixClientCommunicator::obtainServerConnection() throw(IOException, bad_exception) {
		
		//open Socket
		int serverSocket = socket(AF_UNIX, SOCK_STREAM, 0);
		if( serverSocket < 0 )
			throw IOException("Could not open socket. socket() failed.");
			
		//socket is in blocking-mode per default
		//_logger->logMessage(2, "Switch socket to blocking-mode.");
		//if( fcntl(serverSocket, F_SETFL, fcntl(serverSocket, F_GETFL, 0) & (~O_NONBLOCK)) != 0 )
		//	throw IOException("Could not switch to blocking-mode. fcntl() failed.");

		//try to get the name of unix-domain-socket from configfile
		string domainName;
		try {
			domainName = _mrc->getConfigurationProperty("IO","DomainSocketName");
		}
		catch(KeyNotFoundException& e) {
			close(serverSocket);
			throw IOException("Could not get DomainSocketName from section IO. KeyNotFoundException occured: " + e.getMessage());
		}
		catch(ConfigNotLoadedException& e) {
			throw IOException("Could not get DomainSocketName from section IO. ConfigNotLoadedException occured: " + e.getMessage());
		}

		//initializing data for socket
		sockaddr_un socketData;
		socketData.sun_family = AF_UNIX;
		domainName.copy(socketData.sun_path, domainName.size());
		socketData.sun_path[domainName.size()] = 0;
		
		//establish connection to master-server
		_logger->logMessage(10, "Establish connection to master-server via socket "+domainName);
		if( connect(serverSocket, reinterpret_cast<struct sockaddr*>(&socketData), sizeof(socketData)) < 0 ) {
			close(serverSocket);
			throw IOException("Could not establish connection to Master-Server. connect() failed.");
		}
		
		//get UnixServerConnection
		_serverConnection = UnixServerConnection::instance(serverSocket);
		_serverConnectionInv = true;
	}
	/**
	 *
	 */
	ServerConnection* UnixClientCommunicator::getServerConnection () throw(IOException, bad_exception) {
		if( !_serverConnectionInv )
			obtainServerConnection();
			
		return _serverConnection;
	}
} 
