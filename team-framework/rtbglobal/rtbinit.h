/************************************************************************
    $Id: rtbinit.h,v 1.2 2005/01/06 17:59:29 jonico Exp $
    
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

    $Log: rtbinit.h,v $
    Revision 1.2  2005/01/06 17:59:29  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef RTBINIT_H
#define RTBINIT_H
#include "../stdnamespace.h"
#include "../exceptions/parserexception.h"
#include "../exceptions/ioexception.h"

/**
* Namespace RTBGlobal
*/namespace RTBGlobal {
	using std::string;
	using Exceptions::ParserException;
	using std::bad_exception;
	using Exceptions::RTBException;
	using Exceptions::IOException;
	 
	/**
	* Class RTBInit
	*/
	class RTBInit {
	/*
	* Public stuff
	*/
	public:
		/*
		* Operations
		*/
		/**
		* Calls the RTBConfig Parser to parse the config file, register the pasring result in the MRC
		* @param configFileName Path to the file containing the absolute necessary configuration data
		*/
		static void  ParseConfigFile (const string& configFileName) throw (ParserException, bad_exception);
		
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
		static void  SendInitialMessages() throw (IOException, bad_exception);
	
		/**
	 	 * Starts the game as a client or as the RTB MasterServer (automatically found out by the MRC)
		 * @return true, if no error occured in the whole sequence, false if something went wrong and we could not continue playing
		 */
		static bool  StartGame () throw (RTBException, bad_exception);
	};
}
#endif //RTBINIT_H

