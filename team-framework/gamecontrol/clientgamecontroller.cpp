/************************************************************************
    $Id: clientgamecontroller.cpp,v 1.3 2005/01/06 17:59:27 jonico Exp $
    
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

    $Log: clientgamecontroller.cpp,v $
    Revision 1.3  2005/01/06 17:59:27  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#include <common.h>

#include "clientgamecontroller.h"
#include "../rtbglobal/masterresourcecontrol.h"
#include "../io/iofactory.h"
#include "../parser/rtbparser.h"
#include "../parser/rtbparser.h"
#include "../parser/rtbparseresult.h"
#include "../parser/message_to_robot_type.h"
#include "../parser/game_option_type.h"
#include "../io/rtbconnection.h"
#include "../io/serverconnection.h"
#include "../exceptions/parserexception.h"
#include "../exceptions/ioexception.h"
#include "../exceptions/strategyexception.h"
#include "clientcoordinator.h"
#include <ios>

namespace GameControl {
	using Parser::RTBParser;
	using Parser::RTBParseResult;
	using Parser::game_option_type;
	using std::endl;
	using std::exception;
	using std::ios_base;
	using Exceptions::ParserException;
	using Exceptions::IOException;
	using Exceptions::StrategyException;
	
	
	/**
	 * Methods
	 */
	/**
	 * 
	 */
	bool ClientGameController::ClientGameController::start () throw (bad_exception) {
		// main exception block 
		try {
			_logger->logMessage(3,"Starting main method in ClientGameController for robot: "+_robotName);
			bool exitmainloop=false;
			const RTBParseResult* pr=0;
			string command;
			while(!exitmainloop) {
				// read initial messages from the RTB - Server, before we start entering a round
				command=_rtbConnection->getLine();
				pr=RTBParser::processRTBMessage(command);
				switch(pr->messageType) {
					case Parser::EXIT_ROBOT: {
						_logger->logMessage(5,"Received ExitRobot command, exiting....");
						exitmainloop=true;
						break;
					}
					case Parser::GAME_STARTS: {
						_logger->logMessage(5,"Received GameStart command, resetting coordinator...");
						_clientCoordinator->resetGame();
						break;
					}
					case Parser::ROBOTS_LEFT: {
						_logger->logMessage(5,"Received initial RobotsLeft command, delegating it to Coordinator...");
						RTBParser::delegateLastMessage(_clientCoordinator.get());
						break;

					}
					case Parser::GAME_FINISHES: {
						_logger->logMessage(5,"Received message, that game is over now (we have survived the last game 8-) )");
						break;
					}
					case Parser::GAME_OPTION: {
						_logger->logMessage(5,"Received robot option, updating GameOptionsRepository ...");
						RTBParser::updateGameOptions(&_gameOptionsRepository);
						// find out, if we can play with our team in this environment
						if (pr->intValue[0]==Parser::SEND_ROBOT_COORDINATES) {
							if (_gameOptionsRepository.SEND_ROBOT_COORDINATES!=double(2)) {
								_logger->logMessage(50,"Coordinates are not absolute, we cannot join the game 8-(");
								exitmainloop=true;
							}
							// now game is running, process round per round
							else {
								_logger->logMessage(10,"Game started...");
								bool gameisover=false;
								while (!gameisover) {
									// clear messagebuffer
									_messagebuffer.clear();
									_messagebuffer.str(_emptystring);
									_logger->logMessage(1,"Round started...");
									bool roundisover=false;
									while (!roundisover) {
										command=_rtbConnection->getLine();
										// append command to the messagebuffer
										_messagebuffer << command << endl;
										pr=RTBParser::processRTBMessage(command);
										switch (pr->messageType) {
											case Parser::ENERGY: {
												_logger->logMessage(5,"Received Energy command, round is finished now..");		
												roundisover=true;
												break;
											}
											case Parser::DEAD: {
												_logger->logMessage(5,"We are reported to be dead, exit current game...");
												roundisover=true;
												gameisover=true;
												break;
											}
											case Parser::WARNING: {
												_logger->logMessage(20,"Received warning from RTB Server: "+pr->stringValue);
												break;
											}
											case Parser::RADAR: {
												_logger->logMessage(4,"Received Radar command...");
												break;
											}
											case Parser::INFO: {
												_logger->logMessage(4,"Received Info command...");
												break;
											}
											case Parser::COORDINATES: {
												_logger->logMessage(4,"Received Coordinates command...");
												break;
											}
											case Parser::ROBOT_INFO: {
												_logger->logMessage(4,"Received Robot Info command...");
												break;
											}
											case Parser::ROTATION_REACHED: {
												_logger->logMessage(4,"Received Rotation Reached command...");
												break;
											}
	 										case Parser::ROBOTS_LEFT: {
												_logger->logMessage(4,"Received Robots Left command...");
												break;
											}
											case Parser::COLLISION:	{
												_logger->logMessage(4,"Received Collision command...");
												break;
											}
											default: {
												throw ParserException("Received command from RTBServer, that was not expected at this moment: "+command);	
											}
																					
										}
										// now delegate message to the coordinator
										_logger->logMessage(2,"Delegating message ...");
										RTBParser::delegateLastMessage(_clientCoordinator.get());				
									}
									// here, we send the messages to the master server
									_logger->logMessage(2,"Sending received messages  to the master server ...");
									_serverConnection->sendLine(_messagebuffer.str());
									// here, we inform the coordinator, that no further messages will arrive from the rtb server this round
									_logger->logMessage(2,"Informing ClientCoordinator that round is over ...");
									_clientCoordinator->processReceivedRTBMessages();
									if (!gameisover) {
										// now we read the servertips and give them to the coordinator
										_logger->logMessage(2,"Get servertips from the master server ...");
										while (_clientCoordinator->reactOnServerTip(_serverConnection->getLine()));		
										// new round will begin now
										_logger->logMessage(1,"Round finished...");
									}
								}
								_logger->logMessage(10,"Game finished...");
							}
						}
						break;
					}
					
					case Parser::WARNING: {
						_logger->logMessage(20,"Received warning from RTB Server: "+pr->stringValue);
						RTBParser::delegateLastMessage(_clientCoordinator.get());
						break;	
					}
					default: {
						throw ParserException("Received command from RTBServer, that was not expected at this moment: "+command);	
					}
				}
				
			}
		}
		catch (IOException& e) {
			_logger->logMessage(100,"Main loop in ClientGameController caught exception that was thrown because our Command Parser figured out a serious mistake: "+e.getMessage());
			_logger->logMessage(3,"Exiting ClientGameControl::start because of occured exception");
			return false;
		}
		
		catch (StrategyException& e) {
			_logger->logMessage(100,"Main loop in ClientGameController caught exception caused by a broken strategy component: "+e.getMessage());
			_logger->logMessage(3,"Exiting ClientGameControl::start because of occured exception");
			return false;
		}
		
		catch (ParserException& e) {
			_logger->logMessage(100,"Main loop in ClientGameController caught exception that was thrown by a rtb-framework component: "+e.getMessage());
			_logger->logMessage(3,"Exiting ClientGameControl::start because of occured exception");
			return false;
		}
		
		catch (RTBException& e) {
			_logger->logMessage(100,"Main loop in ClientGameController caught exception that was thrown by a rtb-framework component: "+e.getMessage());
			_logger->logMessage(3,"Exiting ClientGameControl::start because of occured exception");
			return false;
		}
		catch (ios_base::failure& e) {
			_logger->logMessage(100,"Main loop in ClientGameController caught exception caused by the stringbuffer used to store the received commands: "+string(e.what()));
			_logger->logMessage(3,"Exiting ClientGameControl::start because of occured exception");
			return false;

		}
		catch (exception& e) {
			_logger->logMessage(100,"Main loop in ClientGameController caught exception that was thrown by a STL component: "+string(e.what()));
			_logger->logMessage(3,"Exiting ClientGameControl::start because of occured exception");
			return false;
		}

		_logger->logMessage(3,"Exiting ClientGameControl::start because rtb server wants us to quit...");
		return true;
	}
	/**
	 * Constructors/Destructors
	 */
	/**
	 *
	 */
	 ClientGameController::ClientGameController (auto_ptr <ClientCommunicator> clientCommunicator) throw (RTBException, bad_exception) :_clientCoordinator(0),_clientCommunicator(clientCommunicator) {
		
		// get MRC
		_mrc=MasterResourceControl::Instance();
		// get Logger
		_logger=_mrc->createLogger("ClientGameController");
		_logger->logMessage(2,"Initializing ClientGameController (constructor)");
		// get name of robot
		_robotName=_mrc->getRuntimeProperty("Main","ActualName");
		// get the server connection
		_serverConnection=_clientCommunicator->getServerConnection();
		// obtain the RTB Connection
		_rtbConnection=_mrc->getIOFactory()->getRTBConnection();
		// create the client coordinator (finite state machine for the processing of the RTB messages)
		_clientCoordinator.reset(new ClientCoordinator(&_gameOptionsRepository, _rtbConnection));
		_messagebuffer.exceptions(ios_base::badbit|ios_base::failbit); 
	}
	/**
	 * Does nothing
	 */
	ClientGameController::~ClientGameController () throw() {

	}
}
