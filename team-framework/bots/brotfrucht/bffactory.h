/************************************************************************
    $Id: bffactory.h,v 1.3 2005/01/06 17:59:26 jonico Exp $
    
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

    $Log: bffactory.h,v $
    Revision 1.3  2005/01/06 17:59:26  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef BFFACTORY_H
#define BFFACTORY_H

#include "../../gamecontrol/gamecontrolnamespace.h"
#include "../../strategies/strategiesnamespace.h"
#include "../../strategies/strategyfactory.h"
#include "../../strategies/serverstatedata.h"
#include "../../strategies/clientstatedata.h"
#include "../../exceptions/strategyexception.h"
#include <memory>
#include "brotfruchtnamespace.h"

/**
* Namespace Brotfrucht
*/
namespace Brotfrucht {
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

	/**
	* Class BFFactory 
	*/
	class BFFactory : public Strategies::StrategyFactory
	{
	public:
		 BFFactory();
		/**
		* This method creates instances of the server states, in particular BFServerRotateState and BFServerNoRotateState.
		* Note: The created state objects should get their repositories passed in the constructor, though the coordinator is only responsable to delete these resources but not to provide access to them.
		* @param serverCoordinator This object should be passed in all constructors of the created state objects.
		* @param gameOptionsRepository This object should also be passed in all constructors of the created state objects.
		*/
		 auto_ptr <ServerStateData>  createServerStateData (ServerCoordinator* serverCoordinator, const GameOptionsRepository* gameOptionsRepository) const throw(StrategyException,bad_exception);
		/**
		* This method creates instances of the client states, in particular BFClientShootHalfState and BFClientShootTwiceState.
		* Note: The created state objects should get their repositories passed in the constructor, though the coordinator is only responsable to delete these resources but not to provide access to them.
		* @param clientCoordinator This object should be passed in all constructors of the created state objects.
		* @param gameOptionsRepository This object should also be passed in all constructors of the created state objects.
		*/
		 auto_ptr <ClientStateData>  createClientStateData (ClientCoordinator* clientCoordinator, const GameOptionsRepository* gameOptionsRepository) const throw(StrategyException,bad_exception);
		/**
		* Returns the string indicating that the server tips have finished, in particular "BFE".
		* Note: The value of this string must not be used as a normel server tip.
		*/
		 string getFinishingString() const throw(bad_exception);
	};
}
#endif //BFFACTORY_H
