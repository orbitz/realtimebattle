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


#include "ArenaController.h"
#include "ArenaRealTime.h"
#include "ArenaReplay.h"

ArenaController::ArenaController()
{
  started = false;

  option_filename = "";
  statistics_filename = "";
  log_filename = "";
  tournament_filename = "";
  message_filename = "";
  replay_filename = "";

  game_mode = ArenaBase::NORMAL_MODE;
  debug_level = 1;
  auto_start_and_end = false;
}

ArenaController::~ArenaController()
{
  if( started ) close_arena();
}

void
ArenaController::start_realtime_arena()
{
  if( started ) close_arena();

  my_arena = new ArenaRealTime();
  
  started = true;
  realtime = true;
}

void
ArenaController::start_replay_arena()
{
  if( started ) close_arena();

  my_arena = new ArenaReplay();
  
  started = true;
  realtime = false;
}

void
ArenaController::close_arena()
{
  if( started )
    {
      delete my_arena;
    }

  started = false;
}
