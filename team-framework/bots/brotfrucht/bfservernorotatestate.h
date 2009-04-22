/************************************************************************
    $Id: bfservernorotatestate.h,v 1.2 2005/01/06 17:59:26 jonico Exp $
    
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

    $Log: bfservernorotatestate.h,v $
    Revision 1.2  2005/01/06 17:59:26  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef BFSERVERNOROTATESTATE_H
#define BFSERVERNOROTATESTATE_H

#include "bfserverbasicstate.h"

/**
* Namespace Brotfrucht
*/
namespace Brotfrucht {

	/**
	* class BFServerNoRotateState
	*/
	class BFServerNoRotateState: public BFServerBasicState {
	public:
		BFServerNoRotateState(ServerCoordinator* c,BFInternalServerSpecificRepository* bfssp, BFSharedServerSpecificRepository* bfsharedssp) throw (StrategyException,bad_exception);
		
		/**
		* This method is called by ClientCoordinatorRevokes::resetGame()
		*/
		void reset () throw (bad_exception, StrategyException);
		/**
		* This method is called when the RTB message "Collision [type] [angle]" occurs. This message is sent whan the robot has been hit by or hits another object,
		* @param type The type of the hit object
		* @param angle The angle the collision happened at in rad, relative to the robot front
		*/
		void receiveRTBMessageCollision(object_type type,double angle) throw (StrategyException, bad_exception);	
	private:	
		BFSharedServerSpecificRepository* _bfsharedssp;

	};
}

#endif
