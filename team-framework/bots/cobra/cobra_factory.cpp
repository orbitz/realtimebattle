/************************************************************************
				  			cobra_factory.cpp
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

#include "cobra_factory.h"
#include "cobra_internalserverspecificrepository.h"
#include "cobra_sharedserverspecificrepository.h"
#include "cobra_clientspecificrepository.h"
#include "cobra_server_basicstate.h"
#include "cobra_client_basicstate.h"


namespace cobra {
	using RTBGlobal::pvector;

cobra_Factory::cobra_Factory()
{
  /*
   * There is no need for a special registration, just put your code
   * you want to have to be executed at the beginning here
   */
}

auto_ptr <ServerStateData>  cobra_Factory::createServerStateData (ServerCoordinator* serverCoordinator, const GameOptionsRepository* gameOptionsRepository) const throw(StrategyException,bad_exception)
{
	auto_ptr<pvector<ServerState> > states(new pvector<ServerState>());

	// trick because of buggy auto_pointer template conversion method
	cobra_InternalServerSpecificRepository* cobra_ssppointer;
	auto_ptr<InternalServerSpecificRepository> cobra_ssp(cobra_ssppointer=new cobra_InternalServerSpecificRepository());

	cobra_SharedServerSpecificRepository* cobra_sharedssp(cobra_SharedServerSpecificRepository::Instance());
	
	/**
	 * Insert here the registrations of all your used serverstate:
	 * states->push_back(new cobra_YOUR_SERVER_STATE(serverCoordinator, cobra_ssppointer, cobra_sharedssp));
	 */
	states->push_back(new cobra_ServerBasicState(serverCoordinator, cobra_ssppointer, cobra_sharedssp));
	return auto_ptr<ServerStateData> (new ServerStateData(states, cobra_ssp, cobra_sharedssp));
}

auto_ptr <ClientStateData>  cobra_Factory::createClientStateData (ClientCoordinator* clientCoordinator, const GameOptionsRepository* gameOptionsRepository) const throw(StrategyException,bad_exception)
{
	auto_ptr<pvector<ClientState> > states(new pvector<ClientState>());

	// trick because of buggy auto_pointer template conversion method
	cobra_ClientSpecificRepository* cobra_csppointer;
	auto_ptr<ClientSpecificRepository> cobra_csp(cobra_csppointer=new cobra_ClientSpecificRepository());

	/**
	 * Insert here the registrations of all your used clientstate:
	 * states->push_back(new cobra_YOUR_CLIENT_STATE(clientCoordinator,cobra_csppointer,gameOptionsRepository));
	 */
	states->push_back(new cobra_ClientBasicState(clientCoordinator,cobra_csppointer,gameOptionsRepository));
	return auto_ptr<ClientStateData> (new ClientStateData(states,cobra_csp));
}

string cobra_Factory::getFinishingString() const throw(bad_exception)
{
	// not too long word (saves time by parsing)
	return "cobra_END";
}

}	// namespace cobra
