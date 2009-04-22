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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "Robot.h"
#include "Shot.h"
#include "ArenaController.h"
#include "ArenaRealTime.h"
#include "ArenaWindow.h"
#include "Extras.h"
#include "Various.h"
#include "Options.h"

Shot::Shot(const Vector2D& c, //const double r, 
           const Vector2D& vel, const double en, const int shot_id) 
  : MovingObject(c, the_opts.get_d(OPTION_SHOT_RADIUS), vel)
{
  alive = true;
  energy = en;

  if( shot_id == -1 )
    id = the_arena.increase_shot_count();
  else
    id = shot_id;
}

void
Shot::move(const double timestep)
{
  object_type closest_shape;
  Shape* colliding_object;
  double time_to_collision = the_arena.
    get_shortest_distance(center, velocity, radius, closest_shape, colliding_object);

  if( time_to_collision > timestep )
    {
      center += timestep*velocity;
    }
  else
    {
      switch( closest_shape )
        {
        case WALL:
          die();
          break;
        case ROBOT:
          {
            Robot* robotp = (Robot*)colliding_object;
            robotp->change_energy(-energy);
            robotp->send_message(COLLISION, SHOT,
                                 vec2angle(center-robotp->get_center()) - robotp->get_robot_angle().pos);
            die();
          }
          break;
        case SHOT:
          {
            Shot* shotp = (Shot*)colliding_object;
            shot_collision(this, shotp); 
          }
          break;
        case COOKIE:
          {
            Cookie* cookiep =(Cookie*)colliding_object;
            cookiep->die();
            the_arena.get_object_lists()[COOKIE].remove( cookiep );
            die();
          }
          break;
        case MINE:
          {
            Mine* minep =(Mine*)colliding_object;
            minep->die();
            the_arena.get_object_lists()[MINE].remove( minep );
            die();
          }
          break;
        default:
          Error(true, "Collided with unknown object", "Robot::move");
          break;
        }
    }
}

void
Shot::move_no_check(const double timestep)
{
  center += timestep*velocity;
}

void
Shot::die()
{
  if( alive )
    {
      alive = false;
#ifndef NO_GRAPHICS
      if (!no_graphics )
        the_gui.get_arenawindow_p()->
          draw_circle( last_drawn_center, last_drawn_radius,
                       *(the_gui.get_bg_gdk_colour_p()), true );
#endif
      
      if( the_arena_controller.is_realtime() )
        realtime_arena.print_to_logfile('D', (int)'S', id);
    }
}

void
shot_collision(Shot* shot1p, Shot* shot2p)
{
  shot_collision(shot1p, shot2p->velocity, shot2p->energy);
  shot2p->die();
  the_arena.get_object_lists()[SHOT].remove( shot2p );
}

void
shot_collision(Shot* shot1p, const Vector2D& shot2_vel, const double shot2_en)
{
  Vector2D vel = ( shot1p->energy * shot1p->velocity + 
                   shot2_en * shot2_vel ) / ( shot1p->energy + shot2_en );

  double en = dot( shot1p->energy * unit(shot1p->velocity) + shot2_en * unit(shot2_vel),  
                   unit(vel));

  if( en < the_opts.get_d(OPTION_SHOT_MIN_ENERGY) || 
      length(vel) < the_opts.get_d(OPTION_SHOT_SPEED) * 0.5 )
    {
      shot1p->die();
    }
  else
    {
      shot1p->velocity = vel;
      shot1p->energy = en;
      realtime_arena.print_to_logfile('D', (int)'S', shot1p->id);
      shot1p->id = the_arena.increase_shot_count();
      realtime_arena.print_to_logfile('S', shot1p->id, shot1p->center[0], shot1p->center[1], 
                                      shot1p->velocity[0], shot1p->velocity[1]);
      

    }
}
