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

#ifndef __SHOT__
#define __SHOT__

#include "MovingObject.h"


class Shot : public MovingObject
{
public:
  Shot(const Vector2D& c, const Vector2D& velocity, 
       const double energy, const int shot_id = -1); 
  ~Shot() {}

  void move(const double timestep);
  void move_no_check(const double timestep);
  void die();
  bool is_alive() { return alive; }
  double get_energy() { return energy; }

  friend void shot_collision(Shot* shot1p, Shot* shot2p);
  friend void shot_collision(Shot* shot1p, const Vector2D& shot2_vel, const double shot2_en);

  //  arenaobject_t get_arenaobject_t() { return SHOT; }

private:
  bool alive;
  double energy;
};

#endif 
