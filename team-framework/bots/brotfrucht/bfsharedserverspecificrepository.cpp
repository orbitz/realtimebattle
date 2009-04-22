/************************************************************************
    $Id: bfsharedserverspecificrepository.cpp,v 1.2 2005/01/06 17:59:26 jonico Exp $
    
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

    $Log: bfsharedserverspecificrepository.cpp,v $
    Revision 1.2  2005/01/06 17:59:26  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#include <common.h>

#include "bfsharedserverspecificrepository.h"
#include "../../rtbglobal/masterresourcecontrol.h"

namespace Brotfrucht {
	using RTBGlobal::MasterResourceControl;
	using Exceptions::RTBException;
	
	/**
	* Method to obtain the one and only object of this class
	*/
	BFSharedServerSpecificRepository* BFSharedServerSpecificRepository::Instance() throw (bad_exception) {
		BFSharedServerSpecificRepository* bfsssp (BFSharedServerSpecificRepository::_instance.get());
		if (bfsssp==0) {
			BFSharedServerSpecificRepository::_instance.reset(bfsssp=new BFSharedServerSpecificRepository());
		}
		return bfsssp;
	
	}
	
	/**
	 * Destructor (does nothing)
	 */
	BFSharedServerSpecificRepository::~BFSharedServerSpecificRepository() throw() {}
	
	/**
	 * Constructor
	 */
	BFSharedServerSpecificRepository::BFSharedServerSpecificRepository() throw (StrategyException, bad_exception):_robotNames(),_logger(0) {
		try {
			_logger=MasterResourceControl::Instance()->createLogger("BrotfruchtServer");
		}
		
		catch (RTBException& ex) {
			throw StrategyException("Could not obtain shared logger for the Brotfrucht server states: "+ex.getMessage());
		}	
	}
	
	
	/**
	* Method to reset the repositories (clears all names out of the vector
	*/
	void BFSharedServerSpecificRepository::reset() throw (bad_exception) {
		_robotNames.clear();
	}

	/**
	* Appends the own robot name to the vector
	*/
	
	void BFSharedServerSpecificRepository::appendRobotName(const string& robotName) throw (bad_exception) {
		_robotNames.push_back(robotName);
	}

	/**
	* Gets the name of the roboter with the given index
	*/
	const string& BFSharedServerSpecificRepository::getRobotName (int i) const throw (bad_exception, out_of_range) {
		return _robotNames.at(i);
	}

	/**
	* Get the number of (friendly) robots
	*/
	int BFSharedServerSpecificRepository::getRobotNumber() const throw () {
		return _robotNames.size();
	}
	
	/**
	 * Get the shared logger for all server brotfrucht strategy states
	 */
	Logger*  BFSharedServerSpecificRepository::getLogger() const throw() {
		return _logger.get();
	}

	
	/**
	* Only instance of this class
	*/
	auto_ptr<BFSharedServerSpecificRepository> BFSharedServerSpecificRepository::_instance(0);

}
