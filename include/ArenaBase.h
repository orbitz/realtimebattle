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

#ifndef __ARENA_BASE__
#define __ARENA_BASE__

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef NO_GRAPHICS
#include <glib.h>
#include <gdk/gdk.h>
#endif

#include <iostream>
#include <fstream>
#include <list>
#include <vector>
using namespace std;

#ifndef NO_GRAPHICS
#include "Gui.h"
#endif

#include "Vector2D.h"
#include "Messagetypes.h"
#include "Structs.h"
#include "Timer.h"
#include "Shape.h"

class ArenaController;
class ArenaRealTime;
class ArenaReplay;
extern ArenaController the_arena_controller;
#define the_arena (*( the_arena_controller.get_the_arena()) )
#define realtime_arena (*( (ArenaRealTime*) (the_arena_controller.get_the_arena()) ))
#define replay_arena   (*( (ArenaReplay*) (the_arena_controller.get_the_arena()) ))

class Robot;
class Options;
class WallLine;
class WallCircle;
class WallInnerCircle;

static const double infinity_rtb = 1.0e10;  //approximately ;-)
static const int max_debug_level = 5;

extern class Options the_opts;
//extern class ArenaRealTime the_arena;
#ifndef NO_GRAPHICS
extern class Gui the_gui;
#endif

extern bool no_graphics;



class ArenaBase
{
public:

  ArenaBase();
  virtual ~ArenaBase();

  enum game_mode_t { DEBUG_MODE, NORMAL_MODE, COMPETITION_MODE };


  double get_shortest_distance(const Vector2D& pos,
                               const Vector2D& vel,
                               const double size,
                               enum object_type& closest_shape,
                               Shape*& colliding_object,
                               const class Robot* the_robot = NULL );

  bool space_available(const Vector2D& pos, const double margin);

  virtual void interrupt_tournament();

  virtual bool timeout_function() = 0;

  virtual bool end_game() = 0;

protected:
  virtual void update() = 0;
  virtual bool start_game() = 0;
  virtual void start_sequence() = 0;
  virtual void end_sequence() = 0;
  virtual void end_tournament() = 0;

public:

  void delete_lists(const bool kill_robots, const bool del_seq_list,
                    const bool del_tourn_list, const bool del_arena_filename_list,
                    const bool del_robot_obj_list=true);
  void save_statistics_to_file(string filename);


  list<Shape*>* get_object_lists() { return object_lists; }
  list<Robot*>* get_all_robots_in_sequence() { return &all_robots_in_sequence; }
  list<Robot*>* get_all_robots_in_tournament() { return &all_robots_in_tournament; }
  string get_current_arena_filename() { return current_arena_filename; }
  int get_game_nr() { return game_nr; }
  int get_games_per_sequence() { return games_per_sequence; }
  int get_sequence_nr() { return sequence_nr; }
  int get_sequences_in_tournament() { return sequences_in_tournament; }
  int get_robots_per_game() { return robots_per_game; }
  int get_number_of_robots() { return number_of_robots; }

  void print_message( const string& messager, const string& text );

  int get_robots_left() { return robots_left; }
  double get_total_time() { return (double)total_time; }
  double get_shooting_penalty();

  int increase_robot_count()  { return robot_count++; }
  int increase_shot_count()   { return shot_count++; }
  int increase_cookie_count() { return cookie_count++; }
  int increase_mine_count()   { return mine_count++; }

  state_t get_state() { return state; }
  Vector2D * get_boundary() { return boundary; }
  enum game_mode_t get_game_mode() { return game_mode; }
  void set_game_mode( const enum game_mode_t gm );

  int get_debug_level() { return debug_level; }
  bool is_max_debug_level() { return debug_level == max_debug_level; }
  virtual int set_debug_level( const int new_level);

  void pause_game_toggle();
  void step_paused_game();

protected:

  void update_timer( const double factor = 1.0 );
  void reset_timer();

  void move_shots(const double time_period);
  void move_shots_no_check(const double time_period);

  bool parse_arena_line(ifstream& file, double& scale,
                        int& succession, double& angle_factor);

  void set_state( const state_t );

  bool find_object_by_id(list<Shape*>& obj_list,
                         list<Shape*>::iterator & li,
                         const int obj_id);
  bool find_object_by_id(list<Robot*>& obj_list,
                         list<Robot*>::iterator & li,
                         const int obj_id);

  list<Shape*> object_lists[LAST_OBJECT_TYPE];

  list<Robot*> all_robots_in_tournament;
  list<Robot*> all_robots_in_sequence;
  list<Vector2D> exclusion_points;

  string current_arena_filename;
  vector<string> arena_filenames;               // list of Strings

  string statistics_file_name;
  string option_file_name;
  ofstream message_file;
  bool use_message_file;

  int robot_count;
  int shot_count;
  int cookie_count;
  int mine_count;

  double timestep;
  double total_time;
  double current_timer;

  Timer timer;

  int game_nr;
  int games_per_sequence;
  int sequence_nr;
  int sequences_in_tournament;

  int number_of_robots;
  int robots_per_game;
  int robots_left;
  int robots_killed_this_round;

  Vector2D boundary[2];   // {top-left, bottom-right}

  state_t state;
  game_mode_t game_mode;

  int debug_level;

  bool halt_next;
  bool pause_after_next_game;
};


#endif
