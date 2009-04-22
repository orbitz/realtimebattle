/************************************************************************
    $Id: coordinator.h,v 1.2 2005/01/06 17:59:27 jonico Exp $
    
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

    $Log: coordinator.h,v $
    Revision 1.2  2005/01/06 17:59:27  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef COORDINATOR_H
#define COORDINATOR_H

#include <string>
#include "../exceptions/strategyexception.h"
#include "../parser/object_type.h"
#include "../parser/warning_type.h"

/**
* Namespace GameControl
*/
namespace GameControl {
	using std::string;
	using std::bad_exception;
	using Exceptions::StrategyException;
	using Parser::object_type;
	using Parser::warning_type;

	/**
	* Class Coordinator
	*/
	class Coordinator {
	public:
		/**
		 * Destructor, does nothing
		 */
		virtual ~Coordinator() throw() {};
		
		
		/**	
		* Calls reset on all state objects and sets the current state to the beginning state. 
		*/
		virtual void resetGame () throw (StrategyException,bad_exception) = 0;
		
		
		/**
		* This method has to be invoked after all messages sent by the rtb server in this round were processed by the receiveRTBMessage..... methods.
		*/
		virtual void processReceivedRTBMessages () throw (StrategyException,bad_exception) = 0;
		virtual string  getAssociatedRobotName () throw (bad_exception) = 0;

		/**
		* This method is called when the RTB message "Radar [dist] [type] [angle]" occurs. This message occurs each turn and gives information about the closest object in the direction the radar points to.
		* @param dist The distance of the observed object
		* @param type The type of the observed object
		* @param angle The angle the observed object is seen at in rad, relative to the robot front
		*/
		virtual void receiveRTBMessageRadar(double dist,object_type type,double angle) throw (StrategyException, bad_exception) = 0;

		/**
		* This method is called when the RTB message "Info [time] [speed] [cannonAngle]" occurs. This message is sent each turn and gives information about the robot itself.
		* @param time The time that has elapsed since the game had started
		* @param speed The velocity of the robot
		* @param cannonAngle The angle the cannon points to
		*/
		virtual void receiveRTBMessageInfo(double time,double speed,double cannonAngle) throw (StrategyException, bad_exception) = 0;

		/**
		* This method is called when the RTB message "Coordinates [x] [y] [angle]" occurs. Generally, this message is sent each turn. If the game option SEND_ROBOT_COORDINATES is sent to 2, absolute coordinates are sent. If this value is 1, the coordinates are given relative to the starting point. On the case 0, this message is not sent.
		* @param x And ...
		* @param y ... are the sent coordinates
		* @param angle is the angle the robot front points to
		*/
		virtual void receiveRTBMessageCoordinates(double x,double y,double angle) throw (StrategyException, bad_exception) = 0;

		/**
		* This method is called when the RTB message "RobotInfo [energy] [isTeamMate]" occurs. This message is always sent after another robot has been detected by the radar. It gives some information about this might-be foe.
		* @param energy The unexact energy level of the detected robot
		* @param isTeamMate Defines whether the robot is a team mate (team mode is not yet implemented in RTB)
		*/
		virtual void receiveRTBMessageRobotInfo(double energy,bool isTeamMate) throw (StrategyException, bad_exception) = 0;

		/**
		* This method is called when the RTB message "RotationReached [what]" occurs. In our case, this message is sent, when a rotation initiated by "RotateTo" or "RotateAmount" has finished or the sweeping has changed direction.
		* param what What this message bears on; 1 = Robot, 2 = Cannon, 4 = Radar
		*/
		virtual void receiveRTBMessageRotationReached(int what) throw (StrategyException, bad_exception) = 0;	// >> Maybe we should introduce
											//    another enum for this TODO

		/**
		* This method is called when the RTB message "Energy [energy]" occurs. This message is sent at the end of each turn and specifies the health state of the robot.
		* @param energy The unexact energy level
		*/
		virtual void receiveRTBMessageEnergy(double energy) throw (StrategyException, bad_exception) = 0;

		/**
		* This method is called when the RTB message "RobotsLeft [num]" occurs. This message is sent at the beginning of the game and after a robot has been killed.
		* @param num The number of remaining robots
		*/
		virtual void receiveRTBMessageRobotsLeft(int num) throw () = 0;

		/**
		* This method is called when the RTB message "Collision [type] [angle]" occurs. This message is sent whan the robot has been hit by or hits another object,
		* @param type The type of the hit object
		* @param angle The angle the collision happened at in rad, relative to the robot front
		*/
		virtual void receiveRTBMessageCollision(object_type type,double angle) throw (StrategyException, bad_exception) = 0;

		/**
		* This method is called when the RTB message "Warning [warning] [message]" occurs. This message when RTB has notify the robot on a problem
		* @param warning The type of the warning
		* @param message The text of the warning
		*/
		virtual void receiveRTBMessageWarning(warning_type warning,const string& message) throw (StrategyException, bad_exception) = 0;

		/**
		* This method is called when the RTB message "Dead" occurs. This message is sent when the robot pegs out.
		*/
		virtual void receiveRTBMessageDead() throw (StrategyException, bad_exception) = 0;
	protected:
	};

}
#endif //COORDINATOR_H

