/************************************************************************
    $Id: unixinstream.h,v 1.2 2005/01/06 17:59:28 jonico Exp $
    
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

    $Log: unixinstream.h,v $
    Revision 1.2  2005/01/06 17:59:28  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef UNIXINSTREAM_H
#define UNIXINSTREAM_H

#include <iostream>
#include "unixinstreambuf.h"

/**
* Namespace IO
*/
namespace IO {

	using std::istream;
	using std::string;

	/**
	* Class UnixInStream
	*/ 
	class UnixInStream : public istream {
	/*
	* Public stuff
	*/
	public:
		/**
		* Constructors
		* @param fd unix-filedescriptor
		*/
		UnixInStream (int fd) throw(IOException, bad_exception);
		
		
		/*
		 * Methods
		 */
		/**
		* reads from buffer and extract a line
		* @return string which contains the line
		*/ 
		string getline(void) throw(IOException, bad_exception);
		
		
	/*
	 * Private stuff
 	 */
	private:
		
		/*
		* Methods
		*/
		/**
		* Streambuf-object to read data from filedecriptor into buffer
		*/
		UnixInStreambuf _unixInStreambuf;
		
		/**
		* unix-filedescriptor
		*/
		int _fd;
		
		/**
		* read data, better to place it here
		*/
		char _readBuffer[65536];
	};
}

#endif //UNIXINSTREAM_H
