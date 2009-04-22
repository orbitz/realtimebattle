/************************************************************************
    $Id: bffactory.cpp,v 1.2 2005/01/06 17:59:26 jonico Exp $
    
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

    $Log: bffactory.cpp,v $
    Revision 1.2  2005/01/06 17:59:26  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#include <common.h>

#include "bffactory.h"
#include "bfservernorotatestate.h"
#include "bfserverrotatestate.h"
#include "bfinternalserverspecificrepository.h"
#include "bfsharedserverspecificrepository.h"
#include "bfclientspecificrepository.h"
#include "bfclientshoothalfstate.h"
#include "bfclientshoottwicestate.h"


namespace Brotfrucht {
	using RTBGlobal::pvector;
	
BFFactory::BFFactory()
{
// we have a new registration method now
//	see my mail: RTBGlobal::MasterResourceControl::Instance()->registrateStrategyFactory("Brotfrucht",this);
}

auto_ptr <ServerStateData>  BFFactory::createServerStateData (ServerCoordinator* serverCoordinator, const GameOptionsRepository* gameOptionsRepository) const throw(StrategyException,bad_exception)
{
	auto_ptr<pvector<ServerState> > states(new pvector<ServerState>());
	
	// trick because of buggy auto_pointer template conversion method
	BFInternalServerSpecificRepository* bfssppointer;
	auto_ptr<InternalServerSpecificRepository> bfssp(bfssppointer=new BFInternalServerSpecificRepository());
	
	BFSharedServerSpecificRepository* bfsharedssp(BFSharedServerSpecificRepository::Instance());
	states->push_back(new BFServerNoRotateState(serverCoordinator,bfssppointer,bfsharedssp));
	states->push_back(new BFServerRotateState(serverCoordinator,bfssppointer,bfsharedssp));
	return auto_ptr<ServerStateData> (new ServerStateData(states,bfssp,bfsharedssp));
}

auto_ptr <ClientStateData>  BFFactory::createClientStateData (ClientCoordinator* clientCoordinator, const GameOptionsRepository* gameOptionsRepository) const throw(StrategyException,bad_exception)
{
	auto_ptr<pvector<ClientState> > states(new pvector<ClientState>());
	
	// trick because of buggy auto_pointer template conversion method
	BFClientSpecificRepository* bfcsppointer;
	auto_ptr<ClientSpecificRepository> bfcsp(bfcsppointer=new BFClientSpecificRepository());
	
	states->push_back(new BFClientShootHalfState(clientCoordinator,bfcsppointer,gameOptionsRepository));
	states->push_back(new BFClientShootTwiceState(clientCoordinator,bfcsppointer,gameOptionsRepository));
	return auto_ptr<ClientStateData> (new ClientStateData(states,bfcsp));
}

string BFFactory::getFinishingString() const throw(bad_exception)
{
	// not too long word (saves time by parsing)
	return "BFE";
}
 
}	// namespace

// we have a new registration method now
// see my mail: namespace{ Brotfrucht::BFFactory __global_factory__I_will_never_ever_need_this_name_again_so_I_do_this_for_fun; }
