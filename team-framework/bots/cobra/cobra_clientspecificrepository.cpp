/************************************************************************
	  			cobra_clientspecificrepository.cpp
Copyright:
    Tino Truppel <tino.truppel@hpi.uni-potsdam.de>

This file was created on Mon, Nov 13 2004

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


The original location of this file is cobra
**************************************************************************/

#include <common.h>
#include "cobra_clientspecificrepository.h"

namespace cobra {
			using std::string;
			using Exceptions::RTBException;

 			/**
			* Method to reset the repositories (clears number of collisions)
			*/
			void  cobra_ClientSpecificRepository::reset() throw(StrategyException, bad_exception) {}

			/**
			 * Destructor
			*/
			cobra_ClientSpecificRepository::~cobra_ClientSpecificRepository() throw() {};

			/**
			 * Constructor
			*/
			cobra_ClientSpecificRepository::cobra_ClientSpecificRepository() throw(StrategyException, bad_exception) {}
}
