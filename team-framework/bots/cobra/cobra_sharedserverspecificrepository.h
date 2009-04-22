/************************************************************************
  			cobra_sharedserverspecificrepository.h
Copyright:
    Tino Truppel <tino.truppel@hpi.uni-potsdam.de>

This file was created on Mon, Dec 13 2004

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


The original location of this file is cobra
**************************************************************************/

#ifndef cobra_SHAREDSERVERSPECIFICREPOSITORY
#define cobra_SHAREDSERVERSPECIFICREPOSITORY

#include "../../strategies/sharedserverspecificrepository.h"
#include "../../exceptions/strategyexception.h"
#include "../../log/logger.h"
#include <memory>
#include "../general/teaminfo.h"

namespace cobra {
	
	/**
	 * Use this Repository for all shared-stuff (for all clients and for the server).
	 * You can implement what you want (e.g. a map or something like that).
	 */
	 
	using std::auto_ptr;
	using std::vector;
	using std::string;
	using std::out_of_range;
	using std::bad_exception;
	using Log::Logger;
	using General::TeamInfo;
	using Exceptions::StrategyException;
	using Strategies::SharedServerSpecificRepository;

	class cobra_SharedServerSpecificRepository: public Strategies::SharedServerSpecificRepository {
		public:
		
			/**
			 * Method to obtain the one and only object of this class
			 */
			static cobra_SharedServerSpecificRepository* Instance() throw (bad_exception);
			
			/**
			 * Method to reset the repositories
			 */
			void reset() throw (bad_exception);

			/**
			 * Destructor
			 */
			~cobra_SharedServerSpecificRepository() throw ();
			
			/*
	 		* Access to TeamInfo
	 		*/
			TeamInfo* getTeamInfo() throw();

		private:
		
			/**
			 * Only instance of this class
			*/
			static auto_ptr<cobra_SharedServerSpecificRepository> _instance;
			 
		 	TeamInfo _teamInfo;

			 /**
			  * Constructor
			  */
			 cobra_SharedServerSpecificRepository()throw (StrategyException, bad_exception);

	};
}

#endif
