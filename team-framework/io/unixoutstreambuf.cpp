/************************************************************************
    $Id: unixoutstreambuf.cpp,v 1.3 2005/01/06 17:59:28 jonico Exp $
    
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

    $Log: unixoutstreambuf.cpp,v $
    Revision 1.3  2005/01/06 17:59:28  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#include <common.h>

#include "unixoutstreambuf.h"
#include <sstream>
#include <errno.h>
/**
 * Namespace
 */
namespace IO {
	
	using std::ostringstream;
	
	/** 
	 * Constructor
	 */
	UnixOutStreambuf::UnixOutStreambuf(int fd) throw()
	: _fd(fd) {
	}
	/** 
	 * Methods
	 */
	/**
	 * writing a character
	 */
	int UnixOutStreambuf::overflow (int character) throw (IOException, bad_exception) {
		if( character != EOF ) {
			char data = character;
			
			//writing successful?
			if( write(_fd, &data, 1) != 1 ) {
			
				//throw exception
				ostringstream int2string;
				int2string << _fd;
				throw IOException("Could not write to fd" + int2string.str() + ": " + strerror(errno));
			}
		}
		return character;
	}
	/**
	 * 
	 */
	streamsize UnixOutStreambuf::xsputn (const char* databuffer, streamsize size) throw(IOException, bad_exception) {
		int num = write(_fd, databuffer, size);
		
		//writing successful?
		if( num >= 0 )
			return num;
		else {
			
			//throw exception
			ostringstream int2string;
			int2string << _fd;
			throw IOException("Could not write to fd" + int2string.str() + ".");
		}
	}
}
