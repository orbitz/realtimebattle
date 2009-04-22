/************************************************************************
    $Id: servergamecontroller.h,v 1.2 2005/01/06 17:59:27 jonico Exp $
    
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

    $Log: servergamecontroller.h,v $
    Revision 1.2  2005/01/06 17:59:27  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef SERVERGAMECONTROLLER_H
#define SERVERGAMECONTROLLER_H

#include "../rtbglobal/rtbglobalnamespace.h"
#include "../io/ionamespace.h"
#include "gamecontrolnamespace.h"

#include <memory>
#include "../rtbglobal/pvector.h"
#include "gamecontroller.h"
#include "gameoptionsrepository.h" 
#include "../io/servercommunicator.h"
#include "../log/logger.h"
#include "../rtbglobal/pvector.h"
#include <string>



/**
 * Namespace GameControl
 */
namespace GameControl {
	using RTBGlobal::pvector;
	using std::vector;
	using IO::RTBConnection;
	using IO::ClientConnection;
	using std::auto_ptr;
	using std::string;
	using IO::ServerCommunicator;
	using Log::Logger;
	using RTBGlobal::MasterResourceControl;
	using Exceptions::RTBException;


/**
* Class ServerGameController
*/ 
class ServerGameController : public GameController {
	/*
	* Public stuff
	*/
	public:
		/**
		* 
		*/
		bool start () throw (bad_exception);
		
	
		/**
		* 
		* @param serverCommunicator 
		*/
		ServerGameController (auto_ptr <ServerCommunicator> serverCommunicator)  throw (RTBException, bad_exception);
		
	
		/**
		* Destructor
		*/
		~ServerGameController () throw();
			
	/*
	* Private stuff
	*/
	private:
		/*
		* Fields
		*/
		RTBConnection* _rtbConnection;
		GameOptionsRepository _gameOptionsRepository;
		auto_ptr <ServerCommunicator> _serverCommunicator;
		pvector <ClientConnection> _clientConnections;
		vector <bool> _alive;
		vector <string> _robotNames;
		auto_ptr <Logger> _logger;
		MasterResourceControl* _mrc;
		int _numberRobots;
		int _numberAliveRobots;
		auto_ptr<ClientCoordinator> _localClientCoordinator;
		auto_ptr<ServerCoordinator> _localServerCoordinator;
		
		
		/**
		* Note: This structure contains both a CoordinatorComposite and lots of ServerCoordinators. The CoordinatorComposite should be the first object in this structure while the structure should be traversed from the last to the first object.
		*/
		pvector <Coordinator> _coordinators;
		/*
		* Methods
		**/
		/**
		* Let all robots be alive again
		*/
		void setAlive();
		/**
		*  Reset all coordinators
		*/
		void resetAll();
		/**
		*  Delegate to all coordinators
		*/
		void delegateToAll();
		
	};
}
#endif //SERVERGAMECONTROLLER_H

