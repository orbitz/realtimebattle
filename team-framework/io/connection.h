/************************************************************************
    $Id: connection.h,v 1.2 2005/01/06 17:59:27 jonico Exp $
    
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

    $Log: connection.h,v $
    Revision 1.2  2005/01/06 17:59:27  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef CONNECTION_H
#define CONNECTION_H

#include "../exceptions/ioexception.h"
#include "../exceptions/strategyexception.h"
#include <string>
#include <memory>


/**
 * Namespace IO
 */
namespace IO {

	using std::string;
	using std::bad_exception;
	using Exceptions::IOException;
	using Exceptions::StrategyException;
	
	/**
	* Class Connection
	*/ 
	class Connection {
	/*
	 * Public stuff
	 */
	public:
		/*
		 * Destructors
		 */
		/**
		 * Destructor, does nothing.
		 */
		virtual ~Connection () throw() {};
		
	
		/*
		 * Operations
		 */
		/**
		 * NOTE: the StrategyException is neccessary (see LocalClientConnection::sendLine)
		 * @param line Message that will be send over the connection (does not append a trailing newline)
		 */
		virtual void sendLine (const string& line) throw(StrategyException, IOException, bad_exception) = 0;
		
	
		/**
		 * @return string that contains a line out of the connection (not terminated with newline)
		 */
		virtual string getLine () throw(IOException, bad_exception) = 0;
		
	
		/**
		 * @return name of the associated target
		 */
		virtual string getTargetName () throw(IOException, bad_exception) = 0;
		
	
	};
}
#endif //CONNECTION_H

