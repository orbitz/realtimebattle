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

#ifndef __ARENA_REALTIME__
#define __ARENA_REALTIME__

#include "ArenaBase.h"

class ArenaRealTime : public ArenaBase
{
public:
  ArenaRealTime();
  virtual ~ArenaRealTime();

  void clear();

  bool timeout_function();
  static void start_tournament_from_tournament_file
  ( const list<start_tournament_info_t*>& robotfilename_list, 
    const list<start_tournament_info_t*>& arenafilename_list, 
    const int robots_p_game, const int games_p_sequence, 
    const int n_o_sequences, ArenaRealTime* ar_p );
  void start_tournament(const list<start_tournament_info_t*>& robotfilename_list, 
                        const list<start_tournament_info_t*>& arenafilename_list, 
                        const int robots_p_game, 
                        const int games_p_sequence, 
                        const int n_o_sequences);

  bool end_game();

  void broadcast(enum message_to_robot_type ...);
  
  bool parse_arena_file(string& filename);
  
  void set_filenames( string& log_fname, const string& statistics_fname, 
                      const string& tournament_fname,
                      string& message_fname, const string& option_fname );

  void print_to_logfile(  char first_letter ... );
  
  bool get_use_message_file() { return use_message_file; }
  Vector2D get_random_position();
  vector<string> get_arena_filenames() { return arena_filenames; }
  int get_current_arena_nr() { return current_arena_nr; }

  int set_debug_level( const int new_level);

  int get_max_debug_level() { return max_debug_level; }

  void quit_ordered();

  long int find_free_colour(const long int home_colour, const long int away_colour, 
                            const class Robot*, const bool random_colours = false);


private:

  void check_initialization();

  void update();  

  void update_robots();
  void read_robot_messages();

  void check_robots();
  void add_cookie();
  void add_mine();

  bool is_colour_allowed(const long int col, const double min_dist, const class Robot*);
  double colour_dist(const long int col1, const long int col2);

  bool start_game();
  void start_sequence();
  void end_sequence();
  bool start_sequence_follow_up();
  void end_sequence_follow_up();
  void end_tournament();


  string tournament_file_name;
  
  ofstream LOG_FILE;
  bool use_log_file;
  int update_count_for_logging;

  int** robots_in_sequence;

  double next_check_time;

  int number_of_arenas;
  int current_arena_nr;
};


#endif 

