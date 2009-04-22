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

#include <math.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <stdarg.h>
#include <sys/stat.h>
#include <stdio.h>

using namespace std;

//#include "Gui.h"
#include "ArenaRealTime.h"
#include "ArenaController.h"
#include "ArenaWindow.h"
//#include "MovingObject.h"
//#include "Shape.h"
#include "IntlDefs.h"
#include "Extras.h"
#include "Various.h"
#include "Shot.h"
#include "Options.h"
#include "Wall.h"
#include "Robot.h"
#include "Broadcast.h"

#ifndef NO_GRAPHICS
# include "ScoreWindow.h"
# include "ControlWindow.h"
# include "MessageWindow.h"
#endif

#ifndef NO_GRAPHICS
extern class ControlWindow* controlwindow_p;
#endif

#ifndef NO_NETWORK
    #include "ClientInterface.h"
#endif

ArenaRealTime::ArenaRealTime()
{
  set_game_mode( (ArenaBase::game_mode_t)the_arena_controller.game_mode );
  set_filenames( the_arena_controller.log_filename,
                 the_arena_controller.statistics_filename,
                 the_arena_controller.tournament_filename,
                 the_arena_controller.message_filename,
                 the_arena_controller.option_filename );
  robots_in_sequence = NULL;
}

ArenaRealTime::~ArenaRealTime()
{
  for(int i=0; i < sequences_in_tournament; i++)
    delete [] robots_in_sequence[i];

  if( robots_in_sequence != NULL )
    delete robots_in_sequence;

  try{
    Broadcast::Instance()->finalize();
  }catch(const bad_exception &e){
    cerr<< e.what() << endl;
  }

  if( use_message_file && message_file ) message_file.close();
}


void
ArenaRealTime::clear()
{
  delete_lists(true, true, true, true);

  for(int i=0; i < sequences_in_tournament; i++)
    delete [] robots_in_sequence[i];

  sequence_nr = 0;
  sequences_in_tournament = 0;
  
  set_state( NOT_STARTED );
}

void
ArenaRealTime::set_filenames( string& log_fname,
                              const string& statistics_fname, 
                              const string& tournament_fname,
                              string& message_fname,
                              const string& option_fname )
{
  bool log_stdout = false;

  if( !log_fname.empty() ){
	if( log_fname=="-" || log_fname=="STDOUT" ){
		log_stdout = true;
	}

    try{
      Broadcast::Instance()->set_logfile(log_fname);
    }catch(const bad_exception &e){
      cerr<< e.what() << endl;
    }
  }
	
  if( message_fname.empty() )
    {
      use_message_file = false;
    }
  else if( message_fname == "-" || message_fname == "STDOUT" )
    {
      if( !log_stdout )
        {
          use_message_file = true;
          //message_file.attach( STDOUT_FILENO );
	  message_file.ios::rdbuf(cout.rdbuf());
        }
      else
        {
          use_message_file = false;
        }
    }
  else
    {
      use_message_file = true;
      message_file.open( message_fname.c_str(), ios::out);
      if( !message_file )
        {
          Error( false, "Couldn't open message file. Message file disabled",
                 "ArenaRealTime::set_filenames" );
          use_message_file = false;
        }
      
    }

  statistics_file_name = statistics_fname;

  tournament_file_name = tournament_fname;

  option_file_name = option_fname;
}

bool
ArenaRealTime::parse_arena_file(string& filename)
{
  Vector2D vec1, vec2, vec0;

  ifstream file(filename.c_str());
  if( !file )
  {
    Error(false, "Couldn't open arena file" + filename, "ArenaRealtime::parse_arena_file");
    return false;
  }

  int succession = 1;
  double scale = the_opts.get_d(OPTION_ARENA_SCALE);
  double angle_factor = 1.0;
  do
    {
      if ( !file.good() || !parse_arena_line(file, scale, succession, angle_factor) )
      {
          Error(false, "Error parsing arena file " + filename,
                "ArenaRealtime::parse_arena_file");
          file.close();
          return false;
      }

    } while( !file.eof() );

  file.close();


    try{
      if(Broadcast::Instance()->is_active()){
      char buffer[500];
      
      ifstream file(filename.c_str());
      if( !file )
      {
        Error(false, "Couldn't open arena file for log file " + filename,
              "ArenaRealtime::parse_arena_file");
        return false;
      }
  
      do
        {
          file >> ws;
          file.get(buffer, 500, '\n');
          if( buffer[0] != '\0' ) print_to_logfile('A', buffer);
        } 
      while( buffer[0] != '\0' );
      
      }

    }catch(const bad_exception &e){
      cerr<< e.what() << endl;
    }

  return true;
}

void
ArenaRealTime::print_to_logfile(char first_letter ... )
{
  try{
  
  if(Broadcast::Instance()->is_active()==false) return; // broadcast is inactive

  ostream& bcast = *Broadcast::Instance();
    
  va_list args;
  va_start(args, first_letter);

  // log 'R' only each OPTION_LOG_EVERY_NTH_UPDATE_INTERVAL
  if( update_count_for_logging != 0 && first_letter == 'R' )
    {
      va_end(args);
      return;
    }

  bcast << first_letter;

  int prec = 2;
  bcast << setiosflags(ios::fixed) << setprecision(prec);
  ostringstream hex2string;

  switch(first_letter)
    {
    case 'R': // Robot position info
      bcast << va_arg(args, int   ) << " ";     // robot id;
      bcast << va_arg(args, double) << " ";  // x
      bcast << va_arg(args, double) << " ";  // y
      bcast << va_arg(args, double) << " ";  // robot angle
      bcast << va_arg(args, double) << " ";  // cannon angle
      bcast << va_arg(args, double) << " ";  // radar angle
      bcast << va_arg(args, double);         // energy
      break;

    case 'T': // Time
      bcast << setprecision(prec+1) << va_arg(args, double);  // time elapsed
      break;

    case 'P': // Print a robot message
      bcast << va_arg(args, int   ) << " ";  // robot id
      bcast << va_arg(args, char* );         // message to print
      break;

    case 'C': // Cookie
      bcast << va_arg(args, int   ) << " ";  // cookie id
      bcast << va_arg(args, double) << " ";  // x
      bcast << va_arg(args, double);         // y
      break;

    case 'M': // Mine
      bcast << va_arg(args, int   ) << " ";  // mine id
      bcast << va_arg(args, double) << " ";  // x
      bcast << va_arg(args, double);         // y
      break;

    case 'S': // Shot
      bcast << va_arg(args, int   ) << " ";  // shot id
      bcast << va_arg(args, double) << " ";  // x
      bcast << va_arg(args, double) << " ";  // y
      bcast << setprecision(prec+1) << va_arg(args, double) << " ";  // dx/dt
      bcast << setprecision(prec+1) << va_arg(args, double);         // dy/dt
      break;
      
    case 'D': // Die
      {
        int obj_type = va_arg(args, int  );
        bcast << (char)obj_type    << " ";  // object type to kill
        bcast << va_arg(args, int) << " ";  // object id
        if( obj_type == 'R' )
          {
            bcast << setprecision(1) 
                     <<  va_arg(args, double) << " "; // robot points received
            bcast << va_arg(args, int);            // position this game
          }
      }
      break;

    case 'G': // Game begins
      bcast << va_arg(args, int  ) << " ";  // sequence number
      bcast << va_arg(args, int  ) << " ";  // game number
      //      LOG_FILE << va_arg(args, char*);         // arena filename
      break;

    case 'H': // Header
      bcast << va_arg(args, int  ) << " ";  // number of games per sequence
      bcast << va_arg(args, int  ) << " ";  // number of robots per sequence
      bcast << va_arg(args, int  ) << " ";  // number of sequences
      bcast << va_arg(args, int  ) << " ";  // number of robots
      //      LOG_FILE << va_arg(args, char*);         // name of optionfile
      break;

    case 'L': // List of robot properties
      {
        bcast << va_arg(args, int  ) << " ";  // robot id
        hex2string << std::hex << va_arg(args, long );
        bcast << hex2string.str() << " ";
       
        string name = va_arg(args, char*);   // robot name
        if( name == "" ) name = "Anonymous";
        bcast << name;
      }
      break;

    case 'A': // Arena file line
      bcast << va_arg(args, char*);  // line of arena file
      break;

    case 'O':
      {
        char option_type = (char)va_arg(args, int);
        bcast << va_arg(args, char*);                        // Option label
        switch( option_type )
          {
          case 'D':
            bcast << va_arg(args, double) ; // Option value
            break;
          case 'L':
            bcast << va_arg(args, long);   // Option value
            break;
          case 'H':
            {
              hex2string << std::hex << va_arg(args, long);
              bcast << hex2string.str();   // Option value
            }
            break;
          case 'S':
            bcast << va_arg(args, char*);   // Option value
            break;
          }
      }
      break;

    default:
      Error(true, "Unrecognized first letter in logfile", "ArenaRealTime::print_to_logfile");
      break;
    }

  bcast << endl;
  va_end(args);
    
  }catch(const bad_exception &e){
    cerr<< e.what() << endl;
  }   
}

Vector2D
ArenaRealTime::get_random_position()
{
  return Vector2D( boundary[0][0] + (boundary[1][0] - boundary[0][0])*
                   (double)rand()/(double)RAND_MAX, 
                   boundary[0][1] + (boundary[1][1] - boundary[0][1])*
                   (double)rand()/(double)RAND_MAX );
}

void
ArenaRealTime::broadcast(const message_to_robot_type msg_type ...)
{
  va_list args;
  va_start(args, msg_type);
  string str = string(message_to_robot[msg_type].msg) + " ";
  for(int i=0; i<message_to_robot[msg_type].number_of_args; i++)
    {
      ostringstream number2string;
      switch(message_to_robot[msg_type].arg_type[i])
        {
        case NONE: 
          Error(true, "Couldn't send message, no arg_type", "ArenaRealTime::broadcast");
          break;
        case INT:
          {
            number2string << va_arg(args, int);
            str += number2string.str() + " ";
          }
          break;
        case DOUBLE:
          {
            number2string << std::hex << va_arg(args, double);
            str += number2string.str() + " ";
          }
          break;
        case STRING:
          str += string(va_arg(args, char*)) + " ";
          break;   
        case HEX:
          {
            number2string << std::hex << va_arg(args, int);
            str += number2string.str() + " "; 
          }
          break;
        default:
          Error(true, "Couldn't send message, unknown arg_type", "ArenaRealTime::broadcast");
        }
    }
  str += '\n';

 for( list<Shape*>::const_iterator li(object_lists[ROBOT].begin()); li!=object_lists[ROBOT].end(); ++li)
   *(((Robot*)(*li))->get_outstreamp()) << str;
}

void
ArenaRealTime::quit_ordered()
{
  set_state( EXITING );
}

bool
ArenaRealTime::timeout_function()
{
#ifndef NO_GRAPHICS
      int old_total = (int)total_time;
#endif 

  if( state != PAUSED )
    {
      update_timer ();
    }

  switch(state)
    {
    case NOT_STARTED:
      if( the_arena_controller.auto_start_and_end )
        parse_tournament_file( tournament_file_name,
                               (StartTournamentFunction)
                               ArenaRealTime::start_tournament_from_tournament_file,
                               this );
      break;

    case FINISHED:
      if( the_arena_controller.auto_start_and_end )
        {
          if( statistics_file_name != "" )
            save_statistics_to_file( statistics_file_name );

          Quit();
        }
      break;
      
    case STARTING_ROBOTS:
      {
        read_robot_messages();

        if( total_time > next_check_time )
          if( !start_sequence_follow_up() )
          {
            Error(false, "Error starting game sequence, ending tournament!", "ArenaRealTime::timeout_function");
            // Finish tournament
            game_nr = games_per_sequence;
            sequence_nr = sequences_in_tournament;
            end_game();
          }
      }
      break;
      
    case SHUTTING_DOWN_ROBOTS:
      {     
        list<Robot*>::const_iterator li;
        for( li = all_robots_in_sequence.begin(); li != all_robots_in_sequence.end(); ++li )
          (*li)->get_messages();
        
        if( total_time > next_check_time ) end_sequence_follow_up();
      }
      break;
      
    case GAME_IN_PROGRESS:
      {
        update();
#ifndef NO_GRAPHICS
        if((int)total_time > old_total && !no_graphics)
          the_gui.get_scorewindow_p()->set_window_title();
#endif
        if( robots_left <= 1 || total_time > the_opts.get_d(OPTION_TIMEOUT) ) 
          {
            end_game();
          }
        else
          {
            if( game_mode == COMPETITION_MODE && total_time > next_check_time ) check_robots();
            
            // Place mines and cookies
            if( ((double)rand()) / (double)RAND_MAX <= timestep*the_opts.get_d(OPTION_COOKIE_FREQUENCY) )
              add_cookie();
            
            if( ((double)rand()) / (double)RAND_MAX <= timestep*the_opts.get_d(OPTION_MINE_FREQUENCY) )
              add_mine();

              if( halt_next )
                {
                  set_state( PAUSED );
                  halt_next = false;
                }
          }
      }
      break;

    case PAUSED:
      break;

    case PAUSING_BETWEEN_GAMES:
      if( !pause_after_next_game ) start_game();
      break;

    case EXITING:
      return false;
      
    case BEFORE_GAME_START:
    case NO_STATE:
      Error(true, "Arena shouldn't be in this state here!!", "ArenaRealTime::timeout_function");
    }

  return true;
}

void
ArenaRealTime::update()
{
  print_to_logfile('T', total_time);

#ifndef NO_GRAPHICS
  if( state == GAME_IN_PROGRESS && !no_graphics)
    the_gui.get_messagewindow_p()->freeze_clist();
#endif

  read_robot_messages();
  move_shots(timestep);
  update_robots();

#ifndef NO_GRAPHICS
  if( state == GAME_IN_PROGRESS && !no_graphics)
    {
      the_gui.get_arenawindow_p()->draw_moving_objects( true );
      the_gui.get_messagewindow_p()->thaw_clist();
    }
#endif

  update_count_for_logging++;
  if( update_count_for_logging == the_opts.get_l(OPTION_LOG_EVERY_NTH_UPDATE_INTERVAL) )
    update_count_for_logging = 0;
}

void
ArenaRealTime::add_cookie()
{
  double en = the_opts.get_d(OPTION_COOKIE_MIN_ENERGY) + 
    (the_opts.get_d(OPTION_COOKIE_MAX_ENERGY) - the_opts.get_d(OPTION_COOKIE_MIN_ENERGY)) * 
    (double)rand() / (double)RAND_MAX;
  bool found_space = false;
  double r = the_opts.get_d(OPTION_COOKIE_RADIUS);
  Vector2D pos;

  for( int i=0; i<100 && !found_space; i++ )
    {
      pos = get_random_position();
      found_space = space_available(pos, r*2.0);
    }
  
  if( !found_space ) return;

  Cookie* cookiep = new Cookie(pos, en);
  object_lists[COOKIE].push_back( cookiep );

  print_to_logfile('C', cookiep->get_id(), pos[0], pos[1]);
}

void
ArenaRealTime::add_mine()
{
  double en = the_opts.get_d(OPTION_MINE_MIN_ENERGY) + 
    (the_opts.get_d(OPTION_MINE_MAX_ENERGY) - the_opts.get_d(OPTION_MINE_MIN_ENERGY)) * 
    (double)rand() / (double)RAND_MAX;
  bool found_space = false;
  double r = the_opts.get_d(OPTION_MINE_RADIUS);
  Vector2D pos;

  for( int i=0; i<100 && !found_space; i++)
    {
      pos = get_random_position();
      found_space = space_available(pos, r*2.0);
    }
  
  if( !found_space )  return;

  Mine* minep = new Mine(pos, en);
  object_lists[MINE].push_back( minep );

  print_to_logfile('M', minep->get_id(), pos[0], pos[1]);
}

void
ArenaRealTime::check_robots()
{
  list<Robot*>::const_iterator li;
  for( li = all_robots_in_sequence.begin(); li != all_robots_in_sequence.end(); ++li )
    {
      if( (*li)->is_process_running() )
        {
          (*li)->check_process();
        }      
    } 

  next_check_time = total_time + the_opts.get_d(OPTION_CHECK_INTERVAL);
}

void
ArenaRealTime::read_robot_messages()
{
  list<Robot*>::const_iterator li;
  for( li = all_robots_in_sequence.begin(); li != all_robots_in_sequence.end(); ++li )
    {
      if( (*li)->is_alive() || state != GAME_IN_PROGRESS )  
        (*li)->get_messages();
    }
}

void
ArenaRealTime::update_robots()
{
  Robot* robotp;

  robots_killed_this_round = 0;

  for( list<Shape*>::const_iterator li(object_lists[ROBOT].begin()); li!=object_lists[ROBOT].end(); ++li)
    {
      robotp = (Robot*)(*li);
      if( robotp->is_alive() )
        {
          robotp->update_radar_and_cannon(timestep);  
          robotp->change_velocity(timestep);
          robotp->move(timestep);        
        }
      if( robotp->is_alive() ) 
        robotp->get_messages();
    }

  // Check if robots have died and send energy level
  for( list<Shape*>::iterator li(object_lists[ROBOT].begin()); li!=object_lists[ROBOT].end();)
    {
      robotp = (Robot*)(*li);
      if( !robotp->is_alive() ) 
        {
          li=object_lists[ROBOT].erase(li);
          robots_killed_this_round++;
        }
      else
        {
          double lvls = (double)the_opts.get_l(OPTION_ROBOT_ENERGY_LEVELS);
          robotp->send_message( ENERGY, rint( robotp->get_energy() / lvls ) * lvls );
	  ++li;
        }
    }

  robots_left -= robots_killed_this_round;

  if( robots_killed_this_round > 0 )
    {
      for( list<Shape*>::const_iterator li(object_lists[ROBOT].begin()); li!=object_lists[ROBOT].end(); ++li)
        {
          robotp = (Robot*)(*li);
          //          robotp->add_points(robots_killed_this_round);
#ifndef NO_GRAPHICS
          if( robots_left < 15 && !no_graphics ) 
            robotp->display_score();
#endif
        }

      list<Robot*>::const_iterator li2;
      for( li2 = all_robots_in_sequence.begin(); li2 != all_robots_in_sequence.end(); ++li2 )
        if( (*li2)->is_dead_but_stats_not_set() )
          (*li2)->set_stats(robots_killed_this_round);
      
      broadcast(ROBOTS_LEFT, robots_left);
    }

  
  for( list<Shape*>::const_iterator li(object_lists[ROBOT].begin()); li!=object_lists[ROBOT].end(); ++li)
    ((Robot*)(*li))->send_signal();
}


// coulor_dist is supposed to be a subjective distance between two colours, normalized
// to be between 0.0 (same colour) and 1.0 (furthest away).
double
ArenaRealTime::colour_dist(const long col1, const long int col2)
{
  double red1   = (col1 & 0x0000ff);
  double green1 = (col1 & 0x00ff00) >> 8;
  double blue1  = (col1 & 0xff0000) >> 16;

  double red2   = (col2 & 0x0000ff);
  double green2 = (col2 & 0x00ff00) >> 8;
  double blue2  = (col2 & 0xff0000) >> 16;

  return  ( fabs(red1  -red2  ) * log(1.0 + sqrt(red1  +red2  )) + 
            fabs(green1-green2) * log(1.0 + sqrt(green1+green2)) +
            fabs(blue1 -blue2 ) * log(1.0 + sqrt(blue1 +blue2 )) ) / 2417.8;


//    return( ((double)abs((col1 & 0xff)*0x101 - col2.blue))*
//            log(1.0 + sqrt((col1 & 0xff)*0x101 + col2.blue))/log(2.0) +
//            ((double)abs(((col1 & 0xff00) >> 8)*0x101 - col2.green))*
//            log(1.0 + sqrt(((col1 & 0xff00) >> 8)*0x101 + col2.green))/log(2.0) +
//            ((double)abs(((col1 & 0xff0000) >> 16)*0x101 - col2.red))*
//            log(1.0 + sqrt(((col1 & 0xff0000) >> 16)*0x101 + col2.red))/log(2.0));
}

bool
ArenaRealTime::is_colour_allowed(const long colour, const double min_dist, const Robot* robotp)
{
  double d;
  list<Robot*>::const_iterator li;
  for( li = all_robots_in_sequence.begin(); li != all_robots_in_sequence.end(); ++li )
    {
      if(*li != robotp)
        {
          d = colour_dist( colour, (*li)->get_rgb_colour() );
          if( d < min_dist ) return false;          
        }
    }
  
  d = colour_dist( colour, the_opts.get_l( OPTION_BACKGROUND_COLOUR ) );
  if( d < min_dist ) return false;

  return true;
}

long int
ArenaRealTime::find_free_colour(const long int home_colour, 
                                 const long int away_colour, 
                                 const Robot* robotp, const bool random_colours)
{  
  // first check, if Robot is in a team
  if (robotp->isRobotInTeam()) {
	  // robot will get the same color as its team mates
	  list<Robot*>::const_iterator li;
	  for( li = all_robots_in_sequence.begin(); li != all_robots_in_sequence.end(); ++li )
	    {
		if (*li!=robotp && (*li)->is_colour_given() && (*li)->isRobotInTeam() && (*li)->getTeamName()==robotp->getTeamName())
			return (*li)->get_rgb_colour();
	    }
	  
  }

  long int tmp_colour;

  for(double min_dist = 0.1; min_dist > 0.01 ; min_dist *= 0.8)
    {
      if( !random_colours )
        {
          if( is_colour_allowed(home_colour, min_dist, robotp) ) return home_colour;
          if( is_colour_allowed(away_colour, min_dist, robotp) ) return away_colour;
        }
      for(int i=0; i<25; i++)
        {
          tmp_colour = rand() & 0xffffff;
          if( is_colour_allowed(tmp_colour, min_dist*2, robotp) ) return tmp_colour;
        }                  
    }
   Error(true, "Impossible to find colour", "ArenaRealTime::find_free_colour");

   return 0;
}

int
ArenaRealTime::set_debug_level( const int new_level)
{
  ArenaBase::set_debug_level(new_level);

  if( GAME_IN_PROGRESS )
    {
      broadcast(GAME_OPTION, DEBUG_LEVEL, (double)debug_level);
    }

  return debug_level;
}

bool
ArenaRealTime::start_game()
{
  // put the arena together
  if( pause_after_next_game )
    {
      set_state( PAUSING_BETWEEN_GAMES );
      return true;
    }  
  
  current_arena_nr = current_arena_nr % number_of_arenas+1;
  string filename;
  try {
    filename = arena_filenames.at(current_arena_nr-1);
  }
  catch(...) {
    Error(true, "No such element", "ArenaRealTime::start_game" );
  }

  print_to_logfile('G', sequence_nr, game_nr + 1);

  if ( !parse_arena_file(filename) )
  {
    Error(false, "Error parsing arena file " + filename, "ArenaRealTime::start_game");
    return false;
  }

  int charpos;
  try
    {
      if( (charpos = filename.rfind('/',filename.size())) != -1 )
        current_arena_filename = filename.substr(charpos+1, filename.size()-charpos-1);
      else
      {
        Error(false, "Incomplete arena file path " + filename, "ArenaRealTime::start_game");
        return false;
      }
    }
  catch(exception& e)
    {
      Error(true, e.what(), "ArenaRealTime::start_game");
    }

  char msg[128];
  snprintf( msg, 127, _("Game %d of sequence %d begins on arena"),
            game_nr+1, sequence_nr );
  print_message( "RealTimeBattle", string(msg) + " " + current_arena_filename );

  // reset some variables

  shot_count = 0;
  cookie_count = 0;
  mine_count = 0;


  // Place robots on the arena

  Robot* robotp;
  bool found_space;
  double angle;
  Vector2D pos;

  robots_left = 0;
  
  set_state( BEFORE_GAME_START );

  list<Robot*>::const_iterator li;
  for( li = all_robots_in_sequence.begin(); li != all_robots_in_sequence.end(); ++li )
    {
      robotp = *li;
      robotp->get_messages();

      found_space = false;
      for( int i=0; i<100 && !found_space; i++)
        {
          pos = get_random_position();
          found_space = space_available(pos, the_opts.get_d(OPTION_ROBOT_RADIUS)*1.2);
        }

      if( !found_space )
      {
        Error(false, "Couldn't find space for all robots", "ArenaRealTime::start_game");
        return false;
      }
      angle = ((double)rand())*2.0*M_PI/RAND_MAX;
      robotp->set_values_before_game(pos, angle);
      object_lists[ROBOT].push_back(robotp);
      robots_left++;
      robotp->live();
    }

  //  print_to_logfile('G', sequence_nr, games_per_sequence - games_remaining_in_sequence + 1);

  broadcast(GAME_STARTS);
  broadcast(ROBOTS_LEFT, robots_left);
  the_opts.broadcast_opts();


  update_count_for_logging = 0;
  print_to_logfile('T', 0.0);

  for( list<Shape*>::const_iterator li(object_lists[ROBOT].begin()); li!=object_lists[ROBOT].end(); ++li)
    {
      robotp = (Robot*)(*li);
      robotp->send_signal();
      robotp->move(0.0);  // To log current position
    }

  set_state( GAME_IN_PROGRESS );
  game_nr++;

#ifndef NO_GRAPHICS
  if( !no_graphics )
    {
      the_gui.get_arenawindow_p()->clear_area();
      the_gui.get_arenawindow_p()->drawing_area_scale_changed();
      the_gui.get_scorewindow_p()->update_robots();

      reset_timer();  // Time should be zero in score window
      the_gui.get_scorewindow_p()->set_window_title();
      the_gui.get_arenawindow_p()->set_window_title();
    }
#endif

  reset_timer();  // Game starts !
  next_check_time = total_time + the_opts.get_d(OPTION_CHECK_INTERVAL);
  return true;
}

bool
ArenaRealTime::end_game()
{
  Robot* robotp;
  int robots_died_by_timeout = robots_left;
  robots_left = 0;

  list<Robot*>::const_iterator li;
  for( li = all_robots_in_sequence.begin(); li != all_robots_in_sequence.end(); ++li )
    {
      robotp = *li;
      if( robotp->is_alive() || robotp->is_dead_but_stats_not_set() )
        {
          robotp->die();
          robotp->set_stats(robots_died_by_timeout, true);
        }
    }

  broadcast(GAME_FINISHES);

  delete_lists(false, false, false, false);
  
  if(game_nr == games_per_sequence) 
  {
    end_sequence();
    return true;
  }
  else
    return start_game();
}


void
ArenaRealTime::start_sequence()
{
  game_nr = 0;
  current_arena_nr = 0;

  // Make list of robots in this sequence

  for(int i=0; i<robots_per_game; i++)
    {
      list<Robot*>::const_iterator li(all_robots_in_tournament.begin());
      for(int index(1); (li!=all_robots_in_tournament.end()) && (index<robots_in_sequence[sequence_nr][i]); ++li)
        ++index;
      if( li == all_robots_in_tournament.end() )
        Error(true, "No such element", "ArenaRealTime::start_sequence");
        
      all_robots_in_sequence.push_back( *li);
    }

  // execute robot processes


  Robot* robotp;
  list<Robot*>::const_iterator li;
  for( li = all_robots_in_sequence.begin(); li != all_robots_in_sequence.end(); ++li )
    {
      robotp = *li;
      if( robotp->is_networked() )
        {
          *robotp->get_outstreamp() << "@R" << endl;
          robotp->set_values_at_process_start_up();
        }      
      else
        robotp->start_process();
    }
  
  // wait a second before checking

  set_state( STARTING_ROBOTS );
  sequence_nr++;
  reset_timer();
  next_check_time = total_time + the_opts.get_d(OPTION_ROBOT_STARTUP_TIME);
}

bool
ArenaRealTime::start_sequence_follow_up()
{
  // check if the process have started correctly
  Robot* robotp = NULL;

  list<Robot*>::iterator li;
  for( li = all_robots_in_sequence.begin(); li != all_robots_in_sequence.end(); )
    {
      robotp = *li;
      if( !(robotp->is_networked()) &&
          !(robotp->is_process_running()) ) 
        {
          li = all_robots_in_sequence.erase(li);
          robots_left--;
        }
      else
        { 
          ++li;
          if( !robotp->set_and_get_has_competed() )
            print_to_logfile('L', robotp->get_id(), robotp->get_rgb_colour(), 
                             robotp->get_robot_name().c_str());
          
          if( !robotp->is_name_given() )
            {
              robotp->send_message(WARNING, NAME_NOT_GIVEN, "");
              char msg[128];
              snprintf( msg, 127, _("Robot with filename %s has not given any name"),
                        robotp->get_robot_filename().c_str() );
              print_message( "RealTimeBattle", string(msg) );
            }

          if( !robotp->is_colour_given() )
            {
              robotp->send_message(WARNING, COLOUR_NOT_GIVEN, "");
              robotp->set_colour( find_free_colour( 0, 0, robotp, true ) );
              robotp->set_colour_given( true );
            }
        }

    }
  return start_game();
}

void
ArenaRealTime::end_sequence()
{
  // kill all robot processes

  list<Robot*>::const_iterator li;
  for( li = all_robots_in_sequence.begin(); li != all_robots_in_sequence.end(); ++li )
    {
      (*li)->end_process();
    }

  // wait a second before checking

  set_state( SHUTTING_DOWN_ROBOTS);
  next_check_time = total_time + 1.0;
}

void
ArenaRealTime::end_sequence_follow_up()
{
  // check if the process have stopped, otherwise kill
  
  Robot* robotp;

  list<Robot*>::iterator li;
  for( li = all_robots_in_sequence.begin(); li != all_robots_in_sequence.end(); )
    {
      robotp = *li;
      if( robotp->is_process_running() ) robotp->kill_process_forcefully();
      robotp->delete_pipes();
      li = all_robots_in_sequence.erase(li);
    }

  if( sequence_nr == sequences_in_tournament ) 
    end_tournament();
  else
    start_sequence();
}

void
ArenaRealTime::start_tournament_from_tournament_file
( const list<start_tournament_info_t*>& robotfilename_list, 
  const list<start_tournament_info_t*>& arenafilename_list, 
  const int robots_p_game, const int games_p_sequence, 
  const int n_o_sequences, ArenaRealTime* ar_p )
{
  ar_p->start_tournament( robotfilename_list, arenafilename_list, robots_p_game,
                          games_p_sequence, n_o_sequences );
}

void
ArenaRealTime::
start_tournament(const list<start_tournament_info_t*>& robotfilename_list, 
                 const list<start_tournament_info_t*>& arenafilename_list, 
                 const int robots_p_game, 
                 const int games_p_sequence, 
                 const int n_o_sequences)
{
  clear();

  // Open windows if they were closed, else clear them 

#ifndef NO_GRAPHICS
  if( !no_graphics )
    {
      if( the_gui.is_messagewindow_up() )
        MessageWindow::clear_clist( NULL, the_gui.get_messagewindow_p() );
      else //if( !use_message_file )
        the_gui.open_messagewindow();

      if( !the_gui.is_scorewindow_up() ) the_gui.open_scorewindow();
      if( !the_gui.is_arenawindow_up() ) the_gui.open_arenawindow();
    }
#endif

  // Create robot classes and put them into start_sequencethe list all_robots_in_tournament

  number_of_robots = 0;
  robot_count = 0;
  Robot* robotp;
  start_tournament_info_t* infop = NULL;
  string stringp;

  list<start_tournament_info_t*>::const_iterator li;
  for( li = robotfilename_list.begin(); li != robotfilename_list.end(); ++li )
    {
      infop = *li;
      robotp = new Robot(infop->filename);
      all_robots_in_tournament.push_back( robotp );
      number_of_robots++;
    }

  // Create list of arena filenames
  number_of_arenas = 0;
  
  for( li = arenafilename_list.begin(); li != arenafilename_list.end(); ++li )
    {
      stringp = string((*li)->filename);
      arena_filenames.push_back( stringp );
      number_of_arenas++;
    }

  robots_per_game = robots_p_game;
  games_per_sequence = games_p_sequence;
  sequences_in_tournament = n_o_sequences;

  // make list of robots to compete in the sequences
  int games_per_round = binomial(number_of_robots, robots_per_game);
  int complete_rounds = n_o_sequences / games_per_round;
  int rem_games = n_o_sequences % games_per_round;

  robots_in_sequence = new int*[n_o_sequences];
  for(int i=0; i<n_o_sequences; i++) robots_in_sequence[i] = new int[robots_per_game];
  
  int current_sequence[robots_per_game];
  int current_nr = 0;
  //  for(int i=0; i<robots_per_game; i++) current_sequence[i] = i+1;
  
  // set complete rounds first
  for(int round_nr=0; round_nr < complete_rounds; round_nr++)
    {
      int k, i, j;

      for(i=0; i<robots_per_game; i++) current_sequence[i] = i+1;
      current_sequence[robots_per_game-1]--;   // To be increased first time

      
      for(i=0; i< games_per_round; i++)
        {
          
          k = robots_per_game - 1;
          while( current_sequence[k] == number_of_robots + 1 - robots_per_game + k )
            k--;

          if( k < 0 ) Error(true, "Problem generating list of participants, k < 0", 
                            "ArenaRealTime::start_tournament");

          current_sequence[k]++;
          for(j=k+1; j<robots_per_game; j++) current_sequence[j] = current_sequence[j-1]+1;

          for(j=0; j<robots_per_game; j++) 
            robots_in_sequence[current_nr][j] = current_sequence[j];

          current_nr++;
        }
      reorder_pointer_array((void**)robots_in_sequence, games_per_round);
    }

  // the remaining game will be randomly chosen

  int robot_matches_played[number_of_robots];
  for(int i=0; i<number_of_robots; i++) robot_matches_played[i] = 0;

  bool robot_playing_this_match[number_of_robots];
  int min_matches_played = 0;
  int number_of_robots_on_min_matches = number_of_robots;
  int nr;

  for(int i=0; i< rem_games; i++)
    {
      for(int i2=0; i2<number_of_robots; i2++) robot_playing_this_match[i2] = false;

      for(int j=0; j<robots_per_game; j++)
        {
          do 
            nr = (int)floor(number_of_robots*((double)rand() / (double)RAND_MAX));
          while( robot_playing_this_match[nr] || robot_matches_played[nr] != min_matches_played );

          robot_playing_this_match[nr] = true;
          robot_matches_played[nr]++;
          number_of_robots_on_min_matches--;
          if( number_of_robots_on_min_matches == 0 ) 
            {
              min_matches_played++;
              number_of_robots_on_min_matches = number_of_robots;
            }

          robots_in_sequence[current_nr][j] = nr + 1;   // robot count will start from 1
        }      
      current_nr++;
    }

  // set random seed
  srand(timer.get_random_seed());

  #ifndef NO_NETWORK
  // set ClientInterface to not accept new connection
  try{
    ClientInterface::Instance()->set_accepting(false);
  }catch(const bad_exception &e){
      cerr<< e.what() << endl;
  }
  #endif   
	
  // start first sequence

  print_to_logfile('H', games_per_sequence, robots_per_game, 
                   sequences_in_tournament, number_of_robots);

  the_opts.log_all_options();

  sequence_nr = 0;
  start_sequence();
}

void
ArenaRealTime::end_tournament()
{
  set_state( FINISHED );

#ifndef NO_GRAPHICS
  if( !no_graphics )
    {
      //      if( !use_message_file )
      //        the_gui.close_messagewindow();
      //      the_gui.close_scorewindow();
      the_gui.close_arenawindow();
    }
#endif
}
