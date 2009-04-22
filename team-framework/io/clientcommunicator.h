/************************************************************************
    $Id: clientcommunicator.h,v 1.2 2005/01/06 17:59:27 jonico Exp $
    
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

    $Log: clientcommunicator.h,v $
    Revision 1.2  2005/01/06 17:59:27  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef CLIENTCOMMUNICATOR_H
#define CLIENTCOMMUNICATOR_H

#include "serverconnection.h"

/**
 * Namespace IO
 */
namespace IO {

        /** 
		 * Class ClientCommunicator
         */ 
        class ClientCommunicator {
        /*
         * Public stuff
         */
        public:
                /*
                 * Fields
                 */
                /*
                 * 
                 */
                /*
                 * Constructors
                 */
                /*
                 * Accessor Methods
                 */
                /*
                 * Operations
                 */
                /**
                 * Destructor, has to delete serverconnection.
                 */
                virtual ~ClientCommunicator () throw() {}
                        
                
                /**
                 * Establish connection to server.
				 * @return Connection to communicator directly with the server.
                 */
                virtual ServerConnection* getServerConnection () throw(IOException, bad_exception) = 0;
                        
                
        /*
         * Protected stuff
         */
        protected:
                /*
                 * Fields
                 */
                /*
                 * 
                 */
                /*
                 * Constructors
                 */
                /*
                 * Accessor Methods
                 */
                /*
                 * Operations
                 */
        /*
         * Private stuff
         */
        private:
                /*
                 * Fields
                 */
                /*
                 * 
                 */
                /*
                 * Constructors
                 */
                /*
                 * Accessor Methods
                 */
                /*
                 * Operations
                 */
        };
}
#endif //CLIENTCOMMUNICATOR_H 
