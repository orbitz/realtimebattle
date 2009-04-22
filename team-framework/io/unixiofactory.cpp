/************************************************************************
    $Id: unixiofactory.cpp,v 1.4 2005/07/09 11:45:55 jonico Exp $
    
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

    $Log: unixiofactory.cpp,v $
    Revision 1.4  2005/07/09 11:45:55  jonico
    Ignored signal SIGPIPE in order not to be killed in case of broken pipes.

    Revision 1.3  2005/01/06 17:59:28  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#include <common.h>

#include "unixiofactory.h"
#include "unixservercommunicator.h"
#include "unixclientcommunicator.h"
#include "unixrtbconnection.h"
#include "../rtbglobal/masterresourcecontrol.h"

#include <string>
#include <fcntl.h>
#include <signal.h>

/**
 * Namespace
 */
namespace IO {

	using RTBGlobal::MasterResourceControl;
	using std::string;

	/**
	 * Constructor
	 */
	UnixIOFactory::UnixIOFactory () throw(bad_exception)
	: _mrc (MasterResourceControl::Instance()) 
 	{
		// register this initial object by the MRC
	// new registration method now
	//	_mrc->registrateIOFactory(this);
		// we do not want to be killed in case of a broken pipe, so we ignore SIGPIPE
		signal(SIGPIPE,SIG_IGN);

	}
	/**
	 * Destructor
	 */
	UnixIOFactory::~UnixIOFactory () throw() {

	}
	/**
	 * Methods
	 */
	auto_ptr <ServerCommunicator>  UnixIOFactory::createServerCommunicator () const throw(IOException, ServerIsPresentException, bad_exception) {

		return auto_ptr<ServerCommunicator>(new UnixServerCommunicator());
	}
	/**
	 *
	 */
	void UnixIOFactory::switch2BlockingMode() const throw (IOException, bad_exception) {
	
		if( fcntl(0, F_SETFL, fcntl(0, F_GETFL, 0) & (~O_NONBLOCK)) != 0 )
			throw(IOException("Could not switch stdin to blocking-mode."));
			
		if( fcntl(1, F_SETFL, fcntl(1, F_GETFL, 0) & (~O_NONBLOCK)) != 0 )
			throw(IOException("Could not switch stdout to blocking-mode."));
	}
	/**
	 *
	 */
	auto_ptr <ClientCommunicator>  UnixIOFactory::createClientCommunicator () const throw(IOException, bad_exception) {
		
		return auto_ptr<ClientCommunicator>(new UnixClientCommunicator);
	}
	/**
	 *
	 */
	RTBConnection*  UnixIOFactory::getRTBConnection () const throw(IOException, bad_exception) {
		return UnixRTBConnection::instance();
	}

	namespace { // anonymous namespace
// new registration method now
//		UnixIOFactory __ioFactory; // we need a global object to register the factory by the MRC
		
	}
}
