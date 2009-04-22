/************************************************************************
    $Id: bfsharedserverspecificrepository.h,v 1.2 2005/01/06 17:59:26 jonico Exp $
    
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

    $Log: bfsharedserverspecificrepository.h,v $
    Revision 1.2  2005/01/06 17:59:26  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef BFSHAREDSERVERSPECIFICREPOSITORY
#define BFSHAREDSERVERSPECIFICREPOSITORY

#include "../../strategies/sharedserverspecificrepository.h"
#include "../../exceptions/strategyexception.h"
#include "../../log/logger.h"
#include <memory>
#include <vector>
#include <string>
#include <stdexcept>

/**
* Namespace Brotfrucht
*/
namespace Brotfrucht {
	using std::auto_ptr;
	using std::vector;
	using std::string;
	using std::out_of_range;
	using std::bad_exception;
	using Log::Logger;
	using Exceptions::StrategyException;
	using Strategies::SharedServerSpecificRepository;
	
	/**
	* Class BFSharedServerSpecificRepository
	* At first, BrotFruchtSharedServerSpecificRepository will be a Singleton object, that will only contain one single element: a vector with the names of all participating friendly robots
	*/
	class BFSharedServerSpecificRepository: public Strategies::SharedServerSpecificRepository {
		public:
			/**
			* Method to obtain the one and only object of this class
			*/
			static BFSharedServerSpecificRepository* Instance() throw (bad_exception);
			/**
			* Method to reset the repositories (clears all names out of the vector
			*/
			void reset() throw (bad_exception);
		
			/**
			* Appends the own robot name to the vector
			*/
			void appendRobotName(const string& robotName) throw (bad_exception);
		
			/**
			* Gets the name of the roboter with the given index
			*/
			const string& getRobotName (int i) const throw (bad_exception, out_of_range);
		
			/**
			* Get the number of (friendly) robots
			*/
			int getRobotNumber() const throw ();
			
			/**
			 * Destructor (does nothing)
			*/
			~BFSharedServerSpecificRepository() throw ();
			
			/**
			 * Get the shared logger for all server brotfrucht strategy states
			 */
			 Logger* getLogger() const throw();
			 
		private:
			/**
			 * Only instance of this class
			 */
			 static auto_ptr<BFSharedServerSpecificRepository> _instance;
			 
			 /**
			  * vector with the names of the robots
			  */
			  vector <string> _robotNames;
	
			  /**
			  * for logging messages  
			  */
			  auto_ptr<Logger> _logger; 
			  
			  /**
			   * Constructor
			   */
			  BFSharedServerSpecificRepository()throw (StrategyException, bad_exception);
			  
	};
}

#endif
