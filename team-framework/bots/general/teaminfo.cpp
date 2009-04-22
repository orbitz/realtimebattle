/************************************************************************
    $Id: teaminfo.cpp,v 1.15 2005/10/05 21:11:15 krolfy Exp $
    
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

    $Log: teaminfo.cpp,v $
    Revision 1.15  2005/10/05 21:11:15  krolfy
    .

    Revision 1.14  2005/10/05 16:33:14  krolfy
    Fixes in cobra according to new TeamInfo and RobotInfo

    Revision 1.13  2005/07/28 08:19:10  krolfy
    Most of the server side ready

    Revision 1.12  2005/03/22 10:36:29  krolfy
    Changed all int to unsigned int; some handling in case of empty lists

    Revision 1.11  2005/03/07 18:08:36  jonico
    Replaced --id with id-1 and other cosmetic things:
    Do not use the -- operator if there is no reason for it.

    Revision 1.10  2005/03/07 17:55:21  jonico
    Little change in signature of registerRobotForRound

    Revision 1.9  2005/03/07 17:48:01  krolfy
    Introduced ring lists

    Revision 1.6  2005/03/01 14:41:46  krolfy
    First compilable version of EnemiesInfo

    Revision 1.5  2005/02/28 09:43:55  krolfy
    Cosmetics

    Revision 1.4  2005/02/22 21:03:14  jonico
    Now teaminfo classes work



**************************************************************************/

#include <common.h>
#include "teaminfo.h"
#include <cmath>

namespace General {

	Position::Position(double X,double Y) throw() :x(X),y(Y)  {}

	RobotInfo::RobotInfo(const string& Name) throw(bad_exception): name(Name),isDead(false),pos(0.0,0.0),ID(0),dirRobot(0),dirCannon(0),dirRadar(0),speed(0)  {}

	RobotInfo::RobotInfo() throw() :pos(0.0,0.0),isDead(true),ID(0),dirRobot(0),dirCannon(0),dirRadar(0),speed(0)  {}

	Position relativeToAbsolute(const Position& self,double dist,double angle)
	{
		return Position(self.x+dist*cos(angle),self.y+dist*sin(angle));
	}

	TeamInfo::TeamInfo() throw(bad_exception):_numTeam(0),_capacity(0),_robots(0)  {}

	unsigned int TeamInfo::registerRobotForRound(const RobotInfo& robot) throw(bad_exception)
	{
		if(_numTeam<_capacity)
			_robots[_numTeam]=&robot;
		else _robots.push_back(&robot);
		return ++_numTeam;
	}

	void TeamInfo::resetForNextRound() throw()
	{
		_numTeam=0;
	}

	unsigned int TeamInfo::isFriend(const Position& pos,double tolerance) const throw()
	{
		tolerance*=tolerance;
		for(unsigned int i=0;i<_numTeam;++i)
		{
			if(_robots[i]->isDead)	continue;
			double x=_robots[i]->pos.x-pos.x;
			double y=_robots[i]->pos.y-pos.y;
			if(x*x+y*y<tolerance)	return i+1;
		}
		return 0;
	}

	const RobotInfo* TeamInfo::getFriendInfo(unsigned int id) const throw(out_of_range,bad_exception)
	{
		return _robots.at(id-1);
	}

	unsigned int TeamInfo::getTeamSize() const throw()
	{
		return _numTeam;
	}
}
