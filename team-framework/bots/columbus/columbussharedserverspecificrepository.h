/************************************************************************
    $Id: columbussharedserverspecificrepository.h,v 1.4 2005/07/29 10:59:51 krolfy Exp $
    
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

    $Log: columbussharedserverspecificrepository.h,v $
    Revision 1.4  2005/07/29 10:59:51  krolfy
    Columbus server side ready but untested

    Revision 1.3  2005/07/28 08:19:10  krolfy
    Most of the server side ready

    Revision 1.2  2005/01/06 17:59:26  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef COLUMBUSSHAREDSERVERSPECIFICREPOSITORY
#define COLUMBUSSHAREDSERVERSPECIFICREPOSITORY

#include "../../strategies/sharedserverspecificrepository.h"
#include "../../exceptions/strategyexception.h"
#include "../../log/logger.h"
#include "../general/maps/potentialmap.h"
#include "../general/teaminfo.h"
#include "../general/enemiesinfo.h"
#include <memory>
#include <vector>
#include <string>
#include <stdexcept>

/**
* Namespace Columbus
*/
namespace Columbus {
	using std::auto_ptr;
	using std::vector;
	using std::string;
	using std::out_of_range;
	using std::bad_exception;
	using Log::Logger;
	using Exceptions::StrategyException;
	using Strategies::SharedServerSpecificRepository;
	using Maps::PotentialMap;
	using General::Position;
	using General::TeamInfo;
	using General::EnemiesInfo;
	
	/**
	 * Class ColumbusSharedServerSpecificRepository
	 * At first, BrotFruchtSharedServerSpecificRepository will be a Singleton object, that will only contain one single element: a vector with the names of all participating friendly robots
	 */
class ColumbusSharedServerSpecificRepository: public Strategies::SharedServerSpecificRepository
{
public:
	/**
	 * Method to obtain the one and only object of this class
	 */
	static ColumbusSharedServerSpecificRepository* Instance() throw (bad_exception);
	/**
	 * Method to reset the repositories (clears all names out of the vector
	 */
	void reset() throw (bad_exception);
	
	/**
	 * Destructor (does nothing)
	 */
	~ColumbusSharedServerSpecificRepository() throw ();
	
	/**
	 * @return the shared logger for all server columbus strategy states
	 */
	Logger* getLogger() const throw();
	
	/**
	 * @return the map object
	 */
	PotentialMap& getMap() throw();
	
	/**
	 * @return the team information object
	 */
	TeamInfo& getTeam() throw();
	
	/**
	 * @return the enemy information object
	 */
	EnemiesInfo& getEnemies() throw();

	/**
	 * @return the position of the current enemy that shall be attacked by the whole team
	 */
	const Position& currentEnemyPosition() const throw();
	
	/**
	 * Start/Stop an attack;
	 * @param b true/false = start/stop
	 */
	void attack(bool b) throw();

	/**
	 * @return whether the team attacks or not
	 */
	bool isAttack() throw();

	/**
	 * Define who is the current enemy that shall be attacked by the whole team
	 * @param pos pointer to the position of the robot
	 */
	void setCurrentEnemyPosition(const Position& pos) throw();

private:
	/**
	 * Only instance of this class
	 */
	static auto_ptr<ColumbusSharedServerSpecificRepository> _instance;
	
	auto_ptr<Logger> _logger; 
	PotentialMap _map;
	TeamInfo _team;
	EnemiesInfo _enemies;
	Position _currentEnemy;
	bool _attack;
	
	/**
	 * Constructor
	 */
	ColumbusSharedServerSpecificRepository() throw (StrategyException, bad_exception);
};

}

#endif
