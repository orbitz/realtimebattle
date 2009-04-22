/************************************************************************
    $Id: empty_clientspecificrepository.h,v 1.5 2005/01/06 17:59:26 jonico Exp $
    
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

    $Log: empty_clientspecificrepository.h,v $
    Revision 1.5  2005/01/06 17:59:26  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef empty_CLIENTSPECIFICREPOSITORY
#define empty_CLIENTSPECIFICREPOSITORY

#include "../../strategies/clientspecificrepository.h"
#include "../../exceptions/strategyexception.h"

namespace empty {

	/**
	 * Use this Repository for all client-stuff. You can implement what you want.
	 */
	 
 	using Exceptions::StrategyException;
	using std::bad_exception;
	using Strategies::ClientSpecificRepository;

	class empty_ClientSpecificRepository: public Strategies::ClientSpecificRepository {
		public:
					
			/**
			 * Method to reset the repositories (clears number of collisions, (re)starts timer)
			*/
			void reset() throw(StrategyException, bad_exception);

			/**
			 * Destructor
			 */
			~empty_ClientSpecificRepository() throw();

			/**
			 * Constructor
			 */
			empty_ClientSpecificRepository() throw(StrategyException, bad_exception);
		private:
	};
}


#endif
