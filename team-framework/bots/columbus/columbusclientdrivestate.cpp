/************************************************************************
    $Id: columbusclientdrivestate.cpp,v 1.6 2005/08/23 15:37:05 krolfy Exp $
    
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

    $Log: columbusclientdrivestate.cpp,v $
    Revision 1.6  2005/08/23 15:37:05  krolfy
    *** empty log message ***

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


#include "columbusclientdrivestate.h"
#include "columbusclientspecificrepository.h"
#include "../../gamecontrol/gameoptionsrepository.h"

namespace Columbus {

ColumbusClientDriveState::ColumbusClientDriveState(ClientCoordinator* i,ColumbusClientSpecificRepository* columbuscsp,const GameOptionsRepository* go) throw (StrategyException,bad_exception):ColumbusClientBaseState(i,columbuscsp,go),_csp(columbuscsp),_go(go) {}

ColumbusClientDriveState::~ColumbusClientDriveState () throw() {}

void  ColumbusClientDriveState::sendRTBMessages () throw (StrategyException,bad_exception)
{
	static double omega =	_go->ROBOT_CANNON_MAX_ROTATE>_go->ROBOT_RADAR_MAX_ROTATE?
				_go->ROBOT_CANNON_MAX_ROTATE:_go->ROBOT_RADAR_MAX_ROTATE;
	sendRTBMessageRotate(6,omega);
}
	
}

