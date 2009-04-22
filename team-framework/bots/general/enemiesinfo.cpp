/************************************************************************
    $Id: enemiesinfo.cpp,v 1.7 2005/05/25 11:00:47 krolfy Exp $
    
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

    $Log: enemiesinfo.cpp,v $
    Revision 1.7  2005/05/25 11:00:47  krolfy
    Since vector<bool> is specialized, BoolMap is not necessary

    Revision 1.6  2005/03/31 14:27:19  krolfy
    little error corrected in line 78 (const_ forgotten)

    Revision 1.5  2005/03/22 15:13:04  krolfy
    Changed return type of getEnemyPosition()

    Revision 1.4  2005/03/22 10:36:29  krolfy
    Changed all int to unsigned int; some handling in case of empty lists

    Revision 1.3  2005/03/21 15:37:09  krolfy
    Memory leak fixed

    Revision 1.2  2005/03/07 17:48:00  krolfy
    Introduced ring lists

    Revision 1.1  2005/03/01 14:41:46  krolfy
    First compilable version of EnemiesInfo




**************************************************************************/

#include <common.h>
#include "enemiesinfo.h"
#include <cmath>

namespace General {

	EnemiesInfo::EnemiesInfo() throw(bad_exception):
		_numEnemies(0),_numStoredEnemies(0),_begin(_enemies.begin()) {}

	void EnemiesInfo::resetForNextRound() throw()
	{
		_numEnemies=0;
		_begin=_enemies.begin();
	}

	void EnemiesInfo::addEnemyPosition(const Position& pos) throw(bad_exception)
	{
		if(!_numEnemies) return;
		if(_numStoredEnemies<_numEnemies)
		{
			++_numStoredEnemies;
			_begin=_enemies.insert(_begin,pos);
		} else {
			if(_begin==_enemies.begin()) _begin=_enemies.end();
			*(--_begin)=pos;
		}
	}

	const Position* EnemiesInfo::getEnemyPosition(unsigned int topicality) const throw(bad_exception)
	{
		if(!(_numEnemies||_numStoredEnemies))
			return 0;
		topicality%=_numStoredEnemies;
		std::list<Position>::const_iterator I=_begin;
		for(unsigned int i=0;i!=topicality;++i)
			if(++I==_enemies.end())
				I=_enemies.begin();
		return &*I;
	}

	const Position& EnemiesInfo::getRecentEnemyPosition() const throw(bad_exception)
	{
		return *_begin;
	}

	void EnemiesInfo::setNumberOfEnemies(unsigned int num)
	{
		_numEnemies=num;
	}

	unsigned int EnemiesInfo::getNumberOfEnemies() const
	{
		return _numEnemies;
	}

	unsigned int EnemiesInfo::getNumberOfStoredEnemies() const
	{
		return _numStoredEnemies;
	}
}
