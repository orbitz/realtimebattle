/************************************************************************
    $Id: masterresourcecontrol.cpp,v 1.2 2005/01/06 17:59:29 jonico Exp $
    
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

    $Log: masterresourcecontrol.cpp,v $
    Revision 1.2  2005/01/06 17:59:29  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#include <common.h>
#include "masterresourcecontrol.h"
#include <iostream>
#include "../exceptions/serverispresentexception.h"
#include "../gamecontrol/clientgamecontroller.h"
#include "../gamecontrol/servergamecontroller.h"
#include "../io/iofactory.h"
#include <string>
#include <sstream>


namespace RTBGlobal {
	using std::clog;
	using std::istringstream;
	using Exceptions::ServerIsPresentException;
	using GameControl::ClientGameController;
	using GameControl::ServerGameController;
	using Exceptions::RTBException;
	/**
	* Constructor
	*/
	MasterResourceControl::MasterResourceControl () throw (bad_exception): _configMap(0), _runtimeMap(), _ioFactory(0), _strategyFactoryMap(), _logDriverPrototypeMap() {
		// does nothing at the moment
	}
	/**
	* Destructor (does nothing at the moment)
	*/
	MasterResourceControl::~MasterResourceControl () throw() {
		// does nothing at the moment
	}
	
	/**
	 * static fields
	*/
	/**
	 * instance will be set to null at the beginning
	*/
	auto_ptr<MasterResourceControl> MasterResourceControl::_instance(0);
	/**
	* Methods
	*/

	/**
	 * Obtains the one and only MasterResourceControl object (singleton)
	 */
	MasterResourceControl* MasterResourceControl::Instance () throw (bad_exception){
		MasterResourceControl* mrc(MasterResourceControl::_instance.get());
		if (mrc==0) {
			MasterResourceControl::_instance.reset(mrc=new MasterResourceControl());
		}
		return mrc;
	}
	/**
	* 
	*/
	void MasterResourceControl::EmergencyLog (const string& logMessage) throw () {
		// best general method to express, that there is something wrong,I think:
		clog << logMessage << std::endl;
	}
	/**
	* 
	*/
	void MasterResourceControl::registrateIOFactory (auto_ptr <IOFactory> ioFactory)  throw (bad_exception){
		_ioFactory=ioFactory;
	}
	/**
	* 
	*/
	void MasterResourceControl::registrateStrategyFactory (const string& strategyName, auto_ptr<StrategyFactory> strategyFactory)  throw (bad_exception){
		// warning: if two strategy factories will register under the same name, we will have a memory leak
		_strategyFactoryMap[strategyName]=strategyFactory.release();
	}
	/**
	* 
	*/
	void MasterResourceControl::registrateLogDriverPrototype (const string& logDriverName, auto_ptr <LogDriver> logDriverPrototype) throw (bad_exception){
		// warning: if two logdriverPrototypes will register under the same name, we will have a memory leak
		_logDriverPrototypeMap[logDriverName]=logDriverPrototype.release();		
	}
	/**
	* 
	*/
	auto_ptr <GameController> MasterResourceControl::getGameController () const throw (ResourceNotFoundException, bad_exception)  {
		try {
			auto_ptr<GameController> gameController(0);
			const IOFactory* iof(getIOFactory());
			
			try {
				gameController.reset(new ServerGameController(iof->createServerCommunicator()));
				
			}
			catch (ServerIsPresentException& dummy) {
				// becoming master server failed
				gameController.reset(new ClientGameController(iof->createClientCommunicator()));
			}
			return gameController;
	
		}
		catch (RTBException& ex) {
			throw ResourceNotFoundException("Could not create a GameController, because an exception occured: "+ex.getMessage());
		}
	}
	
	/**
	* 
	*/
	const IOFactory* MasterResourceControl::getIOFactory () const throw (ResourceNotFoundException, bad_exception) {
		if (_ioFactory.get()==0)
			throw ResourceNotFoundException("IOFactory was not present at this stage!");
		return _ioFactory.get();	
	}
	/**
	* Returns a strategy factory that will be able to create the needed state objects.
	*/
	const StrategyFactory * MasterResourceControl::getStrategyFactory (const string& strategyName) const throw (ResourceNotFoundException, bad_exception)  {
		pmap<string, StrategyFactory>::const_iterator i(_strategyFactoryMap.find(strategyName));
		if (i!=_strategyFactoryMap.end())
			return i->second;
		throw ResourceNotFoundException("No strategy was registered under the name " + strategyName);
	}
	/**
	* 
	*/
	auto_ptr <Logger> MasterResourceControl::createLogger (const string& section) const throw (ResourceNotFoundException, bad_exception) {
		// this method cannot be implemented until we do not know how we have specified the configuration keys
		try {
			// first find out the name of the logDriverPrototype to use for the logger
			string logDriverPrototypeName(getConfigurationProperty(section,"logDriverName"));
			// now retrieve the corresponding logDriverPrototype
			pmap <string, LogDriver>::const_iterator i(_logDriverPrototypeMap.find(logDriverPrototypeName));
			if (i==_logDriverPrototypeMap.end())
				throw ResourceNotFoundException("Did not find any logDriver registered under string: "+logDriverPrototypeName);
			// now retrieve the logDriver's priority
			string prioritystring (getConfigurationProperty(section,"logPriority"));
			istringstream numberconverter(prioritystring);
			int priority(-1);
			numberconverter >> priority;
			if (priority<0 || !numberconverter) // number conversion failed
				throw ResourceNotFoundException("Value of logPriority was not a valid number!");
			// now retrieve the logDriver's parameter
			string logDriverParameters(getConfigurationProperty(section,"logDriverParameters"));
			// now initialize the logDriver
			auto_ptr<LogDriver> logDriver(i->second->clone());
			logDriver->setLogDriverParameters(logDriverParameters);
			return auto_ptr<Logger>(new Logger(section,priority,logDriver));
		}
		catch (RTBException& ex) {
			throw ResourceNotFoundException("While creating the logger, an exception occured: "+ex.getMessage());
		}
	}
	/**
	* 
	*/
	void MasterResourceControl::setConfigurationData (const map<string, map<string, string> >& configurationData)  throw (bad_exception){
		_configMap.reset (new map<string, map<string, string> >(configurationData));
	}
	/**
	* 
	*/
	const string& MasterResourceControl::getConfigurationProperty (const string& section, const string& key) const throw (ConfigNotLoadedException, KeyNotFoundException, bad_exception) {
		map<string, map<string,string> >* configMap(_configMap.get());
		if (configMap==0)	
			throw ConfigNotLoadedException();
		map<string, map<string, string> >::const_iterator i(configMap->find(section));
		if (i!=configMap->end()) {
			map<string,string>::const_iterator j((i->second).find(key));
			if (j!=(i->second).end())
				return j->second;
			throw KeyNotFoundException("No configuration key named "+key+ " in existing section "+section+ " found!");
		}
		throw KeyNotFoundException("No configuration section named "+ section+ " found!");		
	}
	/**
	* 
	*/
	void MasterResourceControl::setRuntimeProperty (const string& section, const string& key, const string& value)  throw (bad_exception) {
		(_runtimeMap[section])[key]=value;
	}
	/**
	* Returns a runtime property value associated with a key in a section.
	*/
	const string& MasterResourceControl::getRuntimeProperty (const string& section, const string& key) const throw (KeyNotFoundException, bad_exception) {
		map<string, map<string, string> >::const_iterator i(_runtimeMap.find(section));
		if (i!=_runtimeMap.end()) {
			map<string,string>::const_iterator j((i->second).find(key));
			if (j!=(i->second).end())
				return j->second;
			throw KeyNotFoundException("No runtime key named "+key+ " in existing section "+section+ " found!");
		}
		throw KeyNotFoundException("No runtime section named "+ section+ "found!");	
	}
}
