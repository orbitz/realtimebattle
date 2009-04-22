/************************************************************************
    $Id: unixoutstream.cpp,v 1.2 2005/01/06 17:59:28 jonico Exp $
    
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

    $Log: unixoutstream.cpp,v $
    Revision 1.2  2005/01/06 17:59:28  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#include <common.h>

#include "unixoutstream.h"
#include <sstream>

/**
 * Namespace
 */
namespace IO {

	using std::ostringstream;
	
	/**
	 * Constructors
	 */
	UnixOutStream::UnixOutStream (int fd) throw(bad_exception)
	: ostream(0), _unixOutStreambuf(fd), _fd(fd) {
		rdbuf(&_unixOutStreambuf);
		
		//set exception-behavior
		exceptions(ios_base::badbit | ios_base::failbit);
	}
	/**
	 * Methods
	 */
	void UnixOutStream::sendline(const string& line) throw(IOException, bad_exception) {
		if( line.size() > 0 ) {
			try {
				_sendString.clear();
				unsigned int datalen;
				int lastNewline = 0;
				unsigned char wordlen;
				int aktualNewline;
				
				//convert string from pattern:
				//	data + \n + data + \n + data
				// a trailing newline at the end of the whole line will be ignored
				//in the following pattern:
				//	lineLen(2 Bytes)+data(lineLen Bytes)+lineLen(2 Bytes)+data(lineLen Bytes)+...
				//=> multiple lines can be send
				while(true) {
					aktualNewline = line.find('\n', lastNewline);
					
					if( aktualNewline < 0 ) {
						aktualNewline=line.size();
						datalen = aktualNewline - lastNewline;
						if (datalen==0)
							break;
						wordlen = datalen >> 8;
						_sendString += wordlen;
						wordlen = datalen & 255;
						_sendString += wordlen;
						_sendString += line.substr(lastNewline, datalen);
						break;
					}
						
						
						
					datalen = aktualNewline - lastNewline;
				
					wordlen = datalen >> 8;
					_sendString += wordlen;
					wordlen = datalen & 255;
					_sendString += wordlen;
					_sendString += line.substr(lastNewline, datalen);
					
					lastNewline = aktualNewline + 1;
				}
				// it does not matter, whether this function is called with zero bytes
				write(_sendString.c_str(), _sendString.size());
			
			} 
			catch(ostream::failure& e) {
				ostringstream int2string;
				int2string << _fd;
				throw IOException("Error while writing to fd" + int2string.str()+ ": " + e.what());
			}
		}
	}
}
