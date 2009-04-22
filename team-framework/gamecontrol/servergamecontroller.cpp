/*
    $Id: servergamecontroller.cpp,v 1.4 2005/10/07 07:33:51 krolfy Exp $
    
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

    $Log: servergamecontroller.cpp,v $
    Revision 1.4  2005/10/07 07:33:51  krolfy
    Fixed some problems

    Revision 1.3  2005/01/06 17:59:27  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#include <common.h>
#include "servergamecontroller.h"
#include "clientcoordinator.h"
#include "servercoordinator.h"
#include "../rtbglobal/masterresourcecontrol.h"
#include "../io/iofactory.h"
#include "../io/localclientconnection.h"
#include "../parser/rtbparser.h"
#include "../parser/rtbparser.h"
#include "../parser/rtbparseresult.h"
#include "../parser/message_to_robot_type.h"
#include "../parser/game_option_type.h"
#include "../io/rtbconnection.h"
#include "../exceptions/parserexception.h"
#include "../exceptions/ioexception.h"
#include "../exceptions/strategyexception.h"
#include "../exceptions/serverispresentexception.h"
#include "coordinatorcomposite.h"



namespace GameControl {
	using Parser::RTBParser;
	using Parser::RTBParseResult;
	using Parser::game_option_type;
	using std::exception;
	using IO::LocalClientConnection;
	using Exceptions::ParserException;
	using Exceptions::IOException;
	using Exceptions::StrategyException;
	using Exceptions::ServerIsPresentException;
	

	/**
	* Methods
	*/
	
	void ServerGameController::resetAll() {
		for (pvector<Coordinator>::iterator i=_coordinators.begin();i!=_coordinators.end();++i)
			(*i)->resetGame();

	}
	
	void ServerGameController::delegateToAll() {
		for (pvector<Coordinator>::iterator i=_coordinators.begin();i!=_coordinators.end();++i)
			RTBParser::delegateLastMessage(*i);

	}
	
	/**
	* 
	*/
	bool ServerGameController::start () throw (bad_exception) {
		// main exception block 
		try {
			_logger->logMessage(3,"Starting main method in ServerGameController");
			bool exitmainloop=false;
			const RTBParseResult* pr=0;
			string command;
			while(!exitmainloop) {
				// set all clients to live again
				setAlive();
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
						resetAll();
						break;
					}
					case Parser::ROBOTS_LEFT: {
						_logger->logMessage(5,"Received initial RobotsLeft command, delegating it to Coordinator...");
						delegateToAll();
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
								_logger->logMessage(10,"Starting new game ...");
								while (true) {
									_logger->logMessage(1,"Started round ...");
									pvector<Coordinator>::iterator currentc=_coordinators.begin();
									vector<bool>::iterator currentstat=_alive.begin();
									vector<string>::iterator currentname=_robotNames.begin();
									pvector<ClientConnection>::iterator currentcon=_clientConnections.begin();
									// process all robots in forward direction
									while (currentc!=_coordinators.end()) {
										if (*currentstat) {
											_logger->logMessage(7,"Process round for robot: "+(*currentname));
											bool roundisover=false;
											while (!roundisover) {
												command=(*currentcon)->getLine();
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
														*currentstat=false;
														--_numberAliveRobots;
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
												RTBParser::delegateLastMessage(*currentc);				
											}
	
										}
										// next robot
										++currentc;
										++currentname;
										++currentstat;
										++currentcon;
									}
									// all robots are dead, game is over now
									if (!_numberAliveRobots)
										break;
										
									// now generate server tips for alive robots
									do {
										// next robot
										--currentc;
										--currentname;
										--currentstat;
										
										if (*currentstat) {
											_logger->logMessage(7,"Generate server tips for robot: "+(*currentname));
											(*currentc)->processReceivedRTBMessages();
										}

										
									} while (currentc!=_coordinators.begin());
									_logger->logMessage(1,"Finished round ...");				
								}
								_logger->logMessage(10,"Finished game ...");
							}
						}
						break;
					}
					
					case Parser::WARNING: {
						_logger->logMessage(20,"Received warning from RTB Server: "+pr->stringValue);
						RTBParser::delegateLastMessage(*(_coordinators.begin()));
						break;	
					}
					default: {
						throw ParserException("Received command from RTBServer, that was not expected at this moment: "+command);	
					}
				}
				
			}
		}
		catch (IOException& e) {
			_logger->logMessage(100,"Main loop in ServerGameController caught exception that was thrown because our Command Parser figured out a serious mistake: "+e.getMessage());
			_logger->logMessage(3,"Exiting ServerGameControl::start because of occured exception");
			return false;
		}
		
		catch (StrategyException& e) {
			_logger->logMessage(100,"Main loop in ServerGameController caught exception caused by a broken strategy component: "+e.getMessage());
			_logger->logMessage(3,"Exiting ServerGameControl::start because of occured exception");
			return false;
		}
		
		catch (ParserException& e) {
			_logger->logMessage(100,"Main loop in ServerGameController caught exception that was thrown by a rtb-framework component: "+e.getMessage());
			_logger->logMessage(3,"Exiting ServerGameControl::start because of occured exception");
			return false;
		}
		
		catch (RTBException& e) {
			_logger->logMessage(100,"Main loop in ServerGameController caught exception that was thrown by a rtb-framework component: "+e.getMessage());
			_logger->logMessage(3,"Exiting ServerGameControl::start because of occured exception");
			return false;
		}
		catch (exception& e) {
			_logger->logMessage(100,"Main loop in ServerGameController caught exception that was thrown by a STL component: "+string(e.what()));
			_logger->logMessage(3,"Exiting ServerGameControl::start because of occured exception");
			return false;
		}

		_logger->logMessage(3,"Exiting ServerGameControl::start because rtb server wants us to quit...");
		return true;
	}


	/**
	* Constructors/Destructors
	*/

	/**
	* 
	*/
	ServerGameController::ServerGameController (auto_ptr <ServerCommunicator> serverCommunicator) throw (RTBException, bad_exception):_rtbConnection(0),_serverCommunicator(serverCommunicator),_logger(0),_numberRobots(0),_numberAliveRobots(0), _localClientCoordinator(0),_localServerCoordinator(0) {
		// get MRC
		_mrc=MasterResourceControl::Instance();
		// get Logger
		_logger=_mrc->createLogger("ServerGameController");
		_logger->logMessage(2,"Initializing ServerGameController (constructor)");
		// obtain the RTB Connection
		_rtbConnection=_mrc->getIOFactory()->getRTBConnection();
		_logger->logMessage(3,"Initializing the local client ...");
		// create the local client coordinator (finite state machine for the processing of the RTB messages)
		_localClientCoordinator.reset(new ClientCoordinator(&_gameOptionsRepository, _rtbConnection));
		// create the local client connection
		_clientConnections.push_back(new LocalClientConnection(_rtbConnection,_localClientCoordinator.get()));
		// create the local server coordinator
		_localServerCoordinator.reset(new ServerCoordinator(*(_clientConnections.begin()),&_gameOptionsRepository));
		// finally, create the coordinator composite
		_coordinators.push_back(new CoordinatorComposite(_localClientCoordinator.get(),_localServerCoordinator.get()));
		// set the name of the local robot
		_robotNames.push_back((*_clientConnections.begin())->getTargetName());
		// add a field in the alive stats
		_alive.push_back(true);
		// increment number of robots
		++_numberRobots;
		_logger->logMessage(10,"Waiting for remote clients ...");
		
	
		try {
			while(true) {
				auto_ptr<ClientConnection> rc(_serverCommunicator->createRemoteClientConnection());
				string tn=rc->getTargetName();
				_logger->logMessage(10,"Adding connection from robot: "+tn);
				_robotNames.push_back(tn);
				_coordinators.push_back(new ServerCoordinator(rc.get(),&_gameOptionsRepository));
				_clientConnections.push_back(rc.release());
				// add a field in the alive stats
				_alive.push_back(true);
				++_numberRobots;
			}
		}
		
		catch (ServerIsPresentException& e) {
			_logger->logMessage(10,"Finished waiting for remote clients, starting game now ...");	
		}
	}
	
	void ServerGameController::setAlive() {
		for (vector<bool>::iterator i=_alive.begin();i!=_alive.end();++i)
			*i=true;
		_numberAliveRobots=_numberRobots;
	}
	
	/**
	* 
	*/
	ServerGameController::~ServerGameController () throw() {
	
	}
}
