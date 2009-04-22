/************************************************************************
    $Id: columbusclientattackstate.h,v 1.3 2005/08/17 14:08:43 krolfy Exp $
    
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

    $Log: columbusclientattackstate.h,v $
    Revision 1.3  2005/08/17 14:08:43  krolfy
    *** empty log message ***

    Revision 1.2  2005/08/10 16:00:50  krolfy
    Columbus behaves odd, can SIGSEGV

    Revision 1.1  2005/08/05 14:29:19  krolfy
    Derived ColumbusClientStates


**************************************************************************/

#ifndef COLUMBUSCLIENTATTACKSTATE_H
#define COLUMBUSCLIENTATTACKSTATE_H

#include "columbusclientbasestate.h"
#include "columbusnamespace.h"

/**
* Namespace Columbus
*/
namespace Columbus {

	/**
	* Class ColumbusClientAttackState
	*
	* This class is the base of all columbus client states. Here, the default actions, that will be done by every state
	* (unless it defines something else) are determined. States, that want to extend and not to entirely replace the action
	* of this base class, should simpkly call the corresponding method in this class before executinh their own stuff
	*/ 
	class ColumbusClientAttackState: public ColumbusClientBaseState {
	public:
		ColumbusClientAttackState(ClientCoordinator* i,ColumbusClientSpecificRepository* columbuscsp,const GameOptionsRepository* go) throw (StrategyException,bad_exception);
		virtual  ~ColumbusClientAttackState () throw();
		/**
		* This method is called when all server tips in this round were received. After this method, the state is to send its last messages. 
		*/
		virtual void  sendRTBMessages () throw (StrategyException,bad_exception);

	private:
		ColumbusClientSpecificRepository* _csp;
		const GameOptionsRepository* _go;
		int _shoot;
	};
	
}

#endif
