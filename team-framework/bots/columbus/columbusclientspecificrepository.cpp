/************************************************************************
    $Id: columbusclientspecificrepository.cpp,v 1.4 2005/08/02 12:18:42 krolfy Exp $
    
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

    $Log: columbusclientspecificrepository.cpp,v $
    Revision 1.4  2005/08/02 12:18:42  krolfy
    Work on columbus clientside started

    Revision 1.3  2005/01/06 17:59:26  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#include <common.h>

#include "columbusclientspecificrepository.h"
#include "../../rtbglobal/masterresourcecontrol.h"

namespace Columbus {
	using std::string;
	using RTBGlobal::MasterResourceControl;
	using Exceptions::RTBException;
	
void  ColumbusClientSpecificRepository::reset() throw(StrategyException, bad_exception) {
}

Logger* ColumbusClientSpecificRepository::getLogger() const throw() {
	return _logger.get();
}


ColumbusClientSpecificRepository::~ColumbusClientSpecificRepository() throw() {};
	
ColumbusClientSpecificRepository::ColumbusClientSpecificRepository() throw(StrategyException, bad_exception): _logger(0) {
	try {
		_logger=MasterResourceControl::Instance()->createLogger("ColumbusClient");
	}
	catch (RTBException& ex) {
		throw StrategyException("Could not obtain logger for the Columbus client states: "+ex.getMessage());
	}	
}

void ColumbusClientSpecificRepository::setCurrentEnemyPosition(const Position& pos) throw()
{
	_currentEnemy=pos;
}

void ColumbusClientSpecificRepository::setMyEnemyPosition(const Position& pos) throw()
{
	_myEnemy=pos;
}

const Position& ColumbusClientSpecificRepository::currentEnemyPosition() const throw()
{
	return _currentEnemy;
}

const Position& ColumbusClientSpecificRepository::myEnemyPosition() const throw()
{
	return _myEnemy;
}

RobotInfo& ColumbusClientSpecificRepository::getRobotInfo() throw()
{
	return _robot;
}

}
