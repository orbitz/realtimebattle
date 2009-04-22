/************************************************************************
    $Id: initcomponents.h,v 1.2 2005/01/06 17:59:18 jonico Exp $
    
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

    $Log: initcomponents.h,v $
    Revision 1.2  2005/01/06 17:59:18  jonico
    Now all files in the repository have their new header format.


**************************************************************************/

#ifndef INITCOMPONENTS_H
#define INITCOMPONENTS_H

/**
 * This function is responsable for registrating all components of the framework
 * It is necessary to have such a method because global objects in anonymous namespaces do not initialize in a presumable
 * order and we got lots of trouble (segfaults, ...). To avoid this, EVERY new pluggable component has to be registered in
 * this function that is called by the main method
 * This method can throw any type of exception
 */

void initComponents();

#endif
