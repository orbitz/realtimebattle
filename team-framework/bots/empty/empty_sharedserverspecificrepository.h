/************************************************************************
    $Id: empty_sharedserverspecificrepository.h,v 1.7 2005/01/06 17:59:26 jonico Exp $
    
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

    $Log: empty_sharedserverspecificrepository.h,v $
    Revision 1.7  2005/01/06 17:59:26  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef empty_SHAREDSERVERSPECIFICREPOSITORY
#define empty_SHAREDSERVERSPECIFICREPOSITORY

#include "../../strategies/sharedserverspecificrepository.h"
#include "../../exceptions/strategyexception.h"
#include "../../log/logger.h"
#include <memory>

namespace empty {
	
	/**
	 * Use this Repository for all shared-stuff (for all clients and for the server).
	 * You can implement what you want (e.g. a map or something like that).
	 */
	 
	using std::auto_ptr;
	using std::bad_exception;
	using Log::Logger;
	using Exceptions::StrategyException;
	using Strategies::SharedServerSpecificRepository;

	class empty_SharedServerSpecificRepository: public Strategies::SharedServerSpecificRepository {
		public:
		
			/**
			 * Method to obtain the one and only object of this class
			 */
			static empty_SharedServerSpecificRepository* Instance() throw (bad_exception);
			
			/**
			 * Method to reset the repositories
			 */
			void reset() throw (bad_exception);

			/**
			 * Destructor
			 */
			~empty_SharedServerSpecificRepository() throw ();

		private:
		
			/**
			 * Only instance of this class
			 */
			 static auto_ptr<empty_SharedServerSpecificRepository> _instance;

			 /**
			  * Constructor
			  */
			 empty_SharedServerSpecificRepository()throw (StrategyException, bad_exception);

	};
}

#endif
