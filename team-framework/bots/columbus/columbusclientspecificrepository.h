/************************************************************************
    $Id: columbusclientspecificrepository.h,v 1.4 2005/08/02 12:18:42 krolfy Exp $
    
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

    $Log: columbusclientspecificrepository.h,v $
    Revision 1.4  2005/08/02 12:18:42  krolfy
    Work on columbus clientside started

    Revision 1.3  2005/07/29 12:33:05  krolfy
    Added isFreeVisionLine method to map classes

    Revision 1.2  2005/01/06 17:59:26  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef COLUMBUSCLIENTSPECIFICREPOSITORY
#define COLUMBUSCLIENTSPECIFICREPOSITORY

#include "../../strategies/clientspecificrepository.h"
#include "../../exceptions/strategyexception.h"
#include "../../log/logger.h"
#include "../general/teaminfo.h"
#include <memory>
#include <vector>

/**
* Namespace Columbus
*/
namespace Columbus {
 
 	using Exceptions::StrategyException;
	using std::auto_ptr;
	using Log::Logger;
	using std::bad_exception;
	using Strategies::ClientSpecificRepository;
	using General::Position;
	using General::RobotInfo;
	
	/**
	 * Class ColumbusClientSpecificRepository
	 */
class ColumbusClientSpecificRepository: public Strategies::ClientSpecificRepository
{
public:
	ColumbusClientSpecificRepository() throw(StrategyException, bad_exception);
	~ColumbusClientSpecificRepository() throw ();

	/**
	 * Method to reset the repositories
	 */
	void reset() throw(StrategyException, bad_exception);

	Logger* getLogger() const throw();

	/**
	 * Marks the enemy the whole team is going to attack
	 * @param the position of the enemy
	 */
	void setCurrentEnemyPosition(const Position& pos) throw();
	
	/**
	 * Marks the enemy the robot will actually attack
	 * @param the position of the enemy
	 */
	void setMyEnemyPosition(const Position& pos) throw();
	
	/**
	 * @return the enemy the whole team is going to attack
	 */
	const Position& currentEnemyPosition() const throw();
	
	/**
	 * @return the enemy the robot will actually attack
	 */
	const Position& myEnemyPosition() const throw();

	/**
	 * @return information concerning the robot itself
	 */
	RobotInfo& getRobotInfo() throw();
	
private:	
	auto_ptr<Logger> _logger;
	Position _myEnemy,_currentEnemy;
	RobotInfo _robot;
};

}


#endif
