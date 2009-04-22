/************************************************************************
    $Id: logger.h,v 1.2 2005/01/06 17:59:28 jonico Exp $
    
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

    $Log: logger.h,v $
    Revision 1.2  2005/01/06 17:59:28  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef LOGGER_H
#define LOGGER_H

#include "lognamespace.h"
#include "logdriver.h"
#include "../stdnamespace.h"
#include <memory>
#include "../exceptions/ioexception.h"


/**
 * Namespace Log
 */
namespace Log {
	using std::string;
	using std::auto_ptr;
	using std::bad_exception;
	using Exceptions::IOException;
	
	/**
	* Class Logger
	*/
	class Logger {
	/*
	* Public stuff
	*/
	public:
		/**
		* Constructor
		*/
		Logger(const string& section, int priority, auto_ptr <LogDriver> logDriver) throw(bad_exception);
		
		
		/*
		* Operations
		*/
		/**
		*
		* @param priority
		* @param message
		*/
		virtual void  logMessage (int priority, const string& message) throw(bad_exception, IOException);


		/**
		* Destructor, does nothing
		*/
		virtual  ~Logger () throw();


	/*
	* Protected stuff
	*/
	protected:
		/**
		*
		* @param priority the requested priority
		* @return true if priority high enough, otherwise false
		*/
		bool  isPriorityHighEnough (int priority) throw();


	/*
	* Private stuff
	*/
	private:
		/*
		* Fields
		*/
		auto_ptr <LogDriver> _logDriver;
		string _section;
		int _priority;
	};
}
#endif //LOGGER_H

