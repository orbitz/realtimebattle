/************************************************************************
    $Id: columbusclientbasestate.cpp,v 1.11 2005/10/07 07:33:51 krolfy Exp $
    
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

    $Log: columbusclientbasestate.cpp,v $
    Revision 1.11  2005/10/07 07:33:51  krolfy
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

    Revision 1.10  2005/10/05 21:11:15  krolfy
    .

    Revision 1.9  2005/08/23 15:37:05  krolfy
    *** empty log message ***

    Revision 1.8  2005/08/17 14:08:43  krolfy
    *** empty log message ***

    Revision 1.7  2005/08/16 11:02:20  krolfy
    Chenged RTBLogDriver to multi line output

    Revision 1.6  2005/08/10 16:00:50  krolfy
    Columbus behaves odd, can SIGSEGV

    Revision 1.5  2005/08/10 13:12:31  krolfy
    Added some debug lines, minor fixes

    Revision 1.4  2005/08/05 16:37:40  krolfy
    First testing version of Columbus ready

    Revision 1.3  2005/08/02 12:18:41  krolfy
    Work on columbus clientside started

    Revision 1.2  2005/01/06 17:59:26  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#include <common.h>

#include "columbusclientbasestate.h"
#include "../../gamecontrol/gameoptionsrepository.h"
#include "columbusclientspecificrepository.h"
#include "columbusclientstateenum.h"
#include <cmath>


namespace Columbus {
ColumbusClientBaseState::ColumbusClientBaseState(ClientCoordinator* i,ColumbusClientSpecificRepository* columbuscsp,const GameOptionsRepository* go) throw (StrategyException,bad_exception):ClientState(i),_csp(columbuscsp),_go(go) {}

ColumbusClientBaseState::~ColumbusClientBaseState () throw() {}

void ColumbusClientBaseState::reactOnServerTip (const string& tip) throw (StrategyException,bad_exception) {
	_sreader.str(tip);
	char c;
	double x,y;
	_sreader >> c;
	switch(c)
	{
		case 'E':	_sreader >> x >> y;
				_csp->setCurrentEnemyPosition(Position(x,y));
/*				x=(y=0);
				x-=_csp->getRobotInfo().pos.x;
				y-=_csp->getRobotInfo().pos.y;
				sendRTBMessageDebugLine(0,0,atan2(y,x)-_csp->getRobotInfo().dirRobot,sqrt(x*x+y*y));*/
				break;
		case 'D':	_sreader >> x >> y;
				driveTowards(Position(x,y));
				break;
		case 'T':	_csp->setMyEnemyPosition(_csp->currentEnemyPosition());
				setCurrentClientState(ATTACK);
				break;
		case 'S':	_counter=31;
				setCurrentClientState(DRIVE);
				break;
		default:	_csp->getLogger()->logMessage(100,"Unknown server tip in columbus: "+tip);
	}
	_sreader.clear();
}

void ColumbusClientBaseState::driveTowards (const Position& pos) throw (bad_exception)
{
	static double para=M_PI*_go->ROBOT_MAX_ACCELERATION;
	static double M2PI=M_PI+M_PI;
	static RobotInfo *i=&(_csp->getRobotInfo());
	double dx=pos.x-i->pos.x;
	double dy=pos.y-i->pos.y;
	double force=dx*dx+dy*dy;
	force=(M_1_PI-M_1_PI/(1+force))*.8+.2;
	double dir=dx==0.&&dy==0.?0.:atan2(dy,dx)-i->dirRobot;
	dir-=M2PI*int(dir/M2PI);
	while(dir<-M_PI)
		dir+=M2PI;
	while(dir>=M_PI)
		dir-=M2PI;
	sendRTBMessageDebugLine(0,0,dir,.6+force);
	double ad=fabs(dir);
	if(ad>M_PI_2)
		sendRTBMessageBrake(force*(ad-M_PI));
	else	sendRTBMessageAccelerate(force*(para-_go->ROBOT_MAX_ACCELERATION*ad));
	sendRTBMessageRotate(1,force*dir*_go->ROBOT_MAX_ROTATE);
}

void ColumbusClientBaseState::roundFinished () throw (StrategyException,bad_exception) {}

void ColumbusClientBaseState::sendRTBMessages () throw (StrategyException,bad_exception) {}

void ColumbusClientBaseState::reset () throw (bad_exception, StrategyException) {}

void ColumbusClientBaseState::receiveRTBMessageRadar(double dist,object_type type,double angle) throw (StrategyException, bad_exception) 
{
	_radarDist=dist;
	_radarWhat=type;
	static RobotInfo* i=&(_csp->getRobotInfo());
	i->dirRadar=i->dirRobot+angle;
}

void ColumbusClientBaseState::receiveRTBMessageInfo(double time,double speed,double cannonAngle) throw (StrategyException, bad_exception) {
	static RobotInfo* i=&(_csp->getRobotInfo());
	i->dirCannon=i->dirRobot+cannonAngle;
}

void ColumbusClientBaseState::receiveRTBMessageCoordinates(double x,double y,double angle) throw (StrategyException, bad_exception)
{
	_csp->getRobotInfo().dirRadar=angle;
	if(_counter)
		switch(++_counter)
		{
			case 30:
			case 60: _counter=0;
				 setCurrentClientState(DRIVE);
		}
	else setCurrentClientState(DRIVE);
	static General::RobotInfo *self=&(_csp->getRobotInfo());
	x-=self->pos.x;
	y-=self->pos.y;
	self->pos.x+=x;
	self->pos.y+=y;
	Position obj=General::relativeToAbsolute(self->pos,_radarDist,angle+_radarAngle+self->dirRobot);
	if(_radarWhat==Parser::ROBOT)
		if(_counter<30)
		{
			_counter=1;
			_csp->setMyEnemyPosition(obj);
			setCurrentClientState(ATTACK);
		}
}

void ColumbusClientBaseState::receiveRTBMessageRobotInfo(double energy,bool isTeamMate) throw (StrategyException, bad_exception) {}

void ColumbusClientBaseState::receiveRTBMessageRotationReached(int what) throw (StrategyException, bad_exception) {}

void ColumbusClientBaseState::receiveRTBMessageEnergy(double energy) throw (StrategyException, bad_exception) {}

void ColumbusClientBaseState::receiveRTBMessageRobotsLeft(int num) throw (StrategyException, bad_exception) {}

void ColumbusClientBaseState::receiveRTBMessageCollision(object_type type,double angle) throw (StrategyException, bad_exception) {}

void ColumbusClientBaseState::receiveRTBMessageWarning(warning_type warning,const string& message) throw (StrategyException, bad_exception) {}

void ColumbusClientBaseState::receiveRTBMessageDead() throw (StrategyException, bad_exception) {}

}
