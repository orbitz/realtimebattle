/************************************************************************
    $Id: empty_factory.cpp,v 1.8 2005/01/06 17:59:26 jonico Exp $
    
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

    $Log: empty_factory.cpp,v $
    Revision 1.8  2005/01/06 17:59:26  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#include <common.h>

#include "empty_factory.h"
#include "empty_internalserverspecificrepository.h"
#include "empty_sharedserverspecificrepository.h"
#include "empty_clientspecificrepository.h"
#include "empty_client_derivatedstate.h"
#include "empty_server_derivatedstate.h"


namespace empty {
	using RTBGlobal::pvector;

empty_Factory::empty_Factory()
{
  /*
   * There is no need for a special registration, just put your code
   * you want to have to be executed at the beginning here
   */
}

auto_ptr <ServerStateData>  empty_Factory::createServerStateData (ServerCoordinator* serverCoordinator, const GameOptionsRepository* gameOptionsRepository) const throw(StrategyException,bad_exception)
{
	auto_ptr<pvector<ServerState> > states(new pvector<ServerState>());

	// trick because of buggy auto_pointer template conversion method
	empty_InternalServerSpecificRepository* empty_ssppointer;
	auto_ptr<InternalServerSpecificRepository> empty_ssp(empty_ssppointer=new empty_InternalServerSpecificRepository());

	empty_SharedServerSpecificRepository* empty_sharedssp(empty_SharedServerSpecificRepository::Instance());
	
	/**
	 * Insert here the registrations of all your used serverstate:
	 * states->push_back(new empty_YOUR_SERVER_STATE(serverCoordinator, empty_ssppointer, empty_sharedssp));
	 */
	states->push_back(new empty_ServerDerivatedState(serverCoordinator, empty_ssppointer, empty_sharedssp));
	return auto_ptr<ServerStateData> (new ServerStateData(states, empty_ssp, empty_sharedssp));
}

auto_ptr <ClientStateData>  empty_Factory::createClientStateData (ClientCoordinator* clientCoordinator, const GameOptionsRepository* gameOptionsRepository) const throw(StrategyException,bad_exception)
{
	auto_ptr<pvector<ClientState> > states(new pvector<ClientState>());

	// trick because of buggy auto_pointer template conversion method
	empty_ClientSpecificRepository* empty_csppointer;
	auto_ptr<ClientSpecificRepository> empty_csp(empty_csppointer=new empty_ClientSpecificRepository());

	/**
	 * Insert here the registrations of all your used clientstate:
	 * states->push_back(new empty_YOUR_CLIENT_STATE(clientCoordinator,empty_csppointer,gameOptionsRepository));
	 */
	states->push_back(new empty_ClientDerivatedState(clientCoordinator,empty_csppointer,gameOptionsRepository));
	return auto_ptr<ClientStateData> (new ClientStateData(states,empty_csp));
}

string empty_Factory::getFinishingString() const throw(bad_exception)
{
	// not too long word (saves time by parsing)
	return "empty_END";
}

}	// namespace empty
