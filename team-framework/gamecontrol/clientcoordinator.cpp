/************************************************************************
    $Id: clientcoordinator.cpp,v 1.2 2005/01/06 17:59:27 jonico Exp $
    
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

    $Log: clientcoordinator.cpp,v $
    Revision 1.2  2005/01/06 17:59:27  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#include <common.h>
#include "clientcoordinator.h"
#include "../io/rtbconnection.h"
#include "../rtbglobal/masterresourcecontrol.h"
#include "../strategies/strategyfactory.h"
#include "../strategies/clientstatedata.h"

using GameControl::ClientCoordinator;
using Strategies::ClientStateData;
using RTBGlobal::MasterResourceControl;
using Strategies::StrategyFactory;
using Strategies::State;
using std::out_of_range;
using std::string;
using std::ostringstream;

bool ClientCoordinator::reactOnServerTip (const string& serverTip) throw (StrategyException,bad_exception) {
	if(serverTip==_finishingString)
	{
		_logger->logMessage(1,"Receiving server tips has been finished, sending messages to rtb now ...");
		currentClientState->sendRTBMessages();
		flushMessages();
		return false;
	}
	currentClientState->reactOnServerTip(serverTip);
	return true;
}

ClientCoordinator::~ClientCoordinator () throw() {}

ClientCoordinator::ClientCoordinator (	const GameOptionsRepository* gameOptionsRepository, RTBConnection* rtbConnection) throw (bad_exception,ResourceNotFoundException,StrategyException,ConfigNotLoadedException, KeyNotFoundException):_clientStates(0),_logger(0),_rtbConnection(rtbConnection) {
	MasterResourceControl *mrc(MasterResourceControl::Instance());
	const StrategyFactory *fac(mrc->getStrategyFactory(mrc->getConfigurationProperty("Strategy","StrategyName")));
	auto_ptr<ClientStateData> csd(fac->createClientStateData(this,gameOptionsRepository));
	_clientStates=csd->clientStates;
	_clientSpecificRepository=csd->clientSpecificRepository;
	_finishingString=fac->getFinishingString();
	_logger=mrc->createLogger("ClientCoordinator");
	_robotName=getAssociatedRobotName();
}

State* ClientCoordinator::getCurrentState () throw () {
	return currentClientState;
}

void ClientCoordinator::resetGame () throw (StrategyException,bad_exception) {
	// this line is necessary
	_messageBuffer.clear();
	_messageBuffer.str(_emptyString);
	// resets repository
	_clientSpecificRepository->reset();
	// Revokes reset on all state objects and sets the current state to the beginning state.
	for(pvector<ClientState>::iterator i=_clientStates->begin();i!=_clientStates->end();++i)
		(*i)->reset();
	setCurrentClientState(0);
	_logger->logMessage(4,"The game has been reset.");		
}

void ClientCoordinator::processReceivedRTBMessages () throw (StrategyException,bad_exception) {
// Calls roundFinished on the current client state object.
	_logger->logMessage(1,"Receiving RTB Messages has been finished.");
	currentClientState->roundFinished();
}

void ClientCoordinator::setCurrentClientState (unsigned char clientStateIndex) throw (StrategyException,bad_exception) {
	ostringstream O;
	O << int (clientStateIndex);
	currentClientStateName=O.str();
	try {
		currentClientState=_clientStates->at(clientStateIndex);
	}
	catch (out_of_range& ex) {
		throw StrategyException(string("ClientCoordinator : Requested state had invalid index : ")+currentClientStateName+" ("+ex.what()+')');
	}
	_logger->logMessage(3,"State has changed to No. "+currentClientStateName);
}

void ClientCoordinator::flushMessages() throw (bad_exception)
{
	_rtbConnection->sendLine(_messageBuffer.str());
	// this line is necessary
	_messageBuffer.clear();
	_messageBuffer.str(_emptyString);
	_logger->logMessage(2,"Sending messages to RTB complete.");
}

string ClientCoordinator::getAssociatedRobotName () throw (bad_exception)
{
	return _rtbConnection->getTargetName();
}
