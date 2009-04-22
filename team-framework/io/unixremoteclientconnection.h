/************************************************************************
    $Id: unixremoteclientconnection.h,v 1.2 2005/01/06 17:59:28 jonico Exp $
    
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

    $Log: unixremoteclientconnection.h,v $
    Revision 1.2  2005/01/06 17:59:28  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef UNIXREMOTECLIENTCONNECTION_H
#define UNIXREMOTECLIENTCONNECTION_H

#include "remoteclientconnection.h"
#include "unixinstream.h"
#include "unixoutstream.h"


/**
 * Namespace IO
 */
namespace IO {

	/**
	* Class RemoteClientConnection
	* This class is only a symbolic substitute for the concrete implementation of a platform specific solution
	*/ 
	class UnixRemoteClientConnection : public RemoteClientConnection {
	/*
	* Public stuff
	*/
	public:
		/**
		* Constructors
		* @param clientSocket socket (unix-filedescriptor)
		*/
		UnixRemoteClientConnection (int clientSocket) throw(IOException, bad_exception);
	
		
		/**
		* Destructor, has to close connection
		*/
		~UnixRemoteClientConnection () throw();

		/**
		* 
		* @param line Message that will be send over the connection (does not append a trailing newline)
		*/
		void sendLine (const string& line) throw(StrategyException, IOException, bad_exception);
		
	
		/**
		* @return string that contains a line out of the connection (not terminated with newline)
		*/
		string getLine () throw(IOException, bad_exception);
		
	
		/**
		* @return name of the associated target
		*/
		string getTargetName () throw();
		

		/**
		 *  Returns, whether the client is the local client or one of the remote clients
		 */
		bool isLocal () throw();
		

	/*
	* Private stuff
 	*/
	private:
	
		/**
		* unix-socket-fd to remoteclient
		*/
		int _clientSocket;
		
		/**
		* corresponding robotname
		*/
		string _targetName;
		
		/**
		* stream for reading data from remoteclient
		*/
		UnixInStream _inStream;
		
		/**
		* stream for writing data to remoteclient
		*/
		UnixOutStream _outStream;
	};
}
#endif //UNIXREMOTECLIENTCONNECTION_H

