/************************************************************************
    $Id: rtbinit.cpp,v 1.4 2005/08/17 19:19:33 jonico Exp $
    
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

    $Log: rtbinit.cpp,v $
    Revision 1.4  2005/08/17 19:19:33  jonico
    Introduced new option in IO section to facilitate blocking / non-blocking problem

    Revision 1.3  2005/06/29 15:53:04  jonico
    changed my email address

    Revision 1.2  2005/01/06 17:59:29  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#include <common.h>
#include "rtbinit.h"
#include <string>
#include "../exceptions/confignotloadedexception.h"
#include "../exceptions/keynotfoundexception.h"
#include "../io/iofactory.h"
#include "../io/rtbconnection.h"
#include "../parser/rtbparser.h"
#include "../parser/robot_option_type.h"
#include "../parser/message_to_robot_type.h"
#include "../parser/rtbparseresult.h"
#include <sstream>
#include "masterresourcecontrol.h"
#include "../gamecontrol/gamecontroller.h"
#include <ios>
#include "../parser/configurationparser.h"

namespace RTBGlobal {
	using Exceptions::RTBException;
	using Exceptions::KeyNotFoundException;
	using Exceptions::ConfigNotLoadedException;
	using IO::IOFactory;
	using IO::RTBConnection;
	using std::endl;
	using Parser::RTBParser;
	using Parser::ConfigurationParser;
	using Parser::RTBParseResult;
	using std::ostringstream;
	using std::istringstream;
	using std::ios_base;
	
	/**
	* Methods
	*/
	/**
	* 
	*/
	void RTBInit::ParseConfigFile (const string& configFileName) throw (ParserException, bad_exception) {
		ConfigurationParser parser;
		parser.addFile(configFileName);
		MasterResourceControl::Instance()->setConfigurationData(parser.getConfigurationData());	
	}
	/**
	* This method first has to switch to blocking mode
	* This method has to send:
	* 1. The Robot Option USE_NON_BLOCKING 1 (this is tricky, we do not want to be killed by the RTB Server, so we chose non blocking for it, but set our descriptors to block
	* 2. The Robot Option SEND_ROTATION_REACHED with a specified value in the configuration file
	* 3. the name and the color of the robot if requested (first sequence)
	* This method has to read:
	* 1. The Initialize Command
	* 2. If it is not the first sequence in the tournament, the YourColour and YourNameCommand
	* This methods registers the following runtime properties:
	* 1. Section Main, Key FirstSequence : "true" / "false"
	* 2. Section Main, Key ActualColor : actual color as hex string, if first sequence, this value will be set to RobotHomeColor in section Main of the configuration file
	* 3. Section Main, Key ActualName: actual name of the roboter given by the server, if first sequence, this value will be set to RobotName in section Main of the configuration file
	*/
	void RTBInit::SendInitialMessages () throw (IOException, bad_exception) {
		try {
			// first obtain a pointer to the IOFactory
			MasterResourceControl* mrc(MasterResourceControl::Instance());
			const IOFactory* ioFactory(mrc->getIOFactory());
			// should we use fcntl to make our connection block ?
			string shouldUseFcntl(mrc->getConfigurationProperty("IO","UseFcntl"));
			if (shouldUseFcntl=="true")
				// now switch to blocking mode
				 ioFactory->switch2BlockingMode();
			// now state the opposite to the RealTime Battle Server
			RTBConnection* rtbConnection(ioFactory->getRTBConnection());
			// we need a ostringstream to generate our commands to the RealTime Battle Server
			ostringstream commands;
			// set exception mask
			commands.exceptions(ios_base::badbit|ios_base::failbit);
			if (shouldUseFcntl=="true")
			// first state that we won't block (we do so, but we do not want to get restarted)
				commands << "RobotOption " << Parser::USE_NON_BLOCKING << " " << 1 << endl; 
			else
				commands << "RobotOption " << Parser::USE_NON_BLOCKING << " " << 0 << endl; 
			// now retrieve the value for RotationReachedFacility
			string rotFacilityStr(mrc->getConfigurationProperty("Main","RotationReachedValue"));
			int rotFacility(-1);
			istringstream numberParser(rotFacilityStr);
			numberParser >> rotFacility;
			if (rotFacility<0 || rotFacility >2)
				throw ParserException("Key RotationReachedValue has no value between 0 and 2 but :"+rotFacilityStr);
			commands << "RobotOption " << Parser::SEND_ROTATION_REACHED << " " << rotFacility << endl;
			string firstCommand(rtbConnection->getLine());
			const RTBParseResult* pr=RTBParser::processRTBMessage(firstCommand);
			if (pr->messageType!=Parser::INITIALIZE)
				throw ParserException("First command was not INITIALIZE but: "+firstCommand);
			// find out, if we are in the first sequence
			switch (pr->intValue[0]) {
				case 0: { // it is not the first sequence, set runtime properties
					mrc->setRuntimeProperty("Main","FirstSequence","false");
					// read YourName and YourColour
					string secondCommand(rtbConnection->getLine());
					pr=RTBParser::processRTBMessage(secondCommand);
					if (pr->messageType!=Parser::YOUR_NAME)
						throw ParserException("Second command was not YourName but: "+secondCommand);
					mrc->setRuntimeProperty("Main","ActualName",pr->stringValue);
					string thirdCommand(rtbConnection->getLine());
					pr=RTBParser::processRTBMessage(thirdCommand);
					if (pr->messageType!=Parser::YOUR_COLOUR)
						throw ParserException("Third command was not YourColour but: "+thirdCommand);
					ostringstream numberGenerator;
					numberGenerator.setf(ios_base::hex,ios_base::basefield);
					numberGenerator << pr->intValue[0];
					mrc->setRuntimeProperty("Main","ActualColor",numberGenerator.str());
					break;
				}
				case 1: { // it is the first sequence, send our color and name to the RTBServer
					mrc->setRuntimeProperty("Main","FirstSequence","true");
					// now we have to find out our name and color
					string robotName(mrc->getConfigurationProperty("Main","RobotName"));
					mrc->setRuntimeProperty("Main","ActualName",robotName);
					string robotHomeColorString(mrc->getConfigurationProperty("Main","RobotHomeColour"));
					string robotAwayColorString(mrc->getConfigurationProperty("Main","RobotAwayColour"));
					// parse Color
					numberParser.setf(ios_base::hex,ios_base::basefield);
					numberParser.clear();
					numberParser.str(robotHomeColorString);
					int robotHomeColor(-1);
					numberParser >> robotHomeColor;
					if (robotHomeColor==-1)
						throw ParserException("RobotHomeColour in section Main was no proper value but: "+robotHomeColorString);
					// register HomeColor as ActualColor in Runtime Properties
					mrc->setRuntimeProperty("Main","ActualColor", robotHomeColorString);
					numberParser.clear();
					numberParser.str(robotAwayColorString);
					int robotAwayColor(-1);
					numberParser >> robotAwayColor;
					if (robotAwayColor==-1)
						throw ParserException("RobotAwayColour in section Main was no proper value but: "+robotAwayColorString);
					// construct commands
					commands << "Name " << robotName << endl;
					commands.setf(ios_base::hex,ios_base::basefield);
					commands << "Colour " << robotHomeColor << " " << robotAwayColor << endl;
					break;
				}
				default: 
					throw ParserException("First command was no proper INITIALIZE command (wrong parameter): "+firstCommand);
			}
			// finally write commands to RealTime Battle Server
			rtbConnection->sendLine(commands.str());
		}
		catch (ResourceNotFoundException& ex) {
				throw IOException("RTBInit::SendInitialMessages was not able to obtain the IOFactory object: "+ex.getMessage());
		}
		catch (ParserException& ex) {
			throw IOException("Unexpected data received from the RealTime Battle Server in RTBInit::sendInitialMessages: "+ex.getMessage());
		}
		catch (ConfigNotLoadedException& ex) {
			throw IOException("RTBInit::SendInitialMessages was not able to obtain a key needed for initial communication setup  because no configuration data was set: "+ex.getMessage());
		
		}
		catch (KeyNotFoundException& ex) {
			throw IOException("RTBInit::SendInitialMessages was not able to obtain key needed for initial communication setup because key was not present in configuration data: "+ex.getMessage());
		}
		// our ostringstream encountered an error
		catch (ios_base::failure& ex) {
			throw IOException(string("RTBInit::SendInitialMessages : Could not create command for RTB!")+ex.what());
			
		}
		catch (IOException& ex) {
			throw IOException("RTBInit::SendInitialMessages was not able to properly communicate with the RealTime Battle server: "+ex.getMessage());
		}
	}
	
	/**
	* Starts the game as a client or as the RTB MasterServer (automatically found out by the MRC)
	* @return true, if no error occured in the whole sequence, false if something went wrong and we could not continue playing
	*/
	bool RTBInit::StartGame () throw (bad_exception, RTBException) {
		return MasterResourceControl::Instance()->getGameController()->start();
	}
}
