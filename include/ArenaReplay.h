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

#ifndef __ARENA_REPLAY__
#define __ARENA_REPLAY__

#include "ArenaBase.h"
#include "fstream"

using namespace std;

class ArenaReplay : public ArenaBase
{
public:
  struct object_pos_info_t
  {
    object_pos_info_t( object_type o, int i, Vector2D p, float s,
                       float e, Vector2D v ) :
      obj(o), id(i), pos(p), start_time(s), end_time(e), vel(v) {}
    object_pos_info_t( object_type o, int i, Vector2D p, float s, float e ) :
      obj(o), id(i), pos(p), start_time(s), end_time(e) {}
    object_type obj;
    int id;
    Vector2D pos;
    float start_time; 
    float end_time;   
    Vector2D vel; // For shots
  };
  //  enum speed_t { REWIND, PLAY, FAST_FORWARD };

  ArenaReplay                   ();
  ~ArenaReplay                  ();

  bool timeout_function         ();
  void start_tournament         ();
  bool end_game                 ();
  void set_filenames            ( string& replay_fname, string& message_fname,
                                  const string& statistics_fname,
                                  const string& option_fname );
  void   change_speed           ( const bool forward, const bool fast );
  void   change_game            ( const int inc_game, const int inc_seq );
  bool   step_forward           ( const int n_o_steps, const bool clear_time = true );
  void   step_forward_or_backward(const bool forward );
  void   change_replay_time     ( const double time );

  char   search_forward         ( const string& search_letters );
  string search_forward         ( const list<string>& search_strings );
  string search_backwards       ( const string& search_letters );

  double get_current_replay_time() { return current_replay_time; }
  double get_length_of_current_game();
  bool is_log_from_stdin        () { return log_from_stdin; }

private: 
  void update                   ();
  bool start_game               ();
  void start_sequence           ();
  void end_sequence             ();
  void end_tournament           ();

  char parse_log_line           ();
  void parse_log_line_forward   ( const char first_letter );
  void parse_log_line_rewind    ( const char first_letter );
  void parse_this_interval      ();
  void parse_this_time_index    ();
  void beginning_of_prev_line   ();
  void beginning_of_current_line();
  void recreate_lists           ();

  void make_statistics_from_file();
  void get_time_positions_in_game();
  int  find_streampos_for_time  ( const float cur_time );
  object_pos_info_t* find_object_in_log( object_type obj, int id );

  ifstream log_file;

  //  double last_replay_time;
  double current_replay_time;

  bool log_from_stdin;

  //  speed_t speed;
  double fast_forward_factor;

  streampos** game_position_in_log;

  struct time_pos_info_t
  {
    streampos pos;
    float time;
  };

  list<object_pos_info_t*> object_positions_in_log;
  time_pos_info_t* time_position_in_log;
  int last_time_info;

  // These three vars are used when creating the arena
  double arena_scale;
  double arena_angle_factor;
  int arena_succession;
};


#endif
