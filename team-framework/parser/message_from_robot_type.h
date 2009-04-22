/************************************************************************
    $Id: message_from_robot_type.h,v 1.2 2005/01/06 17:59:28 jonico Exp $
    
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

    $Log: message_from_robot_type.h,v $
    Revision 1.2  2005/01/06 17:59:28  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef __MESSAGEFROMROBOTTYPE__
#define __MESSAGEFROMROBOTTYPE__

/**
* Namespace Parser
*/
namespace Parser {
	enum message_from_robot_type 
	{
	  UNKNOWN_MESSAGE_FROM_ROBOT = -1,
	  ROBOT_OPTION,
	  NAME,
	  COLOUR,
	  //  LOAD_DATA,
	  ROTATE,
	  ROTATE_TO,
	  ROTATE_AMOUNT,
	  SWEEP,
	  ACCELERATE,
	  BRAKE,
	  BREAK,
	  SHOOT,
	  PRINT,
	  DEBUG,
	  DEBUG_LINE,
	  DEBUG_CIRCLE

	  //  BIN_DATA_FROM,
	  //  ASCII_DATA_FROM,
	  //  SAVE_DATA_FINISHED
	};
}

#endif 
