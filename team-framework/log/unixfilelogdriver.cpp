/************************************************************************
    $Id: unixfilelogdriver.cpp,v 1.2 2005/01/06 17:59:28 jonico Exp $
    
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

    $Log: unixfilelogdriver.cpp,v $
    Revision 1.2  2005/01/06 17:59:28  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#include <common.h>

#include "unixfilelogdriver.h"
#include <string>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


// #include "../rtbglobal/masterresourcecontrol.h"

/**
 * Namespace
 */
namespace Log {
	// using RTBGlobal::MasterResourceControl;
	// Class UnixFileLogDriver
	// UnixfileLogdriver will log into a file, that is not createtd at the moment
	// If several components chose to log in the same file, it will work with this driver
	// UnixFileLogDriver is not thread safe (the whole framework is not)
	// 
	/**
	* Public stuff
	*/
	/**
	* Destructor
	* If _isCloned is false, it will close all descriptors in the FDMap
	*/
	UnixFileLogDriver::~UnixFileLogDriver () throw() {
		if (!_isCloned) {
			if (_clonedFDMap) {
				map <long long int, int>::const_iterator i=_clonedFDMap->begin();
				while (i!=_clonedFDMap->end()) 
					close ((i++)->second);
			}
		}
	}
	/**
	* Constructor for the first UnixFileLogDriver object
	* Constructor will register the object by the MRC with the name UnixFileLogDriver
	*/
	UnixFileLogDriver::UnixFileLogDriver() throw (bad_exception):_isCloned(false),_isInitialized(false),_fd(-1),_rootFDMap(new map <long long int, int>()),_clonedFDMap(NULL) {
		_clonedFDMap=_rootFDMap.get();
		
		// register this initial object by the MRC
		// new registration method now
		// MasterResourceControl::Instance()->registrateLogDriverPrototype("UnixFileLogDriver",this);
	}
	
	/**
	*  Operations
	*/

	/**
	* Method that is called by the MRC to duplicate the whole object
	*/
	auto_ptr <LogDriver>  UnixFileLogDriver::clone () const throw (bad_exception)
	{
		return auto_ptr<LogDriver>(new UnixFileLogDriver(_isInitialized, _fd, _clonedFDMap));
	}
	

	/**
	* 
	* @param message Message that will be logged in the file
	* If logger object was not initialized properly, this method will only throw an exception
	*/
	void  UnixFileLogDriver::logMessage (const string& message) throw (IOException, bad_exception) 
	{
		if (!_isInitialized)
			throw IOException("UnixFileLogDriver was not configured properly so logging is impossible!");
		int result=write(_fd,(message+"\n").c_str(),message.size()+1);
		if (result==-1)
			throw IOException((string("UnixFileLogDriver got an error while writing to a file descriptor: ")+strerror(errno)));
		// syncing was found out to be too slow for the framework and heavy load of logging, so I commented it out
		// fdatasync(_fd);
	}
	

	/**
	* 
	* @param logDriverParameters The parameter has to be in the form file:filename, for example file:/tmp/foo
	* If the specified file already exists or it cannot be created, this method will throw an exception and
	* the UnixFileLogDriver object will not work. If the file was already opened by another component of the same process,
	* logging will work properly.
	*/
	void  UnixFileLogDriver::setLogDriverParameters (const string& logDriverParameters)  throw (IOException, bad_exception) 
	{
		_isInitialized=false;
		// first find out, if parameter is correct
		if (logDriverParameters.find("file:",0,5))
			throw IOException("logDriverParameters have incorrect syntax for UnixFileLogger (does not begin with 'file:')!");
		string filename(logDriverParameters,5); // extract filename from parameters
		// second stat the file
		struct stat statbuf;
		int result=stat(filename.c_str(),&statbuf);
		if (result==-1) {
			switch (errno) {
				case ENOENT: { // file does not exist
					_fd=open(filename.c_str(),O_WRONLY|O_CREAT|O_EXCL,S_IRUSR|S_IWUSR);
					if (_fd==-1)
						throw IOException("UnixFileLogDriver got an error while trying to create file "+ filename+ ": "+strerror(errno));
					// register new file descriptor
					int result2=fstat(_fd,&statbuf);
					if (result2==-1) {
						close(_fd);
						throw IOException("UnixFileLogDriver got an error while trying to stat created file "+ filename+ ": "+strerror(errno));
					}
					try {
						(*_clonedFDMap)[computePosition(statbuf)]=_fd;
					}
					catch (...) {
						close(_fd);
						throw;
					}
					_isInitialized=true;
					return;
				}
				default: {
					throw IOException("UnixFileLogDriver got an error while trying to stat file "+ filename+ ": "+strerror(errno));
				}
			}
		}

		
		map < long long int, int >::const_iterator i=_clonedFDMap->find(computePosition(statbuf));
		if (i==_clonedFDMap->end())
			throw IOException("UnixFileLogDriver refused to write in already existing file "+ filename);
		_fd=i->second;
		_isInitialized=true;
	}
	
	/**
	* Private stuff
*/
	/**
	 * Constructor used in the clone Method to create a clone of the current object according to the prototype pattern
	 */
	UnixFileLogDriver::UnixFileLogDriver(int isInitialized, int fd, map < long long int, int >  *clonedFDMap) throw ():_isCloned(true),_isInitialized(isInitialized),_fd(fd),_rootFDMap(NULL),_clonedFDMap(clonedFDMap)
	{
	}
	
	long long int UnixFileLogDriver::computePosition(const struct stat& statbuf) const throw()
	{
	//	buffer[0]=statbuf.st_dev;
	//	printf("device: %il\n",statbuf.st_dev);
	//	buffer[1]=statbuf.st_ino;
	//	printf("indode: %il\n",statbuf.st_ino);
	//	long long int position=*(reinterpret_cast<long long int*>(buffer));
	//	printf("Position: %ill\n",position);
		long long int position=statbuf.st_dev;
		position<<=32;
		return position+statbuf.st_ino;
	}
	namespace { // anonymous namespace
// new registration method now
//		UnixFileLogDriver __unixfilelogdriverprototype; // we need a global object to register the driver by the MRC
		
	}
}

