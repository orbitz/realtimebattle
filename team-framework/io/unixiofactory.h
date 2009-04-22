/************************************************************************
    $Id: unixiofactory.h,v 1.2 2005/01/06 17:59:28 jonico Exp $
    
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

    $Log: unixiofactory.h,v $
    Revision 1.2  2005/01/06 17:59:28  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef UNIXIOFACTORY_H
#define UNIXIOFACTORY_H


#include "iofactory.h"
#include "../log/logger.h"
#include "../rtbglobal/rtbglobalnamespace.h"


/**
* Namespace IO
*/
namespace IO {

	using RTBGlobal::MasterResourceControl;
	using Log::Logger;
	using Exceptions::RTBException;

	/**
	* Class IOFactory
	*/ 
	class UnixIOFactory : public IOFactory {
	/*
	 * Public stuff
	 */
	public:
		/**
		 * Constructor
		 */
		UnixIOFactory () throw(bad_exception);
		
		
		/**
		 * Destructor, does nothing.
		 */
		~UnixIOFactory () throw();

		
		/*
		 * Operations
		 */
		/**
		 * Tries to create a ServerCommunicator if no MasterServer is already present
		 * Otherwise, throws a ServerIsPresentException 
		 */
		auto_ptr <ServerCommunicator> createServerCommunicator () const throw(IOException, ServerIsPresentException, bad_exception);
		
		
		/**
		 * Since the RealTime Battle Server will restart our program (and therefore cause ugly problems)
		 * if we tell it, that we want to use a blocking communication mode, we do not say so (but stating the opposite), but manage
		 * the blocking with manipulating standard in and standard out by ourselves with this method.
		 * This method should be called before any read or write access to the RTBConnection object was made. 
		 */
		void switch2BlockingMode() const throw (IOException, bad_exception); 
		
			
		/**
		* This method create an ClientCommunicator and should be call after createServerCommunicator 
		* throws an ServerIsPresentException.
		*/
		auto_ptr <ClientCommunicator> createClientCommunicator () const throw(IOException, bad_exception);
		
	
		/**
		 * Important: RTBConnection should be initialized only once!!!
		 * NOTE: The concrete factory is in to delete the rtbconnection object!!!
		 */
		RTBConnection* getRTBConnection () const throw(IOException, bad_exception);
		
	/*
	* Private stuff
	*/
	private:
		/*
 		 * Fields
		 */
		/**
		* for resource-requisition
		*/
		MasterResourceControl* _mrc;
	};
}
#endif //UNIXIOFACTORY_H

