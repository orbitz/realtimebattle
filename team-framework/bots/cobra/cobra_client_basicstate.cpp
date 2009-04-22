/************************************************************************
  					cobra_client_basicstate.cpp
Copyright:
    Tino Truppel <tino.truppel@hpi.uni-potsdam.de>

This file was created on Mon, Dec 12 2004

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

#include "cobra_client_basicstate.h"
#include "../../gamecontrol/gameoptionsrepository.h"
#include "../general/teaminfo.h"
#include "cobra_clientspecificrepository.h"

namespace cobra {

	cobra_ClientBasicState::cobra_ClientBasicState(ClientCoordinator* i, cobra_ClientSpecificRepository* cobra_csp, const GameOptionsRepository* cobra_gor) throw (StrategyException,bad_exception):ClientState(i) 
	{
		_shoot = false;
	}

	cobra_ClientBasicState::~cobra_ClientBasicState () throw() {}
	
	/**
	* Processes the server tips
	* @param tip The server tip in string shape
	*/
	void cobra_ClientBasicState::reactOnServerTip (const string& tip) throw (StrategyException,bad_exception) 
	{
		if (tip == "SHOOT")
		{
			_shoot = true;
		} else
		{
			_shoot = false;
		}
	}
	
	/**
	* This method is called when all rtb messages in this round were received. After this method, the first server tips will arrive.
	*/
	void cobra_ClientBasicState::roundFinished () throw (StrategyException,bad_exception) {}
	
	/**
	* This method is called when all server tips in this round were received. After this method, the state is to send its last messages.
	*/
	void cobra_ClientBasicState::sendRTBMessages () throw (StrategyException,bad_exception)
	{
		sendRTBMessageSweep(6,7,-0.3,0.3);
		sendRTBMessageAccelerate(0.6);
	}

	/**
	* This method is called by ClientCoordinatorRevokes::resetGame()
	*/
	void cobra_ClientBasicState::reset () throw (bad_exception, StrategyException) {}

	/**
	* This method is called when the RTB message "Radar [dist] [type] [angle]" occurs. This message occurs each turn and gives information about the closest object in the direction the radar points to.
	* @param dist The distance of the observed object
	* @param type The type of the observed object
	* @param angle The angle the observed object is seen at in rad, relative to the robot front
	*/
	void cobra_ClientBasicState::receiveRTBMessageRadar(double dist,object_type type,double angle) throw (StrategyException, bad_exception)
	{	
		if(type == 0)
		{
      		if(angle < -0.3 || angle > 0.3)
       		{
         		if(angle > 0)
          		{
          			if(dist > 6) 
          				sendRTBMessageRotateAmount(1, 7, 0.4);
					else 
						sendRTBMessageRotateAmount(1, 7, 0.7);
				}
				else
				{
					if(dist > 6) 
						sendRTBMessageRotateAmount(1, 7, -0.4);
					else 
						sendRTBMessageRotateAmount(1, 7, -0.7);
				}
			}
			else if(angle > -0.4 && angle < 0.4)
				if (_shoot) {sendRTBMessageShoot(30);}
		}
		else if(type == 2)
		{
			if(angle > -0.4 && angle < 0.4 && dist < 7)
				sendRTBMessageRotateAmount(1, 7, 0.1);
			if(angle > -0.4 && angle < 0.4 && dist < 2)
				sendRTBMessageRotateAmount(1, 7, 0.7);
			if(angle > -0.1 && angle < 0.2 && dist < 0.01)
				sendRTBMessageRotateAmount(1, 7, 1);
			sendRTBMessageAccelerate(1);
		}
		else if(type == 4 && angle > -0.3 && angle < 0.3 )
		   	sendRTBMessageShoot(0.5);
		else if(type == 3)
		{        
			if(angle > 0.3)
				sendRTBMessageRotateAmount(1, 7, 0.9);
			else if(angle < -0.3)
				sendRTBMessageRotateAmount(1, 7, -0.9);
			sendRTBMessageAccelerate(0.5);
		}
	}
		
	/**
	* This method is called when the RTB message "Info [time] [speed] [cannonAngle]" occurs. This message is sent each turn and gives information about the robot itself.
	* @param time The time that has elapsed since the game had started
	* @param speed The velocity of the robot
	* @param cannonAngle The angle the cannon points to
	*/
	void cobra_ClientBasicState::receiveRTBMessageInfo(double time,double speed,double cannonAngle) throw (StrategyException, bad_exception) {}

	/**
	* This method is called when the RTB message "Coordinates [x] [y] [angle]" occurs. Generally, this message is sent each turn. If the game option SEND_ROBOT_COORDINATES is sent to 2, absolute coordinates are sent. If this value is 1, the coordinates are given relative to the starting point. On the case 0, this message is not sent.
	* @param x And ...
	* @param y ... are the sent coordinates
	* @param angle is the angle the robot front points to
	*/
	void cobra_ClientBasicState::receiveRTBMessageCoordinates(double x,double y,double angle) throw (StrategyException, bad_exception) {}

	/**
	* This method is called when the RTB message "RobotInfo [energy] [isTeamMate]" occurs. This message is always sent after another robot has been detected by the radar. It gives some information about this might-be foe.
	* @param energy The unexact energy level of the detected robot
	* @param isTeamMate Defines whether the robot is a team mate (team mode is not yet implemented in RTB)
	*/
	void cobra_ClientBasicState::receiveRTBMessageRobotInfo(double energy,bool isTeamMate) throw (StrategyException, bad_exception) {}

	/**
	* This method is called when the RTB message "RotationReached [what]" occurs. In our case, this message is sent, when a rotation initiated by "RotateTo" or "RotateAmount" has finished or the sweeping has changed direction.
	* @param what What this message bears on; 1 = Robot, 2 = Cannon, 4 = Radar
	*/
	void cobra_ClientBasicState::receiveRTBMessageRotationReached(int what) throw (StrategyException, bad_exception) {}

	/**
	* This method is called when the RTB message "Energy [energy]" occurs. This message is sent at the end of each turn and specifies the health state of the robot.
	* @param energy The unexact energy level
	*/
	void cobra_ClientBasicState::receiveRTBMessageEnergy(double energy) throw (StrategyException, bad_exception) {}

	/**
	* This method is called when the RTB message "RobotsLeft [num]" occurs. This message is sent at the beginning of the game and after a robot has been killed.
	* @param num The number of remaining robots
	*/
	void cobra_ClientBasicState::receiveRTBMessageRobotsLeft(int num) throw (StrategyException, bad_exception) {}

	/**
	* This method is called when the RTB message "Collision [type] [angle]" occurs. This message is sent whan the robot has been hit by or hits another object,
	* @param type The type of the hit object
	* @param angle The angle the collision happened at in rad, relative to the robot front
	*/
	void cobra_ClientBasicState::receiveRTBMessageCollision(object_type type,double angle) throw (StrategyException, bad_exception) {}

	/**
	* This method is called when the RTB message "Warning [warning] [message]" occurs. This message when RTB has notify the robot on a problem
	* @param warning The type of the warning
	* @param message The text of the warning
	*/
	void cobra_ClientBasicState::receiveRTBMessageWarning(warning_type warning,const string& message) throw (StrategyException, bad_exception) {}

	/**
	* This method is called when the RTB message "Dead" occurs. This message is sent when the robot pegs out.
	*/
	void cobra_ClientBasicState::receiveRTBMessageDead() throw (StrategyException, bad_exception) {}

}
