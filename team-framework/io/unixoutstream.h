/************************************************************************
    $Id: unixoutstream.h,v 1.2 2005/01/06 17:59:28 jonico Exp $
    
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

    $Log: unixoutstream.h,v $
    Revision 1.2  2005/01/06 17:59:28  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef UNIXOUTSTREAM_H
#define UNIXOUTSTREAM_H

#include <iostream>
#include "unixoutstreambuf.h"


/**
* Namespace IO
*/
namespace IO {

	using std::ostream;
	using std::string;

	/**
	* Class UnixOutStream
	*/
	class UnixOutStream : public ostream {
	/*
	* Public stuff
	*/
	public:
		/**
		* Constructors
		* @param fd unix-filedescriptor
		*/
		UnixOutStream (int fd) throw(bad_exception);
		
		
		/*
		* Methods
		*/
		/**
		* convert the string into a simple run-length-encoded lines and send the lines
		* @param line string which contains the line(s) to send
		*/
		void sendline(const string& line) throw(IOException, bad_exception);
		
		
	/*
	* Private stuff
	*/
	private:
		/**
		* Streambuf-object to write data to filedecriptor
		*/
		UnixOutStreambuf _unixOutStreambuf;
		
		/**
		* temporary string for converting lines
		*/
		string _sendString;
		
		/**
		* unix-filedescriptor
		*/
		int _fd;
	};
}

#endif //UNIXOUTSTREAM_H
 
