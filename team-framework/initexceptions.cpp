/************************************************************************
    $Id: initexceptions.cpp,v 1.2 2005/01/06 17:59:18 jonico Exp $
    
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

    $Log: initexceptions.cpp,v $
    Revision 1.2  2005/01/06 17:59:18  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#include <common.h>


#include <exception>
#include <string>
#include "rtbglobal/masterresourcecontrol.h"
#include <stdlib.h>


/**
 * NOTE: These file contains the uncaught exception handler and the unexpected exception handler
 * DO NOT FORGET TO LINK THE CORRESPONDING OBJECT CODE TO THE RTB - BINARY IF YOU WANT PROPER EXCEPTION HANDLING
 */
namespace { // anonymous namespace, all things in here are solely for exception handling purposes
	using std::bad_exception;
	using std::exception;
	using std::string;
	using RTBGlobal::MasterResourceControl;

	
	class __Extended_bad_exception: public bad_exception { // the origninal bad_exception is not capable of saving information about the unexpected exception, this one solves this issue
	
		public:
			__Extended_bad_exception(const string& ex):reason("bad_exception: "+ex) {};
			virtual const char* what() const throw () { return reason.c_str();};
			virtual ~__Extended_bad_exception() throw() {};   
		private:
			string reason;
		
	};
	
	// global unexpected exception handler
	void rtb_unexpected_exception_handler() throw (bad_exception) 
	{
		// try to figure out what type of unexpected exception occured
		try {
			throw;
		}
		catch (std::exception& ex) {
			// we were able to identify the unexpected exception
			throw __Extended_bad_exception("An unexpected (identified) exception has appeared: "+string(ex.what()));
		}
		catch (...) {
			throw __Extended_bad_exception(string("An unexpected (unidentified) exception has appeared!"));
		}
	}

	// global uncaught exception handler
	void rtb_uncaught_exception_handler() throw() 
	{
		// try to figure out what type of exception occured
		try {
			throw;
		}
		catch (std::exception& ex) {
			// we were able to identify the unexpected exception
			MasterResourceControl::EmergencyLog("An uncaught (identified) exception has appeared: "+string(ex.what()));
			abort();
		}
		catch (...) {
			MasterResourceControl::EmergencyLog(string("An uncaught (unidentified) exception has appeared!"));
			abort();
		}	
	}

	
	
	class __RTBHandleSetter { // class to set the exception handler callback methods
		public: 
			__RTBHandleSetter() {
				std::set_unexpected(&rtb_unexpected_exception_handler);
				std::set_terminate(&rtb_uncaught_exception_handler);
			}
			~__RTBHandleSetter() throw() {};
	};
	__RTBHandleSetter __initialHandleSetter; // tries to set exception handler even before main method will start
			
}
