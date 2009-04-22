/************************************************************************
    $Id: columbusclientattackstate.cpp,v 1.10 2005/10/07 07:33:51 krolfy Exp $
    
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

    $Log: columbusclientattackstate.cpp,v $
    Revision 1.10  2005/10/07 07:33:51  krolfy
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

    Revision 1.9  2005/10/05 21:11:15  krolfy
    .

    Revision 1.8  2005/08/23 15:37:05  krolfy
    *** empty log message ***

    Revision 1.7  2005/08/17 14:08:43  krolfy
    *** empty log message ***

    Revision 1.6  2005/08/16 11:02:20  krolfy
    Chenged RTBLogDriver to multi line output

    Revision 1.5  2005/08/10 16:00:50  krolfy
    Columbus behaves odd, can SIGSEGV

    Revision 1.4  2005/08/10 13:12:31  krolfy
    Added some debug lines, minor fixes

    Revision 1.3  2005/08/08 11:32:45  krolfy
    Added three Columbus bots (crash with exception)

    Revision 1.2  2005/08/05 16:37:40  krolfy
    First testing version of Columbus ready

    Revision 1.1  2005/08/05 14:29:19  krolfy
    Derived ColumbusClientStates


**************************************************************************/


#include "columbusclientattackstate.h"
#include "columbusclientspecificrepository.h"
#include "../../gamecontrol/gameoptionsrepository.h"
#include <cmath>

namespace Columbus {

ColumbusClientAttackState::ColumbusClientAttackState(ClientCoordinator* i,ColumbusClientSpecificRepository* columbuscsp,const GameOptionsRepository* go) throw (StrategyException,bad_exception):ColumbusClientBaseState(i,columbuscsp,go),_csp(columbuscsp),_go(go) {}

ColumbusClientAttackState::~ColumbusClientAttackState () throw() {}

void  ColumbusClientAttackState::sendRTBMessages () throw (StrategyException,bad_exception)
{
	static double omega =	_go->ROBOT_CANNON_MAX_ROTATE<_go->ROBOT_RADAR_MAX_ROTATE?
				_go->ROBOT_CANNON_MAX_ROTATE:_go->ROBOT_RADAR_MAX_ROTATE;
	static RobotInfo *i=&(_csp->getRobotInfo());
	static const Position *e=&(_csp->myEnemyPosition());
	double dx=e->x-i->pos.x;
	double dy=e->y-i->pos.y;
	double amp=5./(dx*dx+dy*dy+.1);
	double dd=atan2(dy,dx)-i->dirRobot;
	while(dd<-M_PI)
		(dd+=M_PI)+=M_PI;
	while(dd>=M_PI)
		(dd-=M_PI)-=M_PI;
	sendRTBMessageDebugLine(0,0,dd,1.5);
	sendRTBMessageDebugLine(0,0,dd+amp,1);
	sendRTBMessageDebugLine(0,0,dd-amp,1);
	sendRTBMessageDebugCircle(dd,sqrt(5./amp-.1)*.9,.1);
	sendRTBMessageSweep(2,omega,dd-amp,dd+amp);
	sendRTBMessageSweep(4,omega,dd-amp,dd+amp);
	if(!((++_shoot)%=1+int(.4/amp)))
		sendRTBMessageShoot(_go->SHOT_MIN_ENERGY);
}
	
}

