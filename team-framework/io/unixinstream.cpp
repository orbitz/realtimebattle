/************************************************************************
    $Id: unixinstream.cpp,v 1.2 2005/01/06 17:59:28 jonico Exp $
    
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

    $Log: unixinstream.cpp,v $
    Revision 1.2  2005/01/06 17:59:28  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#include <common.h>

#include "unixinstream.h"
#include <sstream>

/**
 * Namespace
 */
namespace IO {

	using std::ostringstream;
	
	/**
	 * Constructors
	 */
	UnixInStream::UnixInStream (int fd) throw(IOException, bad_exception)
	: istream(0), _unixInStreambuf(fd), _fd(fd) {
		rdbuf(&_unixInStreambuf);
			
		//set exception-behavior
		exceptions(ios_base::badbit | ios_base::failbit);
	}
	/**
	 * Methods
	 */
	string UnixInStream::getline(void) throw(IOException, bad_exception) {
		try {
			//read datalength
			char lenBuffer[2];
			read(lenBuffer, 2);
			int datalen = (lenBuffer[0] << 8) | lenBuffer[1];
			
			read(_readBuffer, datalen);
			_readBuffer[datalen] = 0;
			return string(_readBuffer);
		}
		catch(istream::failure& e) {
			ostringstream int2string;
			int2string << _fd;
			throw IOException("Error while reading from fd" + int2string.str()+ ": " + e.what());
		}		
	}

}

