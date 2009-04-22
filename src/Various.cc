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
#include <limits.h>

#ifndef NO_GRAPHICS
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#endif

#include <sys/stat.h>
#if HAVE_DIRENT_H
# include <dirent.h>
# define NAMLEN(dirent) strlen((dirent)->d_name)
#else
# define dirent direct
# define NAMLEN(dirent) (dirent)->d_namlen
# if HAVE_SYS_NDIR_H
#  include <sys/ndir.h>
# endif
# if HAVE_SYS_DIR_H
#  include <sys/dir.h>
# endif
# if HAVE_NDIR_H
#  include <ndir.h>
# endif
#endif
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <dirent.h>
#include <algorithm>
#include <cctype>
using namespace std;

#include "Various.h"
#include "IntlDefs.h"
#include <string>
#include "Options.h"
#include "Structs.h"

extern class Options the_opts;
#ifndef NO_GRAPHICS
# include "ControlWindow.h"
extern class ControlWindow* controlwindow_p;
#endif
extern bool no_graphics;

void
Error(const bool fatal, const string& error_msg, const string& function_name)
{
// This code is disabled due to the following reasons:
// First, if a fatal error occurs, you won't see the error message at all
// Second: The error function in rtb is called many times even in cases of "warnings", a dialog distracts the user too much
// Perhaps, we can reenable it again, when point one is saved in a satisfiable manner
// Note: Making the dialog modal is not an option, because we have timers in the background that will trigger in any case

/*#ifndef NO_GRAPHICS

  string info_text = "Error in " + function_name + ":\n\n" + error_msg + "\n";
  if( fatal )
    info_text += "\nFatal Error - program will terminate!";
  list<string> string_list;
  string_list.push_back( string( _("Ok") ) );
  Dialog( info_text, string_list,
          (DialogFunction) ok_click,
          "Error" );
	  

#else
*/

  cerr << "RealTimeBattle: " << _("Error in") << " "
       << function_name << ": " << error_msg << endl;
  //  perror("RealTimeBattle: errno message");

//#endif

  if( fatal == true )
    {
      Quit(false);
    }
}

void
Quit(const bool success)
{
#ifndef NO_GRAPHICS
  if( gtk_main_level() != 0 )
  {
    gtk_main_quit();

    if( !no_graphics )
      delete controlwindow_p;
  }
#endif

  if( !success )
    exit(EXIT_FAILURE);

  exit(EXIT_SUCCESS);
}

int
factorial(const int n)
{
  double fact = 1.0;
  for(int i=1; i<=n; i++)
    fact *= i;

  if( fact < (double)INT_MAX )
    return (int)fact;
  else
    return INT_MAX;
}

int
binomial(const int n, const int k)
{
  int k2 = min_rtb(k, n-k);

  double bin = 1;
  for(int i=n; i>n-k2; i--)
    bin *= i;

  for(int i=1; i<=k2; i++)
    bin /= i;

  if( bin < (double)INT_MAX )
    return (int)bin;
  else
    return INT_MAX;
}

void
reorder_pointer_array(void** array, int size)
{
  int n1, n2;
  void* temp_p;
  for(int i=0; i<size*5; i++)
    {
      n1 = (int)floor(size*((double)rand() / (double)RAND_MAX)) ;
      n2 = (int)floor(size*((double)rand() / (double)RAND_MAX)) ;
      if( n1 != n2 )
        {
          temp_p = array[n1];
          array[n1] = array[n2];
          array[n2] = temp_p;
        }
    }
}

#ifndef NO_GRAPHICS
GdkColor
make_gdk_colour(const long col)
{
  GdkColormap *cmap;
  GdkColor colour;

  cmap = gdk_colormap_get_system();
  colour.red =   ((col & 0xff0000) >> 16 ) * 0x101;
  colour.green = ((col & 0x00ff00) >> 8  ) * 0x101;
  colour.blue =  (col & 0x0000ff) * 0x101;
  if( !gdk_color_alloc (cmap, &colour) )
    Error(true, "Couldn't allocate colour", "Various:make_gdk_color");

  return colour;
}

long
gdk2hex_colour(const GdkColor& col)
{
  return  ( (col.blue & 0xff) |
            ((col.green & 0xff) << 8) |
            ((col.red & 0xff) << 16) );
}
#endif

void
read_dirs_from_system(list<string>& robotdirs, list<string>& arenadirs)
{
  string dirs;

  robotdirs.clear();
  arenadirs.clear();

  dirs = the_opts.get_s(OPTION_ROBOT_SEARCH_PATH);
  split_colonseparated_dirs(dirs, robotdirs);

#ifdef ROBOTDIR
  string str(ROBOTDIR "/");
  robotdirs.push_back( str );
#endif

  dirs = the_opts.get_s(OPTION_ARENA_SEARCH_PATH);
  split_colonseparated_dirs(dirs, arenadirs);

#ifdef ARENADIR
  str = string(ARENADIR "/");
  arenadirs.push_back( str );
#endif
}

// This function splits a string of colonseparated directories into a glist
void
split_colonseparated_dirs(string& dirs, list<string>& str_list)
{
  string current_dir = dirs;
  int pos, lastpos = 0;
  try 
  {
    while( (pos = dirs.find(':', lastpos)) != -1 )
      {
        current_dir = string( dirs.substr(lastpos, pos-lastpos));
        if(current_dir.at(current_dir.size() - 1) != '/')
          current_dir += '/';

        string str(current_dir);
        str_list.push_back( str );

        lastpos = pos+1;
      }
  
    if(current_dir != "")
      {
        current_dir = dirs.substr(lastpos, dirs.size()-lastpos);
        if(current_dir.at(current_dir.size() - 1) != '/')
          current_dir += '/';

        string str(current_dir);
        str_list.push_back( str );
      }
  }
  catch(exception& e)
  {
    Error(true, e.what(), "split_colonseparated_dirs");
  }
}

bool
check_if_filename_is_robot(const string& fname, bool* err_in_file ) // err_in_file not currently used
{
  struct stat filestat;
  if( stat( fname.c_str(), &filestat ) != 0 )
    return false;


  // Check if file is a regular file that can be executed and ends with .robot (or .robot.exe for cygwin)
  try
    {
      if( S_ISREG( filestat.st_mode) &&
        ( filestat.st_mode & ( S_IXOTH | S_IXGRP | S_IXUSR )) &&
#ifdef __CYGWIN__
        ( string(".robot") == fname.substr(fname.size()-6, 6) ||
          string(".robot.exe") == fname.substr(fname.size()-10, 10) ) )

#else
        ( string(".robot") == fname.substr(fname.size()-6, 6) ) )
#endif
      return true;
    }
  catch(exception& e)
    {
    Error(true, e.what(), "check_if_filename_is_robot");
    }


  return false;
}

bool
check_if_filename_is_arena( string& fname, bool* err_in_file)
{
  bool retval = false;
  struct stat filestat;
  try
    {
      if( 0 == stat( fname.c_str(), &filestat ) && fname.size() > 6 )
        // Check if file is a regular file that is readable and ends with .arena
        if( S_ISREG( filestat.st_mode) &&
            ( filestat.st_mode & ( S_IROTH | S_IRGRP | S_IRUSR ) )  &&
            ( string(".arena") == fname.substr(fname.size()-6,6) ) )
        {
          // So far so good. Now do a rudimentary sanity check.
          ifstream fin( fname.c_str() );
          char text[20];
          fin.get(text, 7);
          if( 0 == strcmp(text, "scale " ) )
            retval = true;
          else
          {
            Error(false, "Error in arenafile '" + fname + "': 'scale' not first command. Skipping file.",
                  "Various::check_if_filename_is_arena");
            *err_in_file = true;
          }
          fin.close();
        }
    }
  catch(exception& e)
    {
    Error(true, e.what(), "check_if_filename_is_arena");
    }

  return retval;
}

bool
check_logfile( string& fname )
{
  struct stat filestat;
  if( 0 == stat( fname.c_str(), &filestat ) )
    // Check if file is a regular and readable file
    if( S_ISREG( filestat.st_mode) &&
        ( filestat.st_mode & ( S_IROTH | S_IRGRP | S_IRUSR ) ) )
    {
      // So far so good. Now do a rudimentary sanity check.
      ifstream fin( fname.c_str() );
      long line = 0;
      for(;;)
      {
        char buffer[200];
        fin.getline(buffer, 200);
        if( fin.eof() )
        {
          fin.close();
          return true;
        }

        line++;
        switch(buffer[0])
        {
          case 'H':
            if( 1 != line )
            {
              Error(false, "Header not first line in logfile!", "Various::check_logfile");
              return false;
            }
          case 'A':
          case 'G':
          case 'O':
          case 'L':
          case 'R':
          case 'T':
          case 'P':
          case 'C':
          case 'M':
          case 'S':
          case 'D':
            // Ok
            break;
          default:
            if( 1 == line )
              Error(false, "No header found in logfile!", "Various::check_logfile");
            else
            {
              ostringstream number2string;
              number2string << line;
              Error(false, "Unrecognized first letter in logfile line " + number2string.str() + ": " + string(1,buffer[0]),
                     "Various::check_logfile");
            }
            fin.close();
            return false;
        }
      }
    }
}

void
check_for_robots_and_arenas( string& word,
                             list<start_tournament_info_t*>& tour_list,
                             list<string>& dir_list,
                             const bool check_robots )
{
  bool found = false;
  bool err_in_file = false;
  string full_file_name = "";
  try
    {
      if( word.size() > 1 )
        if( word.substr(word.size()-2, 2 ) == "/*" )
          {
            search_directories( word.substr(0, word.size()-1), tour_list, check_robots );
            return;
          }
    }
  catch(exception& e)
    {
      Error(true, e.what(), "check_for_robots_and_arenas");
    }
  if( word.size() == 1 && word[0] == '*' )
    {

      list<string>::const_iterator li;
      for( li = dir_list.begin(); li != dir_list.end(); ++li )
        search_directories( *li, tour_list, check_robots );
      return;
    }
  if( word.find('/') != -1 )
    {
      if((check_robots && check_if_filename_is_robot( word, &err_in_file )) ||
         (!check_robots && check_if_filename_is_arena( word, &err_in_file )))
        {
          full_file_name = word;
          found = true;
        }
    }
//  if( !found && !err_in_file )
  if( !found )
    {
      list<string>::const_iterator li;
      for( li = dir_list.begin(); li != dir_list.end(); ++li )
        {
          string temp_name = *li + word;

          if((check_robots && check_if_filename_is_robot( temp_name, &err_in_file )) ||
             (!check_robots && check_if_filename_is_arena( temp_name, &err_in_file )))
            {
              full_file_name= temp_name;
              found = true;
              break;
            }
        }
    }
  if( found )
    {
      start_tournament_info_t* info;
      info = new start_tournament_info_t(0, false, full_file_name, "");
      tour_list.push_back( info );
    }
  else if(!err_in_file)
    {
      if(check_robots)
        cerr << "Couldn't find an executable robot with filename " << word << endl;
      else
        cerr << "Couldn't find an arena with filename " << word << endl;
    }
}

void
search_directories( string directory,
                    list<start_tournament_info_t*>& tour_list,
                    const bool check_robots )
{
  bool err_in_file = false;
  DIR* dir;
  if( NULL != ( dir = opendir(directory.c_str()) ) )
    {
      struct dirent* entry;
      while( NULL != ( entry = readdir( dir ) ) )
        {
          string full_file_name = directory + entry->d_name;
          bool res = false;
          if(check_robots)
            res = check_if_filename_is_robot(full_file_name, &err_in_file);
          else
            res = check_if_filename_is_arena(full_file_name, &err_in_file);
          if(res)
            {
              start_tournament_info_t* info;
              info = new start_tournament_info_t(0, false, full_file_name, "");
              tour_list.push_back( info );
            }
        }
      closedir(dir);
    }
}

bool
parse_tournament_file( const string& fname, const StartTournamentFunction function,
                       void* data, bool fatal_error_on_file_failure )
{
  list<string> robotdirs;
  list<string> arenadirs;

  read_dirs_from_system(robotdirs, arenadirs);

  ifstream file(fname.c_str());
  if( !file )
    {
      if( fatal_error_on_file_failure )
        Error( true, "Can't open specified tournament file.",
               "parse_tournament_file" );
      else
        return false;
    }

  int games_p_s = 1;
  int robots_p_s = 2;
  int n_o_sequences = 1;
  int looking_for = 0; // 0 = keyword, 1 = robot, 2 = arena
  istringstream string2number;

  list<start_tournament_info_t*> robot_list;
  list<start_tournament_info_t*> arena_list;

  for(;;)
    {
      char buffer[200];
      if ((file >> buffer).eof()){


          int robots_counted = robot_list.size();
          int arenas_counted = arena_list.size();

          if (games_p_s == -1)
            games_p_s = arenas_counted;

          if (robots_p_s == -1)
            robots_p_s = robots_counted;

          if (n_o_sequences == -1)
            n_o_sequences=binomial(robots_counted, games_p_s);

          robots_p_s = min_rtb(robots_counted,robots_p_s);

          if(robots_p_s < 2)
            {
              if( fatal_error_on_file_failure ) 
              {
                ostringstream number2string;
                number2string << robots_p_s;
                Error(true, "Can't start tournament with only " + number2string.str() +
                      " robots per sequence",
                      "parse_tournament_file");
              }
              else
                return false;
            }

          if(games_p_s < 1)
            {
              if(fatal_error_on_file_failure) 
              {
                ostringstream number2string;
                number2string << games_p_s;
                Error(true, "Must have at least one game per sequence. "
                      "Current value is: " + number2string.str(),
                      "parse_tournament_file");
              }
              else
                return false;
            }

          if(n_o_sequences < 1)
            {
              if(fatal_error_on_file_failure) 
              {
                ostringstream number2string;
                number2string << n_o_sequences;
                Error(true, "Must have at least one sequence. Current value is: " +
                      number2string.str(),
                      "parse_tournament_file");
              }
              else
                return false;
            }

          // Startup the tournament

          (*function)( robot_list, arena_list, robots_p_s,
                       games_p_s, n_o_sequences, data );

          return true;
        }

      string word(buffer);
      string word_lower;
      word_lower.resize(word.size());
      transform (word.begin(),word.end(), word_lower.begin(), (int(*)(int))tolower);

      if((word_lower == "games/sequence:") ||
         (word_lower == "g/s:"))
        {
          looking_for = 0;
          file >> buffer;
          if( buffer[0] == '*' )
            games_p_s = -1;
          else
            {
              string2number.clear();
              string2number.str(buffer);
              string2number >> games_p_s;
            }
        }
      else if((word_lower == "robots/sequence:") ||
              (word_lower == "r/s:"))
        {
          looking_for = 0;
          file >> buffer;
          if( buffer[0] == '*' )
            robots_p_s = -1;
          else
          {
            string2number.clear();
            string2number.str(buffer);
            string2number >> robots_p_s;
          }
        }
      else if((word_lower == "sequences:") ||
              (word_lower == "seq:"))
        {
          looking_for = 0;
          file >> buffer;
          if( buffer[0] == '*' )
            n_o_sequences = -1;
          else
          {
            string2number.clear();
            string2number.str(buffer);
            string2number >> n_o_sequences;
          }
        }
      else if((word_lower == "robots:") ||
              (word_lower == "r:"))
        looking_for = 1;
      else if((word_lower == "arenas:") ||
              (word_lower == "a:"))
        looking_for = 2;
      else
        {

          switch(looking_for)
            {
            case 0:
              looking_for = 0;
              cerr << "Unrecognized keyword in tournament file: " << word << endl;
              break;
            case 1:
              check_for_robots_and_arenas( word, robot_list, robotdirs, true);
              break;
            case 2:
              check_for_robots_and_arenas( word, arena_list, arenadirs, false);
              break;
            }
        }
    }
  return false;
}

void
create_tmp_rtb_dir()
{
  string dirname = the_opts.get_s( OPTION_TMP_RTB_DIR );
  struct stat filestat;
  if( 0 != stat( dirname.c_str(), &filestat ) )
    mkdir( dirname.c_str(), S_IRWXU | S_IRWXG | S_IRWXO );
}

#ifndef NO_GRAPHICS

void
entry_handler( GtkWidget * entry, entry_t * entry_info )
{
  string entry_text = gtk_entry_get_text(GTK_ENTRY(entry));
  string old_entry_text = entry_text;
  bool point_found = false;

  try {
    for(int i=0;i<entry_text.size();i++) {
      switch( entry_info->datatype )
        {
        case ENTRY_INT:
          if( !((entry_text[i] >= '0' && entry_text[i] <= '9') ||
                (entry_text[i] == '-' && i == 0 && entry_info->allow_sign ))  )
            entry_text.erase(i,1);
          break;
        case ENTRY_DOUBLE:
        /* if( !((entry_text[i] >= '0' && entry_text[i] <= '9') ||
                (entry_text[i] == '.') ||
                (entry_text[i] == '-' && i == 0 && entry_info->allow_sign ))  )
            entry_text.erase(i);
          if( entry_text[i] == '.' && !point_found )
            point_found = true;
          else if( entry_text[i] == '.' && point_found )
            entry_text.erase(i);*/
          break;
        case ENTRY_HEX:
          if( !(((entry_text[i] >= '0' && entry_text[i] <= '9') ||
                 (entry_text[i] >= 'a' && entry_text[i] <= 'f') ||
                 (entry_text[i] >= 'A' && entry_text[i] <= 'F')) ||
                (entry_text[i] == '-' && i == 0 ) && entry_info->allow_sign ) )
            entry_text.erase(i,1);
          break;
        case ENTRY_CHAR:
          break;
        case ENTRY_BOOL:
          break;
        }
    }
  }
  catch(exception& e) {
    Error(true, e.what(), "entry_handler");
  }
 
  if(old_entry_text != entry_text)
    gtk_entry_set_text(GTK_ENTRY(entry),entry_text.c_str());
}

gint
int_compare(GtkCList* clist, gconstpointer ptr1, gconstpointer ptr2)
{
  char* text1 = NULL;
  char* text2 = NULL;

  GtkCListRow* row1 = (GtkCListRow*) ptr1;
  GtkCListRow* row2 = (GtkCListRow*) ptr2;

  switch (row1->cell[clist->sort_column].type)
    {
    case GTK_CELL_TEXT:
      text1 = GTK_CELL_TEXT (row1->cell[clist->sort_column])->text;
      break;
    case GTK_CELL_PIXTEXT:
      text1 = GTK_CELL_PIXTEXT (row1->cell[clist->sort_column])->text;
      break;
    default:
      break;
    }

  switch (row2->cell[clist->sort_column].type)
    {
    case GTK_CELL_TEXT:
      text2 = GTK_CELL_TEXT (row2->cell[clist->sort_column])->text;
      break;
    case GTK_CELL_PIXTEXT:
      text2 = GTK_CELL_PIXTEXT (row2->cell[clist->sort_column])->text;
      break;
    default:
      break;
    }

  if (!text2)
    return (text1 != NULL);

  if (!text1)
    return -1;

  istringstream text1_c(text1), text2_c(text2);
  int text1_num, text2_num;
  text1_c >> text1_num;
  text2_c >> text2_num;
  return (text1_num - text2_num);
}

gint
float_compare(GtkCList *clist, gconstpointer ptr1, gconstpointer ptr2)
{
  char* text1 = NULL;
  char* text2 = NULL;

  GtkCListRow* row1 = (GtkCListRow*) ptr1;
  GtkCListRow* row2 = (GtkCListRow*) ptr2;

  switch (row1->cell[clist->sort_column].type)
    {
    case GTK_CELL_TEXT:
      text1 = GTK_CELL_TEXT (row1->cell[clist->sort_column])->text;
      break;
    case GTK_CELL_PIXTEXT:
      text1 = GTK_CELL_PIXTEXT (row1->cell[clist->sort_column])->text;
      break;
    default:
      break;
    }

  switch (row2->cell[clist->sort_column].type)
    {
    case GTK_CELL_TEXT:
      text2 = GTK_CELL_TEXT (row2->cell[clist->sort_column])->text;
      break;
    case GTK_CELL_PIXTEXT:
      text2 = GTK_CELL_PIXTEXT (row2->cell[clist->sort_column])->text;
      break;
    default:
      break;
    }

  if (!text2)
    return (text1 != NULL);

  if (!text1)
    return -1;

  istringstream text1_c(text1), text2_c(text2);
  double n1, n2;
  text1_c >> n1;
  text2_c >> n2;

  if(n1 > n2)
    return 1;
  else if(n2 > n1)
    return -1;

  return 0;
}

gint
string_case_sensitive_compare(GtkCList *clist, gconstpointer ptr1, gconstpointer ptr2)
{
  char *text1 = NULL;
  char *text2 = NULL;

  GtkCListRow *row1 = (GtkCListRow *) ptr1;
  GtkCListRow *row2 = (GtkCListRow *) ptr2;

  switch (row1->cell[clist->sort_column].type)
    {
    case GTK_CELL_TEXT:
      text1 = GTK_CELL_TEXT (row1->cell[clist->sort_column])->text;
      break;
    case GTK_CELL_PIXTEXT:
      text1 = GTK_CELL_PIXTEXT (row1->cell[clist->sort_column])->text;
      break;
    default:
      break;
    }

  switch (row2->cell[clist->sort_column].type)
    {
    case GTK_CELL_TEXT:
      text2 = GTK_CELL_TEXT (row2->cell[clist->sort_column])->text;
      break;
    case GTK_CELL_PIXTEXT:
      text2 = GTK_CELL_PIXTEXT (row2->cell[clist->sort_column])->text;
      break;
    default:
      break;
    }

  if (!text2)
    return (text1 != NULL);

  if (!text1)
    return -1;

  return strcmp (text1, text2);
}

gint
string_case_insensitive_compare(GtkCList *clist, gconstpointer ptr1, gconstpointer ptr2)
{
  char *text1 = NULL;
  char *text2 = NULL;

  GtkCListRow *row1 = (GtkCListRow *) ptr1;
  GtkCListRow *row2 = (GtkCListRow *) ptr2;

  switch (row1->cell[clist->sort_column].type)
    {
    case GTK_CELL_TEXT:
      text1 = GTK_CELL_TEXT (row1->cell[clist->sort_column])->text;
      break;
    case GTK_CELL_PIXTEXT:
      text1 = GTK_CELL_PIXTEXT (row1->cell[clist->sort_column])->text;
      break;
    default:
      break;
    }

  switch (row2->cell[clist->sort_column].type)
    {
    case GTK_CELL_TEXT:
      text2 = GTK_CELL_TEXT (row2->cell[clist->sort_column])->text;
      break;
    case GTK_CELL_PIXTEXT:
      text2 = GTK_CELL_PIXTEXT (row2->cell[clist->sort_column])->text;
      break;
    default:
      break;
    }

  if (!text2)
    return (text1 != NULL);

  if (!text1)
    return -1;

  return strcasecmp (text1, text2);
}

#endif
