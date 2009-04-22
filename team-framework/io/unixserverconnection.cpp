/************************************************************************
    $Id: unixserverconnection.cpp,v 1.4 2005/07/11 17:17:30 jonico Exp $
    
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

    $Log: unixserverconnection.cpp,v $
    Revision 1.4  2005/07/11 17:17:30  jonico
    removed some fcntl includes, improved socket filename deletion, introduced blocking parameter for security infrastructure

    Revision 1.3  2005/01/06 17:59:28  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#include <common.h>

#include "unixserverconnection.h"
#include "../exceptions/keynotfoundexception.h"
#include "../rtbglobal/masterresourcecontrol.h"

//STL
#include <sstream>
#include <ios>

//IPC
#include <fcntl.h>

/**
 * Namespace
 */
namespace IO {

	using Exceptions::KeyNotFoundException;
	using RTBGlobal::MasterResourceControl;
	using std::ostringstream;
	using std::endl;
	
	/**
	 * initialize static members
	 */
	auto_ptr <UnixServerConnection> UnixServerConnection::_instance(0);
	/**
	 * 
	 */
	UnixServerConnection* UnixServerConnection::instance (int serverSocket) throw (IOException, bad_exception) {
		UnixServerConnection* serverConnection(UnixServerConnection::_instance.get());
		if( serverConnection == 0 ) {
			UnixServerConnection::_instance.reset(serverConnection = new UnixServerConnection(serverSocket));
		}
		return serverConnection;
	}
	/**
	 * 
	 */
	UnixServerConnection::~UnixServerConnection () throw() {
		close(_serverSocket);
	}
	/**
	 * 
	 */
	UnixServerConnection::UnixServerConnection (int serverSocket) throw(IOException, bad_exception)
	: ServerConnection(), _inStream(serverSocket), _outStream(serverSocket) {
	   	_serverSocket = serverSocket;

		//try to get the robotname from configfile	
		MasterResourceControl* mrc = MasterResourceControl::Instance();
		string robotName;
		try {
			robotName = mrc->getRuntimeProperty("Main","ActualName");
		}
		catch(KeyNotFoundException& e) {
			close(_serverSocket);
			throw IOException("Could not get ActualName from section Main. KeyNotFoundException occured: " + e.getMessage());
		}
			

		//append pid to robotname
		ostringstream int2string;
		int2string << getpid();
		robotName += " (pid: ";
		robotName += int2string.str();
		robotName += ")";
		
		//send robotname to master-server
		sendLine(robotName);

		//recieve server-robotname from master-server
		_serverName = getLine();
	}
	/**
	 *
	 * @param line Message that will be send over the connection (does not append a trailing newline)
	 */
	void UnixServerConnection::sendLine (const string& line) throw(StrategyException, IOException, bad_exception) {
		_outStream.sendline(line);
	}
	/**
	 * @return string that contains a line out of the connection (not terminated with newline)
	 */
	string UnixServerConnection::getLine () throw(IOException, bad_exception) {
		return _inStream.getline();
	}
	/**
	 * @return name of the associated target
	 */
	string UnixServerConnection::getTargetName () throw(IOException, bad_exception) {
		return _serverName;
	}
}
