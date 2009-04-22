/************************************************************************
    $Id: sysloglogdriver.h,v 1.2 2005/01/06 17:59:28 jonico Exp $
    
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

    $Log: sysloglogdriver.h,v $
    Revision 1.2  2005/01/06 17:59:28  jonico
    Now all files in the repository have their new header format.


**************************************************************************/
#ifndef SYSLOGLOGDRIVER_H
#define SYSLOGLOGDRIVER_H

#include <map>
#include <string>
#include <memory>
#include <syslog.h>
#include "../exceptions/ioexception.h"
#include "logdriver.h"

namespace Log {

	using std::map;
	using std::string;
	using std::auto_ptr;
	using Exceptions::IOException;
	
	/**
	* Class SyslogLogDriver
	*/
	class SyslogLogDriver: public LogDriver {
	private:
		SyslogLogDriver(const string&) throw (IOException, bad_exception);
		/**
		* contains the number of instances
		*/
		SyslogLogDriver(const string& syslogIdent, int syslogOption, int syslogPriority) throw(bad_exception);
		
		/**
		* contains the number of instances
		*/
		static int _instanceCount;

		/**
		* true, if the syslog connection is opened
		*/
		static bool _logOpened;
	
		/**
		* true, if the maps _options and _priorities are initialized
		*/
		static bool _mapsInitialized;

		/**
		* true, if this instance is initialized (i.e. the log driver
		* parameters are set)
		*/
		bool _instanceInitialized;

		/**
		* the number of the instance that logged the last time
		*/
		static int _lastLoggingInstance;
		static int _lastLoggingSyslogOption;
		static string _lastLoggingSyslogIdent;

		/**
		* each instance gets a unique number
		*/
		int _instance;

		int _syslogOption;
		string _syslogIdent;
		int _syslogPriority;
		static map<string, int> _options;
		static map<string, int> _priorities;
		int parseOption(const string&) throw (IOException,bad_exception);
		int parsePriority(const string&) throw (IOException,bad_exception);
		map<string, string> parseParameterString(const string&) throw (IOException,bad_exception);
		string trim(const string&) throw (bad_exception);
		void init() throw (bad_exception);
		void openlog() throw ();
	public:
		SyslogLogDriver() throw (bad_exception);
		/** 
		* extract ident, option and priority from logDriverParameters 
		* parameter format: <add it here >
		* default: "option: LOG_CONS|LOG_PID, ident: Realtimebattle, priority: LOG_INFO"
		*/
		void setLogDriverParameters(const string&) throw (IOException, bad_exception); 
		void setLogDriverParameters(const string& syslogIdent, int syslogOption, int syslogPriority) throw();
		void logMessage(const string&) throw (IOException,bad_exception);
		auto_ptr<LogDriver> clone() const throw (bad_exception);
		~SyslogLogDriver() throw();
	};

}

#endif /* SYSLOGLOGDRIVER_H */
