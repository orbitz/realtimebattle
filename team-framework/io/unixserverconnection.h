/************************************************************************
    $Id: unixserverconnection.h,v 1.2 2005/01/06 17:59:28 jonico Exp $
    
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

    $Log: unixserverconnection.h,v $
    Revision 1.2  2005/01/06 17:59:28  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef UNIX_SERVERCONNECTION_H
#define UNIX_SERVERCONNECTION_H

#include "serverconnection.h"
#include "unixinstream.h"
#include "unixoutstream.h"
#include <memory>

/**
 * Namespace IO
 */
namespace IO {
	
	using std::auto_ptr;
	
	/**
	* Class ServerConnection
	* Note: Has to be implemented as a SINGLETON
	*/ 
	class UnixServerConnection : public ServerConnection {
	/*
	 * Public stuff
	 */
	public:
		/**
		 * Constructor
		 * @param serverSocket socket form an already etablished connection to the server 
		 */
		UnixServerConnection (int serverSocket) throw(IOException, bad_exception);
		
		
		/**
		 * Destructor, does nothing
		 */
		~UnixServerConnection () throw();
		
		
		/*
		 * Methods
		 */
		/**
		 * return an instance (->singleton pattern)
		 */
		static UnixServerConnection* instance(int serverSocket) throw(IOException, bad_exception);
		
		
		/**
		 * 
		 * @param line Message that will be send over the connection (does not append a trailing newline)
		 */
		void  sendLine (const string& line) throw(StrategyException, IOException, bad_exception);
	

		/**
		 * @return string that contains a line out of the connection (not terminated with newline)
		 */
		string  getLine () throw(IOException, bad_exception);
	

		/**
		 * @return name of the associated target
		 */
		string  getTargetName () throw(IOException, bad_exception);
		
		
	/*
	 * Private stuff
	 */
	private:
		
		/**
		* pointer for singleton
		*/
		static auto_ptr <UnixServerConnection> _instance;
		
		/**
		* unix-socket-fd to remoteclient
		*/
		int	_serverSocket;
		
		/**
		* corresponding robotname
		*/
		string 	_serverName;
		
		/**
		* stream for reading data from remoteclient
		*/
		UnixInStream _inStream;
		
		/**
		* stream for writing data to remoteclient
		*/
		UnixOutStream _outStream;
		
		/*
		* Methods
		*/
		/**
		* Private constructor (->singleton pattern)
		*/
		UnixServerConnection () throw(IOException, bad_exception);
	};
}
#endif //UNIX_SERVERCONNECTION_H

