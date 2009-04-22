/************************************************************************
    $Id: confignotloadedexception.h,v 1.2 2005/01/06 17:59:27 jonico Exp $
    
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

    $Log: confignotloadedexception.h,v $
    Revision 1.2  2005/01/06 17:59:27  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef CONFIGNOTLOADEDEXCEPTION_H
#define CONFIGNOTLOADEDEXCEPTION_H

#include "rtbexception.h"

/**
 * Namespace Exception
 */
namespace Exceptions {
	/**
	* Class ConfigNotLoadedException
	* This exception should be thrown by the MRC in case of requests to it before having obtained
	* the configuration data from the parser
	*/
	class ConfigNotLoadedException : public RTBException {
	/*
	* Public stuff
	*/
	public:
		/*
		* Constructors
		*/
		/**
		* Constructs an ConfigNotLoadedException with a concrete errormessage
		* @param message Concrete error message
		*/
		ConfigNotLoadedException(const string& errormessage): _message("ConfigNotLoadedException: "+ errormessage) {};
		/**
		* Constructs an ConfigNotLoadedException with default errormessage
		*/
		ConfigNotLoadedException(): _message("ConfigNotLoadedException: Request to MRC while config data was not loaded") {}; 
		/*
		* Operations
		*/
		/**
		* Returns the concrete error message
		*/
		virtual const string&  getMessage () const throw();
		
	
		/**
		* destructor, does nothing by default
		*/
		virtual  ~ConfigNotLoadedException () throw();
			
	/*
	* Private stuff
	*/
	private:
		/*
		* Fields
		*/
		/**
		* Contains the concrete error message
		*/
		string _message;
	};
}
#endif //CONFIGNOTLOADEDEXCEPTION_H

