/************************************************************************
    $Id: navigatemap.h,v 1.3 2005/08/23 15:37:05 krolfy Exp $
    
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

    $Log: navigatemap.h,v $
    Revision 1.3  2005/08/23 15:37:05  krolfy
    *** empty log message ***

    Revision 1.2  2005/07/28 08:19:19  krolfy
    Most of the server side ready

    Revision 1.1  2005/05/26 16:20:55  krolfy
    First testing version of PotentialMap


**************************************************************************/

#ifndef NAVIGATEMAP_H
#define NAVIGATEMAP_H

#include "topographicmap.h"
#include "../../../parser/object_type.h"

/**
* Namespace Maps
*/
namespace Maps {
	using Parser::object_type;
/**
 * The base class of all navigation map classes
 */
class NavigateMap:public TopographicMap<object_type>
{
public:
	/**
	 * Creates an empty map. The parameters are passed on to internal TopographicMaps
	 * @param width initial map width in grid squares
	 * @param height initial map height in grid squares
	 * @param density edge length of the grid squares
	 */
	NavigateMap(unsigned int width,unsigned int height,float density) throw (bad_exception);
	virtual ~NavigateMap() throw() {}

	/**
	 * This method gives concrete information where to drive.
	 * @param self the position of the calling bot
	 * @param target the position the bot wants to reach
	 * @return the position the bot shall drive towards
	 */
	virtual Position navigationHint(const Position& self,const Position& target) throw (bad_exception)=0;

	/**
	 * To be called when a new round on a new map starts
	 */
	virtual void reset() throw()=0;
};

} // Namespace

#endif

