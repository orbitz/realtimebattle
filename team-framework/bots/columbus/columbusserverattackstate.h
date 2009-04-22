/************************************************************************
    $Id: columbusserverattackstate.h,v 1.2 2005/08/02 12:18:42 krolfy Exp $

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

    $Log: columbusserverattackstate.h,v $
    Revision 1.2  2005/08/02 12:18:42  krolfy
    Work on columbus clientside started

    Revision 1.1  2005/07/29 10:59:51  krolfy
    Columbus server side ready but untested

**************************************************************************/

#ifndef COLUMBUSSERVERATTACKSTATE_H
#define COLUMBUSSERVERATTACKSTATE_H

#include "columbusserverbasestate.h"
#include "columbusnamespace.h"

#include <sstream>

/**
* Namespace Columbus
*/
namespace Columbus {

	using Strategies::ServerState;
	using Exceptions::StrategyException;
	using std::string;
	using std::bad_exception;
	using Parser::object_type;
	using Parser::warning_type;
	using GameControl::ServerCoordinator;
	using std::ostringstream;

	class ColumbusServerAttackState: public ColumbusServerBaseState {
	public:

		/**
		 * Intended as a frontend for PotentialMap::navigationHint
		 */
		virtual void sendNavigationServerTips() throw(bad_exception);

		/**
		 * Defines what to do with an enemy found.
		 * @param pos the position of the enemy
		 */
		virtual void handleEnemy(const General::Position& pos) throw(bad_exception);

		/**
		 * Make footprints, also see PotentialMap::makeFootPrint.
		 * @param pos where to make the footprint
		 * @param speed the depth of ther footprint shall be be dependent on the robot's movement
		 */
		virtual void makeFootPrint(const General::Position& pos,const General::Position& speed) throw(bad_exception);

		ColumbusServerAttackState(ServerCoordinator* c,ColumbusInternalServerSpecificRepository* columbusssp, ColumbusSharedServerSpecificRepository* columbussharedssp) throw (StrategyException,bad_exception);

		virtual ~ColumbusServerAttackState () throw();
		
	private:
		ColumbusInternalServerSpecificRepository* _issp;
		ostringstream _numberconv;
		ColumbusSharedServerSpecificRepository* _sssp;
		double _radarDist,_radarAngle;
	};
}

#endif
