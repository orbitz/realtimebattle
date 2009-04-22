/************************************************************************
    $Id: potentialmap.cpp,v 1.11 2005/10/07 07:40:34 krolfy Exp $

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

    $Log: potentialmap.cpp,v $
    Revision 1.11  2005/10/07 07:40:34  krolfy
    Removed cerr debug lines

    Revision 1.10  2005/10/07 07:33:51  krolfy
    sources/alpha/bots/columbus/columbusclientattackstate.cpp
    sources/alpha/bots/columbus/columbusclientbasestate.cpp
    sources/alpha/bots/columbus/columbusserverattackstate.cpp
    sources/alpha/bots/columbus/columbusserverbasestate.cpp
    sources/alpha/bots/columbus/columbusserverdiscoverystate.cpp
    sources/alpha/bots/columbus/columbusserverescapestate.cpp
    sources/alpha/bots/columbus/columbusserverinitstate.cpp
    sources/alpha/bots/columbus/columbusservertrackstate.cpp
    sources/alpha/bots/general/maps/potentialmap.cpp
    sources/alpha/bots/general/maps/topographicmap_float.h
    sources/alpha/gamecontrol/servercoordinator.cpp
    sources/alpha/gamecontrol/servergamecontroller.cpp

    Revision 1.9  2005/08/23 15:37:05  krolfy
    *** empty log message ***

    Revision 1.8  2005/08/08 11:32:45  krolfy
    Added three Columbus bots (crash with exception)

    Revision 1.7  2005/07/29 12:33:13  krolfy
    Added isFreeVisionLine method to map classes

    Revision 1.6  2005/07/28 08:19:19  krolfy
    Most of the server side ready

    Revision 1.5  2005/06/02 17:35:35  krolfy
    Changed mines potential, introduced footprint potential

    Revision 1.4  2005/05/31 14:35:40  krolfy
    Undid the last change

    Revision 1.3  2005/05/31 14:26:29  krolfy
    Just a literal change

    Revision 1.2  2005/05/31 14:14:32  krolfy
    Changed the enemy potential to a better function

    Revision 1.1  2005/05/26 16:20:55  krolfy
    First testing version of PotentialMap


**************************************************************************/
#include <iostream>
using namespace std;
#include "potentialmap.h"
#include <cmath>
#include <common.h>

namespace Maps {

PotentialMap::PotentialMap(unsigned int width,unsigned int height,float density) throw(bad_exception):
	NavigateMap(width,height,density),
	_walls(0,width,height,density),
	_mines(0,width,height,density),
	_track(0,width,height,density),
	_density(density)
{
	addFinalValue(Parser::WALL);
	addFinalValue(Parser::MINE);
}

//
// Mine potential:
//	20/(r^2+4)
//	gradient at most about 4.5
//
// Wall potential:
//	1/(r^2+0.6)^2
//	gradient at most about 3.0
//
// Enemy potential:
//	16*ln(r^2+128)+3072/(r^2+128)
//	gradient at most about 0.8
//
// Footprint potential:
//	gradient at most about 1.3
//

void PotentialMap::addChange(const Position& self,const Position& obj,const object_type& type) throw(bad_exception)
{
	TopographicMap<object_type>::addChange(self,obj,type);
	const float dq=_density*_density;
	switch(type)
	{
		case Parser::MINE:
			{
				Position p=obj;
				p.y+=5*_density;
				int x,y;
				for(p.x-=5*_density,x=0;x!=11;p.x+=_density,++x)
					for(p.y-=11*_density,y=0;y!=11;p.y+=_density,++y)
						_mines.at(p)+=(20/((x*x+y*y)*dq+4));
			}
		case Parser::WALL:
			if(_walls.at(obj)<2.7)
			{
				Position p=obj;
				p.y+=3*_density;
				int x,y;
				float d;
				for(p.x-=2*_density,x=0;x!=5;p.x+=_density,++x)
					for(p.y-=5*_density,y=0;y!=5;p.y+=_density,++y,d=1)
						_walls.at(p)+=((d/=(x*x+y*y)*dq+0.6)*=d);
			}
			break;
	}
}

Position PotentialMap::navigationHint(const Position& self,const Position& target) throw (bad_exception)
{
	Position r=self;
	float dx=self.x-target.x;
	float dy=self.x-target.x;
	float rq=dx*dx+dy*dy+128.;
	float f=rq;
	((f-=192.)*=32.)/=(rq*=rq);
	r.x-=(dx*=f);
	r.y-=(dy*=f);
	Position a=_walls.gradient(self);
	r.x-=a.x;
	r.y-=a.y;
	a=_mines.gradient(self);
	r.x-=a.x;
	r.y-=a.y;
	a=_track.gradient(self);
	r.x-=a.x;
	r.y-=a.y;
	return r;
}

Position PotentialMap::navigationHint(const Position& self,const Position& target,float fEnemy,float fWalls,float fMines,float fTrack) throw (bad_exception)
{
	Position r=self;
	float dx=self.x-target.x;
	float dy=self.x-target.x;
	float rq=dx*dx+dy*dy+128.;
	float f=rq;
	((f-=192.)*=32.)/=(rq*=rq);
	r.x-=fEnemy*(dx*=f);
	r.y-=fEnemy*(dy*=f);
	Position a=_walls.gradient(self);
	r.x-=fWalls*a.x;
	r.y-=fWalls*a.y;
	a=_mines.gradient(self);
	r.x-=fMines*a.x;
	r.y-=fMines*a.y;
	a=_track.gradient(self);
	r.x-=fTrack*a.x;
	r.y-=fTrack*a.y;
	return r;
}

void PotentialMap::reset() throw()
{
	_walls.reset();
	_mines.reset();
}

void PotentialMap::makeFootPrint(const Position& pos,float depth) throw(bad_exception)
{
	Position p=pos;
	float d;
	int x,y;
	p.y+=3*_density;
	if(depth<0)
		for(p.x-=2*_density,x=0;x!=5;p.x+=_density,++x)
			for(p.y-=5*_density,y=0;y!=5;p.y+=_density,++y,d=depth/(1+x*x+y*y))
				(_track.at(p)*=(1.0-d))-=2.0*d;
	else
		for(p.x-=2*_density,x=0;x!=5;p.x+=_density,++x)
			for(p.y-=5*_density,y=0;y!=5;p.y+=_density,++y,d=depth/(1+x*x+y*y))
				(_track.at(p)*=(1.0+d))-=2.0*d;
}

/*bool PotentialMap::isFreeVisionLine(const Position& self,const Position& obj) const throw (bad_exception)
{
	return _items.isFreeVisionLine(self,obj);
}
*/
}
