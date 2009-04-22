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

#include <iostream>

using namespace std;

#include "Extras.h"
#include "ArenaController.h"
#include "ArenaRealTime.h"
#include "ArenaWindow.h"
#include "Options.h"

Extras::Extras(const Vector2D& c, const double r, const double e, const long int col)
  : Circle(c, r, col), energy(e)
{
  alive = true;
#ifndef NO_GRAPHICS
  if( !no_graphics )
    draw_shape( false );
#endif
}

Cookie::Cookie(const Vector2D& c, const double e, const int c_id) 
  : Extras(c, the_opts.get_d(OPTION_COOKIE_RADIUS), e, 
           the_opts.get_l(OPTION_COOKIE_COLOUR))
{ 
  if( c_id == -1 )
    id = the_arena.increase_cookie_count();
  else
    id = c_id;

   log_file_char = 'C';
}

Mine::Mine(const Vector2D& c, const double e, const int m_id ) 
  : Extras(c, the_opts.get_d(OPTION_MINE_RADIUS), e, 
           the_opts.get_l(OPTION_MINE_COLOUR))
{ 
  if( m_id == -1 )
    id = the_arena.increase_mine_count();
  else
    id = m_id;

   log_file_char = 'M';
}

void
Extras::die()
{
   alive = false;
#ifndef NO_GRAPHICS
   if (!no_graphics )
     the_gui.get_arenawindow_p()->
       draw_circle( last_drawn_center, last_drawn_radius,
                    *(the_gui.get_bg_gdk_colour_p()), true );
#endif

   if( the_arena_controller.is_realtime() )
     realtime_arena.print_to_logfile( 'D', (int)log_file_char, id );
}
