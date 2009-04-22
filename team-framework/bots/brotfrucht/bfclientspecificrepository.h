/************************************************************************
    $Id: bfclientspecificrepository.h,v 1.2 2005/01/06 17:59:26 jonico Exp $
    
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

    $Log: bfclientspecificrepository.h,v $
    Revision 1.2  2005/01/06 17:59:26  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef BFCLIENTSPECIFICREPOSITORY
#define BFCLIENTSPECIFICREPOSITORY

#include "../../strategies/clientspecificrepository.h"
#include <time.h>
#include "../../exceptions/strategyexception.h"
#include "../../log/logger.h"
#include <memory>

/**
* Namespace Brotfrucht
*/
namespace Brotfrucht {
 
 	using Exceptions::StrategyException;
	using std::auto_ptr;
	using Log::Logger;
	using std::bad_exception;
	using Strategies::ClientSpecificRepository;
	
	/**
	* Class BFClientSpecificRepository
	*
	* This repository will conbtain (at first) the position of the robot and the number of collisions it had.
	* Furthermore, it contains a timer when it had to shoot again
	*/
	class BFClientSpecificRepository: public Strategies::ClientSpecificRepository {
		public:
			/**
			* Method to reset the repositories (clears number of collisions, (re)starts timer)
			*/
			void reset() throw(StrategyException, bad_exception);
			
			/**
			 * Method to set the shoting interval in nanoseconds
			 */
			void setShootingInterval(double i) throw();
			
			/**
			 * Method to get the shooting intervall in nanoseconds
			 */
			double getShootingInterval()const throw();
			
			/**
			* Increment collision count
			*/
			void oneMoreCollision() throw();
			
			/**
			 * Get number of collisons the roboter has had since the last reset
			 */ 
			int getCollisionCount() const throw();			
		
			/**
			* (Re)Start the timer with the current systemtime
			* Throws exception if starting the timer does not work
			*/
			void startTimer() throw(StrategyException);
			
			/**
			 * Get nanoseconds passed since the timer was started
			 * Throws exception if getting the current time failed
			 */ 
			double getTimePassed() const throw(StrategyException);
			
			/**
			 * Get the logger for the client brotfrucht strategy states
			 */
			 Logger* getLogger() const throw();
			
			
			/**
			 * Destructor (does nothing)
			*/
			~BFClientSpecificRepository() throw();
			
			/**
			 * Constructor
			*/
			BFClientSpecificRepository() throw(StrategyException, bad_exception);
		private:			 
			 /**
			  * number of hits of the robot
			  */
			  int _hits;

			  /**
			   * start of the timer
			   */
			  timespec _timerstart;
			  
			  /**
			   * The logger for the client states of brotfrucht
			  */
			  auto_ptr<Logger> _logger;
			  
			  /**
			   * The shooting intervall in nanoseconds
			   */
			  double _shootingInterval;
	};
}


#endif
