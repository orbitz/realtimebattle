/************************************************************************
    $Id: game_option_type.h,v 1.2 2005/01/06 17:59:28 jonico Exp $
    
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

    $Log: game_option_type.h,v $
    Revision 1.2  2005/01/06 17:59:28  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef __GAMEOPTIONTYPE__
#define __GAMEOPTIONTYPE__

/**
* Namespace Parser
*/
namespace Parser {
	enum game_option_type
	{
	  ROBOT_MAX_ROTATE=0,
	  ROBOT_CANNON_MAX_ROTATE=1,
	  ROBOT_RADAR_MAX_ROTATE=2,

	  ROBOT_MAX_ACCELERATION=3,
	  ROBOT_MIN_ACCELERATION=4,

	  ROBOT_START_ENERGY=5,
	  ROBOT_MAX_ENERGY=6,
	  ROBOT_ENERGY_LEVELS=7,

	  SHOT_SPEED=8,
	  SHOT_MIN_ENERGY=9,
	  SHOT_MAX_ENERGY=10,
	  SHOT_ENERGY_INCREASE_SPEED=11,

	  TIMEOUT=12,

	  DEBUG_LEVEL=13,            // 0 - no debug, 5 - highest debug level

	  SEND_ROBOT_COORDINATES= 14    // 0 - no coordinates, 
					// 1 - coordniates are given relative the starting position
					// 2 - absolute coordinates 
	};
}


#endif 
