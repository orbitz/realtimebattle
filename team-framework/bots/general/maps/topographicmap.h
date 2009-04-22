/************************************************************************
    $Id: topographicmap.h,v 1.12 2005/08/23 15:37:05 krolfy Exp $
    
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

    $Log: topographicmap.h,v $
    Revision 1.12  2005/08/23 15:37:05  krolfy
    *** empty log message ***

    Revision 1.11  2005/08/10 09:48:13  krolfy
    Bugfixed ugly for loop

    Revision 1.10  2005/07/29 12:33:14  krolfy
    Added isFreeVisionLine method to map classes

    Revision 1.9  2005/05/26 16:20:55  krolfy
    First testing version of PotentialMap

    Revision 1.8  2005/05/25 12:43:56  krolfy
    gradient method for TopographicMap<float>

    Revision 1.7  2005/05/25 11:20:33  krolfy
    _density made const

    Revision 1.6  2005/05/25 11:00:47  krolfy
    Since vector<bool> is specialized, BoolMap is not necessary

    Revision 1.5  2005/05/18 14:35:20  krolfy
    TopographicMap is working. All contents moved from .cpp to .h

    Revision 1.4  2005/05/11 13:35:08  krolfy
    First (hopefully) usable version

    Revision 1.3  2005/04/14 14:20:23  jonico

    now we have explicit template instatiation

    Revision 1.2  2005/04/06 11:13:50  krolfy
    Changed all (double x,double y) to General::Position

    Revision 1.1  2005/04/05 08:49:28  krolfy
    First publication of the map classes


**************************************************************************/

#ifndef TOPOGRAPHICMAP_H
#define TOPOGRAPHICMAP_H

#include "basemap.h"
#include <vector>
#include <list>
/**
* Namespace Maps
*/
namespace Maps {
	using std::vector;
	using std::list;

/**
 * A map using a two-dimensional vector to store pixelized data of the arena, auto-resizing
 * Most often this will be used as TopographicMap<Parser::object_type>
 * For _Tp, besides a copy constructor, the following operators are needed:
 * operator==(const Tp&,const Tp&)
 * operator<(const Tp&,const Tp&)
 * operator=(Tp&,const Tp&)
 */
template<typename _Tp>
class TopographicMap:public BaseMap<_Tp>
{
	vector<vector<_Tp> > _map;
	list<_Tp> _final;
	int _width,_height,_lastX,_lastY;
	_Tp _default,_empty;
	const float _density;

	/*
	 * After the call of this method, you can directly access _map like
	 * _map.at(x).at(y)
	 * Note that for non-negative pos.x, even x are used,
	 * for negative pos.x odd x, and the same for y.
	 * @param pos The requested position in RTB standard coordinates
	 */
	void _getIndices(const Position& pos,int& x,int& y) const throw()
	{
		(x=int(pos.x/_density+.5))<0?++(x*=-2):x<<=1;
		(y=int(pos.y/_density+.5))<0?++(y*=-2):y<<=1;
	}

public:
	/**
	 * Creates an empty map.
	 * @param defaultValue The map will initially be filled with this
	 * @param width initial map width in grid squares
	 * @param height initial map height in grid squares
	 * @param density edge length of the grid squares
	 */
	TopographicMap(const _Tp& defaultValue,unsigned int width=50,unsigned int height=50,float density=.7071) throw (bad_exception):
		_default(defaultValue),
		_empty(defaultValue),
		_width(width),
		_height(height),
		_density(density),
		_map(width,vector<_Tp>(height,defaultValue)) {}

	virtual ~TopographicMap() throw() {}

	/*
	 * If the map is too small, this method enlarges it. It also reserves little extra space so that actual enlargement is not needed that often.
	 * New entries will be filled with the default value
	 * @param w new minimum width of the map
	 * @param h new minimum height of the map
	 */
	void resize(int w,int h) throw (bad_exception)
	{
		if(h>=_height)
		{
			++(h+=h>>3);
			for(int i=0;i!=_width;++i)
				_map.at(i).resize(h,_default);
			_height=h;
		}
		if(w>=_width)
			_map.resize(_width=++(w+=w>>3),vector<_Tp>(_height,_default));
	}

	/**
	 * Every time after having received both the commands "Coordiantes" and "Radar", the ServerState is to call this method if it saw something worth to draw onto the map.
	 * @param self the position of the ServerState's robot
	 * @param obj the position of the observed object
	 * @param type the type of the observed object
	 */
	virtual void addChange(const Position& self,const Position& obj,const _Tp& type) throw (bad_exception)
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
			for(Position p=self;d;p.x-=dx,p.y-=dy,--d)
			{
				_getIndices(p,x,y);
				_Tp *value=&_map.at(x).at(y);
				for(typename list<_Tp>::iterator i=_final.begin();i!=_final.end();++i)
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

	/**
	 * Check a chord for being a free vision line
	 * If one of the endpoints is out of bounds, false is returned.
	 * @param self first endpoint of the chord
	 * @param obj second endpoint of the chord
	 * @return whether there are only "empty" values (see setEmptyValue) on the chord
	 */
	virtual bool isFreeVisionLine(const Position& self,const Position& obj) const throw (bad_exception)
	{
		int x,y,X,Y,d;
		_getIndices(self,x,y);
		_getIndices(obj,X,Y);
		x<<=1;	y<<=1;
		X<<=1;	Y<<=1;
		if(	x>=_width||y>=_height||
			x<=-_width||y<=-_height||
			X>=_width||Y>=_height||
			X<=-_width||Y<=-_height	)
			return false;
		x>>=1;	y>>=1;
		X>>=1;	Y>>=1;
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
			for(Position p=self;d;p.x-=dx,p.y-=dy,_getIndices(p,x,y),--d)
				if(_map.at(x).at(y)!=_empty)
					return false;
		}
		return true;
	}

	/**
	 * To be called when a new round on a new map starts
	 */
	virtual void reset() throw()
	{
		for(typename vector<vector<_Tp> >::iterator i=_map.begin();i!=_map.end();++i)
			for(typename vector<_Tp>::iterator j=i->begin();j!=i->end();++j)
				*j=_default;
	}

	/**
	 * Access using the RTB standard coordinates
	 * @param x x coordinate of the requested position
	 * @param y y coordinate of the requested position
	 * @return the value at the requested position
	 */
	_Tp& at(const Position& pos) throw(bad_exception)
	{
		_getIndices(pos,_lastX,_lastY);
		resize(_lastX,_lastY);
		return _map.at(_lastX).at(_lastY);
	}

	/*
	 * Set the value that will be used whenever reset is invoked
	 * @param value the new default value
	 */
	void setDefaultValue(const _Tp& value) throw()
	{
		_default=value;
	}

	/*
	 * Set the value that will be considered as empty space
	 * If this method is never invoked, the value is the first parameter of the constructor.
	 * @param value the new value meaning "empty space"
	 */
	void setEmptyValue(const _Tp& value) throw()
	{
		_empty=value;
	}

	/*
	 * A final value, once assigned to a grid square, cannot be overwritten with the "empty" value (see setEmptyValue)
	 * For example, this can prevent walls from vanishing from the map due to rounding errors.
	 * @param value the new final value
	 */
	void addFinalValue(const _Tp& value)
	{
		for(typename list<_Tp>::iterator i=_final.begin();i!=_final.end();++i)
			if(value<*i)
			{
				_final.insert(i,value);
				return;
			} else if(*i==value)
				return;
		_final.push_back(value);
	}

	/*
	 * Make a former final value no longer final (See addFinalValue)
	 * @param value the old final value
	 */
	void deleteFinalValue(const _Tp& value)
	{
		for(typename list<_Tp>::iterator i=_final.begin();i!=_final.end();++i)
			if(*i==value)
			{
				_final.erase(i);
				return;
			}
	}

	/*
	 * @return the edge length od the map grid squares
	 */
	float getDensity() const throw()
	{
		return _density;
	}

	/*
	 * For TopographicMap<float>, there is a specialization.
	 * @return the gradient at pos if _Tp==float, otherwise 0
	 */
	Position gradient(const Position& pos)
	{
		return Position();
	}
};

} // Namespace

#include "topographicmap_float.h"

#endif
