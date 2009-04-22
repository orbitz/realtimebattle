/************************************************************************
    $Id: columbusserverbasestate.cpp,v 1.11 2005/10/07 07:33:51 krolfy Exp $
    
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

    $Log: columbusserverbasestate.cpp,v $
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

    Revision 1.10  2005/08/23 15:37:05  krolfy
    *** empty log message ***

    Revision 1.9  2005/08/10 13:12:31  krolfy
    Added some debug lines, minor fixes

    Revision 1.8  2005/08/02 12:18:42  krolfy
    Work on columbus clientside started

    Revision 1.7  2005/07/29 10:59:51  krolfy
    Columbus server side ready but untested

    Revision 1.6  2005/07/28 08:19:10  krolfy
    Most of the server side ready

    Revision 1.5  2005/01/25 15:21:21  krolfy
    Started some work on the ServerStates

    Revision 1.4  2005/01/06 17:59:26  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#include <common.h>

#include "columbusserverbasestate.h"
#include "columbusinternalserverspecificrepository.h"
#include "columbussharedserverspecificrepository.h"
#include "../../gamecontrol/servercoordinator.h"
#include "../../parser/object_type.h"
#include "../general/enemiesinfo.h"
#include "../general/maps/potentialmap.h"
#include "columbusserverstateenum.h"
#include <iostream>
using namespace std;

namespace Columbus {

	using General::Position;

	ColumbusServerBaseState::ColumbusServerBaseState(	ServerCoordinator* i,
								ColumbusInternalServerSpecificRepository* issp,
								ColumbusSharedServerSpecificRepository* sssp)
		throw (StrategyException,bad_exception):
			ServerState(i),_issp(issp),_sssp(sssp),_counter(0)
	{
		issp->getRobotInfo().name=i->getAssociatedRobotName();
		sssp->getMap().addFinalValue(Parser::ROBOT);
	}
	
	ColumbusServerBaseState::~ColumbusServerBaseState () throw() {}
	
	/**
	 * This method is called from the coordinator's processReceivedRTBMessages method and should send tips to the clients.
	 */
	void  ColumbusServerBaseState::sendServerTips () throw (StrategyException,bad_exception) {
_sssp->getLogger()->logMessage(1,"## SEND ##");
		sendNavigationServerTips();
_sssp->getLogger()->logMessage(1,"## SENT ##");
		if(_sssp->isAttack())
		{
_sssp->getLogger()->logMessage(1,"## ATTACK ##");
			const Position *pos=&(_sssp->currentEnemyPosition());
			_numberconv << pos->x << ' ' << pos->y;
			sendServerTipString("E "+_numberconv.str());
			_numberconv.clear();
			_numberconv.str("");
		}
_sssp->getLogger()->logMessage(1,"## DONE ##");
	}

	
	/**
	* This method is called by ServerCoordinatorRevokes::resetGame()
	*/
	void ColumbusServerBaseState::reset () throw (bad_exception, StrategyException) {
		_issp->getRobotInfo().isDead=false;
	}

	/**
	* This method is called when the RTB message "Radar [dist] [type] [angle]" occurs. This message occurs each turn and gives information about the closest object in the direction the radar points to.
	* @param dist The distance of the observed object
	* @param type The type of the observed object
	* @param angle The angle the observed object is seen at in rad, relative to the robot front
	*/
	void ColumbusServerBaseState::receiveRTBMessageRadar(double dist,object_type type,double angle) throw (StrategyException, bad_exception) {
		_radarDist=dist;
		_radarWhat=type;
		_radarAngle=angle;
	}

	/**
	* This method is called when the RTB message "Info [time] [speed] [cannonAngle]" occurs. This message is sent each turn and gives information about the robot itself.
	* @param time The time that has elapsed since the game had started
	* @param speed The velocity of the robot
	* @param cannonAngle The angle the cannon points to
	*/
	void ColumbusServerBaseState::receiveRTBMessageInfo(double time,double speed,double cannonAngle) throw (StrategyException, bad_exception) {}

	/**
	* This method is called when the RTB message "Coordinates [x] [y] [angle]" occurs. Generally, this message is sent each turn. If the game option SEND_ROBOT_COORDINATES is sent to 2, absolute coordinates are sent. If this value is 1, the coordinates are given relative to the starting point. On the case 0, this message is not sent.
	* @param x And ...
	* @param y ... are the sent coordinates
	* @param angle is the angle the robot front points to
	*/
	void ColumbusServerBaseState::receiveRTBMessageCoordinates(double x,double y,double angle) throw (StrategyException, bad_exception) {
		Position *self=&(_issp->getRobotInfo().pos);
		x-=self->x;
		y-=self->y;
		self->x+=x;
		self->y+=y;
		Position obj=General::relativeToAbsolute(*self,_radarDist,angle+_radarAngle);
		_sssp->getMap().addChange(*self,obj,_radarWhat);
		makeFootPrint(*self,x*x+y*y);
		if(_radarWhat==Parser::ROBOT && _sssp->getTeam().isFriend(obj))
			sendServerTipString("S");
	}

	/**
	* This method is called when the RTB message "RobotInfo [energy] [isTeamMate]" occurs. This message is always sent after another robot has been detected by the radar. It gives some information about this might-be foe.
	* @param energy The unexact energy level of the detected robot
	* @param isTeamMate Defines whether the robot is a team mate (team mode is not yet implemented in RTB)
	*/
	void ColumbusServerBaseState::receiveRTBMessageRobotInfo(double energy,bool isTeamMate) throw (StrategyException, bad_exception) {}

	/**
	* This method is called when the RTB message "RotationReached [what]" occurs. In our case, this message is sent, when a rotation initiated by "RotateTo" or "RotateAmount" has finished or the sweeping has changed direction.
	* param what What this message bears on; 1 = Robot, 2 = Cannon, 4 = Radar
	*/
	void ColumbusServerBaseState::receiveRTBMessageRotationReached(int what) throw (StrategyException, bad_exception) {}	// >> Maybe we should introduce another
									//    enum for this (TODO)

	/**
	* This method is called when the RTB message "Energy [energy]" occurs. This message is sent at the end of each turn and specifies the health state of the robot.
	* @param energy The unexact energy level
	*/
	void ColumbusServerBaseState::receiveRTBMessageEnergy(double energy) throw (StrategyException, bad_exception) {
		if(energy<30)
			setCurrentServerState(ESCAPE);
	}

	/**
	* This method is called when the RTB message "RobotsLeft [num]" occurs. This message is sent at the beginning of the game and after a robot has been killed.
	* @param num The number of remaining robots
	*/
	void ColumbusServerBaseState::receiveRTBMessageRobotsLeft(int num) throw (StrategyException, bad_exception) {
	_sssp->getEnemies().setNumberOfEnemies(num-_sssp->getTeam().getTeamSize());
}

	/**
	* This method is called when the RTB message "Collision [type] [angle]" occurs. This message is sent whan the robot has been hit by or hits another object,
	* @param type The type of the hit object
	* @param angle The angle the collision happened at in rad, relative to the robot front
	*/
	void ColumbusServerBaseState::receiveRTBMessageCollision(object_type type,double angle) throw (StrategyException, bad_exception) {
	}

	/**
	* This method is called when the RTB message "Warning [warning] [message]" occurs. This message when RTB has notify the robot on a problem
	* @param warning The type of the warning
	* @param message The text of the warning
	*/
	void ColumbusServerBaseState::receiveRTBMessageWarning(warning_type warning,const string& message) throw (StrategyException, bad_exception) {}

	/**
	* This method is called when the RTB message "Dead" occurs. This message is sent when the robot pegs out.
	*/
	void ColumbusServerBaseState::receiveRTBMessageDead() throw (StrategyException, bad_exception) {
		_issp->getRobotInfo().isDead=true;
	}
}
