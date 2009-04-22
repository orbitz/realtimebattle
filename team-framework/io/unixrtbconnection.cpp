/************************************************************************
    $Id: unixrtbconnection.cpp,v 1.2 2005/01/06 17:59:28 jonico Exp $
    
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

    $Log: unixrtbconnection.cpp,v $
    Revision 1.2  2005/01/06 17:59:28  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#include <common.h>

#include "unixrtbconnection.h"
#include "../exceptions/keynotfoundexception.h"
#include "../rtbglobal/masterresourcecontrol.h"
#include <iostream>
#include <sstream>
#include <ios>

/**
 * Namespace
 */
namespace IO {

	using RTBGlobal::MasterResourceControl;
	using Exceptions::KeyNotFoundException;
	using std::cout;
	using std::cin;
	using std::endl;
	using std::ostringstream;
	
	/**
	 * static members
	 */
	auto_ptr <UnixRTBConnection> UnixRTBConnection::_instance(0);
	/**
	 * Constructor (private)
	 */
	UnixRTBConnection::UnixRTBConnection () throw(bad_exception)
	: _inStream(0), _outStream(1) /*means getLine to stdin(fd=0) and sendLine to stdout(fd=1)*/ {
	}
	/**
	* Destructor
	*/
	UnixRTBConnection::~UnixRTBConnection () throw() {
	}
	/**
	 * Operations
	 */
	/**
	 * this static method is the only way to get an instance from UnixRTBConnetion (singleton)
	 * @return Pointer to UnixRTBConnection
	 */
	UnixRTBConnection* UnixRTBConnection::instance() throw(bad_exception) {
		UnixRTBConnection* rtbConnection(UnixRTBConnection::_instance.get());
		if( rtbConnection == 0 )
			UnixRTBConnection::_instance.reset(rtbConnection = new UnixRTBConnection);
			
		return rtbConnection;
	}
	/**
	 * 
	 * @param line Message that will be send over the connection (does not append a trailing newline)
	 */
	void UnixRTBConnection::sendLine (const string& line) throw(StrategyException, IOException, bad_exception) {
		try {
			_outStream << line;
		}
		catch(istream::failure& e) {
			string msg("Error while writing to fd1: ");
			throw IOException(msg + e.what());
		}		
	}
	/**
	 * @return string that contains a line out of the connection (not terminated with newline)
	 */
	string UnixRTBConnection::getLine () throw(IOException, bad_exception) {
		string line;
		
		try {
			getline(_inStream, line);
		}
		catch(istream::failure& e) {
			string msg("Error while reading from fd0: ");
			throw IOException(msg + e.what());
		}		
		
		return line;
	}
	/**
	 * @return name of the associated target
	 */
	string UnixRTBConnection::getTargetName () throw(IOException, bad_exception) {
		
		//get ActualName
		MasterResourceControl* mrc = MasterResourceControl::Instance();
		string robotName;
		try {
			robotName = mrc->getRuntimeProperty("Main","ActualName");
		}
		catch(KeyNotFoundException& e) {
			throw IOException("Could not get ActualName from section Main. KeyNotFoundException occured: " + e.getMessage());
		}
		
		//append pid to robotname
		ostringstream int2string;
		int2string << getpid();
		robotName += " (pid: ";
		robotName += int2string.str();
		robotName += ")";
		
		return robotName;
	}
}
 
