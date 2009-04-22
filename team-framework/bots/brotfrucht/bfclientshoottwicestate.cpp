/************************************************************************
    $Id: bfclientshoottwicestate.cpp,v 1.2 2005/01/06 17:59:26 jonico Exp $
    
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

    $Log: bfclientshoottwicestate.cpp,v $
    Revision 1.2  2005/01/06 17:59:26  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#include <common.h>

#include "bfclientshoottwicestate.h"
#include "bfclientstateenum.h"
#include "bfclientspecificrepository.h"

namespace Brotfrucht {

	BFClientShootTwiceState::BFClientShootTwiceState(ClientCoordinator* i,BFClientSpecificRepository* bfcsp,const GameOptionsRepository* go) throw (StrategyException,bad_exception):BFClientBasicState(i,bfcsp,go),_bfcsp(bfcsp) {}
	
	/**
	* This method is called when the RTB message "RobotsLeft [num]" occurs. This message is sent at the beginning of the game and after a robot has been killed.
	* @param num The number of remaining robots
	*/
	void BFClientShootTwiceState::receiveRTBMessageRobotsLeft(int num) throw (StrategyException, bad_exception) {
		BFClientBasicState::receiveRTBMessageRobotsLeft(num);
		_bfcsp->getLogger()->logMessage(1,"Another robot died, reducing shoot frequency to the normal level...");
		_bfcsp->setShootingInterval(_bfcsp->getShootingInterval()*2);
		// we change the state now!!!
		setCurrentClientState(SHOOTHALF);
	}
}

