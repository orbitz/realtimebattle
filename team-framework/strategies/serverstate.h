/************************************************************************
    $Id: serverstate.h,v 1.2 2005/01/06 17:59:29 jonico Exp $
    
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

    $Log: serverstate.h,v $
    Revision 1.2  2005/01/06 17:59:29  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef SERVERSTATE_H
#define SERVERSTATE_H

#include "state.h"
#include "../stdnamespace.h"
#include "strategiesnamespace.h"
#include "../gamecontrol/gamecontrolnamespace.h"
#include "../exceptions/strategyexception.h"

/**
* Namespace Strategies 
*/
namespace Strategies {
	using std::string;
	using std::bad_exception;
	using GameControl::ServerCoordinator;
	using Exceptions::StrategyException;

/**
* Class ServerState
*/
class ServerState : public State {
public:
	ServerState(ServerCoordinator* c) throw();
	
	/**
	* Destructor, does nothing
	*/
	virtual ~ServerState () throw();
	/**
	 * This method is called from the coordinator's processReceivedRTBMessages method and should send tips to the clients.
	 */
	virtual void  sendServerTips () throw (StrategyException,bad_exception) = 0;
protected:
	void  setCurrentServerState (unsigned char serverStateIndex) throw ();
	/**
	 * Adds a new tip to the tip buffer of the ServerCoordinator
	 * @param tip The server tip in string shape
	 */
	void  sendServerTipString (const string& tip) throw (StrategyException,bad_exception);
private:
	ServerCoordinator *_serverCoordinator;
};

}
#endif //SERVERSTATE_H

