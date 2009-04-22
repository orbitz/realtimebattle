/************************************************************************
  					cobra_server_basicstate.cpp
Copyright:
    Tino Truppel <tino.truppel@hpi.uni-potsdam.de>

This file was created on Mon, Dec 13 2004

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


The original location of this file is cobra
**************************************************************************/

#include <common.h>
#include "cobra_server_basicstate.h"
#include "cobra_internalserverspecificrepository.h"
#include "cobra_sharedserverspecificrepository.h"
#include "../general/teaminfo.h"
#include "../../parser/object_type.h"


namespace cobra {
	using General::Position;
	using Parser::object_type;
	
	cobra_ServerBasicState::cobra_ServerBasicState(ServerCoordinator* i, cobra_InternalServerSpecificRepository* cobra_issp, cobra_SharedServerSpecificRepository* cobra_sharedssp) throw (StrategyException,bad_exception): ServerState(i),_robot("cobra")
	{
		_cobra_sharedssp = cobra_sharedssp;
	}

	cobra_ServerBasicState::~cobra_ServerBasicState () throw() {}

	/**
	 * This method is called from the coordinator's processReceivedRTBMessages method and should send tips to the clients.
	 */
	void cobra_ServerBasicState::sendServerTips () throw (StrategyException,bad_exception) {}


	/**
	* This method is called by ServerCoordinatorRevokes::resetGame()
	*/
	void cobra_ServerBasicState::reset () throw (bad_exception, StrategyException) 
	{
		_robot.ID=_cobra_sharedssp->getTeamInfo()->registerRobotForRound(_robot);
	}

	/**
	* This method is called when the RTB message "Radar [dist] [type] [angle]" occurs. This message occurs each turn and gives information about the closest object in the direction the radar points to.
	* @param dist The distance of the observed object
	* @param type The type of the observed object
	* @param angle The angle the observed object is seen at in rad, relative to the robot front
	*/
	void cobra_ServerBasicState::receiveRTBMessageRadar(double dist,object_type type,double angle) throw (StrategyException, bad_exception) 
	{
		if (type == Parser::ROBOT)
		{
			Position pos = relativeToAbsolute(_robot.pos, dist, angle+_robot.dirRobot);
			if (!_cobra_sharedssp->getTeamInfo()->isFriend(pos, 1))
			{
				sendServerTipString("SHOOT");
			} else
			{
				sendServerTipString("STOPSHOOT");
			}
		}
	}

	/**
	* This method is called when the RTB message "Info [time] [speed] [cannonAngle]" occurs. This message is sent each turn and gives information about the robot itself.
	* @param time The time that has elapsed since the game had started
	* @param speed The velocity of the robot
	* @param cannonAngle The angle the cannon points to
	*/
	void cobra_ServerBasicState::receiveRTBMessageInfo(double time,double speed,double cannonAngle) throw (StrategyException, bad_exception) {}

	/**
	* This method is called when the RTB message "Coordinates [x] [y] [angle]" occurs. Generally, this message is sent each turn. If the game option SEND_ROBOT_COORDINATES is sent to 2, absolute coordinates are sent. If this value is 1, the coordinates are given relative to the starting point. On the case 0, this message is not sent.
	* @param x And ...
	* @param y ... are the sent coordinates
	* @param angle is the angle the robot front points to
	*/
	void cobra_ServerBasicState::receiveRTBMessageCoordinates(double x,double y,double angle) throw (StrategyException, bad_exception) 
	{
		 _robot.pos.x=x;_robot.pos.y=y;
		 _robot.dirRobot=angle;		 
	}

	/**
	* This method is called when the RTB message "RobotInfo [energy] [isTeamMate]" occurs. This message is always sent after another robot has been detected by the radar. It gives some information about this might-be foe.
	* @param energy The unexact energy level of the detected robot
	* @param isTeamMate Defines whether the robot is a team mate (team mode is not yet implemented in RTB)
	*/
	void cobra_ServerBasicState::receiveRTBMessageRobotInfo(double energy,bool isTeamMate) throw (StrategyException, bad_exception) {}

	/**
	* This method is called when the RTB message "RotationReached [what]" occurs. In our case, this message is sent, when a rotation initiated by "RotateTo" or "RotateAmount" has finished or the sweeping has changed direction.
	* @param what What this message bears on; 1 = Robot, 2 = Cannon, 4 = Radar
	*/
	void cobra_ServerBasicState::receiveRTBMessageRotationReached(int what) throw (StrategyException, bad_exception) {}	// >> Maybe we should introduce another
									//    enum for this (TODO)

	/**
	* This method is called when the RTB message "Energy [energy]" occurs. This message is sent at the end of each turn and specifies the health state of the robot.
	* @param energy The unexact energy level
	*/
	void cobra_ServerBasicState::receiveRTBMessageEnergy(double energy) throw (StrategyException, bad_exception) {}

	/**
	* This method is called when the RTB message "RobotsLeft [num]" occurs. This message is sent at the beginning of the game and after a robot has been killed.
	* @param num The number of remaining robots
	*/
	void cobra_ServerBasicState::receiveRTBMessageRobotsLeft(int num) throw (StrategyException, bad_exception) {}

	/**
	* This method is called when the RTB message "Collision [type] [angle]" occurs. This message is sent whan the robot has been hit by or hits another object,
	* @param type The type of the hit object
	* @param angle The angle the collision happened at in rad, relative to the robot front
	*/
	void cobra_ServerBasicState::receiveRTBMessageCollision(object_type type,double angle) throw (StrategyException, bad_exception) {}

	/**
	* This method is called when the RTB message "Warning [warning] [message]" occurs. This message when RTB has notify the robot on a problem
	* @param warning The type of the warning
	* @param message The text of the warning
	*/
	void cobra_ServerBasicState::receiveRTBMessageWarning(warning_type warning,const string& message) throw (StrategyException, bad_exception) {}

	/**
	* This method is called when the RTB message "Dead" occurs. This message is sent when the robot pegs out.
	*/
	void cobra_ServerBasicState::receiveRTBMessageDead() throw (StrategyException, bad_exception) 
	{
		_robot.isDead=true;
	}
}
