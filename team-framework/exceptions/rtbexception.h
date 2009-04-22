/************************************************************************
    $Id: rtbexception.h,v 1.2 2005/01/06 17:59:27 jonico Exp $
    
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

    $Log: rtbexception.h,v $
    Revision 1.2  2005/01/06 17:59:27  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef RTBEXCEPTION_H
#define RTBEXCEPTION_H

#include <string>
#include <exception>

/**
 * Namespace Exception
 */
namespace Exceptions {
	using std::exception;
	using std::string;

	/**
	* Class RTBException
	*/
	class RTBException : public exception{
	/*
	* Public stuff
	*/
	public:
		/*
		* Operations
		*/
		/**
		* Method to obtain an error message in RTBException flavour
		*/
		virtual const string& getMessage () const throw()= 0;
	
		/**
		* Method to obtain an error message in std::exception flavour
		*/
		virtual const char* what() const throw();	
	
		/**
		* destructor, does nothing by default
		*/
		virtual  ~RTBException () throw() {};
		
	
	};
}
#endif //RTBEXCEPTION_H

