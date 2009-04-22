/************************************************************************
    $Id: topographicmap_float.h,v 1.5 2005/10/07 07:40:34 krolfy Exp $
    
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

    $Log: topographicmap_float.h,v $
    Revision 1.5  2005/10/07 07:40:34  krolfy
    Removed cerr debug lines

    Revision 1.4  2005/10/07 07:33:51  krolfy
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

    Revision 1.3  2005/08/23 15:37:05  krolfy
    *** empty log message ***

    Revision 1.2  2005/05/26 16:20:56  krolfy
    First testing version of PotentialMap

    Revision 1.1  2005/05/25 12:43:56  krolfy
    eradient method for TopographicMap<float>



**************************************************************************/

#ifndef TOPOGRAPHICMAP_FLOAT_H
#define TOPOGRAPHICMAP_FLOAT_H

#include <iostream>
using namespace std;
namespace Maps {
	using std::vector;
	using std::list;

template<>
class TopographicMap<float>:public BaseMap<float>
{
	vector<vector<float> > _map;
	list<float> _final;
	int _width,_height,_lastX,_lastY;
	float _default,_empty;
	const float _density;

	void _getIndices(const Position& pos,int& x,int& y) throw()
	{
		(x=int(pos.x/_density+.5))<0?++(x*=-2):x<<=1;
		(y=int(pos.y/_density+.5))<0?++(y*=-2):y<<=1;
	}

public:
	TopographicMap(const float& defaultValue,unsigned int width=50,unsigned int height=50,float density=.7071) throw (bad_exception):
		_default(defaultValue),
		_empty(defaultValue),
		_width(width),
		_height(height),
		_density(density),
		_map(width,vector<float>(height,defaultValue)) {}

	virtual ~TopographicMap() throw() {}

	void resize(int w,int h) throw (bad_exception)
	{
		++(w+=w>>3);
		++(h+=h>>3);
		if(h>=_height)
		{
			for(int i=0;i!=_width;++i)
				_map.at(i).resize(h,_default);
			_height=h;
		}
		if(w>=_width)
			_map.resize(_width=w,vector<float>(_height,_default));
	}

	virtual void addChange(const Position& self,const Position& obj,const float& type) throw (bad_exception)
	{
		int x,y,d;
		_getIndices(self,x,y);
		_getIndices(obj,_lastX,_lastY);
		resize(x>_lastX?x:_lastX,y>_lastY?y:_lastY);
		double Dx=self.x-obj.x,Dy=self.y-obj.y,dx,dy;
		if(Dx!=0 || Dy!=0)
		{
			Dx/=(dx=Dx<0?-_density:_density);
			Dy/=(dy=Dy<0?-_density:_density);
			if(Dx<Dy)
			{
				d=int(Dy);
				dx*=Dx/=Dy;
			} else {
				d=int(Dx);
				dy*=Dy/=Dx;
			}
			float *value;
			for(Position p=self;d;p.x-=dx,p.y-=dy,_getIndices(p,x,y),value=&_map.at(x).at(y),--d)
			{
				for(list<float>::iterator i=_final.begin();i!=_final.end();++i)
					if(*i==*value)
					{
						value=0;
						break;
					}
				if(value)
					*value=_empty;
			}
		}
		_map.at(_lastX).at(_lastY)=type;
	}

	virtual void reset() throw()
	{
		for(vector<vector<float> >::iterator i=_map.begin();i!=_map.end();++i)
			for(vector<float>::iterator j=i->begin();j!=i->end();++j)
				*j=_default;
	}

	float& at(const Position& pos) throw(bad_exception)
	{
		_getIndices(pos,_lastX,_lastY);
		resize(_lastX,_lastY);
		return _map.at(_lastX).at(_lastY);
	}

	void setDefaultValue(const float& value) throw()
	{
		_default=value;
	}

	void setEmptyValue(const float& value) throw()
	{
		_empty=value;
	}

	void addFinalValue(const float& value)
	{
		for(list<float>::iterator i=_final.begin();i!=_final.end();++i)
			if(value<*i)
			{
				_final.insert(i,value);
				return;
			} else if(*i==value)
				return;
		_final.push_back(value);
	}

	void deleteFinalValue(const float& value)
	{
		for(list<float>::iterator i=_final.begin();i!=_final.end();++i)
			if(*i==value)
			{
				_final.erase(i);
				return;
			}
	}

	float getDensity() const throw()
	{
		return _density;
	}

	Position gradient(const Position& pos)
	{
		int x1,y1;
		int x=int(pos.x/_density);
		if(x<-1)	(x1=++(x*=-2))-=2;
		else if(x!=-1)	(x1=x<<=2)+=2;
		else		{ x=1;x1=0; }
		int y=int(pos.y/_density);
		if(y<-1)	(y1=++(y*=-2))-=2;
		else if(y!=-1)	(y1=y<<=2)+=2;
		else		{ y=1;y1=0; }
		resize(x>0?x:-x,y>0?y:-y);
		float olur=_map.at(x1).at(y)-_map.at(x).at(y1);
		float ulor=_map.at(x1).at(y1)-_map.at(x).at(y);
		return Position((olur+ulor)/_density,(olur-ulor)/_density);
	}
};

} // Namespace

#endif
