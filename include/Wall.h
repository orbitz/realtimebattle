/*
RealTimeBattle, a robot programming game for Unix
Copyright (C) 1998-2000  Erik Ouchterlony and Ragnar Ouchterlony

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foundation,
Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef __WALL__
#define __WALL__

#include "GeometricalObjects.h"

class WallCircle : public Circle
{
public:
  WallCircle(const Vector2D& c, const double r, 
             const double b_c, const double hardn) : Circle(c, r) 
    { bounce_coeff = b_c; hardness_coeff = hardn; }

  ~WallCircle() {}
};

class WallInnerCircle : public InnerCircle
{
public:
  WallInnerCircle(const Vector2D& c, const double r, 
             const double b_c, const double hardn) : InnerCircle(c, r) 
    { bounce_coeff = b_c; hardness_coeff = hardn; }
  ~WallInnerCircle() {}
};

class WallLine : public Line
{
public:
  WallLine(const Vector2D& sp, const Vector2D& d, const double len, const double th, 
           const double b_c, const double hardn) : Line(sp, d, len, th) 
    { bounce_coeff = b_c; hardness_coeff = hardn; }
  ~WallLine() {}
};


class WallArc : public Arc
{
public:
  WallArc(const Vector2D& c, const double& r1, const double& r2,
          const double a1, const double a2, 
          const double b_c, const double hardn) : Arc(c, r1, r2, a1, a2) 
    { bounce_coeff = b_c; hardness_coeff = hardn; }
  ~WallArc() {}
};


#endif 
