/************************************************************************
    $Id: unixfilelogdriver.h,v 1.2 2005/01/06 17:59:28 jonico Exp $
    
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

    $Log: unixfilelogdriver.h,v $
    Revision 1.2  2005/01/06 17:59:28  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef UNIXFILELOGDRIVER_H
#define UNIXFILELOGDRIVER_H


#include "logdriver.h"
#include <map>
struct stat;

/**
 * Namespace Log
 */
namespace Log {
	using std::map;
	/**
	* Class UnixFileLogDriver
	*
	* UnixfileLogdriver will log into a file, that is not createtd at the moment
	* If several components chose to log in the same file, it will work with this driver
	* UnixFileLogDriver is not thread safe (the whole framework is not)
	*/ 
	class UnixFileLogDriver: public LogDriver {
	/*
	* Public stuff
	*/
	public:
		/**
		* Destructor
		* If _isCloned is false, it will close all descriptors in the FDMap
		*/
		virtual ~UnixFileLogDriver () throw();
		/**
		* Constructor for the first UnixFileLogDriver object
		* Constructor will register the object by the MRC with the name UnixFileLogDriver
		*/
		UnixFileLogDriver() throw (bad_exception);
		
		/*
		*  Operations
		*/
	
		/**
		* Method that is called by the MRC to duplicate the whole object
		*/
		virtual auto_ptr <LogDriver>  clone () const throw (bad_exception);
		
	
		/**
		* 
		* @param message Message that will be logged in the file
		* If logger object was not initialized properly, this method will only throw an exception
		*/
		virtual void  logMessage (const string& message) throw (IOException, bad_exception);
		
	
		/**
		* 
		* @param logDriverParameters The parameter has to be in the form file:filename, for example file:/tmp/foo
		* If the specified file already exists or it cannot be created, this method will throw an exception and
		* the UnixFileLogDriver object will not work. If the file was already opened by another component of the same process,
		* logging will work properly.
		*/
		virtual void  setLogDriverParameters (const string& logDriverParameters) throw (IOException, bad_exception);
	
	/*
	* Private stuff
	*/
	private:
		/**
		 * if _isCloned is true, this object was created with the clone method
		 */
		bool _isCloned;
		
		/**
		 * if _isInitialized is true, you can use this object to log messages
		 */
		bool _isInitialized;
		
		/**
		 * file descriptor to the corresponding file
		*/
		int _fd;
		
		/**
		 * map with all filedesriptors opened by UnixFileLogDriver objects cloned by this special root object
		 * if _isCloned is set, this pointer will contain NULL
		*/
		auto_ptr< map <long long int, int> > _rootFDMap;
		
		/**
		 * A pointer to the root map
		 * This pointer will exist in all objects
		 */	
		map < long long int, int >* _clonedFDMap;
		
		/**
		 * Constructor used in the clone Method to create a clone of the current object according to the prototype pattern
		 */
		UnixFileLogDriver(int isInitialized, int fd, map < long long int, int > *clonedFDMap) throw ();	
		
		/**
		 * Computes the position of a file for the index in the map
		 */
		long long int computePosition(const struct stat& statbuf) const throw();
	};
}
#endif //UNIXFILELOGDRIVER_H

