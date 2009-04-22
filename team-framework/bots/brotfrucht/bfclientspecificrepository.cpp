/************************************************************************
    $Id: bfclientspecificrepository.cpp,v 1.2 2005/01/06 17:59:26 jonico Exp $
    
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

    $Log: bfclientspecificrepository.cpp,v $
    Revision 1.2  2005/01/06 17:59:26  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#include <common.h>

#include "bfclientspecificrepository.h"
#include <string>
#include <errno.h>
#include <string.h>
#include "../../rtbglobal/masterresourcecontrol.h"

namespace Brotfrucht {
			using std::string;
			using RTBGlobal::MasterResourceControl;
			using Exceptions::RTBException;
			
 			/**
			* Method to reset the repositories (clears number of collisions)
			*/
			void  BFClientSpecificRepository::reset() throw(StrategyException, bad_exception) {
				_hits=0;
				startTimer();
			}
			/**
			 * Method to set the shoting interval in nanoseconds
			 */
			void BFClientSpecificRepository::setShootingInterval(double i) throw() {
				_shootingInterval=i;
			}
			
			/**
			 * Method to get the shooting intervall in nanoseconds
			 */
			double BFClientSpecificRepository::getShootingInterval()const throw() {
				return _shootingInterval;
			}
			
			/**
			* Increment collision count
			*/
			void BFClientSpecificRepository::oneMoreCollision() throw() {
				++_hits;
			}
			
			/**
			 * Get number of collisons the roboter has had since the last reset
			 */ 
			int BFClientSpecificRepository::getCollisionCount() const throw() {
				return _hits;
			}			
		
			/**
			* (Re)Start the timer with the current systemtime
			*/
			void BFClientSpecificRepository::startTimer() throw(StrategyException) {
				if (clock_gettime(CLOCK_REALTIME,&_timerstart))
					throw StrategyException(string("Setting the timer for a new shoot failed in Brotfrucht strategy: ")+strerror(errno));
			}
			
			/**
			 * Get nanoseconds passed since the timer was started
			 */ 
			double BFClientSpecificRepository::getTimePassed() const throw(StrategyException) {
				timespec dummy;
				if (clock_gettime(CLOCK_REALTIME,&dummy))
					throw StrategyException(string("Getting the passed time for a new shoot failed in Brotfrucht strategy: ")+strerror(errno));
				return double(1000000000.0)*(dummy.tv_sec-_timerstart.tv_sec)+dummy.tv_nsec-_timerstart.tv_nsec;
			}
			
			/**
			 * Get the logger for the client brotfrucht strategy states
			 */
			 Logger* BFClientSpecificRepository::getLogger() const throw() {
			 	return _logger.get();
			 }

			
			/**
			 * Destructor (does nothing)
			*/
			BFClientSpecificRepository::~BFClientSpecificRepository() throw() {};
			
			/**
			 * Constructor
			*/
			BFClientSpecificRepository::BFClientSpecificRepository() throw(StrategyException, bad_exception): _hits(0), _logger(0),_shootingInterval(1000000000) {
				try {
					_logger=MasterResourceControl::Instance()->createLogger("BrotfruchtClient");
				}
				catch (RTBException& ex) {
					throw StrategyException("Could not obtain logger for the Brotfrucht client states: "+ex.getMessage());
				}	
			}
}
