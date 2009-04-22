/************************************************************************
    $Id: nulllogdriver.h,v 1.2 2005/01/06 17:59:28 jonico Exp $
    
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

    $Log: nulllogdriver.h,v $
    Revision 1.2  2005/01/06 17:59:28  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef NULLLOGDRIVER_H
#define NULLLOGDRIVER_H


#include "logdriver.h"

/**
 * Namespace Log
 */
namespace Log {
	
	/** 
	* Class NullLogDriver
	*
	* Sometimes you want to prevent a component from logging anything. One thing you can try is to specify a very high logging priority, but you cannot sure that there won't be a message with even higher priority. So NullLogger is supposed to solve this problem while logging anything.
	*/
	class NullLogDriver: public LogDriver {
	/*
	* Public stuff
	*/
	public:
		/**
		* Destructor, does nothing
		*/
		virtual ~NullLogDriver () throw() {}
		
		
		/**
		* Constructor for the first and all following NullLogDriver object
		*/
		NullLogDriver() throw () {}
		
	
		/**
		* Method that is called by the MRC to duplicate the whole object
		*/
		virtual auto_ptr <LogDriver>  clone () const throw (bad_exception) {
			return auto_ptr<LogDriver>(new NullLogDriver());
		}
		
	
		/**
		* 
		* @param message Message that will be suppresed completely
		* If logger object was not initialized properly, this method will only throw an exception
		*/
		virtual void  logMessage (const string&) throw () {}
		
	
		/**
	 	 * 
		 * @param logDriverParameters this logger does not need any parameters
		 */
		virtual void  setLogDriverParameters (const string& logDriverParameters) throw (IOException, bad_exception) {}
	};
}
#endif

