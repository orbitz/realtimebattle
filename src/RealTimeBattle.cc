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

#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include <sys/types.h>
#ifdef TIME_WITH_SYS_TIME 
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

# include <sys/wait.h>


#include <errno.h>
#ifdef HAVE_GETOPT_H
# include <getopt.h>
#else
# include "getopt.h"
#endif

#ifdef HAVE_LOCALE_H
# include <locale.h>
#endif

#include <iostream>

#ifdef FLOATINGPOINT_H
#include <floatingpoint>
#endif

#ifndef NO_GRAPHICS
#include "ControlWindow.h"
#endif 

#ifndef NO_NETWORK
#include "ClientInterface.h"
#endif

#include "ArenaRealTime.h"
#include "IntlDefs.h"
#include "Options.h"
#include "ArenaReplay.h"
#include "ArenaController.h"
#include <string>
#include <sstream>
#include "Various.h"
//#include "Server.h"

#ifndef WAIT_ANY
#define WAIT_ANY (pid_t)-1
#endif

class Options the_opts;
//class ArenaRealTime the_arena;
class ArenaController the_arena_controller;

//class Server my_server;

#ifndef NO_GRAPHICS
class Gui the_gui;
class ControlWindow* controlwindow_p;
#endif

bool no_graphics;



void 
print_help_message()
{
  cout << endl;
  cout << _(" Usage: RealTimeBattle [options] ") << endl << endl;
  cout << _(" Options:") << endl;
  cout << _("    --debug_mode,                -d   debug mode") << endl;
  cout << _("    --debug_level [0-5],         -D   sets the initial debug level. Implies -d") << endl;
  cout << _("    --normal_mode,               -n   normal mode (default)") << endl;
  cout << _("    --competition_mode,          -c   competition mode") << endl ;
  cout << endl;
  cout << _("    --no_graphics,               -g   no graphics will be displayed") << endl ;
  cout << _("    --option_file [file],        -o   selects option-file (default: $HOME/.rtbrc)")  << endl;
  cout << endl;
  cout << _("    --log_file [file],           -l   make log file, if 'file' is '-'\n"
            "                                      the log is sent to STDOUT") << endl;
  cout << endl;
  cout << _("    --tournament_file [file],    -t   specify a tournament file to\n"
            "                                      autostart a tournament") << endl;
  cout << endl;
  cout << _("    --statistics_file [file],    -s   file to print the statistics to\n"
            "                                       when autostarting") << endl;
  cout << endl;
  cout << _("    --message_file [file],       -m   redirect messages to 'file'.\n"
            "                                      With '-' as 'file', STDOUT is used.\n"
            "                                      If both log and messages are sent\n"
            "                                      to STDOUT, '-m' will be ignored") << endl;
  cout << endl;
  cout << _("    --replay [file]              -r   a log file to replay.\n"
            "                                      if '-' is specified as file,\n"
            "                                      input is taken from STDIN") << endl;
  cout << endl;
  cout << _("    --help,                      -h   prints this message") << endl;
  cout << _("    --version,                   -v   prints the version number") << endl;
#ifndef NO_NETWORK
  cout << _("    --port_number                -p   specifies the port to listen on (default 32134)") << endl;
#endif
  cout << endl;
}

#ifndef NO_GRAPHICS
gint
update_function(gpointer data)
{  
  gint res = 1;

  if( the_arena_controller.is_started() )
    res = (gint)the_arena.timeout_function();

  #ifndef NO_NETWORK
    ClientInterface::Instance()->loop();
  #endif
  
  return res;
}
#else 
void
update_function(const long int interval_usec)
{
  struct timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = interval_usec;
  bool res = true;

  do
    {
      timeout.tv_sec = 0;
      timeout.tv_usec = interval_usec;
      select(FD_SETSIZE, NULL, NULL, NULL, &timeout);
      if( the_arena_controller.is_started() )
        res = the_arena.timeout_function();
	  
        #ifndef NO_NETWORK
        ClientInterface::Instance()->loop();
        #endif
    } 
  while( res );

}
#endif 

void
sig_handler (int signum)
{
  int pid;
  int status;
  while (1)
    {
      pid = waitpid (WAIT_ANY, &status, WNOHANG);
      if (pid < 0)
        {
          if( errno != ECHILD ) 
            Error(true, "waitpid failed", "RealTimeBattle.cc:sig_handler");
          break;
        }
      if (pid == 0)
        break;
    }
  //  if( signum == SIGCHLD ) cerr << "Sigchld caught!" << endl;
  //  if( signum == SIGPIPE ) cerr << "Sigpipe caught!" << endl;
  signal(signum, sig_handler);
}

void
sigfpe_handler(int signum)
{
  signal(signum, sigfpe_handler);
}

void
parse_command_line(int argc, char **argv)
{
  int version_flag=false, help_flag=false, graphics_flag=true;int port=32134;
  int c;
  istringstream string2number;

  extern char* optarg;
  extern int optind;

  static struct option long_options[] =
  {
    //option, argument?, flag, value
    {"version", 0, &version_flag, true},
    {"help", 0, &help_flag, true},

    {"debug_mode", 0, &the_arena_controller.game_mode,
     ArenaBase::DEBUG_MODE},
    {"debug_level", 1, 0, 0},
    {"normal_mode", 0, &the_arena_controller.game_mode,
     ArenaBase::NORMAL_MODE},
    {"competition_mode", 0, &the_arena_controller.game_mode,
     ArenaBase::COMPETITION_MODE},
    {"option_file", 1, 0, 0},
    {"log_file", 1, 0, 0},
    {"statistics_file", 1, 0, 0},
    {"tournament_file", 1, 0, 0},
    {"message_file", 1, 0, 0},
    {"replay", 1, 0, 0},

    {"no_graphics", 0, &graphics_flag, false},
#ifndef NO_NETWORK
    {"port_number",1,0,0},
#endif
    {0, 0, 0, 0}
  };

  for(;;)
    {
      int option_index = 0;
#ifndef NO_NETWORK 
      c = getopt_long( argc, argv, "dncD:vho:l:s:t:m:r:gp:",
                       long_options, &option_index );
#else
      c = getopt_long( argc, argv, "dncD:vho:l:s:t:m:r:g",
                       long_options, &option_index );
#endif

      // Detect the end of the options.
      if (c == -1)
        break;
     
      switch (c)
        {

        case 0:
          // If this option sets a flag, do nothing else now.
          if( long_options[option_index].flag != 0 )
            break;
          
          switch( option_index )
            {
            case 3:
              {
                string2number.clear();
                string2number.str(optarg);
                int optarg_num;
                string2number >> optarg_num;
                the_arena_controller.debug_level = optarg_num;
                the_arena_controller.game_mode = ArenaBase::DEBUG_MODE;
              }
              break;
            case 6: 
              the_arena_controller.option_filename = string(optarg);
              break;
            case 7:
              the_arena_controller.log_filename = string(optarg);
              break;
            case 8:
              the_arena_controller.statistics_filename = string(optarg);
              break;
            case 9:
              the_arena_controller.tournament_filename = string(optarg);
              the_arena_controller.auto_start_and_end = true;
              break;
            case 10:
              the_arena_controller.message_filename = string(optarg);
              break;
            case 11:
              the_arena_controller.replay_filename = string(optarg);
              break;
#ifndef NO_NETWORK
	     case 13: {
                string2number.clear();
                string2number.str(optarg);
		int optarg_num(-1);
                string2number >> optarg_num;
		if (optarg_num<1||optarg_num>65535) {
              		Error( true, "Illegal port number",
                     "RealTimeBattle.cc:parse_command_line" );
	              exit( EXIT_FAILURE );
		}
		else port=optarg_num;
		break;
	     }
#endif
            default:
              Error( true, "Bad error, this shouldn't happen",
                     "RealTimeBattle.cc:parse_command_line" );
              exit( EXIT_FAILURE );
            }
          break;


        case 'd':
          the_arena_controller.game_mode = ArenaBase::DEBUG_MODE;
          break;

        case 'D':
          {
            string2number.clear();
            string2number.str(optarg);
            int optarg_num;
            string2number >> optarg_num;
            the_arena_controller.debug_level = optarg_num;
            the_arena_controller.game_mode = ArenaBase::DEBUG_MODE;
          }
          break;

        case 'n':
          the_arena_controller.game_mode = ArenaBase::NORMAL_MODE;
          break;

        case 'c':
          the_arena_controller.game_mode = ArenaBase::COMPETITION_MODE;
          break;

        case 'v':
          version_flag = true;
          break;

        case 'h':
          help_flag = true;
          break;

        case 'o':
          the_arena_controller.option_filename = string(optarg);
          break;

        case 'l':
          the_arena_controller.log_filename = string(optarg);
          break;

#ifndef NO_NETWORK
     case 'p': {
	string2number.clear();
	string2number.str(optarg);
	int optarg_num(-1);
	string2number >> optarg_num;
	if (optarg_num<1||optarg_num>65535) {
		Error( true, "Illegal port number",
	     "RealTimeBattle.cc:parse_command_line" );
	      exit( EXIT_FAILURE );
	}
	else port=optarg_num;
	break;
     }
#endif

        case 's':
          the_arena_controller.statistics_filename = string(optarg);
          break;

        case 't':
          the_arena_controller.tournament_filename = string(optarg);
          the_arena_controller.auto_start_and_end = true;
          break;

        case 'm':
          the_arena_controller.message_filename = string(optarg);
          break;

        case 'r':
          the_arena_controller.replay_filename = string(optarg);
          break;

        case 'g':
          graphics_flag = false;
          break;

        default:
          print_help_message();
          exit( EXIT_FAILURE );
        }
    }

#ifndef NO_NETWORK
    ClientInterface::set_port(port);
#endif
  
  if( the_arena_controller.debug_level > max_debug_level )
    the_arena_controller.debug_level = 5;
  if( the_arena_controller.debug_level < 0 )
    the_arena_controller.debug_level = 0;

  if(optind != argc) 
    {
      print_help_message();
      exit( EXIT_FAILURE );
    }

  if( version_flag )
      cout << "RealTimeBattle " << VERSION << endl;

  if( help_flag )
    print_help_message();

  if( help_flag || version_flag ) exit( EXIT_SUCCESS );

  if( the_arena_controller.option_filename.empty() )
    {
      the_opts.get_options_from_rtbrc();
      the_arena_controller.option_filename = ".rtbrc";
    }
  else
    the_opts.read_options_file( the_arena_controller.option_filename,true );

 // the_arena_controller.auto_start_and_end
 // = ( ( the_arena_controller.tournament_filename != "" ) ||
 //     ( the_arena_controller.replay_filename == "-" ) );
 
  the_arena_controller.auto_start_and_end
    = ( !the_arena_controller.tournament_filename.empty() ); 


  no_graphics = !graphics_flag;

  if( the_arena_controller.auto_start_and_end == false 
#ifndef NO_GRAPHICS      
      && no_graphics
#endif      
      )
    {
      cerr << "No graphics and no tournament file gives me nothing to do. Leaving ..." << endl;
      exit( EXIT_SUCCESS );
    }
}


int 
main ( int argc, char* argv[] )
{

#ifdef FLOATINGPOINT_H
  fpsetmask ( ~ (FP_X_INV | FP_X_DZ | FP_X_IMP | FP_X_OFL | FP_X_UFL | FP_X_DNML) );
  //fpsetmask ( ~ (FP_X_INV | FP_X_DZ | FP_X_IMP) );
#endif

#ifdef HAVE_LOCALE_H
  setlocale( LC_MESSAGES, "" );
  setlocale( LC_NUMERIC, "POSIX" );
#endif

#ifdef ENABLE_NLS
  bindtextdomain ("RealTimeBattle", RTB_LOCALEDIR);
  // allow german umlauts
  bind_textdomain_codeset ("RealTimeBattle", "UTF-8");
  textdomain ("RealTimeBattle");
#endif
  
#ifndef NO_GRAPHICS
  gtk_init (&argc, &argv);
#endif 
  
  
  parse_command_line(argc, argv);

  if( !the_arena_controller.tournament_filename.empty() )
    the_arena_controller.start_realtime_arena();
  else if( !the_arena_controller.replay_filename.empty() )
    the_arena_controller.start_replay_arena();

  signal(SIGCHLD, sig_handler);
  signal(SIGPIPE, sig_handler);
  signal(SIGFPE, sigfpe_handler);

#ifndef NO_GRAPHICS
  if( !no_graphics )
    {
      the_gui.set_colours();
      controlwindow_p = 
        new ControlWindow( -1, -1,
                           the_opts.get_l( OPTION_CONTROL_WINDOW_POS_X ),
                           the_opts.get_l( OPTION_CONTROL_WINDOW_POS_Y ) );
    }


  gint timeout_tag;     

  double interval = 1000.0*the_opts.get_d( OPTION_UPDATE_INTERVAL ) - 10.0; 
  
  //updating too fast?
  if ( interval < 40 ) interval = 40;


  timeout_tag = gtk_timeout_add( (unsigned int) interval,
                                 GtkFunction(update_function), (gpointer) NULL );
 
  gtk_main();
#else 
  
  update_function( (long int)(the_opts.get_d( OPTION_UPDATE_INTERVAL ) * 1000000.0) );
  
#endif 

  return EXIT_SUCCESS;
}
