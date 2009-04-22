/************************************************************************
    $Id: logdriver.h,v 1.2 2005/01/06 17:59:28 jonico Exp $
    
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

    $Log: logdriver.h,v $
    Revision 1.2  2005/01/06 17:59:28  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef LOGDRIVER_H
#define LOGDRIVER_H

#include <memory>
#include "../exceptions/ioexception.h"
#include "../stdnamespace.h"


/**
 * Namespace Log
 */
namespace Log {
	using std::auto_ptr;
	using Exceptions::IOException;
	using std::bad_exception;
	using std::string;

	/**
	* Class LogDriver
	*/ 
	class LogDriver {
	/*
	* Public stuff
	*/
	public:
		/**
		* Destructor, does nothing
		*/
		virtual ~LogDriver () throw () {};
		
		
		/*
		*  Operations
		*/
	
		/**
		* Method that is called by the MRC to duplicate the whole object
		*/
		virtual auto_ptr <LogDriver>  clone () const  throw (bad_exception) = 0;
		
	
		/**
		* 
		* @param message Message that will be logged
		*/
		virtual void  logMessage (const string& message) throw (bad_exception, IOException) = 0;
		
	
		/**
		* 
		* @param logDriverParameters Every driver can specify its own configuration parameters that will be passed with this string after the cloning process has taken place.
		*/
		virtual void  setLogDriverParameters (const string& logDriverParameters) throw (bad_exception, IOException)= 0;
		
	
	};
}
#endif //LOGDRIVER_H

