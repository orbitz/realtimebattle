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

#ifndef __SHAPE__
#define __SHAPE__

class Vector2D;

#ifndef NO_GRAPHICS
#include <gdk/gdktypes.h>
#include <gdk/gdk.h> 
#endif


/*

  Shape is the base class for all objects in the arena. The class hierarchy
  is as follows:

                  ---------
                  | Shape | 
                  ---------
            _____/    |     \_____________
           /          |                   \
   --------       ----------               ---------------
   | Line |       | Circle |               | InnerCircle |
   --------       ----------               ---------------
      |            |  |    \                       \
      |            |  |     \                       \
 ------------      |  |     --------------       -------------------
 | WallLine |     /   |     | WallCircle |       | WallInnerCircle |
 ------------    /     \    --------------       -------------------
                /       \
       ----------        ----------------
       | Extras |        | MovingObject |
       ----------        ----------------
       /      \             |        \      
      /        \            |         \     
 ----------  --------    --------  ---------
 | Cookie |  | Mine |    | Shot |  | Robot |
 ----------  --------    --------  ---------

 */







class Shape
{
public:
  Shape();
  Shape(int long colour);

  virtual ~Shape() {}

  virtual double get_distance(const Vector2D& pos, const Vector2D& vel, 
                              const double size) = 0;
  virtual bool within_distance(const Vector2D& pos, const double size) = 0;
  virtual Vector2D get_normal(const Vector2D& pos) = 0;

  int get_id() { return id; }
  long int get_rgb_colour() { return rgb_colour; }

  double get_bounce_coeff() { return bounce_coeff; }
  double get_hardness_coeff() { return hardness_coeff; }

  //  virtual arenaobject_t get_arenaobject_t() = 0;
  //  friend void bounce_on_wall(class Robot& robot, const class Shape& wall, const Vector2D& normal);

#ifndef NO_GRAPHICS
  virtual void draw_shape(bool erase) = 0;
  GdkColor& get_gdk_colour() { return gdk_colour; }
  void set_colour(const GdkColor& gdk_colour);
#endif

  void set_colour(const long int colour);

protected:
#ifndef NO_GRAPHICS
  GdkColor gdk_colour;
#endif
  long int rgb_colour;

  double bounce_coeff;
  double hardness_coeff;
  int id;
};

#endif 
