/************************************************************************
    $Id: rtbparser.cpp,v 1.4 2005/08/10 16:00:50 krolfy Exp $
    
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

    $Log: rtbparser.cpp,v $
    Revision 1.4  2005/08/10 16:00:50  krolfy
    Columbus behaves odd, can SIGSEGV

    Revision 1.3  2005/08/10 13:12:31  krolfy
    Added some debug lines, minor fixes

    Revision 1.2  2005/01/06 17:59:28  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#include <common.h>
#include <stdexcept>
#include "rtbparser.h"
#include "object_type.h"
#include "game_option_type.h"
#include "warning_type.h"
#include "../gamecontrol/coordinator.h"
#include "../gamecontrol/gameoptionsrepository.h"


namespace Parser {
	using std::string;
	using std::ios_base;
	using GameControl::GameOptionsRepository;
	using std::out_of_range;

	// define static variables
	RTBParseResult RTBParser::_parseResult;
	istringstream RTBParser::_parsingStream;
	string RTBParser::_tmpString;

	const RTBParseResult* RTBParser::processRTBMessage (const string& message) throw(ParserException, bad_exception)
	{
		// this line is necessary, too
		_parsingStream.clear();
		_parsingStream.str(message);
		_parsingStream >> _tmpString;
		try { switch(_tmpString.at(2)) {
		case 'd':	_parseResult.messageType=RADAR;
		/* Radar */	_parsingStream	>> _parseResult.doubleValue[0]
						>> _parseResult.intValue[0]
						>> _parseResult.doubleValue[1];
				return &_parseResult;

		case 'f':	_parseResult.messageType=INFO;
		/* Info */	_parsingStream	>> _parseResult.doubleValue[0]
						>> _parseResult.doubleValue[1]
						>> _parseResult.doubleValue[2];
				return &_parseResult;

		case 'o':	_parseResult.messageType=COORDINATES;
		/*Coordinates*/	_parsingStream	>> _parseResult.doubleValue[0]
						>> _parseResult.doubleValue[1]
						>> _parseResult.doubleValue[2];
				return &_parseResult;

		case 'e':	_parseResult.messageType=ENERGY;
		/* Energy */		_parsingStream	>> _parseResult.doubleValue[0];

				return &_parseResult;
		case 'l':	_parseResult.messageType=COLLISION;
		/* Collision */	_parsingStream	>> _parseResult.intValue[0]
						>> _parseResult.doubleValue[0];
				return &_parseResult;

		case 't':	_parseResult.messageType=ROTATION_REACHED;
		/* Rotation */	_parsingStream	>> _parseResult.intValue[0];
		/* Reached */	return &_parseResult;

		case 'r':	_parseResult.messageType=WARNING;
		/* Warning */		_parsingStream	>> _parseResult.intValue[0];
				_parseResult.stringValue.clear();
				getline(_parsingStream,_parseResult.stringValue); // better way to do this job
				return &_parseResult;

		case 'a':	_parseResult.messageType=DEAD;
		/* Dead */	return &_parseResult;
										case 'b':switch(_tmpString.at(5)) {
		case 'I':	_parseResult.messageType=ROBOT_INFO;
		/* Robot */	_parsingStream	>> _parseResult.doubleValue[0]
		/* Info */			>> _parseResult.intValue[0];
				return &_parseResult;

		case 's':	_parseResult.messageType=ROBOTS_LEFT;
		/* Robots */	_parsingStream	>> _parseResult.intValue[0];
		/* Left */	return &_parseResult;
									}	case 'i':switch(_tmpString.at(0)) {
		case 'I':	_parseResult.messageType=INITIALIZE;
		/*Initialize*/	_parsingStream	>> _parseResult.intValue[0];
				return &_parseResult;

		case 'E':	_parseResult.messageType=EXIT_ROBOT;
		/* Exit */	return &_parseResult;
		/* Robot */
									}	case 'm':switch(_tmpString.at(4)) {
		case 'O':	_parseResult.messageType=GAME_OPTION;
		/* Game */	_parsingStream	>> _parseResult.intValue[0]
		/* Option */			>> _parseResult.doubleValue[0];
				return &_parseResult;

		case 'S':	_parseResult.messageType=GAME_STARTS;
		/* Game */	return &_parseResult;
		/* Starts */

		case 'F':	_parseResult.messageType=GAME_FINISHES;
		/* Game */	return &_parseResult;
		/* Finishes */		
									}	case 'u':switch(_tmpString.at(4)) {
		case 'C':	_parseResult.messageType=YOUR_COLOUR;
		/* Your */	_parsingStream.setf(ios_base::hex,ios_base::basefield);
		/* Colour */	_parsingStream >> _parseResult.intValue[0];
				_parsingStream.setf(ios_base::dec,ios_base::basefield);
				return &_parseResult;

		case 'N':	_parseResult.messageType=YOUR_NAME;
		/* Your */	_parseResult.stringValue.clear();
		/* Name */	getline(_parsingStream,_parseResult.stringValue); // better way to do this job
				return &_parseResult;
		}}} catch (out_of_range& ex) {}
		if(_tmpString.empty())
			_parseResult.stringValue="Parser found an empty RTB message.";
		else	((_parseResult.stringValue	 ="Parser found an unknown RTB message entitled \"")
							+=_tmpString)
							+="\".";
		throw ParserException(_parseResult.stringValue);
	}

	void RTBParser::delegateLastMessage (Coordinator* coordinator) throw (StrategyException,bad_exception)
	{
		switch(_parseResult.messageType)
		{
		case RADAR:		coordinator->receiveRTBMessageRadar (
						_parseResult.doubleValue[0],
						object_type(_parseResult.intValue[0]),
						_parseResult.doubleValue[1]);
					return;
		case INFO:		coordinator->receiveRTBMessageInfo (
						_parseResult.doubleValue[0],
						_parseResult.doubleValue[1],
						_parseResult.doubleValue[2]);
					return;
		case COORDINATES:	coordinator->receiveRTBMessageCoordinates (
						_parseResult.doubleValue[0],
						_parseResult.doubleValue[1],
						_parseResult.doubleValue[2]);
					return;
		case ENERGY:		coordinator->receiveRTBMessageEnergy (
						_parseResult.doubleValue[0]);
					return;
		case COLLISION:	coordinator->receiveRTBMessageCollision (
						object_type(_parseResult.intValue[0]),
						_parseResult.doubleValue[0]);
					return;
		case ROTATION_REACHED:	coordinator->receiveRTBMessageRotationReached(
						_parseResult.intValue[0]);
					return;
		case WARNING:		coordinator->receiveRTBMessageWarning (		// >> This may cause
						warning_type(_parseResult.intValue[0]),	//    an exception
						_parseResult.stringValue);		//    to be thrown
					return;
		case DEAD:		coordinator->receiveRTBMessageDead ();
					return;
		case ROBOT_INFO:	coordinator->receiveRTBMessageRobotInfo(
						_parseResult.doubleValue[0],
						_parseResult.intValue[0]);
					return;
		case ROBOTS_LEFT:	coordinator->receiveRTBMessageRobotsLeft(
						_parseResult.intValue[0]);
					return;
			default:
				throw StrategyException("Parser was requested to delegate an illegal parsing result.");

		}
	}

	void RTBParser::updateGameOptions (GameOptionsRepository* gameOptionsRepository) throw(ParserException,bad_exception)
	{
		if(_parseResult.messageType!=GAME_OPTION) throw ParserException("Parser was requested to update a game option without a concrete game option specified.");
		switch(game_option_type(_parseResult.intValue[0]))
		{
			case ROBOT_MAX_ROTATE:
				gameOptionsRepository->ROBOT_MAX_ROTATE=_parseResult.doubleValue[0];
				return;
			case ROBOT_CANNON_MAX_ROTATE:
				gameOptionsRepository->ROBOT_CANNON_MAX_ROTATE=_parseResult.doubleValue[0];
				return;
			case ROBOT_RADAR_MAX_ROTATE:
				gameOptionsRepository->ROBOT_RADAR_MAX_ROTATE=_parseResult.doubleValue[0];
				return;
			case ROBOT_MAX_ACCELERATION:
				gameOptionsRepository->ROBOT_MAX_ACCELERATION=_parseResult.doubleValue[0];
				return;
			case ROBOT_MIN_ACCELERATION:
				gameOptionsRepository->ROBOT_MIN_ACCELERATION=_parseResult.doubleValue[0];
				return;
			case ROBOT_START_ENERGY:
				gameOptionsRepository->ROBOT_START_ENERGY=_parseResult.doubleValue[0];
				return;
			case ROBOT_MAX_ENERGY:
				gameOptionsRepository->ROBOT_MAX_ENERGY=_parseResult.doubleValue[0];
				return;
			case ROBOT_ENERGY_LEVELS:
				gameOptionsRepository->ROBOT_ENERGY_LEVELS=_parseResult.doubleValue[0];
				return;
			case SHOT_SPEED:
				gameOptionsRepository->SHOT_SPEED=_parseResult.doubleValue[0];
				return;
			case SHOT_MIN_ENERGY:
				gameOptionsRepository->SHOT_MIN_ENERGY=_parseResult.doubleValue[0];
				return;
			case SHOT_MAX_ENERGY:
				gameOptionsRepository->SHOT_MAX_ENERGY=_parseResult.doubleValue[0];
				return;
			case SHOT_ENERGY_INCREASE_SPEED:
				gameOptionsRepository->SHOT_ENERGY_INCREASE_SPEED=_parseResult.doubleValue[0];
				return;
			case TIMEOUT:
				gameOptionsRepository->TIMEOUT=_parseResult.doubleValue[0];
				return;
			case DEBUG_LEVEL:
				gameOptionsRepository->DEBUG_LEVEL=_parseResult.doubleValue[0];
				return;
			case SEND_ROBOT_COORDINATES:
				gameOptionsRepository->SEND_ROBOT_COORDINATES=_parseResult.doubleValue[0];
				return;
		}
		_parseResult.stringValue="Parser found an unknown game option number.";
		throw ParserException(_parseResult.stringValue);
	}
}

