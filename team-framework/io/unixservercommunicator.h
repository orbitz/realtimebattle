/************************************************************************
    $Id: unixservercommunicator.h,v 1.3 2005/07/11 17:17:30 jonico Exp $
    
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

    $Log: unixservercommunicator.h,v $
    Revision 1.3  2005/07/11 17:17:30  jonico
    removed some fcntl includes, improved socket filename deletion, introduced blocking parameter for security infrastructure

    Revision 1.2  2005/01/06 17:59:28  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef UNIXSERVERCOMMUNICATOR_H
#define UNIXSERVERCOMMUNICATOR_H

#include "servercommunicator.h"
#include "../log/logger.h"
#include "../exceptions/rtbexception.h"
#include "../rtbglobal/masterresourcecontrol.h"
#include <string>

/**
* Namespace IO
*/
namespace IO {

	using Log::Logger;
	using RTBGlobal::MasterResourceControl;
	using Exceptions::RTBException;
	using std::string;

	/**
	* Class ServerCommunicator
	*/ 
	class UnixServerCommunicator : public ServerCommunicator {
	/*
	* Public stuff
	*/
	public:
		/**
		* Constructor
		*/
		UnixServerCommunicator() throw(IOException, ServerIsPresentException, bad_exception);
		
		 
		/*
		* Operations
		*/
		/**
		* Establish connection to remoteclient.
		* @return Connection to communicator directly with the remoteclient.
		*/
		auto_ptr <ClientConnection>  createRemoteClientConnection () throw(ServerIsPresentException, IOException, bad_exception);
			
		
		/**
		 * 
		 */
		~UnixServerCommunicator () throw();
	
	
	/*
	 * Private stuff
	 */
	private:
		
		/**
		* socket to server (unix-filedescriptor)
		*/
		int	_serverSocket;

		/**
		 * flag if server socket is bound to a filename
		 */
		bool _serverSocketBound;

		/**
		 * flag if server socket is 
		 */
		
		/**
		* for logging messages...
		*/
		auto_ptr <Logger> _logger;
		
		/**
		* for resource- requisition
		*/
		MasterResourceControl* _mrc;
		
		/**
		* name of the unix-domain-socket
		*/
		string _domainName;
	};
}
#endif //UNIXSERVERCOMMUNICATOR_H

