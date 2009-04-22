/************************************************************************
    $Id: clientstate.h,v 1.2 2005/01/06 17:59:29 jonico Exp $
    
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

    $Log: clientstate.h,v $
    Revision 1.2  2005/01/06 17:59:29  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef CLIENTSTATE_H
#define CLIENTSTATE_H

#include "state.h"
#include "../stdnamespace.h"
#include "../gamecontrol/gamecontrolnamespace.h"
#include "../exceptions/strategyexception.h"

/**
* Namespace Strategies 
*/
namespace Strategies {
	using std::string;
	using std::bad_exception;
	using GameControl::ClientCoordinator;
	using Exceptions::StrategyException;

/**
* Class ClientState
*
* Note: Derived classes must have a reference to their corresponding coordinator object, the GameOptionsRepository object and to their client specific repositories in order to work properly.
* TODO: Implement protected sendRTBMessage... methods with a reference to the coordinator object and the corresponding parameters that depends on the concrete message type that should be generated.
*/
class ClientState : public State {
public:
	/**
	* Constructor
	*/
	ClientState(ClientCoordinator*) throw();
	/**
	* Destructor, does nothing
	*/
	virtual  ~ClientState () throw();
	/**
	 * Processes the server tips
	 * @param tip The server tip in string shape
	 */
	virtual void  reactOnServerTip (const string& tip) throw (StrategyException,bad_exception) = 0;
	/**
	 * This method is called when all rtb messages in this round were received. After this method, the first server tips will arrive. 
	 */
	virtual void  roundFinished () throw (StrategyException,bad_exception) = 0;
	/**
	 * This method is called when all server tips in this round were received. After this method, the state is to send its last messages. 
	 */
	virtual void  sendRTBMessages () throw (StrategyException,bad_exception) = 0;
protected:
	void setCurrentClientState (unsigned char clientStateIndex) throw(StrategyException,bad_exception);


	/**
	 * Requests sending "Rotate [what] [omega]" to RTB, which initiates continuous rotation.
	 * @param what What to rotate; 1 = Robot, 2 = Cannon, 4 = Radar. A sum of these values means that multiple object shall be rotated at the same time
	 * @param omega The angular velocity in rad/s
	 */
	void sendRTBMessageRotate(int what,double omega) throw(bad_exception);	// >> see state.h, line 50

	/**
	 * Requests sending "RotateTo [what] [omega] [endAngle]" to RTB, which initiates limited rotation.
	 * @param what What to rotate; 1 = Robot, 2 = Cannon, 4 = Radar. A sum of these values means that multiple object shall be rotated at the same time
	 * @param omega The angular velocity in rad/s
	 * @param endAngle The angle that shall be rotated to in rad
	 */
	void sendRTBMessageRotateTo(int what,double omega,double endAngle) throw(bad_exception);

	/**
	 * Requests sending "RotateAmount [what] [omega] [angleDiff]" to RTB, which initiates limited rotation, too.
	 * @param what What to rotate; 2 = Cannon, 4 = Radar. A sum of these values means that multiple object shall be rotated at the same time
	 * @param omega The angular velocity in rad/s
	 * @param angleDiff The angle that shall be rotated in rad, relative to the robot
	 */
	void sendRTBMessageRotateAmount(int what,double omega,double angleDiff) throw(bad_exception);

	/**
	 * Requests sending "Sweep [what] [omega] [leftAngle] [rightAngle]" to RTB, which initiates continous sweeping.
	 * @param what What to sweep; 2 = Cannon, 4 = Radar. A sum of these values means that multiple object shall be rotated at the same time
	 * @param omega The angular velocity in rad/s
	 * @param leftAngle The left sweeping limit angle in rad, relative to the robot
	 * @param leftAngle The right sweeping limit angle in rad, relative to the robot
	 */
	void sendRTBMessageSweep(int what,double omega,double leftAngle,double rightAngle) throw(bad_exception);

	/**
	 * Requests sending "Accelerate [accel]" to RTB, which sets a continuous acceleration.
	 * @param accel The new acceleration to be set in units/s^2
	 */
	void sendRTBMessageAccelerate(double accel) throw(bad_exception);

	/**
	 * Requests sending "Brake [portion]" to RTB, which sets continuous braking.
	 * @param brake The new brake deceleration to be set, the maximum value is 1.0, which means to brake with full slide friction
	 */
	void sendRTBMessageBrake(double portion) throw(bad_exception);

	/**
	 * Requests sending "Shoot [energy]" to RTB, which sends out a shot.
	 * @param energy The energy of the new shot
	 */
	void sendRTBMessageShoot(double energy) throw(bad_exception);

	/**
	 * Requests sending "Print [message]" to RTB, which prints a message on the RTB message window.
	 * @param message The text that shall be print
	 */
	void sendRTBMessagePrint(const string& message) throw(bad_exception);

	/**
	 * Requests sending "Debug [message]" to RTB, which prints a message on the RTB message window if in debug mode.
	 * @param message The text that shall be print
	 */
	void sendRTBMessageDebug(const string& message) throw(bad_exception);

	/**
	 * Requests sending "DebugLine [angle1] [dist1] [angle2] [dist]" to RTB, which draws a line onto the RTB arena window. This method has effects only if in debug level 5.
	 * @param angle1 and ...
	 * @param dist1 ... The beginning point of the line in polar coordinates, relative to the robot
	 * @param angle2 and ...
	 * @param dist2 ... The ending point of the line in polar coordinates, relative to the robot
	 */
	void sendRTBMessageDebugLine(double angle1,double dist1,double angle2,double dist2) throw(bad_exception);

	/**
	 * Requests sending "DebugCircle [centerAngle] [centerDist] [radius]" to RTB, which draws a circle onto the RTB arena window. This method has effects only if in debug level 5.
	 * @param centerAngle and ...
	 * @param centerDist ... The center of the circle in polar coordinates, relative to the robot
	 * @param radius The radius of the dircle
	 */
	void sendRTBMessageDebugCircle(double centerAngle,double centerDist,double radius) throw(bad_exception);

private:
	ClientCoordinator *_clientCoordinator;
};

}
#endif //CLIENTSTATE_H

