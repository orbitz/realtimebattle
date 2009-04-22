/************************************************************************
                			cobra_factory.h
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

#ifndef cobra_FACTORY_H
#define cobra_FACTORY_H

#include "../../gamecontrol/gamecontrolnamespace.h"
#include "../../strategies/strategiesnamespace.h"
#include "../../strategies/strategyfactory.h"
#include "../../strategies/serverstatedata.h"
#include "../../strategies/clientstatedata.h"
#include "../../exceptions/strategyexception.h"
#include <memory>
#include "cobra_namespace.h"

namespace cobra {
	using GameControl::ServerCoordinator;
	using GameControl::ClientCoordinator;
	using GameControl::GameOptionsRepository;
	using Strategies::StrategyFactory;
	using Strategies::ServerStateData;
	using Strategies::ClientStateData;
	using Exceptions::StrategyException;
	using std::auto_ptr;
	using std::bad_exception;
	using std::string;
	using Strategies::StrategyFactory;


	class cobra_Factory : public Strategies::StrategyFactory
	{
	public:
		 cobra_Factory();
		 auto_ptr <ServerStateData>  createServerStateData (ServerCoordinator* serverCoordinator, const GameOptionsRepository* gameOptionsRepository) const throw(StrategyException,bad_exception);
		 auto_ptr <ClientStateData>  createClientStateData (ClientCoordinator* clientCoordinator, const GameOptionsRepository* gameOptionsRepository) const throw(StrategyException,bad_exception);
		 string getFinishingString() const throw(bad_exception);
		 
		 /**
		  * It is not necessary to add any method in this class.
		  */
	};
}
#endif //cobra_FACTORY_H
