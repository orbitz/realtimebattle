/************************************************************************
    $Id: unixoutstreambuf.h,v 1.2 2005/01/06 17:59:28 jonico Exp $
    
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

    $Log: unixoutstreambuf.h,v $
    Revision 1.2  2005/01/06 17:59:28  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef UNIXOUTSTREAMBUF_H
#define UNIXOUTSTREAMBUF_H

#include "../exceptions/ioexception.h"
#include <iostream>
#include <memory>

/**
 * Namespace IO
 */
namespace IO {

	using std::streambuf;
	using std::streamsize;
	using std::bad_exception;
	using Exceptions::IOException;

	/**
	* Class UnixOutStreamBuf
	* NOTE: No buffer implemented.
	*/ 
	class UnixOutStreambuf : public streambuf {
	/*
	 * Public stuff
	 */
	public:
		/** 
		 * Constructor
		 * @param fd unix-filedescriptor
		 */
		UnixOutStreambuf(int fd) throw();
		
		
	/*
	 * Private stuff
	 */
	private:
		/**
		* filedescriptor
		*/
		int _fd;
		
		/**
		 * writing a character 
		 */
		int overflow (int character) throw(IOException, bad_exception);
		
		/**
		 * writing any number of characters
		 */
		streamsize xsputn (const char* databuffer, streamsize size) throw(IOException, bad_exception);
	};
}

#endif //UNIXOUTSTREAMBUF_H

