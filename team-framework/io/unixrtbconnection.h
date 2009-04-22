/************************************************************************
    $Id: unixrtbconnection.h,v 1.2 2005/01/06 17:59:28 jonico Exp $
    
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

    $Log: unixrtbconnection.h,v $
    Revision 1.2  2005/01/06 17:59:28  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef UNIXRTBCONNECTION_H
#define UNIXRTBCONNECTION_H

#include "rtbconnection.h"
#include "unixinstream.h"
#include "unixoutstream.h"
#include <memory>

/**
 * Namespace IO
 */
namespace IO {

	using std::auto_ptr;
	
	/**
	* Class RTBConnection
	* The RTB connection capsules the communication to the rtb server. For example, you can implement this as a simple wrapper for cin and cout.
	* Note: Has to be implemented as a SINGLETON
	*/ 
	class UnixRTBConnection : public RTBConnection {
	/*
	* Public stuff
	*/
	public:
		/**
		 * Destructor, does nothing
		 */
		~UnixRTBConnection () throw();
		
		
		/*
		 * Operations
		 */
		/**
		* returns an instance (->singleton pattern)
		*/
		static UnixRTBConnection* instance() throw(bad_exception);
		  
		 
		/**
		 * 
		 * @param line Message that will be send over the connection (does not append a trailing newline)
		 */
		void sendLine (const string& line) throw(StrategyException, IOException, bad_exception);
		
	
		/**
		* @return string that contains a line out of the connection (not terminated with newline)
		*/
		string getLine () throw(IOException, bad_exception);
		
	
		/**
		* @return name of the associated target
		*/
		string getTargetName () throw(IOException, bad_exception);
		
		
	/*
	* Private stuff
	*/
	private:
		
		/**
		* pointer for singleton
		*/
		static auto_ptr <UnixRTBConnection> _instance;
		
		/**
		* stream for reading data from remoteclient
		*/
		UnixInStream _inStream;
		
		/**
		* stream for writing data to remoteclient
		*/
		UnixOutStream _outStream;
		
		/**
		* Private constructor (->singleton pattern)
		*/
		UnixRTBConnection() throw(bad_exception);
	};
}
#endif //UNIXRTBCONNECTION_H
