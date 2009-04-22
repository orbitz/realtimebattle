/************************************************************************
    $Id: clientstate.cpp,v 1.2 2005/01/06 17:59:29 jonico Exp $
    
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

    $Log: clientstate.cpp,v $
    Revision 1.2  2005/01/06 17:59:29  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#include <common.h>
#include <sstream>

#include "clientstate.h"
#include "../gamecontrol/clientcoordinator.h"

using Strategies::ClientState;
using std::ios;
using std::endl;
using Exceptions::StrategyException;

ClientState::ClientState(ClientCoordinator* c) throw():_clientCoordinator(c) {}
	
void ClientState::setCurrentClientState (unsigned char clientStateIndex) throw (StrategyException,bad_exception) {
	_clientCoordinator->setCurrentClientState(clientStateIndex);
}

ClientState::~ClientState() throw() {};

void ClientState::sendRTBMessageRotate(int what,double omega) throw (bad_exception)
	{_clientCoordinator->messageBuffer()<<"Rotate "<<what<<' '<<omega<<endl;}

void ClientState::sendRTBMessageRotateTo(int what,double omega,double endAngle) throw (bad_exception)
	{_clientCoordinator->messageBuffer()<<"RotateTo "<<what<<' '<<omega<<' '<<endAngle<<endl;}

void ClientState::sendRTBMessageRotateAmount(int what,double omega,double angleDiff) throw (bad_exception)
	{_clientCoordinator->messageBuffer()<<"RotateAmount "<<what<<' '<<omega<<' '<<angleDiff<<endl;}

void ClientState::sendRTBMessageSweep(int what,double omega,double leftAngle,double rightAngle) throw (bad_exception)
	{_clientCoordinator->messageBuffer()<<"Sweep "<<what<<' '<<omega<<' '<<leftAngle<<' '<<rightAngle<<endl;}

void ClientState::sendRTBMessageAccelerate(double accel) throw (bad_exception)
	{_clientCoordinator->messageBuffer()<<"Accelerate "<<accel<<endl;}

void ClientState::sendRTBMessageBrake(double portion) throw (bad_exception)
	{_clientCoordinator->messageBuffer()<<"Brake "<<portion<<endl;}

void ClientState::sendRTBMessageShoot(double energy) throw (bad_exception)
	{_clientCoordinator->messageBuffer()<<"Shoot "<<energy<<endl;}

void ClientState::sendRTBMessagePrint(const string& message) throw (bad_exception)
	{_clientCoordinator->messageBuffer()<<"Print "<<message<<endl;}

void ClientState::sendRTBMessageDebug(const string& message) throw (bad_exception)
	{_clientCoordinator->messageBuffer()<<"Debug "<<message<<endl;}

void ClientState::sendRTBMessageDebugLine(double angle1,double dist1,double angle2,double dist2) throw (bad_exception)
	{_clientCoordinator->messageBuffer()<<"DebugLine "<<angle1<<' '<<dist1<<' '<<angle2<<' '<<dist2<<endl;}

void ClientState::sendRTBMessageDebugCircle(double centerAngle,double centerDist,double radius) throw (bad_exception)
	{_clientCoordinator->messageBuffer()<<"DebugCircle "<<centerAngle<<' '<<centerDist<<' '<<radius<<endl;}

