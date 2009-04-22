/************************************************************************
    $Id: main.cpp,v 1.3 2005/05/12 14:49:46 krolfy Exp $
    
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

    $Log: main.cpp,v $
    Revision 1.3  2005/05/12 14:49:46  krolfy
    Spelling mistake

    Revision 1.2  2005/01/06 17:59:18  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#include <common.h>

#include "rtbglobal/rtbinit.h"
#include "rtbglobal/masterresourcecontrol.h"
#include "log/logger.h"
#include "stdnamespace.h"
#include <string>
#include <memory>
#include "initcomponents.h"

using std::string;
using std::exception;
using std::auto_ptr;
using RTBGlobal::MasterResourceControl;
using std::auto_ptr;
using Log::Logger;
using RTBGlobal::RTBInit;

/**
 * prints out how to invoke this program
 * @param name: Name of the program
 */
void usage(const string& name) {
	MasterResourceControl::EmergencyLog(name+": Robots built with the RTB- Team Framework have to be started with a configuration file as parameter!"); 
}

/**
 * global start method (very short)
 * @param argv Takes one argument: path to the configuration file
 */
int main (int argc, char** argv) throw() 
{
	// first find out, if we was invoked with the right parameter
	if (argc!=2) {
		usage(*argv);
		return 1;
	}
	// at the very beginning, initialize the pluggable components
	try {
		initComponents();
	}
	catch (exception& ex) {
		MasterResourceControl::EmergencyLog(string("Identified Exception occured in initComponents: ")+ex.what());
		return 1;
	}
	
	// first parse config file
	try {
		RTBInit::ParseConfigFile(argv[1]);
	}
	catch (exception& ex) {
		MasterResourceControl::EmergencyLog(string("Exception occured in RTBInit::ParseConfigFile: ")+ex.what());
		return 1;
	}
	// second try to get a real logger to avoid sending messages via EmergencyLog
	auto_ptr<Logger> logger(0);
	try {
		// we are section main
		logger=MasterResourceControl::Instance()->createLogger("Main");
	}
	catch (exception& ex) {
		MasterResourceControl::EmergencyLog("We could not get our logger for section main: "+string(ex.what()));
		return 1;
	}
	try { // try block for exceptions thrown by the logger itself
		// third send the initial messages to initialize the connection to the RealTime Battle Server
		try {
			logger->logMessage(3,"Send initial messages to RTB Server ...");
			RTBInit::SendInitialMessages();
			logger->logMessage(2,"Finished sending initial messages to RTB Server ...");
			MasterResourceControl* mrc= MasterResourceControl::Instance();
			logger->logMessage(2,"First sequence in tournament: "+mrc->getRuntimeProperty("Main","FirstSequence"));
			logger->logMessage(2,"Actual name in this sequence: "+mrc->getRuntimeProperty("Main","ActualName"));
			logger->logMessage(2,"Actual color in this sequence: "+mrc->getRuntimeProperty("Main","ActualColor"));
		}
		catch (exception& ex) {
			// priority 100 should be high enough
			logger->logMessage(100,"An exception occured in RTBInit::SendInitialMessages: "+string(ex.what()));
			return 1;
		}
		// last, start the game as client or master server
		try {
			logger->logMessage(3,"Start main loop now ...");
			bool success=RTBInit::StartGame();
			if (success)
				logger->logMessage(3,"Sequence finished successfully. Shutting down now.");
			else
				logger->logMessage(3,"While playing, an unrecoverable error occured. Shutting down now.");
			return !success;
		}
		catch (exception& ex) {
			// priority 100 should be high enough
			logger->logMessage(100,"An exception occured in RTBInit::StartGame: "+string(ex.what()));
			return 1;
		}
	}
	catch (exception& ex) {
		MasterResourceControl::EmergencyLog("Logger for section main throwed an exception: "+string(ex.what()));
		return 1;
	}
}
