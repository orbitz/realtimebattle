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

#ifndef __GEOMETRICAL_OBJECTS__
#define __GEOMETRICAL_OBJECTS__

#include "Shape.h"
#include "Vector2D.h"

// ---------------------  Line : Shape ---------------------

class Line : public Shape
{
public:
  Line();
  Line(const Vector2D& sp, const Vector2D& d, const double len, const double th);
  Line(const Vector2D& sp, const Vector2D& d, const double len, const double th, 
       const double b_c, const double hardn);
  virtual ~Line() {}

  double get_distance(const Vector2D& pos, const Vector2D& vel, const double size);
  bool within_distance(const Vector2D& pos, const double size);
  Vector2D get_normal(const Vector2D& pos);

#ifndef NO_GRAPHICS
  void draw_shape(bool erase);
#endif

  Vector2D get_start_point() { return start_point; }
  Vector2D get_direction() { return direction; }
  double get_length() { return length; }
  double get_thickness() { return thickness; }

protected:
  Vector2D start_point;
  Vector2D direction;
  double length;
  double thickness;

  Vector2D last_drawn_start_point;
  Vector2D last_drawn_direction;
  double last_drawn_length;
  double last_drawn_thickness;
};


// ---------------------  Circle : Shape ---------------------

class Circle : public Shape 
{
public:
  Circle();
  Circle(const Vector2D& c, const double r); 
  Circle(const Vector2D& c, const double r, const long int col); 
  Circle(const Vector2D& c, const double r, const double b_c, const double hardn);
  virtual ~Circle() {}

  double get_distance(const Vector2D& pos, const Vector2D& dir, const double size);
  bool within_distance(const Vector2D& pos, const double size);
  Vector2D get_normal(const Vector2D& pos);

#ifndef NO_GRAPHICS
  void draw_shape(bool erase);
#endif
  
  double get_radius() { return radius; }
  Vector2D get_center() { return center; }
  
protected:
  Vector2D center;
  double radius;

  Vector2D last_drawn_center;
  double last_drawn_radius;
};

// ---------------------  InnerCircle : Shape ---------------------

class InnerCircle : public Shape 
{
public:
  InnerCircle();
  InnerCircle(const Vector2D& c, const double r); 
  //  InnerCircle(const Vector2D& c, const double r, const double b_c, const double hardn);
  ~InnerCircle() {}

  double get_distance(const Vector2D& pos, const Vector2D& dir, const double size);
  bool within_distance(const Vector2D& pos, const double size);
  Vector2D get_normal(const Vector2D& pos);

#ifndef NO_GRAPHICS
  void draw_shape(bool erase);
#endif

  double get_radius() { return radius; }
  Vector2D get_center() { return center; }
   
protected:
  Vector2D center;
  double radius;

  Vector2D last_drawn_center;
  double last_drawn_radius;
};




// ---------------------  Arc : Shape ---------------------

class Arc : public Shape 
{
public:
  Arc();
  Arc(const Vector2D& c, const double r1, const double r2,
      const double a1, const double a2 ); 
  //  Arc(const Vector2D& c, const double r1, const double r2,
  //        const double a1, const double a2,
  //        const double b_c, const double hardn);

  ~Arc() {}

  double get_distance(const Vector2D& pos, const Vector2D& dir, const double size);
  bool within_distance(const Vector2D& pos, const double size);
  Vector2D get_normal(const Vector2D& pos);

#ifndef NO_GRAPHICS
  void draw_shape(bool erase);
#endif

  double get_inner_radius() { return inner_radius; }
  double get_outer_radius() { return outer_radius; }
  Vector2D get_center() { return center; }

  double get_start_angle() { return start_angle; }
  double get_end_angle() { return end_angle; }

   
protected:
  Vector2D center;
  double inner_radius;
  double outer_radius;

  // angles should be between -pi and pi
  double start_angle;
  double end_angle;

  Vector2D last_drawn_center;
  //  double last_drawn_radius;

private:
  bool within_angle( const double a);

  double mid_radiussqr;
};





#endif 
