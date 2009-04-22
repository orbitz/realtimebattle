/************************************************************************
    $Id: empty_sharedserverspecificrepository.cpp,v 1.6 2005/01/06 17:59:26 jonico Exp $
    
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

    $Log: empty_sharedserverspecificrepository.cpp,v $
    Revision 1.6  2005/01/06 17:59:26  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#include <common.h>

#include "empty_sharedserverspecificrepository.h"

namespace empty {

	/**
	* Method to obtain the one and only object of this class
	*/
	empty_SharedServerSpecificRepository* empty_SharedServerSpecificRepository::Instance() throw (bad_exception) {
		empty_SharedServerSpecificRepository* empty_sssp (empty_SharedServerSpecificRepository::_instance.get());
		if (empty_sssp==0) {
			empty_SharedServerSpecificRepository::_instance.reset(empty_sssp=new empty_SharedServerSpecificRepository());
		}
		return empty_sssp;
	}

	/**
	 * Destructor
	 */
	empty_SharedServerSpecificRepository::~empty_SharedServerSpecificRepository() throw() {}

	/**
	 * Constructor
	 */
	empty_SharedServerSpecificRepository::empty_SharedServerSpecificRepository() throw (StrategyException, bad_exception) {}


	/**
	* Method to reset the repositories
	*/
	void empty_SharedServerSpecificRepository::reset() throw (bad_exception) {}

	/**
	* Only instance of this class
	*/
	auto_ptr<empty_SharedServerSpecificRepository> empty_SharedServerSpecificRepository::_instance(0);

}
