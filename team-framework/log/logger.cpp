/************************************************************************
    $Id: logger.cpp,v 1.2 2005/01/06 17:59:28 jonico Exp $
    
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

    $Log: logger.cpp,v $
    Revision 1.2  2005/01/06 17:59:28  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#include <common.h>
#include "logger.h"
#include <string>

/**
 * Constructors/Destructors
 */
/**
 *
 */
namespace Log {
	/** Initial the logger class. Copy states.
	 * @param section Define your section like "Strategy-Group:StupidBot"
	 * @param priority how uch should be logged. The Higher only important messages
	 *        will be logged
	 * @param logDriver use for example unixlogdriver to log the messages in a file
	 * @exception bad_exception 
	 */
	Logger::Logger(const string& section, int priority, auto_ptr <LogDriver> logDriver) throw(bad_exception): _logDriver(logDriver), _section(section), _priority(priority)  {
	}
	/**
	 * Do nothing
	*/
	Logger::~Logger () throw (){

	}
	/**
	 * Methods
	 */
	/** This method prove an incoming priority number with the internal priority state.
	 * @param priority the priority value to check
	 * @return true if the param priority is higher/equal than the internal 
	 *         priority state
	 */
	bool Logger::isPriorityHighEnough (int priority) throw() {
		return priority>=_priority;
	}
	
	/** Log the message. Uses the logdriver to log it. This will be logged:
	 *  <section>: <message>
	 * @param priority the higher the priority the number. Only messages will be 
	 * 		logged which are higher or equal the internal priority state
	 * @param message this string will be logged
	 * @exception IOException will be thrown if the logdriver can not write...
	*/
	void  Logger::logMessage (int priority, const string& message) throw (bad_exception, IOException) {
		if (isPriorityHighEnough(priority)) {
			try {
				_logDriver->logMessage(_section+": "+message);
			}
			catch (IOException& ex) {
				throw IOException ("A LogDriver IOException occured in logger for section: "+_section+": "+ex.getMessage());
			}
		}
	}
}
