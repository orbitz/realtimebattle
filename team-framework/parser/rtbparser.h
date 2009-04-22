/************************************************************************
    $Id: rtbparser.h,v 1.2 2005/01/06 17:59:28 jonico Exp $
    
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

    $Log: rtbparser.h,v $
    Revision 1.2  2005/01/06 17:59:28  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef RTBPARSER_H
#define RTBPARSER_H

#include <sstream>
#include <string>
#include "rtbparseresult.h"
#include "../gamecontrol/gamecontrolnamespace.h"
#include "../exceptions/parserexception.h"
#include "../exceptions/strategyexception.h"

/**
* Namespace Parser
*/
namespace Parser {
	using std::string;
	using std::bad_exception;
	using std::istringstream;
	using GameControl::Coordinator;
	using GameControl::GameOptionsRepository;
	using Exceptions::ParserException;
	using Exceptions::StrategyException;
	

/**
* Class RTBParser
*
* Important: Do not try to instantiate this class or use it with responses from the master server to the client (server tips won't be processed by this parser)
* 
* Note: The parser only determines the type of the given message, reads out the corresponding arguments and returns an datastructure with this result.
* It is NOT responsable for doing a specific action according to the given message. 
*/ 
class RTBParser {
/*
* Public stuff
*/
public:
	/**
	 * This method parses an RTB message.
	 * @param message The incoming RTB message in string shape
	 * @return The parsed RTB message
	 */
	static const RTBParseResult*  processRTBMessage (const string& message) throw (ParserException,bad_exception);
		
	
	/**
	 * If the last message was from a type, that a coordinator object can handle, this method delegates it to the appropiate method.
	 * @param coordinator The Cordinator to handle the last message
	 */
	static void  delegateLastMessage (Coordinator* coordinator) throw (StrategyException, bad_exception);
		
	
	/**
	 * If last message was from type GameOption, updates the GameOptionsRepository.
	 * @param gameOptionsRepository 
	 */
	static void  updateGameOptions (GameOptionsRepository* gameOptionsRepository) throw(ParserException,bad_exception);
/*
* Private stuff
*/
private:
	static  RTBParseResult _parseResult;
	static  istringstream  _parsingStream;
	static  string _tmpString;
};
}
#endif //RTBPARSER_H

