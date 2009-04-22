/************************************************************************
    $Id: unixinstreambuf.cpp,v 1.3 2005/01/06 17:59:28 jonico Exp $
    
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

    $Log: unixinstreambuf.cpp,v $
    Revision 1.3  2005/01/06 17:59:28  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#include <common.h>

#include "unixinstreambuf.h"
#include "../rtbglobal/masterresourcecontrol.h"
#include "../exceptions/keynotfoundexception.h"
#include "../exceptions/confignotloadedexception.h"
#include <sstream>
#include <errno.h>

/**
 * Namespace
 */
namespace IO {

	using RTBGlobal::MasterResourceControl;
	using Exceptions::KeyNotFoundException;
	using Exceptions::ConfigNotLoadedException;
	using std::istringstream;
	using std::ostringstream;
	using std::streamsize;
	using std::string;

	/**
	 * Constructor
	 */
	UnixInStreambuf::UnixInStreambuf(int fd) throw(IOException, bad_exception)
	: _fd(fd),_bufferSize(-1) {

		//try to get the size of readbuffer from configfile
		MasterResourceControl* mrc = MasterResourceControl::Instance();
		string readBufferSize;
		try {
			readBufferSize = mrc->getConfigurationProperty("IO","ReadBufferSize");
		}
		catch(KeyNotFoundException& e) {
			throw IOException("Could not get ReadBufferSize from section IO. KeyNotFoundException occured: " + e.getMessage());
		}
		catch(ConfigNotLoadedException& e) {
			throw IOException("Could not get ReadBufferSize from section IO. ConfigNotLoadedException occured: " + e.getMessage());
		}
		
		istringstream string2int(readBufferSize);

		//initialize buffer
		string2int >> _bufferSize;
		if (_bufferSize<10)
			throw IOException("ReadBufferSize from section IO was not a number above 10 bytes!");
			
		_buffer = new char[_bufferSize];

		//initialize bufferpointers (eback() = gptr() = egptr() = _buffer)
		//implies underflow()
		setg(_buffer, _buffer, _buffer);
	}
	/**
	 * Destructor
	 */
	UnixInStreambuf::~UnixInStreambuf() throw() {
			delete[] _buffer;
	}
	/**
	 * Methods
	 */
	/**
	 * fill buffer by reading from filedescriptor
	 */
	int UnixInStreambuf::underflow () throw (IOException, bad_exception) {
		
		//proof, whether the actual position is at end of buffer
		if( gptr() < egptr() )
			return *gptr();

		//reading data
		int num = read (_fd, _buffer, _bufferSize);
		if( num < 0 ) {
		
				string error(strerror(errno));

				//convert _fd to string
				ostringstream int2string;
				int2string << _fd;

				//throw exception
				throw IOException("Could not read from fd" + int2string.str() + ": " + error);
		}
		
		else if( num == 0 ) {

				//convert _fd to string
				ostringstream int2string;
				int2string << _fd;

				//throw exception
				throw IOException("Could not read from fd" + int2string.str() + " because we reached end of file");
		}
		
		//set bufferpointers
		setg(_buffer, _buffer, _buffer + num);

		//return next character
		return *gptr();
	}
}
