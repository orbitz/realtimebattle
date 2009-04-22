/************************************************************************
    $Id: unixclientcommunicator.h,v 1.2 2005/01/06 17:59:28 jonico Exp $
    
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

    $Log: unixclientcommunicator.h,v $
    Revision 1.2  2005/01/06 17:59:28  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef UNIXCLIENTCOMMUNICATOR_H
#define UNIXCLIENTCOMMUNICATOR_H

#include "clientcommunicator.h"
#include "../log/lognamespace.h"
#include "../rtbglobal/rtbglobalnamespace.h"
#include <memory>

/**
 * Namespace IO
 */
namespace IO {

	using Log::Logger;
	using RTBGlobal::MasterResourceControl;
	using std::auto_ptr;
	using std::bad_exception;

	/**
	* Class UnixClientCommunicator
	*/ 
	class UnixClientCommunicator : public ClientCommunicator {
	/*
	 * Public stuff
	 */
	public:
		/**
		 * Constructors
		 */
		UnixClientCommunicator () throw(IOException, bad_exception);
		
		 
		/*
		 * Destructor
		 */
		/**
		 * Has to delete serverconnection
		 */
		~UnixClientCommunicator () throw();
			
		
		/**
         * Establish connection to server.
		 * @return Connection to communicator directly with the server.
		 */
		ServerConnection* getServerConnection () throw(IOException, bad_exception);
			
		
	/*
	 * Private stuff
	 */
	private:
		/**
		 * Fields
		 */
		/**
		 * Pointer to ServerConnection
		 */
		ServerConnection* _serverConnection;
		
		
		/**
		 * Invariant, which contains the information whether serverConnection is allocated.
		 */
		bool _serverConnectionInv;
		
		
		/**
		 * for logging important io-messages
		 */
		auto_ptr <Logger> _logger;
		
		
		/**
		 * to allocate resources we need a pointer to the MasterResourceControl
		 */
		MasterResourceControl* _mrc;
		
		/*
		 * Operations
		 */
		/**
		 * Obtains a ServerConnection by connecting to the already available unix-domain-socket
		 */
		void obtainServerConnection() throw(IOException, bad_exception);
	};
}

#endif //CLIENTCOMMUNICATOR_H

