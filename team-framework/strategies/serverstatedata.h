/************************************************************************
    $Id: serverstatedata.h,v 1.2 2005/01/06 17:59:29 jonico Exp $
    
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

    $Log: serverstatedata.h,v $
    Revision 1.2  2005/01/06 17:59:29  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef SERVERSTATEDATA_H
#define SERVERSTATEDATA_H

#include <memory>

#include "strategiesnamespace.h"

#include "serverstate.h"
#include "internalserverspecificrepository.h"
#include "sharedserverspecificrepository.h"
#include "../rtbglobal/pvector.h"

/**
* Namespace Strategies 
*/
namespace Strategies {
	using std::auto_ptr;
	using std::bad_exception;
	using RTBGlobal::pvector;

	/**
	* class ServerStateData 
	*/
	class ServerStateData {
	public:
		/**
		* Contains the server states. "serverStates[0]" is the beginning state.
		*/
		auto_ptr < pvector<ServerState> > serverStates;
		/**
		* The only purpose of this field is to assure, that the repository will be deleted and reseted by the Coordinator, to pass this repository
		* and the SharedServerSpecificRepository to the states is task of the concrete stzrategy factory
		*/ 
		auto_ptr <InternalServerSpecificRepository> internalServerSpecificRepository;
		/**
		* The only purpose of this field is to assure, that the repository will be reseted by the Coordinator, to pass this repository
		* and the SharedServerSpecificRepository to the states is task of the concrete stzrategy factory
		*/ 

		SharedServerSpecificRepository* sharedServerSpecificRepository;
	
		ServerStateData (auto_ptr < pvector<ServerState> > serverStates, auto_ptr <InternalServerSpecificRepository> internalServerSpecificRepository,SharedServerSpecificRepository* sharedServerSpecificRepository) throw(bad_exception);
		
		/**
		* Destructor, does nothing
		*/
		~ServerStateData () throw();
	};

}
#endif //SERVERSTATEDATA_H

