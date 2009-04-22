/************************************************************************
    $Id: clientcoordinator.h,v 1.2 2005/01/06 17:59:27 jonico Exp $
    
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

    $Log: clientcoordinator.h,v $
    Revision 1.2  2005/01/06 17:59:27  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef CLIENTCOORDINATOR_H
#define CLIENTCOORDINATOR_H

#include <sstream>
#include <memory>

#include "coordinatorleaf.h"
#include "gamecontrolnamespace.h"
#include "../io/ionamespace.h"

#include "../strategies/clientstate.h"
#include "../strategies/clientspecificrepository.h"
#include "../rtbglobal/pvector.h"
#include "../log/logger.h"
#include "../exceptions/resourcenotfoundexception.h"
#include "../exceptions/keynotfoundexception.h"
#include "../exceptions/confignotloadedexception.h"

/**
* Namespace GameControl
*/
namespace GameControl {
	using Strategies::ClientState;
	using Strategies::State;
	using Strategies::ClientSpecificRepository;
	using IO::RTBConnection;
	using Log::Logger;
	using RTBGlobal::pvector;
	using std::auto_ptr;
	using std::string;
	using std::ostringstream;
	using Exceptions::ResourceNotFoundException;
	using Exceptions::ConfigNotLoadedException;
	using Exceptions::KeyNotFoundException;


/**
* Class ClientCoordinator
* Note: ClientState is friend of this class, because it has to access the private methods!!!
*/
class ClientCoordinator : public CoordinatorLeaf {
/*
*Public stuff
*/
public:
	friend class ClientState;
	
	ClientCoordinator (const GameOptionsRepository* gameOptionsRepository, RTBConnection* rtbConnection) throw (ConfigNotLoadedException, KeyNotFoundException,bad_exception,StrategyException, ResourceNotFoundException);
	~ClientCoordinator () throw();

	/**
	 * Delegates the server tip to the current state object.
	 * @param serverTip The server tip in string shape
	 * @return "false", if the tip was the end of the server tip sequence, otherwise "true"
	 */
	bool reactOnServerTip(const string& serverTip) throw (StrategyException,bad_exception);
	
	
	/**
	 * Revokes reset on all state objects and sets the current state to the beginning state.
	 */
	void  resetGame () throw (StrategyException,bad_exception);
	
	
	/**
	 * Calls roundFinished on the current client state object.
	 */
	void  processReceivedRTBMessages () throw (StrategyException,bad_exception);
	
	
	string  getAssociatedRobotName () throw (bad_exception);
	
	
/*
* Protected stuff
*/
protected:
	State*  getCurrentState () throw ();
	
	
private:
	auto_ptr <ClientSpecificRepository> _clientSpecificRepository;

	ostringstream _messageBuffer;

	auto_ptr < pvector < ClientState > > _clientStates;	// Important: Recognize special vector type!!!
	auto_ptr <Logger> _logger;

	RTBConnection* _rtbConnection;
	void  setCurrentClientState (unsigned char clientStateIndex) throw (StrategyException,bad_exception);

	/**
	 * This method is used by the client state objects to send a concrete message to RTB already encoded in string representation.
	 * @return An output string stream to buffer the messages inside
	 */
	inline ostringstream& messageBuffer () throw () {return _messageBuffer;}

	/**
	 * This method flushes the buffer contents to RTB and empties the buffer afterwards
	 */
	void flushMessages() throw (bad_exception);
	/**
	 * This string is used to reset _messageBuffer 
	 */
	string _emptyString;
	string _finishingString;

	string currentClientStateName;
	ClientState* currentClientState;
	
	string _robotName;
};
}
#endif //CLIENTCOORDINATOR_H

