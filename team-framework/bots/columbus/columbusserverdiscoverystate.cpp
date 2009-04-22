/************************************************************************
    $Id: columbusserverdiscoverystate.cpp,v 1.6 2005/10/07 07:33:51 krolfy Exp $

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

    $Log: columbusserverdiscoverystate.cpp,v $
    Revision 1.6  2005/10/07 07:33:51  krolfy
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

    Revision 1.5  2005/08/23 15:37:05  krolfy
    *** empty log message ***

    Revision 1.4  2005/08/17 14:08:43  krolfy
    *** empty log message ***

    Revision 1.3  2005/08/10 13:12:31  krolfy
    Added some debug lines, minor fixes

    Revision 1.2  2005/08/02 12:18:42  krolfy
    Work on columbus clientside started


**************************************************************************/
#include "columbusserverdiscoverystate.h"
#include "columbusserverstateenum.h"
#include "columbussharedserverspecificrepository.h"
#include "columbusinternalserverspecificrepository.h"

/**
* Namespace Columbus
*/
namespace Columbus {

void ColumbusServerDiscoveryState::sendNavigationServerTips() throw(bad_exception)
{
_sssp->getLogger()->logMessage(1,"## DISC ##");
	if(_sssp->isAttack())
		setCurrentServerState(ATTACK);
	else {
		Position pos;
_sssp->getLogger()->logMessage(1,"map");
		if(_sssp->getEnemies().getNumberOfStoredEnemies())
			pos=_sssp->getMap().navigationHint(_issp->getRobotInfo().pos,_sssp->currentEnemyPosition());
		else	pos=_sssp->getMap().navigationHint(_issp->getRobotInfo().pos,_tmp,0);	
_sssp->getLogger()->logMessage(1,"interpret");
		_numberconv << pos.x << ' ' << pos.y;
		sendServerTipString("D "+_numberconv.str());
		_numberconv.clear();
		_numberconv.str("");
	}
_sssp->getLogger()->logMessage(1,"done");
}

void ColumbusServerDiscoveryState::handleEnemy(const General::Position& pos) throw(bad_exception)
{
	_sssp->setCurrentEnemyPosition(pos);
	_sssp->attack(true);
	setCurrentServerState(TRACK);
}

void ColumbusServerDiscoveryState::makeFootPrint(const General::Position& pos,const General::Position& speed) throw(bad_exception)
{}

ColumbusServerDiscoveryState::ColumbusServerDiscoveryState(ServerCoordinator* c,ColumbusInternalServerSpecificRepository* columbusssp, ColumbusSharedServerSpecificRepository* columbussharedssp) throw (StrategyException,bad_exception): ColumbusServerBaseState(c,columbusssp,columbussharedssp),_sssp(columbussharedssp),_issp(columbusssp) {}

ColumbusServerDiscoveryState::~ColumbusServerDiscoveryState () throw() {}

}
