/************************************************************************
    $Id: columbusstrategyfactory.cpp,v 1.5 2005/08/08 11:32:45 krolfy Exp $
    
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

    $Log: columbusstrategyfactory.cpp,v $
    Revision 1.5  2005/08/08 11:32:45  krolfy
    Added three Columbus bots (crash with exception)

    Revision 1.4  2005/07/29 10:59:51  krolfy
    Columbus server side ready but untested

    Revision 1.3  2005/07/28 08:19:10  krolfy
    Most of the server side ready

    Revision 1.2  2005/01/06 17:59:26  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#include <common.h>

#include "columbusstrategyfactory.h"
#include "columbusinternalserverspecificrepository.h"
#include "columbussharedserverspecificrepository.h"
#include "columbusclientspecificrepository.h"
#include "columbusclientattackstate.h"
#include "columbusclientdrivestate.h"
#include "columbusserverattackstate.h"
#include "columbusserverdiscoverystate.h"
#include "columbusserverescapestate.h"
#include "columbusserverinitstate.h"
#include "columbusservertrackstate.h"


namespace Columbus {
	using RTBGlobal::pvector;
	
ColumbusStrategyFactory::ColumbusStrategyFactory()
{
// we have a new registration method now
//	see my mail: RTBGlobal::MasterResourceControl::Instance()->registrateStrategyFactory("Columbus",this);
}

auto_ptr <ServerStateData>  ColumbusStrategyFactory::createServerStateData (ServerCoordinator* serverCoordinator, const GameOptionsRepository* gameOptionsRepository) const throw(StrategyException,bad_exception)
{
	auto_ptr<pvector<ServerState> > states(new pvector<ServerState>());
	
	// trick because of buggy auto_pointer template conversion method
	ColumbusInternalServerSpecificRepository* columbusssppointer;
	auto_ptr<InternalServerSpecificRepository> columbusssp(columbusssppointer=new ColumbusInternalServerSpecificRepository());
	
	ColumbusSharedServerSpecificRepository* columbussharedssp(ColumbusSharedServerSpecificRepository::Instance());

//	states->push_back(new ColumbusServerInitState(serverCoordinator,columbusssppointer,columbussharedssp));
	states->push_back(new ColumbusServerDiscoveryState(serverCoordinator,columbusssppointer,columbussharedssp));
	states->push_back(new ColumbusServerEscapeState(serverCoordinator,columbusssppointer,columbussharedssp));
	states->push_back(new ColumbusServerTrackState(serverCoordinator,columbusssppointer,columbussharedssp));
	states->push_back(new ColumbusServerAttackState(serverCoordinator,columbusssppointer,columbussharedssp));

	return auto_ptr<ServerStateData> (new ServerStateData(states,columbusssp,columbussharedssp));
}

auto_ptr <ClientStateData>  ColumbusStrategyFactory::createClientStateData (ClientCoordinator* clientCoordinator, const GameOptionsRepository* gameOptionsRepository) const throw(StrategyException,bad_exception)
{
	auto_ptr<pvector<ClientState> > states(new pvector<ClientState>());
	
	// trick because of buggy auto_pointer template conversion method
	ColumbusClientSpecificRepository* columbuscsppointer;
	auto_ptr<ClientSpecificRepository> columbuscsp(columbuscsppointer=new ColumbusClientSpecificRepository());
	
	states->push_back(new ColumbusClientDriveState(clientCoordinator,columbuscsppointer,gameOptionsRepository));
	states->push_back(new ColumbusClientAttackState(clientCoordinator,columbuscsppointer,gameOptionsRepository));

	return auto_ptr<ClientStateData> (new ClientStateData(states,columbuscsp));
}

string ColumbusStrategyFactory::getFinishingString() const throw(bad_exception)
{
	// not too long word (saves time by parsing)
	return "X";
}
 
}	// namespace
