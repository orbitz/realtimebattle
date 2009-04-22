/************************************************************************
    $Id: enemiesinfo.h,v 1.4 2005/03/22 15:13:04 krolfy Exp $

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

    $Log: enemiesinfo.h,v $
    Revision 1.4  2005/03/22 15:13:04  krolfy
    Changed return type of getEnemyPosition()

    Revision 1.3  2005/03/22 10:36:29  krolfy
    Changed all int to unsigned int; some handling in case of empty lists

    Revision 1.2  2005/03/07 17:48:01  krolfy
    Introduced ring lists

    Revision 1.1  2005/03/01 14:41:46  krolfy
    First compilable version of EnemiesInfo




**************************************************************************/

#ifndef ENEMIESINFO_H
#define ENEMIESINFO_H

#include "teaminfo.h"
#include <list>

namespace General
{

/**
 *	Stores information about the whole hostile force
 */
class EnemiesInfo
{
public:
	EnemiesInfo() throw(bad_exception);
	~EnemiesInfo() throw(){}

	/**
	 *	Prepares the object for the next round. It is no real need that this is called,
	 *	but otherwise it may contain invalid data from the previous round.
	 */
	void resetForNextRound() throw();

	/**
	 *	If a robot was found and identified as an enemy, use this method to report its position
	 *	@param pos the enemies's position on the map
	 */
	void addEnemyPosition(const Position& pos) throw(bad_exception);

	/**
	 *	Request the position of an enemy. Since they will have moved after being observed,
	 *	this information may be more precise if it was observed very recently.
	 *	There will ad most as much enemy positions be stored as there are enemies alive.
	 *	@param topicality the topicality of the requested enemy position,
	 *		zero meaning the most recent one. This is an indexed list access.
	 *	@return the position of the requested enemy, zero if the enemy list is empty
	 */
	const Position* getEnemyPosition(unsigned int topicality) const throw(bad_exception);

	/**
	 *	Fast variant of getEnemyPosition(0)
	 */
	const Position& getRecentEnemyPosition() const throw(bad_exception);

	/**
	 *	Use this method to keep the number of enemies actually alive up to date.
	 */
	void setNumberOfEnemies(unsigned int num);

	/**
	 *	@return the number of enemies actually alive
	 */
	unsigned int getNumberOfEnemies() const;

	/**
	 *	@return the number of stored enemy positions
	 */
	unsigned int getNumberOfStoredEnemies() const;

private:
	unsigned int _numEnemies,_numStoredEnemies;
	std::list<Position> _enemies;
	std::list<Position>::iterator _begin;
};

}

#endif
