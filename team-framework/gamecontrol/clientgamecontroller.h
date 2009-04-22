/************************************************************************
    $Id: clientgamecontroller.h,v 1.2 2005/01/06 17:59:27 jonico Exp $
    
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

    $Log: clientgamecontroller.h,v $
    Revision 1.2  2005/01/06 17:59:27  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef CLIENTGAMECONTROLLER_H
#define CLIENTGAMECONTROLLER_H

#include "../io/ionamespace.h"
#include "gamecontrolnamespace.h"
#include "../rtbglobal/rtbglobalnamespace.h"

#include <memory>
#include <sstream>
#include "gamecontroller.h"
#include "gameoptionsrepository.h"
#include "../io/clientcommunicator.h"
#include "../log/logger.h"
#include <string>


/**
 * Namespace GameControl
 */
namespace GameControl {
	using std::auto_ptr;
	using std::string;
	using std::ostringstream;
	using Exceptions::RTBException;
	using IO::RTBConnection;
	using IO::ClientCommunicator;
	using IO::ServerConnection;
	using Log::Logger;
	using RTBGlobal::MasterResourceControl;

/**
* Class ClientGameController
*/ 
class ClientGameController : public GameController {
	/*
	* Public stuff
	*/
	public:
		/**
		* Destructor, does nothing 
		*/
		~ClientGameController () throw();
		
	
		/**
		* This method is active the whole time when a game is running. Here the main data flow is processed,
		* it represents the discrete control loop method of the client. See the petrinets and sequence diagrams for
		* further details.
		*/
		bool  start() throw (bad_exception) ;
		
	
		/**
		* Initializes the data fields and exit, the real work is done in the start method
		* Constructor will pass any exceptions to its creator (since logger may be initialized incorrect)
		* @param clientCommunicator 
		*/
		ClientGameController (auto_ptr <ClientCommunicator> clientCommunicator) throw (RTBException, bad_exception);
		
	
	/*
	* Private stuff
	*/
	private:
		/*
		* Fields
		*/
		GameOptionsRepository _gameOptionsRepository;
		auto_ptr <ClientCoordinator> _clientCoordinator;
		auto_ptr <ClientCommunicator> _clientCommunicator;
		auto_ptr <Logger> _logger;
		MasterResourceControl* _mrc;
		string _robotName;
		ServerConnection* _serverConnection;
		RTBConnection* _rtbConnection;
		/**
		 * Used to store all messages received from the RTB Server to delegate them to the master server
		*/
		ostringstream _messagebuffer;
		/**
		 * String is used to reset our messagebuffer
		*/
		string _emptystring;
		
	};
}
#endif //CLIENTGAMECONTROLLER_H

