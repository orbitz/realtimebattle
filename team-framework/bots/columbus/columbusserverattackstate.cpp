/************************************************************************
    $Id: columbusserverattackstate.cpp,v 1.7 2005/10/07 07:33:51 krolfy Exp $

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

    $Log: columbusserverattackstate.cpp,v $
    Revision 1.7  2005/10/07 07:33:51  krolfy
    sources/alpha/bots/columbus/columbusclientattackstate.cpp
    sources/alpha/bots/columbus/columbusclientbasestate.cpp
    sources/alpha/bots/columbus/columbusserverattackstate.cpp
    sources/alpha/bots/columbus/columbusserverbasestate.cpp
    sources/alpha/bots/columbus/columbusserverdiscoverystate.cpp
    sources/alpha/bots/columbus/columbusserverescapestate.cpp
    sources/alpha/bots/columbus/columbusserverinitstate.cpp
    sources/alpha/bots/columbus/columbusservertrackstate.cpp
    sources/alpha/bots/general/maps/potentialmap.cpp
    sources/alpha/bots/general/maps/topographicmap_float.h
    sources/alpha/gamecontrol/servercoordinator.cpp
    sources/alpha/gamecontrol/servergamecontroller.cpp

    Revision 1.6  2005/08/23 15:37:05  krolfy
    *** empty log message ***

    Revision 1.5  2005/08/17 14:08:43  krolfy
    *** empty log message ***

    Revision 1.4  2005/08/10 13:12:31  krolfy
    Added some debug lines, minor fixes

    Revision 1.3  2005/08/02 12:18:42  krolfy
    Work on columbus clientside started


**************************************************************************/

#include "columbusserverattackstate.h"
#include "columbusserverstateenum.h"
#include "columbussharedserverspecificrepository.h"
#include "columbusinternalserverspecificrepository.h"


/**
* Namespace Columbus
*/
namespace Columbus {

void ColumbusServerAttackState::sendNavigationServerTips() throw(bad_exception)
{
_sssp->getLogger()->logMessage(1,"## ATT ##");
	if(_sssp->isAttack())
	{
		const Position *self=&(_issp->getRobotInfo().pos);
		const Position *enemy=&(_sssp->currentEnemyPosition());
		Position pos=_sssp->getMap().navigationHint(*self,*enemy,1,.8,1,.7);
		_numberconv << pos.x << ' ' << pos.y;
		sendServerTipString("D "+_numberconv.str());
		_numberconv.clear();
		_numberconv.str("");
		if(_sssp->getMap().isFreeVisionLine(*self,*enemy))
			sendServerTipString("T");
	} else setCurrentServerState(DISCOVERY);
_sssp->getLogger()->logMessage(1,"done");
}

void ColumbusServerAttackState::handleEnemy(const General::Position& pos) throw(bad_exception)
{
	_sssp->getEnemies().addEnemyPosition(pos);
}

void ColumbusServerAttackState::makeFootPrint(const General::Position& pos,const General::Position& speed) throw(bad_exception)
{}

ColumbusServerAttackState::ColumbusServerAttackState(ServerCoordinator* c,ColumbusInternalServerSpecificRepository* columbusssp, ColumbusSharedServerSpecificRepository* columbussharedssp) throw (StrategyException,bad_exception): ColumbusServerBaseState(c,columbusssp,columbussharedssp),_sssp(columbussharedssp),_issp(columbusssp) {}

ColumbusServerAttackState::~ColumbusServerAttackState () throw() {}

}
