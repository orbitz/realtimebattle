/************************************************************************
    $Id: initcomponents.cpp,v 1.8 2005/10/05 17:42:17 jonico Exp $
    
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

    $Log: initcomponents.cpp,v $
    Revision 1.8  2005/10/05 17:42:17  jonico
    Integrated Cobra strategy, invented better names for the robots.

    Revision 1.7  2005/08/08 11:32:27  krolfy
    Added three Columbus bots (crash with exception)

    Revision 1.6  2005/04/13 20:31:23  tino_truppel
    Insert cobra-bot.

    Revision 1.5  2005/01/06 17:59:18  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#include <common.h>

#include "initcomponents.h"
#include "io/unixiofactory.h"
#include "rtbglobal/masterresourcecontrol.h"
#include "log/rtblogdriver.h"
#include "log/nulllogdriver.h"
#include "log/unixfilelogdriver.h"
#include "log/sysloglogdriver.h"
#include "bots/brotfrucht/bffactory.h"
#include "bots/empty/empty_factory.h"
#include "bots/columbus/columbusstrategyfactory.h"
#include "bots/cobra/cobra_factory.h"
#include <memory>

/**
 * This function is responsable for registrating all components of the framework
 * It is necessary to have such a method because global objects in anonymous namespaces do not initialize in a presumable
 * order and we got lots of trouble (segfaults, ...). To avoid this, EVERY new pluggable component has to be registered in
 * this function that is called by the main method
 * This method can throw any type of exception
 */
  
void initComponents() 
{
	using RTBGlobal::MasterResourceControl;
	using std::auto_ptr;
	using IO::UnixIOFactory;
	using Log::RTBLogDriver;
	using Log::UnixFileLogDriver;
	using Log::NullLogDriver;
	using Log::SyslogLogDriver;
	using Brotfrucht::BFFactory;
	using empty::empty_Factory;
	using cobra::cobra_Factory;
	using Columbus::ColumbusStrategyFactory;
	
	// first get one instance of the MRC
	MasterResourceControl* mrc=RTBGlobal::MasterResourceControl::Instance();
	
	// register IO factory
	mrc->registrateIOFactory(auto_ptr<IO::IOFactory>(new UnixIOFactory()));
	
	// register rtb logdriver
	mrc->registrateLogDriverPrototype("RTBLogDriver",auto_ptr<Log::LogDriver>(new RTBLogDriver()));
	
	// register unix file log driver
	mrc->registrateLogDriverPrototype("UnixFileLogDriver",auto_ptr<Log::LogDriver>(new UnixFileLogDriver()));
	
	// register null log driver
	mrc->registrateLogDriverPrototype("NullLogDriver",auto_ptr<Log::LogDriver>(new NullLogDriver()));
	
	// register syslog log driver
	mrc->registrateLogDriverPrototype("SyslogLogDriver",auto_ptr<Log::LogDriver>(new SyslogLogDriver()));
	
	// register brotfrucht strategy
	mrc->registrateStrategyFactory("Brotfrucht",auto_ptr<Strategies::StrategyFactory>(new BFFactory()));
	
	// register empty strategy
	mrc->registrateStrategyFactory("empty",auto_ptr<Strategies::StrategyFactory>(new empty_Factory()));
	
	// register cobra strategy
	mrc->registrateStrategyFactory("Cobra",auto_ptr<Strategies::StrategyFactory>(new cobra_Factory()));

	// register Columbus strategy
	mrc->registrateStrategyFactory("Columbus",auto_ptr<Strategies::StrategyFactory>(new ColumbusStrategyFactory()));
}
