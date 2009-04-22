/************************************************************************
    $Id: gameoptionsrepository.h,v 1.2 2005/01/06 17:59:27 jonico Exp $
    
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

    $Log: gameoptionsrepository.h,v $
    Revision 1.2  2005/01/06 17:59:27  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef GAMEOPTIONSREPOSITORY_H
#define GAMEOPTIONSREPOSITORY_H



/**
 * Namespace GameControl
 */
namespace GameControl {
	/**
	* Class GameOptionsRepository
	*/ 
	class GameOptionsRepository {
	/*
	* Public stuff
	*/
	public:
		/*
		* Fields
		*/
		double ROBOT_MAX_ROTATE;
		double ROBOT_CANNON_MAX_ROTATE;
		double ROBOT_RADAR_MAX_ROTATE;
		double ROBOT_MAX_ACCELERATION;
		double ROBOT_MIN_ACCELERATION;
		double ROBOT_START_ENERGY;
		double ROBOT_MAX_ENERGY;
		double ROBOT_ENERGY_LEVELS;
		double SHOT_SPEED;
		double SHOT_MIN_ENERGY;
		double SHOT_MAX_ENERGY;
		double SHOT_ENERGY_INCREASE_SPEED;
		double TIMEOUT;
		double DEBUG_LEVEL;
		double SEND_ROBOT_COORDINATES;
			
	};
}
#endif //GAMEOPTIONSREPOSITORY_H

