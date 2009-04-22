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

#ifndef __ARENA_CONTROLLER__
#define __ARENA_CONTROLLER__

#include "ArenaBase.h"

class ArenaController
{
public:
  ArenaController();
  ~ArenaController();
  
  void start_realtime_arena();
  void start_replay_arena();

  void close_arena();

  ArenaBase* get_the_arena() { return my_arena; }
  bool is_started() { return started; }
  bool is_realtime() { return realtime; }

private:
  
  bool started;
  bool realtime;

  ArenaBase* my_arena;

public: // global names etc.
  string option_filename;
  string statistics_filename;
  string log_filename;
  string tournament_filename;
  string message_filename;
  string replay_filename;

  int game_mode;
  int debug_level;
  bool auto_start_and_end;
};

#endif 
