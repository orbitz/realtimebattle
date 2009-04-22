/************************************************************************
    $Id: localclientconnection.cpp,v 1.2 2005/01/06 17:59:28 jonico Exp $
    
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

    $Log: localclientconnection.cpp,v $
    Revision 1.2  2005/01/06 17:59:28  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#include <common.h>

#include "localclientconnection.h"
#include "../gamecontrol/clientcoordinator.h"
#include "rtbconnection.h"

namespace IO {
	using GameControl::ClientCoordinator;
	using IO::RTBConnection;

	/**
	* Constructors/Destructors
	*/
	LocalClientConnection::LocalClientConnection(RTBConnection* rtbConnection, ClientCoordinator* clientCoordinator) throw (bad_exception):_rtbConnection(rtbConnection),_clientCoordinator(clientCoordinator) {
	}
	/**
	* Methods
	*/
	/**
	*
	*/
	LocalClientConnection::~LocalClientConnection ()  throw() {

	}
	/**
	*
	*/
	string LocalClientConnection::getLine ()  throw(IOException,bad_exception) {
			return _rtbConnection->getLine();
	}
	/**
	*
	*/
	void LocalClientConnection::sendLine (const string& line)  throw(StrategyException, IOException, bad_exception) {
		if( line.size() > 0 ) {
			unsigned int datalen;
			int lastNewline = 0;
			int aktualNewline;
			
			//convert string from pattern:
			//	data + \n + data + \n + data to single lines and delegate them to the client coordinator
			// a trailing newline at the end of the whole line will be ignored
			
			while(true) {
				aktualNewline = line.find('\n', lastNewline);
				
				if( aktualNewline < 0 ) {
					aktualNewline=line.size();
					datalen = aktualNewline - lastNewline;
					if (datalen==0)
						break;
					_clientCoordinator->reactOnServerTip(line.substr(lastNewline, datalen));
					break;
				}
					
				datalen = aktualNewline - lastNewline;
				_clientCoordinator->reactOnServerTip(line.substr(lastNewline, datalen));
				
				lastNewline = aktualNewline + 1;
			}
		}
			
	}
	/**
	*
	*/
	bool LocalClientConnection::isLocal ()  throw() {
		return true;
	}
	/**
	*
	*/
	string LocalClientConnection::getTargetName () throw(IOException,bad_exception) {
			return _rtbConnection->getTargetName()+" (local one)";
	}
}
