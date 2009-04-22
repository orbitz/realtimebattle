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

#include <sys/stat.h>
#include <math.h>
#include <stdio.h>
#include <string>
#include <sstream>
using namespace std;

#include "ArenaReplay.h"
#include "ArenaController.h"
#include "IntlDefs.h"
#include "Various.h"
#include "Options.h"
#include "Extras.h"
#include "Shot.h"

#ifndef NO_GRAPHICS
#include "MessageWindow.h"
#include "ArenaWindow.h"
#include "ScoreWindow.h"
#include "ControlWindow.h"
#endif

#include "Robot.h"

const int max_time_infos = 16384;

extern class ControlWindow* controlwindow_p;

ArenaReplay::ArenaReplay()
{
  reset_timer();
  //  speed = PLAY;
  fast_forward_factor = 1.0;
  state = NOT_STARTED;
  current_replay_time = 0.0;
  game_position_in_log = NULL;
  time_position_in_log = NULL;

  set_game_mode( (ArenaBase::game_mode_t)the_arena_controller.game_mode );
  set_filenames( the_arena_controller.replay_filename,
                 the_arena_controller.message_filename,
                 the_arena_controller.statistics_filename,
                 the_arena_controller.option_filename );
}

ArenaReplay::~ArenaReplay()
{
  if( game_position_in_log != NULL )
    {
      for( int i=0; i<sequences_in_tournament; i++ )
        delete [] game_position_in_log[i];
      delete [] game_position_in_log;
    }

  if( time_position_in_log != NULL )
    delete [] time_position_in_log;
    
  list<object_pos_info_t*>::const_iterator li;
  for(li = object_positions_in_log.begin(); li != object_positions_in_log.end(); ++li)
    delete *li;
}

bool
ArenaReplay::timeout_function()
{
  if( ( state == GAME_IN_PROGRESS || state == BEFORE_GAME_START ) )
    update_timer( fast_forward_factor );
  else if( state == PAUSED )
    update_timer( 0.0 );

  switch( state )
    {
    case NOT_STARTED:
      start_tournament();
      break;
    case GAME_IN_PROGRESS:
      {
#ifndef NO_GRAPHICS
        int old_total = (int)current_replay_time;
#endif

        parse_this_interval();

#ifndef NO_GRAPHICS
        if(state!= FINISHED && (int)current_replay_time - old_total != 0 && !no_graphics)
          {
            the_gui.get_scorewindow_p()->set_window_title();
            if( !log_from_stdin )
              controlwindow_p->set_progress_time( current_replay_time );
          }
#endif
      }
      break;

    case BEFORE_GAME_START:
      parse_this_interval();
#ifndef NO_GRAPHICS
      if( !no_graphics )
        {
          if( controlwindow_p->get_displayed() != ControlWindow::REPLAY_WIDGETS &&
              !log_from_stdin )
            controlwindow_p->display_replay_widgets();
          the_gui.get_arenawindow_p()->drawing_area_scale_changed();
          the_gui.get_arenawindow_p()->draw_everything();
          the_gui.get_scorewindow_p()->update_robots();
          char msg[64];
          snprintf(msg, 63, _("Game %d of sequence %d"), game_nr, sequence_nr);
          print_message( "RealTimeBattle", string(msg) );
        }
#endif
      set_state( GAME_IN_PROGRESS );
      break;

    case FINISHED:
      if( the_arena_controller.auto_start_and_end )
        {
          if( statistics_file_name != "" )
            save_statistics_to_file( statistics_file_name );

          Quit();
        }
      break;

    default:
      break;
    }
  return true;
}

void
ArenaReplay::parse_this_interval()
{
#ifndef NO_GRAPHICS
  if( state == GAME_IN_PROGRESS && !no_graphics)
    the_gui.get_messagewindow_p()->freeze_clist();
#endif
  if( fast_forward_factor > 0.0 )
    while( !log_file.eof() && total_time >= current_replay_time )
      {
        parse_this_time_index();
      }
  else
    {
    while( !log_file.eof() && total_time <= current_replay_time &&
           step_forward(-1, false) )
      {
        parse_this_time_index();
      }
    }
#ifndef NO_GRAPHICS
  if( state == GAME_IN_PROGRESS && !no_graphics)
    the_gui.get_messagewindow_p()->thaw_clist();
#endif

  if( log_file.eof() )
    {
      set_state( FINISHED );
#ifndef NO_GRAPHICS
      if( !no_graphics )
        {
          controlwindow_p->remove_replay_widgets();
          the_gui.close_arenawindow();
        }
#endif
    }
}

void
ArenaReplay::parse_this_time_index()
{
  double last_replay_time = current_replay_time;

  if( log_file.peek() == 'T' )
    parse_log_line();
  else
    {
        //cout << "'T' not first in log_file for parse_this_time_index()" << endl;
        //cout << "log_file.peek(): " << (char)log_file.peek() << endl;
        //cout << "current_replay_time: " << current_replay_time << endl;
    }

  double next_replay_time = current_replay_time;

  while( log_file.peek() != 'T' || current_replay_time == 0.0 )
    {
      if( parse_log_line() == '?' ) return;
      if ( current_replay_time == 0.0 ) next_replay_time = 0.0; // A new game has started
    }


  current_replay_time = next_replay_time;
  move_shots_no_check( current_replay_time - last_replay_time );

#ifndef NO_GRAPHICS
        if( !no_graphics )
          the_gui.get_arenawindow_p()->draw_moving_objects( true );
#endif

  // check if robots have died and set their points
  if( robots_killed_this_round != 0 )
    {
      robots_left -= robots_killed_this_round;

      Robot* robotp;
      for( list<Shape*>::const_iterator li(object_lists[ROBOT].begin()); li!=object_lists[ROBOT].end(); ++li)
        {
          robotp = (Robot*)(*li);
          if( robotp->is_alive() )
            {
              //              robotp->add_points(robots_killed_this_round);
#ifndef NO_GRAPHICS
              if( robots_left < 15 && !no_graphics )
                robotp->display_score();
#endif
            }
        }

      robots_killed_this_round = 0;
    }
}

void
ArenaReplay::start_tournament()
{
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

  if( !log_from_stdin )
    make_statistics_from_file();

  set_state( BEFORE_GAME_START );
}

bool
ArenaReplay::end_game()
{
}

void
ArenaReplay::update()
{
}

bool
ArenaReplay::start_game()
{
}

void
ArenaReplay::start_sequence()
{
}

void
ArenaReplay::end_sequence()
{
}

void
ArenaReplay::end_tournament()
{
}

char
ArenaReplay::parse_log_line()
{
  char first_letter = '?';

  log_file >> ws;
  log_file.get( first_letter );

  if( log_file.eof() ) return '?';
  //  cerr << first_letter;
  switch( first_letter )
    {
    case 'R':
    case 'S':
    case 'M':
    case 'C':
    case 'D': // these five are dependent on which direction we are replaying
      if ( fast_forward_factor > 0.0 )
        parse_log_line_forward( first_letter );
      else
        parse_log_line_rewind( first_letter );
      break;

    case 'T': // Time
      {
        log_file >> current_replay_time;
      }
      break;
    case 'P': // Print a robot message
      {
        int robot_id;
        char message[200];
        log_file >> robot_id;
        log_file.get( message, 200, '\n' );
        list<Shape*>::iterator li;
        find_object_by_id( object_lists[ROBOT], li, robot_id );
        if( li != object_lists[ROBOT].end() )
          {
            Robot* robotp = (Robot*)(*li);
            print_message( robotp->get_robot_name(), string(message) );
          }
        // else: robot sent a message before first game of sequences.
        // Robot data is not yet known to us, thus ignore!
      }
      break;
    case 'G': // Game begins
      {
        delete_lists(false, false, false, false);
        reset_timer();
        current_replay_time = 0;
        robots_killed_this_round = 0;
        robots_left = robots_per_game;

        log_file >> sequence_nr >> game_nr;

        if( !log_from_stdin )
          get_time_positions_in_game();

        list<Robot*>::const_iterator li;
        for( li = all_robots_in_tournament.begin(); li != all_robots_in_tournament.end(); ++li )
          (*li)->set_values_before_game(Vector2D(infinity_rtb,infinity_rtb), 0.0);

        arena_scale = the_opts.get_d(OPTION_ARENA_SCALE);
        arena_angle_factor = 1.0;
        arena_succession = 1;
        set_state( BEFORE_GAME_START );

#ifndef NO_GRAPHICS
        controlwindow_p->change_time_limitations();
#endif
      }
      break;
    case 'H': // Header
      {
        if( log_from_stdin )
          log_file >> games_per_sequence >> robots_per_game
                   >> sequences_in_tournament >> number_of_robots;
        else
          {
            char buffer[400];
            log_file.get( buffer, 400, '\n' );
            log_file >> ws;
          }
      }
      break;
    case 'L': // List of robot properties
      {
        if( log_from_stdin )
          {
            int robot_id, col;
            char robot_colour[7];
            char name[200];
            log_file >> robot_id >> ws;
            log_file.get( robot_colour, 7, ' ');
            istringstream string2hex(robot_colour);
            string2hex >> std::hex >> col;
            log_file.get( name, 200, '\n' );
            Robot* robotp = new Robot( robot_id, col, string(name) );
            all_robots_in_tournament.push_back(robotp); // used by statistics
          }
        else
          {
            char buffer[400];
            log_file.get( buffer, 400, '\n' );
            log_file >> ws;
          }
      }
      break;
    case 'A': // Arena file line
      {
        parse_arena_line( log_file, arena_scale, arena_succession, arena_angle_factor );
      }
      break;
    case 'O': // Option
      {
        char temp;
        char label[200];
        log_file.get( label, 200, ':');
        log_file.get( temp );
        option_return_t opt = the_opts.get_option_from_string( string( label ) );
        switch( opt.datatype )
          {
          case ENTRY_INT:
          case ENTRY_HEX:
            {
              long val;
              log_file >> val;
              //              cerr << label << ": " << val << endl;
              the_opts.get_all_long_options()[opt.option_number].value = val;
            }
            break;
          case ENTRY_DOUBLE:
            {
              double val;
              log_file >> val;
              //              cerr << label << ": " << val << endl;
              the_opts.get_all_double_options()[opt.option_number].value = val;
            }
            break;
          case ENTRY_CHAR:
            {
              char val[400];
              log_file.get( val, 400, '\n' );
              //              cerr << label << ": " << val << endl;
              the_opts.get_all_string_options()[opt.option_number].value = val;
            }
            break;
          case ENTRY_BOOL:
            break;
          default:
            Error( true, "Unknown datatype", "ArenaReplay::parse_log_line" );
            break;
          }
      }
      break;

    case '?':
    default:
      Error( false, "Unrecognized first letter in logfile: " +
             string(1, first_letter), "ArenaReplay::parse_log_line" );
      char buffer[400];
      log_file.get( buffer, 400, '\n' );
      log_file >> ws;
      log_file.clear();
      break;
    }

  log_file >> ws;

  return first_letter;
}

void
ArenaReplay::parse_log_line_forward( const char first_letter )
{
  switch( first_letter )
    {
    case 'R': // Robot pos
      {
        int robot_id;
        double x, y, robot_angle, cannon_angle, radar_angle, energy;
        Robot* robotp = NULL;
        log_file >> robot_id >> x >> y
                 >> robot_angle >> cannon_angle >> radar_angle >> energy;
        
        list<Shape*>::iterator li;
        if( find_object_by_id( object_lists[ROBOT], li, robot_id ) )
          {
            robotp = (Robot*)(*li);
          }
        else
          {
            list<Robot*>::iterator li2;

            if( log_from_stdin &&
                find_object_by_id( all_robots_in_tournament, li2, robot_id ) )
              {
                object_lists[ROBOT].push_back(*li2);
                robotp = *li2;
              }
            else
              Error(true, "Robot not in list", "ArenaReplay::parse_log_line_forward");
          }

        robotp->change_position( x, y, robot_angle, cannon_angle, radar_angle, energy );
        robotp->live();
      }
      break;
    case 'C': // Cookie
      {
        int cookie_id;
        double x, y;
        log_file >> cookie_id >> x >> y;
        Cookie* cookiep = new Cookie( Vector2D(x,y), 0.0, cookie_id);
        object_lists[COOKIE].push_back( cookiep );
      }
      break;
    case 'M': // Mine
      {
        int mine_id;
        double x, y;
        log_file >> mine_id >> x >> y;
        Mine* minep = new Mine( Vector2D(x,y), 0.0, mine_id);
        object_lists[MINE].push_back( minep );
      }
      break;
    case 'S': // Shot
      {
        int shot_id;
        double x, y, dx, dy;
        log_file >> shot_id >> x >> y >> dx >> dy;
        Shot* shotp = new Shot( Vector2D(x,y), Vector2D(dx,dy), 0, shot_id );
        object_lists[SHOT].push_back( shotp );
      }
      break;


    case 'D': // Die
      {
        char object_type = '?';
        int object_id;
        log_file.get( object_type );
        log_file >> object_id;
        switch( object_type )
          {
          case 'R':
            {
              double points_received;
              int pos_this_game;
              log_file >> points_received >> pos_this_game;
              list<Shape*>::iterator li;
              find_object_by_id( object_lists[ROBOT], li, object_id );
              if( li == object_lists[ROBOT].end() )
                Error(true, "Dying robot not in list", "ArenaReplay::parse_log_line_forward");
              Robot* robotp = (Robot*)(*li);
              if( robotp->is_alive() )
                {
                  robotp->die();
                  robots_killed_this_round++;
                  robotp->set_stats( points_received, pos_this_game,
                                     current_replay_time, log_from_stdin);
                  robotp->change_energy( -robotp->get_energy() );
                }
            }
            break;
          case 'C':
            {
              list<Shape*>::iterator li;
              find_object_by_id( object_lists[COOKIE], li, object_id );
              if( li == object_lists[COOKIE].end() )
                Error(false, "Dying cookie not in list", "ArenaReplay::parse_log_line_forward");
              else
                {
                  ((Cookie*)(*li))->die();
                  object_lists[COOKIE].erase(li);
                }
            }
            break;
          case 'M':
            {
              list<Shape*>::iterator li;
              find_object_by_id( object_lists[MINE], li, object_id );
              if( li == object_lists[MINE].end() )
                Error(false, "Dying mine not in list", "ArenaReplay::parse_log_line_forward");
              else
                {
                  ((Mine*)(*li))->die();
                  object_lists[MINE].erase(li);
                }
            }
            break;
          case 'S':
            {
              list<Shape*>::iterator li;
              find_object_by_id( object_lists[SHOT], li, object_id );
              if( li == object_lists[SHOT].end() )
                Error(false, "Dying shot not in list", "ArenaReplay::parse_log_line_forward");
              else
                {
                  ((Shot*)(*li))->die();
                  object_lists[SHOT].erase(li);
                }
            }
            break;
          case '?':
          default:
            Error( false, "Unknown object type is dead", "ArenaReplay::parse_log_line_forward" );
            break;
          }
      }
      break;
    }
}

void
ArenaReplay::parse_log_line_rewind( const char first_letter )
{
  switch( first_letter )
    {
    case 'R':
      {
        int robot_id;
        double x, y, robot_angle, cannon_angle, radar_angle, energy;
        log_file >> robot_id >> x >> y >>
          robot_angle >> cannon_angle >> radar_angle >> energy;
        list<Shape*>::iterator li;
        find_object_by_id( object_lists[ROBOT], li, robot_id );
        if( li == object_lists[ROBOT].end() ) 
          Error(true, "Robot not in list", "ArenaReplay::parse_log_line_rewind");

        Robot* robotp = (Robot*)(*li);

        robotp->change_position( x, y, robot_angle, cannon_angle, radar_angle, energy );
        if( !robotp->is_alive() )
          {
            robotp->live();
            robots_killed_this_round--;
            robotp->set_stats(0.0, 0, 0.0, false); // clear position_this_game
          }
      }
      break;


    case 'C':
      {
        int id;
        double x,y;
        log_file >> id >> x >> y;
        list<Shape*>::iterator li;
        find_object_by_id( object_lists[COOKIE], li, id );
        if( li == object_lists[COOKIE].end() )
          {
            // This happens if it dies the round as it is created
            //Error(false, "Dying cookie not in list", "ArenaReplay::parse_log_line_rewind");
          }
        else
          {
            ((Cookie*)(*li))->die();
            object_lists[COOKIE].erase(li);
          }
      }
      break;
    case 'M':
      {
        int id;
        double x,y;
        log_file >> id >> x >> y;
        list<Shape*>::iterator li;
        find_object_by_id( object_lists[MINE], li, id );
        if( li == object_lists[MINE].end() )
          {
            // This happens if it dies the round as it is created
            //Error(false, "Dying mine not in list", "ArenaReplay::parse_log_line_rewind");
          }
        else
          {
            ((Mine*)(*li))->die();
            object_lists[MINE].erase(li);
          }
      }
      break;
    case 'S':
      {
        int id;
        double x,y,vx,vy;
        log_file >> id >> x >> y >> vx >> vy;
        //        cout << "Shot rewind_dying: " << id << endl;
        list<Shape*>::iterator li;
        find_object_by_id( object_lists[SHOT], li, id );
        if( li == object_lists[SHOT].end() )
          {
            // This happens if it dies the round as it is created
            //Error(false, "Dying shot not in list", "ArenaReplay::parse_log_line_rewind");
          }
        else
          {
            ((Shot*)(*li))->die();
            object_lists[SHOT].erase(li);
          }
      }
      break;

    case 'D':
      char object_type = '?';
      int object_id;
      log_file.get( object_type );
      log_file >> object_id;
      switch( object_type )
        {
        case 'R':
          {
            double pnts;
            int pos;
            log_file >> pnts >> pos;
            // Robot will be revived at the next 'R' line.
          }
          break;
        case 'C': // Cookie
          {
            object_pos_info_t* info = find_object_in_log( COOKIE, object_id );
            if( info->end_time > info->start_time )
              object_lists[COOKIE].push_back
                ( new Cookie( info->pos, 0.0, info->id ) );
          }
          break;
        case 'M': // Mine
          {
            object_pos_info_t* info = find_object_in_log( MINE, object_id );
            if( info->end_time > info->start_time )
              object_lists[MINE].push_back
                ( new Mine( info->pos, 0.0, info->id ) );
          }
          break;
        case 'S': // Shot
          {
            object_pos_info_t* info = find_object_in_log( SHOT, object_id );

            if( info->end_time > info->start_time )
              object_lists[SHOT].push_back
                ( new Shot( info->pos + (current_replay_time - info->start_time)
                            * info->vel, info->vel, 0, info->id ) );
          }
          break;

        }
      break;
    }
}

void
ArenaReplay::set_filenames( string& replay_fname, string& message_fname,
                            const string& statistics_fname,
                            const string& option_fname )
{

  if( replay_fname != "-" )
    {
      log_file.open( replay_fname.c_str() );
      if( !log_file )
        Error( true, "Couldn't open replay file",
               "ArenaReplay::set_filenames" );
      log_from_stdin = false;
    }
  else
    {
//      log_file.attach( STDIN_FILENO );
	log_file.ios::rdbuf(cin.rdbuf());
      if( !log_file )
        Error( true, "Couldn't attach to STDIN",
               "ArenaRealTime::set_filenames" );
      log_from_stdin = true;
    }

  if( message_fname.empty() )
    use_message_file = false;
  else if( message_fname == "-" || message_fname == "STDOUT" )
    {
      use_message_file = true;
//      message_file.attach( STDOUT_FILENO );
	message_file.ios::rdbuf(cout.rdbuf());
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
  option_file_name = option_fname;
}

// Changes game when replaying to fast forward, rewind or normal speed
void
ArenaReplay::change_speed( const bool forward, const bool fast )
{
  if( fast_forward_factor < 0 )
    {  // Parse current time_index in forward direction
      fast_forward_factor = 1.0;
      step_forward( 0, true );
    }

  if( !fast || log_from_stdin )
    {
      fast_forward_factor = 1.0;
    }
  else if( forward )
    {
      fast_forward_factor = the_opts.get_d( OPTION_FAST_FORWARD_FACTOR );
    }
  else
    {
      fast_forward_factor = -the_opts.get_d( OPTION_FAST_FORWARD_FACTOR );
      step_forward( 0, true );
    }
}

// Changes the game or sequence number. Mainly after pressing one of
// the replay buttons in the control panel.
void
ArenaReplay::change_game( const int inc_game, const int inc_seq )
{
  int temp_game = game_nr;
  int temp_seq = sequence_nr;

  temp_game += inc_game;
  temp_seq += inc_seq;

  if( inc_seq == 0 )
    {
    if( temp_game >= games_per_sequence + 1 )
      {
        temp_game -= games_per_sequence;
        temp_seq++;
      }
    else if( temp_game <= 0 )
      {
        temp_game += games_per_sequence;
        temp_seq--;
      }
    }
  else if( inc_seq < 0 )
    temp_game = games_per_sequence;
  else if( inc_seq > 0 )
    temp_game = 1;

  if( temp_seq < 1 || temp_seq > sequences_in_tournament )
    return;

  //  cout << "Seq: " << temp_seq << "  Game: " << temp_game
  //       << "  game_pos: " << game_position_in_log[temp_seq-1][temp_game-1] << endl;

  log_file.seekg( game_position_in_log[temp_seq-1][temp_game-1] );

  //  cout << log_file.tellg() << " " << (char)log_file.peek() << endl;
}


// returns true if the step forward succeded
bool
ArenaReplay::step_forward( const int n_o_steps, const bool clear_time )
{
  if( !log_from_stdin )
    {
      int index = find_streampos_for_time( current_replay_time );

      index += n_o_steps;

      //      cout << "Stepping to index: " << index << endl;

      if( index >= 0 && index <= max_time_infos && time_position_in_log[index].pos > 0 )
        {
          double last_replay_time = current_replay_time;
          current_replay_time = time_position_in_log[index].time;
          log_file.seekg( time_position_in_log[index].pos );

          if( clear_time )
            {
              total_time = time_position_in_log[index].time + 0.00001;
              update_timer(0.0);
              parse_this_time_index();
            }
          if( n_o_steps != 0 )
            move_shots_no_check( current_replay_time - last_replay_time );

#ifndef NO_GRAPHICS
          if( !no_graphics && n_o_steps != 0 )
            {
              the_gui.get_arenawindow_p()->draw_moving_objects( true );
              the_gui.get_scorewindow_p()->set_window_title();
              if( !log_from_stdin )
                controlwindow_p->set_progress_time( current_replay_time );
            }
#endif

          return true;
        }
      else
        return false;
    }
  return true;
}

void
ArenaReplay::step_forward_or_backward( const bool forward )
{
  double fff_tmp = fast_forward_factor;
  if( forward )
    step_forward( 1, true );
  else
    {
      fast_forward_factor = -1.0;
      step_forward( 0, true );
      step_forward( -1, true);
      fast_forward_factor = 1.0;
      step_forward( 0, true );
    }

  fast_forward_factor = fff_tmp;
}

void
ArenaReplay::change_replay_time( const double time )
{
  if( !log_from_stdin && abs_rtb(time - current_replay_time) > 0.001 )
    {
      int index = find_streampos_for_time( time );

      index -= 1; // ??

      //      cout << "Changed to index: " << index << endl;

      if( index >= 0 && index <= max_time_infos && time_position_in_log[index].pos > 0 )
        {
          current_replay_time = time_position_in_log[index].time;
          total_time = time_position_in_log[index].time + 0.00001;
          update_timer(0.0);
          log_file.seekg( time_position_in_log[index].pos );
          recreate_lists();
        }
    }
}

// Recreates the object_list[] array to hold what is correct for the current
// time, not the old time.
void
ArenaReplay::recreate_lists()
{
  // Kill och awaken robots that have died or began to live again.
  {
    list<Shape*>::iterator li;
    for( li = object_lists[ROBOT].begin(); li != object_lists[ROBOT].end(); ++li )
      {
        Robot* robotp = (Robot*)(*li);
        int id = robotp->get_id();
        object_pos_info_t* info;
        if( NULL != (info = find_object_in_log( ROBOT,id ) ) )
        {
          if( info->end_time < current_replay_time && robotp->is_alive() )
            {
              robotp->die(); // TODO: Something more here?
              robotp->change_energy( -robotp->get_energy() );
            }
          else if( info->end_time > current_replay_time && !(robotp->is_alive()) )
            {
              robotp->live(); // TODO: Something more here?
            }
        }
        else
          Error(false, "Robot in object_list is not in object_positions_in_log",
                "ArenaReplay::recreate_lists");
      }
  }

  // Kill all Shots, mines and cookies that are in the object_list
  // Create new shots for the new time.
  {
    for( int type = SHOT; type <= MINE; (type == SHOT ? type+=2 : type++)  )
      {
        list<Shape*>::iterator li;
        for( li = object_lists[type].begin(); li != object_lists[type].end();)
          switch( type )
            {
            case SHOT:
              {
                ((Shot*)(*li))->die();
                li = object_lists[SHOT].erase(li);
              }
              break;
            case COOKIE:
              {
                ((Cookie*)(*li))->die();
                li = object_lists[COOKIE].erase(li);
              }
              break;
            case MINE:
              {
                ((Mine*)(*li))->die();
                li = object_lists[MINE].erase(li);
              }
              break;
            default:
               ++li;
	       break;
            }
      }

    list<object_pos_info_t*>::const_iterator li;
    for( li = object_positions_in_log.begin(); li != object_positions_in_log.end(); ++li )
      {
        object_pos_info_t* info = *li;
        if( current_replay_time > info->start_time &&
            current_replay_time < info->end_time )
          {
            switch( info->obj )
              {
              case SHOT:
                object_lists[SHOT].push_back
                  ( new Shot( info->pos + (current_replay_time - info->start_time)
                              * info->vel, info->vel, 0, info->id ) );
                break;
              case COOKIE:
                object_lists[COOKIE].push_back
                  ( new Cookie( info->pos, 0.0, info->id ) );
                break;
              case MINE:
                object_lists[MINE].push_back
                  ( new Mine( info->pos, 0.0, info->id ) );
                break;
              case ROBOT:
                break;
              default:
                Error(true,
                      "Variable 'type' has a forbidden value. Should never happen",
                      "ArenaReplay::recreate_lists");
              }
          }
      }
  }
}

// Searches the log_file for a line beginning with any of the
// letters in 'search_letters'.
// The file pointer will be directly after the found letter.
// Returns the letter found, or '?' if none found.
char
ArenaReplay::search_forward( const string& search_letters )
{
  if( log_from_stdin )
    return '?';

  char letter;
  char buffer[400];

  log_file >> ws;
  while( !log_file.eof() )
    {
      log_file.clear();

      log_file >> letter;   // check first letter of line
      if( search_letters.find( letter ) != -1 )
        {
          return letter;
        }

      log_file >> ws;
      log_file.get( buffer, 400, '\n' );  // go to next line
    }

  return '?';
}

// Similar to the previous function, but the argument is a list
// of strings (string lengths between 1 and 16) to serach for.
// Returns the string found, or the empty string if none found.
string
ArenaReplay::search_forward( const list<string>& search_strings )
{
  if( log_from_stdin ) return "";

  bool found = false;
  char buffer[400];
  list<string>::const_iterator li;
  int i;
  int read_letters;
  char letter[16];

 log_file >> ws;
 while( !log_file.eof() )
    {
      log_file.clear();

      read_letters = 0;
      for( li = search_strings.begin(); li != search_strings.end() && !found; ++li )
        {
          found = true;
          for( i=0; i < li->size() && found; i++ )
            {
              if( read_letters < i+1 )
                {
                  log_file >> letter[read_letters];
                  read_letters++;
                }
              if( (*li)[i] != letter[i] ) found = false;
            }

          if( found ) return *li;
        }

      log_file.get( buffer, 400, '\n' );  // go to next line
    }

  return "";
}


string
ArenaReplay::search_backwards( const string& search_letters )
{
/* if( log_from_stdin )
    return "";

  char letter='?';
  while( search_letters.find( letter ) == -1 && log_file.tellg() != 0 )
    {
      beginning_of_prev_line();
      letter = log_file.peek();
    }

  if( log_file.tellg() == 0 )
    return "";
  else
    {
      char buffer[400];
      log_file.get( buffer, 400, '\n' );
      beginning_of_current_line();
      return (String) buffer;
    }

 */
  return "";
}

void
ArenaReplay::beginning_of_prev_line()
{
	/*
  char letter='?';
  bool already_found_one_line = false;
  while( ( letter != '\n' || !already_found_one_line ) &&
         log_file.tellg() != 0 )
    {
      log_file.seekg( -1, ios::cur );
      letter = log_file.peek();
      if( letter == '\n' && !already_found_one_line )
        {
          already_found_one_line = true;
          letter = '?';
        }
    }

  log_file.get( letter );
  */
}

void
ArenaReplay::beginning_of_current_line()
{
  /*
  char letter='?';
  while( letter != '\n' && log_file.tellg() != 0 )
    {
      log_file.seekg( -1, ios::cur );
      letter = log_file.peek();
    }
  log_file.get( letter );
 */
}

void
ArenaReplay::make_statistics_from_file()
{
  list<string> str_list;
  str_list.push_back( string("DR") );
  str_list.push_back( string("L") );
  str_list.push_back( string("G") );
  str_list.push_back( string("T") );
  str_list.push_back( string("H") );

  list<Shape*>::iterator li;
  double points_received;
  int pos_this_game, object_id;
  char buffer[400];
  string letters;

  streampos old_pos = log_file.tellg();

  while(  (letters = search_forward( str_list ))  != "" )
    {
      switch( letters[0] )
        {
        case 'D':
          log_file >> object_id >> points_received >> pos_this_game;

          find_object_by_id( object_lists[ROBOT], li, object_id );

          if( li == object_lists[ROBOT].end() ) Error(true, "Dying robot not in list",
                               "ArenaReplay::make_statistics_from_file");
          ((Robot*)(*li))->set_stats( points_received, pos_this_game,
                                     current_replay_time, true);
          break;

        case 'L':
          {
	    // check whether the L is not from the arena file
	    if (log_file.peek()==' ')
		    break;
            int robot_id, col;
            char robot_colour[7];
            char name[200];
            log_file >> robot_id >> ws;
            log_file.get( robot_colour, 7, ' ');
            istringstream string2hex(robot_colour);
            string2hex >> std::hex >> col;
            log_file >> ws;
            log_file.get( name, 200, '\n' );
            Robot* robotp = new Robot( robot_id, col, string(name) );
            object_lists[ROBOT].push_back(robotp); // array better?
            all_robots_in_tournament.push_back(robotp); // used by statistics
          }
          break;

        case 'G':
          {
            streampos temp_pos = log_file.tellg();
	    temp_pos-=1;
            log_file >> sequence_nr >> game_nr;
            game_position_in_log[sequence_nr-1][game_nr-1] = temp_pos;
          }
          break;

        case 'T':
          log_file >> current_replay_time;
          break;

        case 'H':
          if( game_position_in_log != NULL )
            {
              log_file.seekg( old_pos );
              return;
            }

          log_file >> games_per_sequence >> robots_per_game
                   >> sequences_in_tournament >> number_of_robots;

          game_position_in_log = new streampos*[sequences_in_tournament];

          for( int i=0; i<sequences_in_tournament; i++ )
              game_position_in_log[i] = new streampos[games_per_sequence];

          break;

        default:
          Error(true, "Wrong log line found",
                "ArenaReplay::make_statistics_from_file");
          break;
        }
      log_file.get( buffer, 400, '\n' );
    }

  log_file.clear();
  log_file.seekg( old_pos );
  log_file.clear();

  current_replay_time = 0;
}

void
ArenaReplay::get_time_positions_in_game()
{

  string letter_list = "TGHSMCDR";
  char letter;
  char buffer[400];
  int time_pos_index = 0;
  float cur_time;
  if( time_position_in_log != NULL ) delete [] time_position_in_log;

  time_position_in_log = new time_pos_info_t[max_time_infos];
  
  list<object_pos_info_t*>::const_iterator li;
  for(li = object_positions_in_log.begin(); li != object_positions_in_log.end(); ++li)
    delete *li;
  object_positions_in_log.clear();

  for(int i=0; i<max_time_infos; i++)
    {
      time_position_in_log[i].pos = -1;
      time_position_in_log[i].time = 0.0;
    }

  streampos old_pos = log_file.tellg();

  //  cout << "get_time_positions_in_game" << endl;

  while(  (letter = search_forward( letter_list ))  != '?' )
    {
      switch( letter )
        {
        case 'T':
          time_position_in_log[time_pos_index].pos = log_file.tellg();
	  time_position_in_log[time_pos_index].pos-=1;
          log_file >> cur_time;
          time_position_in_log[time_pos_index].time = cur_time;

          time_pos_index++;
          if( time_pos_index >= max_time_infos )
            Error(false, "Too many time-info entries",
                  "ArenaReplay::get_time_positions_in_game");

          break;

        case 'S':
          {
            int shot_id;
            double x, y, dx, dy;
            log_file >> shot_id >> x >> y >> dx >> dy;
            object_positions_in_log.push_back
              ( new object_pos_info_t( SHOT, shot_id, Vector2D( x,y ),
                                       cur_time, the_opts.get_d( OPTION_TIMEOUT ),
                                       Vector2D( dx,dy ) ) );
          }
          break;

        case 'M':
          {
            int mine_id;
            double x, y;
            log_file >> mine_id >> x >> y;
            object_positions_in_log.push_back
              ( new object_pos_info_t( MINE, mine_id, Vector2D( x,y ),
                                       cur_time, the_opts.get_d( OPTION_TIMEOUT ) ) );
          }
          break;
        case 'C':
          {
            int cookie_id;
            double x, y;
            log_file >> cookie_id >> x >> y;

            object_positions_in_log.push_back
              ( new object_pos_info_t( COOKIE, cookie_id, Vector2D( x,y ),
                                       cur_time, the_opts.get_d( OPTION_TIMEOUT ) ) );
          }
          break;

        case 'R':
          {
            int robot_id;
            double x, y, temp;
            log_file >> robot_id >> x >> y >> temp >> temp >> temp >> temp;

            if( NULL == ( find_object_in_log( ROBOT, robot_id ) ) )
              {
                object_positions_in_log.push_back
                  ( new object_pos_info_t( ROBOT, robot_id, Vector2D( x,y ),
                                           0, the_opts.get_d( OPTION_TIMEOUT ) ) );
                list<Robot*>::iterator li;
                find_object_by_id( all_robots_in_tournament, li, robot_id );
                object_lists[ROBOT].push_back( *li );
              }
          }
          break;

        case 'D':
          {
            char obj = '?';
            int object_id;
            log_file.get( obj );
            log_file >> object_id;
            object_type object = NOOBJECT;
            switch( obj )
              {
              case 'S':
                object = SHOT;
                break;

              case 'M':
                object = MINE;
                break;
              case 'C':
                object = COOKIE;
                break;

              case 'R':
                object = ROBOT;
                break;

              default:
                break;
              }

            if( object != NOOBJECT )
              {
                object_pos_info_t* obj_info;
                if( NULL != ( obj_info = find_object_in_log( object, object_id ) ) )
                  {
                    obj_info->end_time = cur_time;
                  }
                else
                {
                  ostringstream number2string;
                  number2string << object_id;
                  Error(false, "Dying object not in list: " + string(1,obj) + " "
                        + number2string.str(), "ArenaReplay::get_time_positions_in_game");
                }
              }
          }
          break;

        case 'G':
        case 'H':
          last_time_info = time_pos_index - 1;
          log_file.clear();
          log_file.seekg( old_pos );
          log_file.clear();
          return;
          break;

        default:
          Error(true, "Wrong log line found",
                "ArenaReplay::get_time_positions_in_game");
          break;

        }
      log_file.get( buffer, 400, '\n' );
    }

  log_file.clear();
  log_file.seekg( old_pos );
  log_file.clear();

  last_time_info = time_pos_index - 1;

}

// Finds the index of time_position_in_log which has the largest time
// smaller than cur_time
int
ArenaReplay::find_streampos_for_time(const float cur_time)
{
  // Can be optimized if necessary
  for(int i=1; i<max_time_infos && time_position_in_log[i].pos > -1; i++)
    if( cur_time < time_position_in_log[i].time )
      return i-1;

  Error(false, "Time not found",  "ArenaReplay::find_streampos_for_time");

  return -1;
}

double
ArenaReplay::get_length_of_current_game()
{
  if( time_position_in_log != NULL )
    if( last_time_info != 0 )
      {
        //        cout << "Game length: " << time_position_in_log[last_time_info].time << endl;
        return time_position_in_log[last_time_info].time;
      }

  //  cout << the_opts.get_d( OPTION_TIMEOUT ) + 1.0 << endl;
  return the_opts.get_d( OPTION_TIMEOUT ) + 1.0;
}

// Returns NULL if the specified object is not found.
ArenaReplay::object_pos_info_t*
ArenaReplay::find_object_in_log( object_type obj, int id )
{
  list<object_pos_info_t*>::const_iterator li;
  for( li = object_positions_in_log.begin(); li != object_positions_in_log.end(); ++li )
    {
      if( obj == (*li)->obj && id == (*li)->id )
        return *li;
    }

  return NULL;
}
