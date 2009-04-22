/************************************************************************
    $Id: object_type.h,v 1.2 2005/01/06 17:59:28 jonico Exp $
    
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

    $Log: object_type.h,v $
    Revision 1.2  2005/01/06 17:59:28  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef __OBJECTTYPE__
#define __OBJECTTYPE__

/**
* Namespace Parser
*/
namespace Parser {
	enum object_type 
	{ 
	  NOOBJECT = -1, 
	  ROBOT = 0, 
	  SHOT = 1, 
	  WALL = 2, 
	  COOKIE = 3, 
	  MINE = 4,
	  LAST_OBJECT_TYPE = 5
	};

	// static const int number_of_object_types = 5; // global variables are very ugly, static is senseless in this case
	const int number_of_object_types = 5; // now it's capsulated in a namespace, but it is ugly though, please find another solution
}

#endif 
