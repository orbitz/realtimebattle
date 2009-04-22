/************************************************************************
    $Id: bfinternalserverspecificrepository.cpp,v 1.2 2005/01/06 17:59:26 jonico Exp $
    
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

    $Log: bfinternalserverspecificrepository.cpp,v $
    Revision 1.2  2005/01/06 17:59:26  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#include <common.h>

#include "bfinternalserverspecificrepository.h"

namespace Brotfrucht {
	/**
	* Method to reset the repositories (clears position)
	*/
	void  BFInternalServerSpecificRepository::reset() throw() {
		_robotX=_robotY=0;
	}

	/**
	* Set the new position of the robot
	*/
	void  BFInternalServerSpecificRepository::setRobotPosition(double x, double y) throw() {
		_robotX=x;
		_robotY=y;
	}
	
	/**
	 * Get the position of the robot
	 */ 
	void  BFInternalServerSpecificRepository::getRobotPosition(double& x, double& y) const throw() {
		x=_robotX;
		y=_robotY;
	}
	
	/**
	 * Destructor (does nothing)
	*/
	BFInternalServerSpecificRepository::~BFInternalServerSpecificRepository() throw() {}
	 
	 /**
	  * Constructor
	  */
	BFInternalServerSpecificRepository::BFInternalServerSpecificRepository() throw():_robotX(0), _robotY(0) {};

}

