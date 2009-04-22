/************************************************************************
    $Id: servercoordinator.cpp,v 1.2 2005/01/06 17:59:27 jonico Exp $
    
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

    $Log: servercoordinator.cpp,v $
    Revision 1.2  2005/01/06 17:59:27  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#include <common.h>
#include "servercoordinator.h"
#include "../io/clientconnection.h"
#include "../rtbglobal/masterresourcecontrol.h"
#include "../strategies/strategyfactory.h"
#include "../strategies/serverstatedata.h"

using GameControl::ServerCoordinator;
using Strategies::ServerStateData;
using GameControl::GameOptionsRepository;
using Strategies::StrategyFactory;
using Strategies::State;
using RTBGlobal::MasterResourceControl;
using IO::ClientConnection;
using std::endl;
using std::out_of_range;
using std::string;

ServerCoordinator::~ServerCoordinator ()throw() {}

ServerCoordinator::ServerCoordinator (	ClientConnection* clientConnection, const GameOptionsRepository* gameOptionsRepository ) throw (ConfigNotLoadedException, KeyNotFoundException,bad_exception, StrategyException,ResourceNotFoundException) :_internalServerSpecificRepository(0),_serverStates(0),_logger(0),_clientConnection(clientConnection) {
	MasterResourceControl *mrc=MasterResourceControl::Instance();
	const StrategyFactory *fac=mrc->getStrategyFactory(mrc->getConfigurationProperty("Strategy","StrategyName"));
	auto_ptr<ServerStateData> ssd(fac->createServerStateData(this,gameOptionsRepository));
	_serverStates=ssd->serverStates;
	_internalServerSpecificRepository=ssd->internalServerSpecificRepository;
	_sharedServerSpecificRepository=ssd->sharedServerSpecificRepository;
	_finishingString=fac->getFinishingString();
	_logger=mrc->createLogger("ServerCoordinator");
	_robotName=getAssociatedRobotName();
	
	// This voodoo is done to save time with doing string concatenation only once
	_logMessage1=_robotName+": The game has been reset.";
	_logMessage2=_robotName+": Receiving messages from client has been finished.";
	_logMessage3=_robotName+": State has changed to No. ";
	_logMessage4=_robotName+": Sending messages to client complete.";
	_logMessage5=_robotName+": ServerCoordinator : Requested state had invalid index : ";
}

State* ServerCoordinator::getCurrentState () throw () {
	return currentServerState;
}

void ServerCoordinator::resetGame () throw (StrategyException,bad_exception) {
// this line is necessary
	_tipBuffer.clear();
	_tipBuffer.str(_emptyString);
// reset repositories
	_internalServerSpecificRepository->reset();
	// reset the shared data only once (in the local coordinator)
	if (_clientConnection->isLocal())
		_sharedServerSpecificRepository->reset();

// Calls reset on all state objects and sets the current state to the beginning state.
	for(pvector<ServerState>::iterator i=_serverStates->begin();i!=_serverStates->end();++i)
		(*i)->reset();
	setCurrentServerState(0);
	_logger->logMessage(4,_logMessage1);
}

void ServerCoordinator::processReceivedRTBMessages () throw (StrategyException,bad_exception) {
// This method calls sendServerTips on the current server state object.
	_logger->logMessage(1,_logMessage2);
	currentServerState->sendServerTips();
	endServerTipSequence();
}

void ServerCoordinator::setCurrentServerState (unsigned char serverStateIndex) throw (bad_exception,StrategyException) {
	try {
		currentServerState=_serverStates->at(serverStateIndex);
	}
	catch (out_of_range& ex) {
		throw StrategyException(_logMessage5+ex.what());
	}
	_currentServerStateIndex=serverStateIndex;
	ostringstream O;
	O <<  int (_currentServerStateIndex);
	currentServerStateName=O.str();
	_logger->logMessage(3,_logMessage3+currentServerStateName);
}

void ServerCoordinator::endServerTipSequence () throw (bad_exception) {
	_tipBuffer << _finishingString;
	_clientConnection->sendLine(_tipBuffer.str());
	// this line is necessary
	_tipBuffer.clear();
	_tipBuffer.str(_emptyString);
	_logger->logMessage(2,_logMessage4);
}


string ServerCoordinator::getAssociatedRobotName () throw (bad_exception)
{
	return _clientConnection->getTargetName();
}
