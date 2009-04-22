/************************************************************************
    $Id: teaminfo.h,v 1.13 2005/10/05 16:33:14 krolfy Exp $

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

    $Log: teaminfo.h,v $
    Revision 1.13  2005/10/05 16:33:14  krolfy
    Fixes in cobra according to new TeamInfo and RobotInfo

    Revision 1.12  2005/08/05 16:38:04  krolfy
    First testing version of Columbus ready

    Revision 1.11  2005/08/05 14:29:19  krolfy
    Derived ColumbusClientStates

    Revision 1.10  2005/07/28 08:19:19  krolfy
    Most of the server side ready

    Revision 1.9  2005/03/22 10:36:29  krolfy
    Changed all int to unsigned int; some handling in case of empty lists

    Revision 1.8  2005/03/07 17:55:21  jonico
    Little change in signature of registerRobotForRound

    Revision 1.7  2005/03/07 17:48:01  krolfy
    Introduced ring lists

    Revision 1.4  2005/03/01 14:41:46  krolfy
    First compilable version of EnemiesInfo

    Revision 1.3  2005/02/22 21:00:58  jonico
    Tried to integrate krolfies changes

    Revision 1.2  2005/02/22 13:59:56  krolfy
    Added comments for doc generation an file headers



**************************************************************************/

#ifndef TEAMINFO_H
#define TEAMINFO_H

#include <string>
#include <stdexcept>
#include <vector>

namespace General
{
	using std::vector;
	using std::string;
	using std::bad_exception;
	using std::out_of_range;

/**
 *	A pair of cartesian coordinates
 */
struct Position
{
	Position(double X=0.0,double Y=0.0) throw();
	double x,y;
};

/**
 *	Some information about a single team mate robot
 *	Keep all directions angles relative to the globat abscissa
 */
struct RobotInfo
{
	RobotInfo(const string& Name) throw(bad_exception);
	RobotInfo() throw();
	string name;
	Position pos;
	double speed,dirRobot,dirRadar,dirCannon;
	bool isDead;
	int ID;
};

/**
 *	This is a simple global function for conversion of relative polar coordinates
 *	to absolute cartesian coordinates
 *	@param self origin if the polar coordinates
 *	@param dist radius
 *	@param angle polar angle relative to the global abscissa
 *	@return absolute cartesian coordinates
 */
Position relativeToAbsolute(const Position& self,double dist,double angle);

/**
 *	Stores information about the whole team
 */
class TeamInfo
{
public:
	TeamInfo() throw(bad_exception);
	~TeamInfo() throw(){}

	/**
	 *	In each round each robot has to register at this object for new.
	 *	@param robot contains information about the robot
	 *	@return the robot's id in this round
	 */
	unsigned int registerRobotForRound(const RobotInfo& robot) throw(bad_exception);

	/**
	 *	Prepares the object for the next round
	 */
	void resetForNextRound() throw();

	/**
	 *`	If a robot detects another robot, it can use this method to identify it as a teammate or enemy 
	 *	@param pos the position of the observed robot; see also function General::relativeToAbsolute()
	 *	@param tolerance Since this method identifies the robots using their coordinates, you need
	 *			some tolerance range, however, you should hardly need to touch this.
	 *	@return Zero if it is an enemy, otherwise the id of the observed team mate.
	 */
	unsigned int isFriend(const Position& pos,double tolerance=1.0) const throw();

	/**
	 *	@param id the id of the requested team mate
	 *	@return information about the requested team mate
	 */
	const RobotInfo* getFriendInfo(unsigned int id) const throw(out_of_range,bad_exception);

	/**
	 *	@return the number of team mates that have registered yet
	 */
	unsigned int getTeamSize() const throw();

private:
	unsigned int _numTeam,_capacity;
	vector<const RobotInfo*> _robots;
};

}

#endif
