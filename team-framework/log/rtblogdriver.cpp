/************************************************************************
    $Id: rtblogdriver.cpp,v 1.4 2005/08/16 11:09:14 krolfy Exp $
    
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

    $Log: rtblogdriver.cpp,v $
    Revision 1.4  2005/08/16 11:09:14  krolfy
    Added multiline to DEBUG and BOTH

    Revision 1.3  2005/08/16 11:02:20  krolfy
    Chenged RTBLogDriver to multi line output

    Revision 1.2  2005/01/06 17:59:28  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#include <common.h>

#include "rtblogdriver.h"
#include <string>
#include "../rtbglobal/masterresourcecontrol.h"
#include "../io/rtbconnection.h"
#include "../io/iofactory.h"

/**
 * Namespace
 */
namespace Log {
	using RTBGlobal::MasterResourceControl;
	using IO::IOFactory;
	using Exceptions::RTBException;

	// Class RTBLogDriver
	// RTBLogdriver will log use the Debug or the Print Command (depends on the logParameters)
	// of the RealTime Battle Server to log messages
	// WARNING: We use blocking IO for our framework, so it is possible, that logging with this driver will block the
	// whole system. Use it with caution and when blocking is no risk for you 

	/**
	* Public stuff
	*/
	/**
	* Destructor
	* Does nothing	
	*/
	RTBLogDriver::~RTBLogDriver () throw() {}
			
	/**
	* Constructor for the first RTBLogDriver object
	* Constructor will register the object by the MRC with the name RTBLogDriver
	*/
	RTBLogDriver::RTBLogDriver() throw (bad_exception):_rtbConnection(NULL),_rtbLogState(UNINITIALIZED) {
		// we cannot obtain a handle to the RTBConnection here, because the IOFactory is possibly not registered yet 
		// (we do not know anything about the order of global object construction)
		
		// register this initial object by the MRC
		// new registration method now
		// MasterResourceControl::Instance()->registrateLogDriverPrototype("RTBLogDriver",this);
	}
	
	/**
	*  Operations
	*/

	/**
	* Method that is called by the MRC to duplicate the whole object
	*/
	auto_ptr <LogDriver>  RTBLogDriver::clone () const throw (bad_exception)
	{
		return auto_ptr<LogDriver>(new RTBLogDriver(_rtbConnection,_rtbLogState));
		
	}
	

	/**
	* 
	* @param message Message that will be logged in the file
	* If logger object was not initialized properly, this method will only throw an exception
	*/
	void  RTBLogDriver::logMessage (const string& message) throw (IOException, bad_exception) 
	{
		int i=message.size(),j;
		switch (_rtbLogState) {
			case UNINITIALIZED:
				throw IOException("RTBLogDriver was not initialized properly");
			case DEBUG: {
					for(;;) {
						if(string::npos==(i=message.rfind("Exception",j=i))) break;
						if(string::npos==(i=message.rfind(": ",i))) break;
						_rtbConnection->sendLine("Debug ..."+message.substr(++i,j-i)+"\n");
					}
					_rtbConnection->sendLine("Debug "+message.substr(0,j)+"\n");
					break;
				    }
			case PRINT: {
					for(;;) {
						if(string::npos==(i=message.rfind("Exception",j=i))) break;
						if(string::npos==(i=message.rfind(": ",i))) break;
						_rtbConnection->sendLine("Print ..."+message.substr(++i,j-i)+"\n");
					}
					_rtbConnection->sendLine("Print "+message.substr(0,j)+"\n");
					break;
				    }
			case BOTH:  {
					for(;;) {
						if(string::npos==(i=message.rfind("Exception",j=i))) break;
						if(string::npos==(i=message.rfind(": ",i))) break;
						_rtbConnection->sendLine("Print ..."+message.substr(++i,j-i)+"\n");
					}
					_rtbConnection->sendLine("Print "+message.substr(0,j)+"\n");
					i=message.size();
					for(;;) {
						if(string::npos==(i=message.rfind("Exception",j=i))) break;
						if(string::npos==(i=message.rfind(": ",i))) break;
						_rtbConnection->sendLine("Debug ..."+message.substr(++i,j-i)+"\n");
					}
					_rtbConnection->sendLine("Debug "+message.substr(0,j)+"\n");
					break;
				    }
		}

	}
	
	/**
	 * 
	 * @param logDriverParameters The parameter has to be in the form logCommand:Debug or logCommand:Print or logCommand:Both
	 * logCommand:Debug : Use the Debug coomand to log messages
	 * logCommand:Print : Use the Print command to log messages
	 * logCommand:Both  : Use the Print and the Debug command to log messages		
	 */
	void  RTBLogDriver::setLogDriverParameters (const string& logDriverParameters)  throw (IOException, bad_exception) 
	{
		// first set state to something that prevents logging with incorrect parameters
		_rtbLogState=UNINITIALIZED;
		// find out, if RTBConnection is already present
		if (!_rtbConnection) {
			try {
				_rtbConnection=MasterResourceControl::Instance()->getIOFactory()->getRTBConnection();
			}
			catch (RTBException& ex) {
				throw IOException("RTBLogDriver was not able to obtain a connection to the RealTime Battle server: "+ex.getMessage());
			}
		}
		// first find out, if parameter is correct
		if (logDriverParameters.find("logCommand:",0,11))
			throw IOException("logDriverParameters have incorrect syntax for RTBLogger (does not begin with 'logCommand:')");
		string logcommand(logDriverParameters,11); // extract logCommand from parameters
		if (logcommand=="Debug")
			_rtbLogState=DEBUG;
		else if (logcommand=="Print")
			_rtbLogState=PRINT;
		else if (logcommand=="Both")
			_rtbLogState=BOTH;
		else
			throw IOException("Unknown logCommand specified for RTBLogger: "+logcommand);		
		
	}
	
	/**
	 * Private stuff
	 */
	/**
	 * Constructor used in the clone Method to create a clone of the current object according to the prototype pattern
	 */
	RTBLogDriver::RTBLogDriver(RTBConnection* rtbConnection, LogState logState) throw ():_rtbConnection(rtbConnection),_rtbLogState(logState)
	{
	}
	
	namespace { // anonymous namespace
// new registration method now
//		RTBLogDriver __rtblogdriverprototype; // we need a global object to register the driver by the MRC
		
	}
}

