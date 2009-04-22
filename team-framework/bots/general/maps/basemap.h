/************************************************************************
    $Id: basemap.h,v 1.5 2005/05/26 16:20:55 krolfy Exp $
    
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

    $Log: basemap.h,v $
    Revision 1.5  2005/05/26 16:20:55  krolfy
    First testing version of PotentialMap

    Revision 1.4  2005/04/14 14:20:23  jonico

    now we have explicit template instatiation

    Revision 1.3  2005/04/06 11:13:50  krolfy
    Changed all (double x,double y) to General::Position

    Revision 1.2  2005/04/05 09:01:50  krolfy
    Rectified the declaration of addChange()

    Revision 1.1  2005/04/05 08:49:27  krolfy
    First publication of the map classes


**************************************************************************/

#ifndef BASEMAP_H
#define BASEMAP_H

#include <stdexcept>
#include "../teaminfo.h"

/**
* Namespace Maps
*/
namespace Maps {
	using std::bad_exception;
	using General::Position;
/**
 * The base class of all map classes
 */
template<typename _Tp>
class BaseMap
{
public:
	BaseMap() throw (bad_exception) {}
	virtual ~BaseMap() throw() {}

	/**
	 * Every time after having received both the commands "Coordiantes" and "Radar", the ServerState is to call this method if it saw something worth to draw onto the map.
	 * @param self the position of the ServerState's robot
	 * @param obj the position of the observed object
	 * @param type the type of the observed object
	 */
	virtual void addChange(const Position& self,const Position& obj,const _Tp& type) throw (bad_exception)=0;

	/**
	 * To be called when a new round on a new map starts
	 */
	virtual void reset() throw()=0;
};

} // Namespace

#endif

