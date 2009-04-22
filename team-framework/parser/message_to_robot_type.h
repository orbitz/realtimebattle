/************************************************************************
    $Id: message_to_robot_type.h,v 1.2 2005/01/06 17:59:28 jonico Exp $
    
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

    $Log: message_to_robot_type.h,v $
    Revision 1.2  2005/01/06 17:59:28  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef __MESSAGETOROBOTTYPE__
#define __MESSAGETOROBOTTYPE__

/**
* Namespace Parser
*/
namespace Parser {
	enum message_to_robot_type 
	{
	  UNKNOWN_MESSAGE_TO_ROBOT = -1,
	  INITIALIZE,
	  YOUR_NAME,
	  YOUR_COLOUR,
	  //  BIN_DATA_TO,
	  //  ASCII_DATA_TO,
	  //  LOAD_DATA_FINISHED,
	  GAME_OPTION,
	  GAME_STARTS,
	  RADAR,
	  INFO,
	  COORDINATES,
	  ROBOT_INFO,
	  ROTATION_REACHED,
	  ENERGY,
	  ROBOTS_LEFT,
	  COLLISION,
	  WARNING,
	  DEAD,
	  GAME_FINISHES,
	  //  SAVE_DATA,
	  EXIT_ROBOT
	};
}

#endif 
