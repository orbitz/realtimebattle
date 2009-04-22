/************************************************************************
    $Id: columbussharedserverspecificrepository.cpp,v 1.4 2005/07/29 10:59:51 krolfy Exp $
    
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

    $Log: columbussharedserverspecificrepository.cpp,v $
    Revision 1.4  2005/07/29 10:59:51  krolfy
    Columbus server side ready but untested

    Revision 1.3  2005/07/28 08:19:10  krolfy
    Most of the server side ready

    Revision 1.2  2005/01/06 17:59:26  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#include <common.h>

#include "columbussharedserverspecificrepository.h"
#include "../../rtbglobal/masterresourcecontrol.h"

namespace Columbus {
using RTBGlobal::MasterResourceControl;
using Exceptions::RTBException;

/**
 * Method to obtain the one and only object of this class
 */
ColumbusSharedServerSpecificRepository* ColumbusSharedServerSpecificRepository::Instance() throw (bad_exception) {
	ColumbusSharedServerSpecificRepository* colsssr (ColumbusSharedServerSpecificRepository::_instance.get());
	if (colsssr==0) {
		ColumbusSharedServerSpecificRepository::_instance.reset(colsssr=new ColumbusSharedServerSpecificRepository());
	}
	return colsssr;

}

ColumbusSharedServerSpecificRepository::~ColumbusSharedServerSpecificRepository() throw() {}

ColumbusSharedServerSpecificRepository::ColumbusSharedServerSpecificRepository() throw (StrategyException, bad_exception):_logger(0) {
	try {
		_logger=MasterResourceControl::Instance()->createLogger("ColumbusServer");
	}
	
	catch (RTBException& ex) {
		throw StrategyException("Could not obtain shared logger for the Columbus server states: "+ex.getMessage());
	}	
}


void ColumbusSharedServerSpecificRepository::reset() throw (bad_exception) {
	_map.reset();
	_team.resetForNextRound();
	_enemies.resetForNextRound();
	_attack=false;
}


PotentialMap&  ColumbusSharedServerSpecificRepository::getMap() throw() {
	return _map;
}

TeamInfo&  ColumbusSharedServerSpecificRepository::getTeam() throw() {
	return _team;
}

EnemiesInfo&  ColumbusSharedServerSpecificRepository::getEnemies() throw() {
	return _enemies;
}


Logger*  ColumbusSharedServerSpecificRepository::getLogger() const throw() {
	return _logger.get();
}

const Position& ColumbusSharedServerSpecificRepository::currentEnemyPosition() const throw()
{
	return _currentEnemy;
}
	
void ColumbusSharedServerSpecificRepository::setCurrentEnemyPosition(const Position& pos) throw()
{
	_currentEnemy=pos;
}

void ColumbusSharedServerSpecificRepository::attack(bool b) throw()
{
	_attack=b;
}

bool ColumbusSharedServerSpecificRepository::isAttack() throw()
{
	return _attack;
}

/**
 * Only instance of this class
 */
auto_ptr<ColumbusSharedServerSpecificRepository> ColumbusSharedServerSpecificRepository::_instance(0);

}
