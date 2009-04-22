/************************************************************************
    $Id: unixinstreambuf.h,v 1.2 2005/01/06 17:59:28 jonico Exp $
    
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

    $Log: unixinstreambuf.h,v $
    Revision 1.2  2005/01/06 17:59:28  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef UNIXINSTREAMBUF_H
#define UNIXINSTREAMBUF_H

#include "../exceptions/ioexception.h"
#include <iostream>
#include <memory>


/**
* Namespace IO
*/
namespace IO {

	using std::streambuf;
	using std::bad_exception;
	using Exceptions::IOException;

	/**
	* Class UnixInStreamBuf
	* NOTE: No putback-buffer implemented.
	*/ 
	class UnixInStreambuf : public streambuf {
	/*
	* Public stuff
	*/
	public:
		/**
		* Constructor
		* @param fd unix-filedescriptor
		*/
		UnixInStreambuf(int fd) throw(IOException, bad_exception);
		
	
		/**
		 * Destructor, has to delete _buffer
		 */
		~UnixInStreambuf() throw();
		
		
	/*
	 * Private stuff
	 */
	private:	
		
		/**
		* filedescriptor
		*/
		const int _fd;
		
		/**
		* size of buffer
		*/
		int _bufferSize;
		
		/**
		* buffer
		*/
		char *_buffer;
		
		/*
		 * Methods
		 */
		/**
		* fill buffer by reading from filedescriptor
		*/
		int underflow () throw(IOException, bad_exception);
	
	};
}

#endif //UNIXINSTREAMBUF_H

 
