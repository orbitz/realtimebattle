/************************************************************************
    $Id: rtblogdriver.h,v 1.2 2005/01/06 17:59:28 jonico Exp $
    
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

    $Log: rtblogdriver.h,v $
    Revision 1.2  2005/01/06 17:59:28  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef RTBLOGDRIVER_H
#define RTBLOGDRIVER_H


#include "logdriver.h"
#include "../io/ionamespace.h"

/**
 * Namespace Log
 */
namespace Log {
	enum LogState {UNINITIALIZED, BOTH, PRINT, DEBUG};
	using IO::RTBConnection;
	
	/**
	* Class RTBLogDriver
	*
	* RTBLogdriver will log using the Debug or the Print Command (depends on the logParameters)
	* of the RealTime Battle Server to log messages
	* WARNING: We use blocking IO for our framework, so it is possible, that logging with this driver will block the
	* whole system. Use it with caution and when blocking is no risk for you 
	*/
	class RTBLogDriver: public LogDriver {
	/*
	* Public stuff
	*/
	public:
		/**
		* Destructor
		* Does nothing
		*/
		virtual ~RTBLogDriver () throw();
		
		
		/**
		* Constructor for the first rtbLogDriver object
		* Constructor will register the object by the MRC with the name rtbLogDriver
		*/
		RTBLogDriver() throw (bad_exception);
		
		/*
		*  Operations
		*/
	
		/**
		* Method that is called by the MRC to duplicate the whole object
		*/
		virtual auto_ptr <LogDriver>  clone () const throw (bad_exception);
		
	
		/**
		* 
		* @param message Message that will be logged in the file
		* If logger object was not initialized properly, this method will only throw an exception
		*/
		virtual void  logMessage (const string& message) throw (IOException, bad_exception);
		
	
		/**
	 	 * 
		 * @param logDriverParameters The parameter has to be in the form logCommand:Debug or logCommand:Print or logCommand:Both
		 * logCommand:Debug : Use the Debug coomand to log messages
		 * logCommand:Print : Use the Print command to log messages
		 * logCommand:Both  : Use the Print and the Debug command to log messages (questionable option)		
		 */
		virtual void  setLogDriverParameters (const string& logDriverParameters) throw (IOException, bad_exception);
	
	/*
	* Private stuff
	*/
	private:
		/**
		 * private constructor used to construct cloned prototypes
		 */
		RTBLogDriver(RTBConnection* rtbConnection, LogState logState) throw();
		
		
		/**
		 * Pointer to the connection to the RealTime Battle Server
		 */
		RTBConnection* _rtbConnection;
		LogState _rtbLogState;
	
	};
}
#endif //RTBLOGDRIVER_H

