/************************************************************************
    $Id: sysloglogdriver.cpp,v 1.3 2005/02/22 20:12:37 jonico Exp $
    
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

    $Log: sysloglogdriver.cpp,v $
    Revision 1.3  2005/02/22 20:12:37  jonico
    Changed some minor behavior to let the framework work together with Solaris

    Revision 1.2  2005/01/06 17:59:28  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#include <common.h>
#include <set>
#include "sysloglogdriver.h"

namespace Log {
	using std::set;

	int SyslogLogDriver::_instanceCount=0;
	bool SyslogLogDriver::_logOpened=false;
	bool SyslogLogDriver::_mapsInitialized=false;
	int SyslogLogDriver::_lastLoggingInstance;
	int SyslogLogDriver::_lastLoggingSyslogOption;
	string SyslogLogDriver::_lastLoggingSyslogIdent;
	map<string, int> SyslogLogDriver::_options;
	map<string, int> SyslogLogDriver::_priorities;

	SyslogLogDriver::SyslogLogDriver() throw (bad_exception)
	{
		init();

		// _instanceInitialized is NOT set true, because the LogDriver parameters
		// have not been set yet.
	}

	SyslogLogDriver::SyslogLogDriver(const string& logDriverParameters) throw
		(IOException,bad_exception)
	{
		init();
		setLogDriverParameters(logDriverParameters);
	}

	SyslogLogDriver::SyslogLogDriver(const string& syslogIdent, int syslogOption, int syslogPriority) throw (bad_exception)
	{
		init();
		setLogDriverParameters(syslogIdent, syslogOption, syslogPriority);
	}

	SyslogLogDriver::~SyslogLogDriver() throw()
	{
		if(_logOpened && _instanceCount==1) { closelog(); _logOpened=false; }
		--_instanceCount;
	}

	void SyslogLogDriver::setLogDriverParameters(const string& syslogIdent, int syslogOption, int syslogPriority) throw() 
	{
		_syslogOption=syslogOption;
		_syslogIdent=syslogIdent;
		_syslogPriority=syslogPriority;
		_instanceInitialized=true;
	}

	void SyslogLogDriver::setLogDriverParameters(const string& logDriverParameters) throw
		(IOException,bad_exception)
	{
	
		string syslogIdent;
		int syslogOption, syslogPriority;
	
		
		map<string, string> parameters=parseParameterString(logDriverParameters);

		if(parameters.find("option")==parameters.end()) {
			// set default value for option
			syslogOption=LOG_CONS|LOG_PID;
		} else {
			syslogOption=parseOption(parameters["option"]);
		}

		if(parameters.find("ident")==parameters.end()) {
			// set default value for ident
			syslogIdent="RTB-Team";
		} else {
			syslogIdent=trim(parameters["ident"]);
		}

		if(parameters.find("priority")==parameters.end()) {
			// set default value for priority
			syslogPriority=LOG_INFO;
		} else {
			syslogPriority=parsePriority(parameters["priority"]);
		}
	
		set<string> validParameters;
		validParameters.insert("option");
		validParameters.insert("ident");
		validParameters.insert("priority");
		for(map<string, string>::iterator i=parameters.begin();i!=parameters.end();++i) {
			if(validParameters.find(i->first)==validParameters.end()) {
				throw IOException(string("Invalid parameter in SyslogLogDriver parameters: '")+(i->first)+"'!");
			}
		}

		setLogDriverParameters(syslogIdent, syslogOption, syslogPriority);
	}


	// the common code of all constructors
	void SyslogLogDriver::init() throw (bad_exception)
	{
		_instanceInitialized=false;

		if(!_mapsInitialized) {
			// valid options
			_options["LOG_CONS"]=LOG_CONS;
			_options["LOG_NDELAY"]=LOG_NDELAY;
			// this option exists on almost every unix #ifdef OS_LINUX
			// _options["LOG_PERROR"]=LOG_PERROR;
			// this option exist on almost every unix #endif
			_options["LOG_PID"]=LOG_PID;
		
			// valid priorities
			_priorities["LOG_EMERG"]=LOG_EMERG;
			_priorities["LOG_ALERT"]=LOG_ALERT;
			_priorities["LOG_CRIT"]=LOG_CRIT;
			_priorities["LOG_ERR"]=LOG_ERR;
			_priorities["LOG_WARNING"]=LOG_WARNING;
			_priorities["LOG_NOTICE"]=LOG_NOTICE;
			_priorities["LOG_INFO"]=LOG_INFO;
			_priorities["LOG_DEBUG"]=LOG_DEBUG;

			_mapsInitialized=true;
		}
	
		_instance=_instanceCount;
		++_instanceCount;
	}

	auto_ptr<LogDriver> SyslogLogDriver::clone() const throw (bad_exception)
	{
	
		// the returned SyslogLogDriver instance should be initialized if this
		// instance is initialized already
		if(_instanceInitialized)
			return auto_ptr<LogDriver>(new SyslogLogDriver(_syslogIdent, _syslogOption, _syslogPriority));
		else 
			return auto_ptr<LogDriver>(new SyslogLogDriver());

	}

	void SyslogLogDriver::openlog() throw()
	{
		::openlog(_syslogIdent.c_str(), _syslogOption, LOG_USER);
		_lastLoggingSyslogOption=_syslogOption;
		_lastLoggingSyslogIdent=_syslogIdent;
		_lastLoggingInstance=_instance;
		_logOpened=true;
	}

	void SyslogLogDriver::logMessage(const string& message) throw
			(IOException, bad_exception)
	{

		if(!_instanceInitialized) {
			throw IOException("SyslogLogDriver: Cannot log: LogDriver parameters are not set yet!");
		}
	
		if(!_logOpened) openlog();

		if(_lastLoggingInstance!=_instance) {
			if(_syslogOption!=_lastLoggingSyslogOption ||
					_syslogIdent!=_lastLoggingSyslogIdent) {
			
				// The instance that logged the last time had a different option- or
				// ident value. So we have to reopen the syslog connection to set
				// our values.
				closelog();
				openlog();
			}
		}
	
		syslog(_syslogPriority, message.c_str());
	}

	map<string, string> SyslogLogDriver::parseParameterString(const string& parameters) throw	
		(IOException, bad_exception)
	{
		// States:
		// BEFOREKEY
		// PARAMKEY
		// PARAMVALUE
		// BEFORECOLON
		int BEFOREKEY=0, PARAMKEY=1, PARAMVALUE=2, BEFORECOLON=3;
		int state;
		map<string, string> parsed;
		string key="";
		string value="";
		state=BEFOREKEY;
		for(int i=0;i<parameters.size();++i) {
			if(state==PARAMKEY) {
				if(isalpha(parameters[i])) {
					key.append(1, parameters[i]);
				} else if(parameters[i]==':') {
					value="";
					state=PARAMVALUE;
				} else if(isspace(parameters[i])) {
					// The key is complete. Now we have to 
					// check, if the key appeared twice
					if(parsed.find(key)!=parsed.end()) {
						// key appeared more than once, exception is thrown
						throw IOException(string("The key '")+key+"' appears more than once in SyslogLogDriver parameter string!");
					}
					state=BEFORECOLON;
				} else {
					throw IOException(string("Invalid character in SyslogLogDriver parameters: '")+parameters[i]+"'!");
				}
			} else if(state==PARAMVALUE) {
				if(parameters[i]!=',') {
					value.append(1, parameters[i]);
				} else if(parameters[i]==',') {
					// the key/value pair is complete.
					parsed[key]=value;
					state=BEFOREKEY;
				}
			} else if(state==BEFOREKEY) {
				if(isalpha(parameters[i])) {
					key="";
					key.append(1, parameters[i]);
					state=PARAMKEY;
				} else if(!isspace(parameters[i])) {
					throw IOException(string("Invalid character in SyslogLogDriver parameters: '")+parameters[i]+"'!");
				}
			} else if(state==BEFORECOLON) {
				if(parameters[i]==':') {
					value="";
					state=PARAMVALUE;
				} else if(!isspace(parameters[i])) {
					throw IOException(string("Invalid character in SyslogLogDriver parameters: '")+parameters[i]+"'!");
				}
			}
		}

		if(state!=PARAMVALUE) {
			throw IOException("Premature end of SyslogLogDriver parameters!");
		}

		if(parsed.find(key)!=parsed.end()) {
			// key appeared more than once, exception is thrown
			throw IOException(string("The key '")+key+"' appears more than once in SyslogLogDriver parameter string!");
		}
		parsed[key]=value;

		return parsed;
	}

	int SyslogLogDriver::parseOption(const string& option) throw
		(IOException, bad_exception)
	{
		// States:
		// BEFORECOMP
		// COMP
		// AFTERCOMP

		int ret=0;
		int BEFORECOMP=0, COMP=1, AFTERCOMP=2;
		int state=BEFORECOMP;
		string component;
		for(int i=0;i<option.size();++i) {
			if(state==BEFORECOMP) {
				if(isalpha(option[i])) {
					component="";
					component.append(1, option[i]);
					state=COMP;
				} else if(!isspace(option[i])) {
					throw IOException(string("Invalid character in SyslogLogDriver parameters: '")+option[i]+"'!");
				}
			} else if(state==COMP) {
				if(isalpha(option[i]) || option[i]=='_') {
					component.append(1, option[i]);
				} else if(isspace(option[i])) {
					if(_options.find(component)==_options.end()) {
						throw IOException(string("Invalid option for syslog in SyslogLogDriver parameter string: '")+component+"'!");
					}
					ret|=_options[component];
					component="";
					state=AFTERCOMP;
				} else if(option[i]=='|') {
					if(_options.find(component)==_options.end()) {
						throw IOException(string("Invalid option for syslog in SyslogLogDriver parameter string: '")+component+"'!");
					}
					ret|=_options[component];
					component="";
					state=BEFORECOMP;
				} else {
					throw IOException(string("Invalid character in SyslogLogDriver parameters: '")+option[i]+"'!");
				}
			} else if(state==AFTERCOMP) {
				if(option[i]=='|') {
					state=BEFORECOMP;
				} else if(!isspace(option[i])) {
					throw IOException(string("Invalid character in SyslogLogDriver parameters: '")+option[i]+"'!");
				}
			}
		}

		if(!(state==AFTERCOMP || state==COMP)) {
			throw IOException("Premature end of syslog option string in SyslogLogDriver parameters!");
		}
	
		if(state==COMP) {
			if(_options.find(component)==_options.end()) {
				throw IOException(string("Invalid option for syslog in SyslogLogDriver parameters: '")+component+"'!");
			}
			ret|=_options[component];
		}

		return ret;
	}

	// delete spaces at the beginnning and at the end
	string SyslogLogDriver::trim(const string& s) throw (bad_exception)
	{
		string ret=s;
		while(isspace(ret[0])) ret.erase(ret.begin());
		while(isspace(ret[ret.size()-1])) ret.erase(ret.end()-1);

		return ret;
	}

	int SyslogLogDriver::parsePriority(const string& priority) throw
		(IOException, bad_exception)
	{
		string prio=trim(priority);

		if(_priorities.find(prio)==_priorities.end()) {
			// invalid priority
			throw IOException(string("Invalid priority in SyslogLogDriver parameter string: '")+prio+"'!");
		}
	
		return _priorities[prio];
	}
}
