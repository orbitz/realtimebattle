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

#ifndef __EXTRAS__
#define __EXTRAS__

#include "GeometricalObjects.h"

// ---------------------  Extras : Circle  ---------------------

class Extras : public Circle
{
public:
  Extras(const Vector2D& c, const double r, const double e, const long int col);
  ~Extras() {}

  bool is_alive() { return alive; }
  virtual void die();
  double get_energy() { return energy; }

protected:
  double energy;
  bool alive;
  char log_file_char;
};

// ---------------------  Cookie : Extras  ---------------------

class Cookie : public Extras
{
public:
  Cookie(const Vector2D& c, const double e, const int c_id = -1);
  ~Cookie() {}
  
  //  arenaobject_t get_arenaobject_t() { return COOKIE; }
};

// ---------------------  Mine : Extras  ---------------------

class Mine : public Extras
{
public:
  Mine(const Vector2D& c, const double e, const int m_id = -1);
  ~Mine() {}

  //  arenaobject_t get_arenaobject_t() { return MINE; }
};

#endif 

