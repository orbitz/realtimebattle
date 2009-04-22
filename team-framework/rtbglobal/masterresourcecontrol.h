/************************************************************************
    $Id: masterresourcecontrol.h,v 1.2 2005/01/06 17:59:29 jonico Exp $
    
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

    $Log: masterresourcecontrol.h,v $
    Revision 1.2  2005/01/06 17:59:29  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef MASTERRESOURCECONTROL_H
#define MASTERRESOURCECONTROL_H

#include <memory>
#include <map>


#include "../gamecontrol/gamecontrolnamespace.h" 
#include "../stdnamespace.h"
#include "../strategies/strategiesnamespace.h"
#include "../log/lognamespace.h"
#include "../io/ionamespace.h"
#include "../exceptions/exceptionsnamespace.h"

#include "../log/logger.h"
#include "../exceptions/ioexception.h"
#include "../exceptions/resourcenotfoundexception.h"
#include "../exceptions/confignotloadedexception.h"
#include "../exceptions/keynotfoundexception.h"
#include "pmap.h"
#include "../strategies/strategyfactory.h"
#include "../log/logdriver.h"

/**
 * Namespace RTBGlobal
 */
namespace RTBGlobal {
	using std::map;
	using std::bad_exception;
	using std::auto_ptr;
	using std::string;
	using GameControl::GameController;
	using IO::IOFactory;
	using Log::LogDriver;
	using Strategies::StrategyFactory;
	using Log::Logger;
	using Exceptions::IOException;
	using Exceptions::ResourceNotFoundException;
	using Exceptions::ConfigNotLoadedException;
	using Exceptions::KeyNotFoundException;
	
	/**
	* Class MasterResourceControl
	* This class is responsable for managing all ressources of the whole system on a central point.
	*/ 
	class MasterResourceControl {
	/*
	* Public stuff
	*/
	public:
		/**
		* Destructor, does nothing
		*/
		~MasterResourceControl () throw();
		
	
		/**
		* Obtains the one and only MasterResourceControl object (singleton)
		*/
		static MasterResourceControl*  Instance () throw (bad_exception);
		
	
		/**
		* Logs a message if situation is so bad, that you cannot use a logger object
		* @param logMessage for emergency reasons
		*/
		static void  EmergencyLog (const string& logMessage) throw ();
		
	
		/**
		* Register the one and only IOFactory with this method
		* @param ioFactory pointer to the specific IOFactory
		*/
		void  registrateIOFactory (auto_ptr<IOFactory> ioFactory) throw (bad_exception);
		
	
		/**
		* Register a strategy under a certain name
		* @param strategyName Strings the Coordinators will use to obtain the factory
		* @param strategyFactory pointer to a concrete strategy factory
		*/
		void  registrateStrategyFactory (const string& strategyName, auto_ptr<StrategyFactory> strategyFactory) throw (bad_exception);
		
	
		/**
		* Register a logDriverPrototype, that will be cloned every time, when a logger should use this driver
		* @param logDriverName name of the log Driver
		* @param logDriverPrototype concrete prototype of logDriver (to be cloned every time)
		*/
		void  registrateLogDriverPrototype (const string& logDriverName, auto_ptr<LogDriver> logDriverPrototype) throw (bad_exception);
		
	
		/** 
		* @return (ClientGameController object if MasterServerSocket is already created, if not ServerGameController object)
		*/
		auto_ptr <GameController>  getGameController () const throw (ResourceNotFoundException, bad_exception);
		
	
		/**
		* @return the registered IOFactory
		*/
		const IOFactory*  getIOFactory () const throw (ResourceNotFoundException, bad_exception);
		
	
		/**
		* @return strategy factory that will be able to create the needed state objects.
		* @param strategyName Names the strategy that will be created from the returned factory.
		*/
		const StrategyFactory*  getStrategyFactory (const string& strategyName) const throw (ResourceNotFoundException, bad_exception);
		
	
		/**
		* Creates a logger object, fills it with the the section's associated logDriver
		* @param section Section for that the logger should work
		*/
		auto_ptr <Logger>  createLogger (const string& section) const throw (ResourceNotFoundException, bad_exception);
		
	
		/**
		* 
		* @param configurationData Contains a map, that maps a section to a map, that maps a key to a value.
		*/
		void  setConfigurationData (const map<string, map<string, string> >& configurationData) throw (bad_exception);
		
	
		/**
		* Returns a the associated key to the corresponding section
		* @param section Names the section.
		* @param key Names the configuration key.
		*/
		const string&  getConfigurationProperty (const string& section, const string& key) const throw (ConfigNotLoadedException, KeyNotFoundException, bad_exception);
		
	
		/**
		* Sets a runtime property (Can be used for states to communicate over the MRC)
		* @param section Names the runtime section.
		* @param key Names the runtime key.
		* @param value Names the associated value.
		*/
		void  setRuntimeProperty (const string& section, const string& key, const string& value) throw (bad_exception);
		
	
		/**
		* @return runtime property value associated with a key in a section.
		* @param section Names the requested section.
		* @param key Names the requested key.
		*/
		const string&  getRuntimeProperty (const string& section, const string& key) const throw ( KeyNotFoundException, bad_exception);
		
	private:
		/*
		* Fields
		*/
		/**
		* Pointer to the only instance of the master ressource control
		*/
		static  auto_ptr <MasterResourceControl> _instance;
		
		/**
		* Contains the configuration data map.
		*/
		auto_ptr <map <string, map<string, string> > > _configMap;
		
		/**
		* Contains the runtime data map.
		*/
		map <string, map<string, string> > _runtimeMap;
		
		/**
		* Pointer to the corresponding IOFactory
		*/
		auto_ptr <IOFactory> _ioFactory;
		
		/**
		* map for the registered Strategy Factories
		*/
		pmap <string, StrategyFactory> _strategyFactoryMap;
		
		/**
		* map for the registered logDriver prototypes
		*/
		pmap <string, LogDriver> _logDriverPrototypeMap;
		/*
		* Constructor
		*/
		/**
		* Private constructor (singleton pattern)
		*/
		MasterResourceControl() throw (bad_exception);
		
	
	};
}
#endif //MASTERRESOURCECONTROL_H

