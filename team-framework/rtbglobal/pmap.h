/************************************************************************
    $Id: pmap.h,v 1.3 2005/01/06 17:59:29 jonico Exp $
    
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

    $Log: pmap.h,v $
    Revision 1.3  2005/01/06 17:59:29  jonico
    Now all files in the repository have their new header format.


**************************************************************************/
#ifndef __pmap_h
#define __pmap_h
#include <map>

/**
* Namespace RTBGlobal
*/
namespace RTBGlobal {
  	using std::map;

	/**
	* class template pmap
	* Important note: IF YOU CALL METHODS OF THIS CLASS, THAT REMOVE ELEMENTS, YOU HAVE TO DELETE THE ELEMENTS BY YOURSELF
	*  IF AN OBJECT OF THIS CLASS WILL BE DESTROYED, IT WILL DELETE ALL IT'S ELEMENT	
	*  Use this class if you want to store dynamic resources, that should be deleted in conjunction with its container.
	*  You can use any functionality (operators / methods / iterators) provided by the map base class of the STL.
	*  @author Johannes Nicolai
	*/
	template <class Key, class __pValue> class pmap: public map <Key, __pValue*> {
	/*
	* Public stuff
	*/
	public:
		/**
		* Constructs a pmap, that stores pointers to __pValue objects
		*/
		inline pmap():map<Key, __pValue*>() {};

		/**
 		* All resources pointed by the stored values, will be deleted if the pmap will be destroyed 
		*/
		inline ~pmap() {
			for (typename map<Key, __pValue*>::iterator i=map<Key,__pValue*>::begin();i!=map<Key,__pValue*>::end();++i)
				delete i->second;
		};
	};
}
#endif
