/************************************************************************
    $Id: generalnamespace.h,v 1.2 2005/08/02 12:18:42 krolfy Exp $
    
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

    $Log: generalnamespace.h,v $
    Revision 1.2  2005/08/02 12:18:42  krolfy
    Work on columbus clientside started

    Revision 1.1  2005/07/29 10:59:51  krolfy
    Columbus server side ready but untested



**************************************************************************/

#ifndef GENERALNAMESPACE_H
#define GENERALNAMESPACE_H

/**
* Namespace Columbus
*/
namespace General {
	class TeamInfo;
	class EnemiesInfo;
	struct Position;
	struct RobotInfo;
	unsigned int registerRobotForRound(const RobotInfo& robot);
}

#endif
