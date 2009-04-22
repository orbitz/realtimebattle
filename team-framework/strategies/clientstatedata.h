/************************************************************************
    $Id: clientstatedata.h,v 1.2 2005/01/06 17:59:29 jonico Exp $
    
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

    $Log: clientstatedata.h,v $
    Revision 1.2  2005/01/06 17:59:29  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef CLIENTSTATEDATA_H
#define CLIENTSTATEDATA_H

#include <memory>

#include "clientstate.h"
#include "clientspecificrepository.h"
#include "../rtbglobal/pvector.h"

/**
* Namespace Strategies
*/
namespace Strategies {
	using std::auto_ptr;
	using RTBGlobal::pvector;
	using std::bad_exception;

/**
* Class ClientStateData 
*/
class ClientStateData {
public:
	/**
	 * Contains the client states. "clientStates[0]" is the beginning state.
	 */
	auto_ptr < pvector <ClientState> > clientStates;
	/**
	 * Important: This field only assures, that the repository will be deleted afterwards, passing it to the states is task of the strategy factory
	 */
	auto_ptr <ClientSpecificRepository> clientSpecificRepository;

	/**
	* Constructor
	*/
	ClientStateData (	auto_ptr < pvector<ClientState > > clientStates,
				auto_ptr <ClientSpecificRepository> clientSpecificRepository	) throw(bad_exception);
				
	/**
	* Destructor, does nothing
	*/
	~ClientStateData () throw();
};

}
#endif //CLIENTSTATEDATA_H

