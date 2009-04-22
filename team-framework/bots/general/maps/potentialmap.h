/************************************************************************
    $Id: potentialmap.h,v 1.5 2005/08/23 15:37:05 krolfy Exp $
    
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

    $Log: potentialmap.h,v $
    Revision 1.5  2005/08/23 15:37:05  krolfy
    *** empty log message ***

    Revision 1.4  2005/07/29 12:33:14  krolfy
    Added isFreeVisionLine method to map classes

    Revision 1.3  2005/07/28 08:19:19  krolfy
    Most of the server side ready

    Revision 1.2  2005/06/02 17:35:35  krolfy
    Changed mines potential, introduced footprint potential

    Revision 1.1  2005/05/26 16:20:55  krolfy
    First testing version of PotentialMap


**************************************************************************/

#ifndef POTENTIALMAP_H
#define POTENTIALMAP_H

#include "navigatemap.h"
#include "topographicmap.h"
#include "../../../parser/object_type.h"

/**
* Namespace Maps
*/
namespace Maps {
/**
 * This is our first map class usable for navigation.  The algorithm is, that we define a sort of ostensive potential all over the map whose gradient equals an also ostensive force which makes the robot driving curves
 */
class PotentialMap:public NavigateMap
{
public:
	/**
	 * Creates an empty map. The parameters are passed on to internal TopographicMaps
	 * @param width initial map width in grid squares
	 * @param height initial map height in grid squares
	 * @param density edge length of the grid squares
	 */
	PotentialMap(unsigned int width=50,unsigned int height=50,float density=.7071) throw (bad_exception);
	virtual ~PotentialMap() throw() {}

	/**
	 * Every time after having received both the commands "Coordiantes" and "Radar", the ServerState is to call this method if it saw something worth to draw onto the map.
	 * @param self the position of the ServerState's robot
	 * @param obj the position of the observed object
	 * @param type the type of the observed object
	 */
	virtual void addChange(const Position& self,const Position& obj,const object_type& type) throw (bad_exception);

	/**
	 * This method gives concrete information where to drive.
	 * NOTE: Personally, I took it that way that if the course correction is urgent, the returned value is far from the calling bot's position.
	 * @param self the position of the calling bot
	 * @param target the position the bot wants to reach
	 * @return the position the bot shall drive towards
	 */
	virtual Position navigationHint(const Position& self,const Position& target) throw (bad_exception);

	/**
	 * Variation of the above method, to vary the importance and sign of the different sub-potentials
	 * 
	 */
	virtual Position navigationHint(const Position& self,const Position& target,float fEnemy,float fWalls=1,float fMines=1,float fTrack=1) throw (bad_exception);

	/**
	 * To be called when a new round on a new map starts
	 */
	virtual void reset() throw();

	/**
	 * Make a footprint which is a hint that the path you are taking is good/bad to use
	 * @param pos Where to make the footprint;
	 * @param depth The depth of the footprint. Positive values mark good paths, negative ones bad paths. The absolute value should be between zero (none at all) and one (very deep). However, it is possible to use values higher than one, and it will result in a really super-deep footprint, but stay below one unless you there is a really good reason.
	 */
	virtual void makeFootPrint(const Position& pos,float depth) throw(bad_exception);

	/**
	 * Check a chord for being a free vision line
	 * @param self first endpoint of the chord
	 * @param obj second endpoint of the chord
	 * @return whether there are only "empty" values (see setEmptyValue) on the chord
	 */
//	virtual bool isFreeVisionLine(const Position& self,const Position& obj) const throw (bad_exception);
private:
	TopographicMap<float> _walls,_mines,_track;
	float _density;
};

} // Namespace

#endif

