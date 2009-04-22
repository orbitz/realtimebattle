/************************************************************************
    $Id: servercoordinator.h,v 1.2 2005/01/06 17:59:27 jonico Exp $
    
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

    $Log: servercoordinator.h,v $
    Revision 1.2  2005/01/06 17:59:27  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef SERVERCOORDINATOR_H
#define SERVERCOORDINATOR_H

#include <string>
#include <memory>
#include <sstream>

#include "coordinatorleaf.h"
#include "gamecontrolnamespace.h"
#include "../stdnamespace.h"
#include "../strategies/strategiesnamespace.h"
#include "../io/ionamespace.h"

#include "../strategies/serverstate.h"
#include "../strategies/internalserverspecificrepository.h"
#include "../rtbglobal/pvector.h"
#include "../log/logger.h"
#include "../exceptions/strategyexception.h"
#include "../exceptions/resourcenotfoundexception.h"
#include "../exceptions/keynotfoundexception.h"
#include "../exceptions/confignotloadedexception.h"

/**
* Namespace GameControl
*/
namespace GameControl {
	using Strategies::State;
	using Strategies::ServerState;
	using Strategies::InternalServerSpecificRepository;
	using Strategies::SharedServerSpecificRepository;
	using IO::ClientConnection;
	using Log::Logger;
	using RTBGlobal::pvector;
	using Exceptions::StrategyException;
	using Exceptions::ResourceNotFoundException;
	using Exceptions::ConfigNotLoadedException;
	using Exceptions::KeyNotFoundException;
	using std::string;
	using std::ostringstream;
	using std::auto_ptr;
	using std::bad_exception;


/**
* Class ServerCoordinator
* Important: ServerState is a friend of this class!!
*/
class ServerCoordinator : public CoordinatorLeaf {
/*
* Public stuff
*/
public:
	friend class ServerState;

	/**
	* Destructor, does nothing
	*/
	~ServerCoordinator () throw();
	
	
	ServerCoordinator (	ClientConnection* clientConnection,
				const GameOptionsRepository* gameOptionsRepository	) throw (ConfigNotLoadedException, KeyNotFoundException,bad_exception,StrategyException,ResourceNotFoundException);
	
	
	/**	
	 * Calls reset on all state objects and sets the current state to the beginning state.
	 */
	void  resetGame () throw (StrategyException,bad_exception);

	/**
	 * This method calls sendServerTips on the current server state object.
	 */
	void  processReceivedRTBMessages () throw (StrategyException,bad_exception);
	
	
	string  getAssociatedRobotName () throw (bad_exception);

/*
* Protected stuff
*/
protected:
	State*  getCurrentState () throw ();
		
private:
	auto_ptr <InternalServerSpecificRepository> _internalServerSpecificRepository;
	SharedServerSpecificRepository* _sharedServerSpecificRepository;
	auto_ptr < pvector <ServerState> > _serverStates;
	auto_ptr <Logger> _logger;

	char _currentServerStateIndex;

	ostringstream _tipBuffer;

	ClientConnection* _clientConnection;

	void  setCurrentServerState (unsigned char serverStateIndex) throw (bad_exception,StrategyException);

	/**
	 * This method is used by the server state objects to send a concrete server tipe to the clients already encoded in string representation.
	 * @return An output string stream to buffer the tips inside
	 */
	inline ostringstream& tipBuffer() throw () {return _tipBuffer;}

	/**
	 * This method writes a stop command (e. g. "FINISH", given by StrategyFactory::getFinishingString()) on the tip buffer connection and flushes it to the ClientConnection. The reactOnServerTip method of ClientCoordinator recognizes this message and will initiate the generation of messages to the rtb server
	 */
	void  endServerTipSequence () throw (bad_exception);
	/**
	 * This string is used to reset _tipBuffer 
	 */
	string _emptyString;
	string _finishingString;
			
	ServerState* currentServerState;
	string currentServerStateName;
	
	// performance voodo
	string _robotName;
	string _logMessage1;
	string _logMessage2;
	string _logMessage3;
	string _logMessage4;
	string _logMessage5;
	
};

}
#endif //SERVERCOORDINATOR_H

