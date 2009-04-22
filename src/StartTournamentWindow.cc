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

#include <gtk/gtk.h>

#define HAVE_DIRENT_H 1

#if HAVE_DIRENT_H
# if HAVE_SYS_TYPES_H
#  include <sys/types.h>
# endif
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

#include "ArenaController.h"
#include "IntlDefs.h"
#include "ArenaRealTime.h"
#include "StartTournamentWindow.h"
#include "Various.h"
#include "Dialog.h"
#include "Options.h"

#include <string>
#include <sstream>
using namespace std;

const string tmp_tournament_file( "/tmp.tour" );

StartTournamentWindow::StartTournamentWindow( const int default_width,
                                              const int default_height,
                                              const int default_x_pos,
                                              const int default_y_pos )
{
  
  tournament_started_flag = false;

  filesel = NULL;

  // The window widget

  window_p = gtk_window_new( GTK_WINDOW_TOPLEVEL );
  gtk_widget_set_name( window_p, "RTB StartTournament" );

  gtk_window_set_title( GTK_WINDOW( window_p ),
                        _("Start New Tournament") );

  gtk_container_border_width( GTK_CONTAINER( window_p ), 12 );

  if( default_width != -1 && default_height != -1 )
    gtk_window_set_default_size( GTK_WINDOW( window_p ),
                                 default_width, default_height );

  if( default_x_pos != -1 && default_y_pos != -1 )
    gtk_widget_set_uposition( window_p, default_x_pos, default_y_pos );

  gtk_signal_connect( GTK_OBJECT( window_p ), "delete_event",
                      (GtkSignalFunc) StartTournamentWindow::delete_event_occured,
                      (gpointer) this );

  // Lists for clists

  list<string> robotdirs;
  list<string> arenadirs;

  read_dirs_from_system( robotdirs, arenadirs );

  // Main box

  GtkWidget* vbox = gtk_vbox_new( FALSE, 10 );
  gtk_container_add( GTK_CONTAINER( window_p ), vbox );
  gtk_widget_show( vbox );

  // Clists and buttons for selecting robots

  for( int j = 0; j < 2; j++ )
    {
      list<start_tournament_info_t*>* tour_list;
      list<start_tournament_info_t*>* dir_list;
      GtkWidget* tour_clist;
      GtkWidget* dir_clist;
      char** tour_title;
      char** dir_title;
      bool robot;

      if( j == 0 )
        {
          tour_list = &selected_robot_tournament;
          dir_list  = &selected_robot_directory;
          tour_clist = robots_in_tournament_clist;
          dir_clist  = robots_in_directory_clist;
          tour_title = new char*( _("Robots in Tournament") );
          dir_title  = new char*( _("Robots in Directory") );
          robot = true;
        }
      else
        {
          tour_list = &selected_arena_tournament;
          dir_list  = &selected_arena_directory;
          tour_clist = arenas_in_tournament_clist;
          dir_clist  = arenas_in_directory_clist;
          tour_title = new char*( _("Arenas in Tournament") );
          dir_title  = new char*( _("Arenas in Directory") );
          robot = false;

          // Make a horizontal ruler between robots and arenas
          GtkWidget* hsep = gtk_hseparator_new();
          gtk_box_pack_start( GTK_BOX( vbox ), hsep, FALSE, TRUE, 0 );
          gtk_widget_show( hsep );

        }

      GtkWidget* hbox = gtk_hbox_new( FALSE, 10 );
      gtk_container_add( GTK_CONTAINER( vbox ), hbox );
      gtk_widget_show( hbox );

      tour_clist = gtk_clist_new_with_titles( 1, tour_title );

      add_clist( tour_clist, hbox );

      char* button_labels[] = { _(" Remove "), _(" Select All "),
                                _(" Unselect All "), _(" Add "),
                                _(" Select All "), _(" Unselect All ") };

      GtkWidget* vbox2 = NULL;

      for( int i=START_TORUNAMENT_REMOVE;
           i<= START_TORUNAMENT_UNSELECT_ALL_DIRECTORY; i++ )
        {
          if( i == START_TORUNAMENT_REMOVE ||
              i == START_TORUNAMENT_ADD )
            {
              vbox2 = gtk_vbox_new( FALSE, 10 );
              gtk_box_pack_start( GTK_BOX( hbox ), vbox2, FALSE, FALSE, 0 );
              gtk_widget_show( vbox2 );
	
            }
          GtkWidget* button = gtk_button_new_with_label( button_labels[i] );
          struct select_buttons_t* s_button;
          
          s_button = new select_buttons_t( robot, i, this );
          gtk_signal_connect( GTK_OBJECT( button ), "clicked",
                              (GtkSignalFunc) StartTournamentWindow::button_selected,
                              (gpointer) s_button );
          gtk_box_pack_start( GTK_BOX( vbox2 ), button, TRUE, FALSE, 0 );
          gtk_widget_show( button );
        }

      dir_clist = gtk_clist_new_with_titles( 1, dir_title );

      add_clist( dir_clist, hbox );

      list<string>::const_iterator li;

      if( robot )
        li = robotdirs.begin();
      else
        li = arenadirs.begin();
      bool temp(robot);
      
      for( ; ; ++li )
        {
          if( (robot && (li == robotdirs.end() ) ) || (!robot && (li == arenadirs.end() ) ) )
            break;
          
          string current_dir = *li;
          DIR* dir;
          bool err_in_file = false; // set by called functions but not currently checked here
          
          if( dir = opendir(current_dir.c_str()) )
            {
              struct dirent * entry;
              while (NULL != ( entry = readdir( dir ) ) )
                {
                  string full_file_name = current_dir + entry->d_name;
                  if( ( robot && check_if_filename_is_robot( full_file_name, &err_in_file ) ) ||
                      ( !robot && check_if_filename_is_arena( full_file_name, &err_in_file ) ) )
                    {
                      char* lst[] = { entry->d_name };

                      int row = gtk_clist_append( GTK_CLIST( dir_clist ), lst );
                      gtk_clist_set_foreground( GTK_CLIST( dir_clist ), row, 
                                                the_gui.get_fg_gdk_colour_p());
                      gtk_clist_set_background( GTK_CLIST( dir_clist ), row, 
                                                the_gui.get_bg_gdk_colour_p());

                      start_tournament_info_t* info;
                      info = new start_tournament_info_t
                        ( row, false,
                          entry->d_name,
                          current_dir.c_str() );
                      dir_list->push_back( info );
                    }
                }
              closedir( dir );
            }
        }

      if( j == 0 )
        {
          robots_in_tournament_clist = tour_clist;
          robots_in_directory_clist = dir_clist;
        }
      else
        {
          arenas_in_tournament_clist = tour_clist;
          arenas_in_directory_clist = dir_clist;
          robot = false;
        }
    }

  GtkWidget* hsep = gtk_hseparator_new();
  gtk_box_pack_start( GTK_BOX( vbox ), hsep, FALSE, FALSE, 0 );
  gtk_widget_show( hsep );

  // Choose Number of games per sequence, Number of robots per sequence
  // and Number of sequences

  char * label_titles[] = { _("Games per sequence"), _("Robots per sequence"),
                            _("Number of sequences") };

  GtkWidget * hbox2 = gtk_hbox_new (FALSE, 10);
  gtk_box_pack_start (GTK_BOX (vbox), hbox2, FALSE, FALSE, 0);
  gtk_widget_show (hbox2);

  GtkWidget * internal_hbox = gtk_hbox_new( FALSE, 10 );
  gtk_box_pack_start (GTK_BOX (hbox2), internal_hbox, TRUE, FALSE, 0);
  gtk_widget_show (internal_hbox);

  GtkWidget * description_table = gtk_table_new( 3, 1, TRUE );
  GtkWidget * entry_table = gtk_table_new( 3, 1, TRUE );
  GtkWidget * button_table = gtk_table_new( 3, 12, TRUE );
  gtk_box_pack_start (GTK_BOX (internal_hbox), description_table,
                      FALSE, FALSE, 0);
  gtk_box_pack_start (GTK_BOX (internal_hbox), entry_table,
                      FALSE, FALSE, 0);
  gtk_box_pack_start (GTK_BOX (internal_hbox), button_table,
                      FALSE, FALSE, 0);

  for( int i=0;i<3;i++ )
    {
      GtkWidget * internal_hbox = gtk_hbox_new( FALSE, 10 );
      gtk_table_attach_defaults( GTK_TABLE( description_table ),
                                 internal_hbox, 0, 1, i, i + 1 );
      gtk_widget_show (internal_hbox);

      GtkWidget * label = gtk_label_new(label_titles[i]);
      gtk_box_pack_start (GTK_BOX (internal_hbox), label, FALSE, FALSE, 0);
      gtk_widget_show(label);

      entries[i] = gtk_entry_new_with_max_length(4);

      switch(i)
        {
        case 0:
        case 2:
          gtk_entry_set_text( GTK_ENTRY( entries[i] ), "1" );
          break;
        case 1:
          gtk_entry_set_text( GTK_ENTRY( entries[i] ), "2" );
          break;
        }

      entry_t * info = new entry_t( ENTRY_INT, false );

      gtk_signal_connect(GTK_OBJECT(entries[i]), "changed",
                         (GtkSignalFunc) entry_handler, info);
      gtk_table_attach_defaults( GTK_TABLE( entry_table ),
                                 entries[i], 0, 1, i, i + 1 );
      gtk_widget_set_usize(entries[i], 36,18);
      gtk_widget_show(entries[i]);

      GtkWidget * button;
      int add = 0;
      if( i == 1 ) add = 3;

      button = gtk_button_new_with_label (_(" Min "));

      min_max_full_buttons_t* mmf_button;
      mmf_button = new min_max_full_buttons_t( i, MMF_MIN, this );
      gtk_signal_connect (GTK_OBJECT (button), "clicked",
                          (GtkSignalFunc) StartTournamentWindow::set_entry,
                          (gpointer) mmf_button );
      gtk_table_attach_defaults( GTK_TABLE( button_table ), button, 0, 3 + add, i, i + 1 );
      gtk_widget_show (button);

      if( i == 0 )
        {
          button = gtk_button_new_with_label (_(" All Arenas "));
          mmf_button = new min_max_full_buttons_t( i, MMF_ALL_ARENAS,
                                                   this );
          gtk_signal_connect (GTK_OBJECT (button), "clicked",
                              (GtkSignalFunc) StartTournamentWindow::set_entry,
                              (gpointer) mmf_button );
          gtk_table_attach_defaults( GTK_TABLE( button_table ), button, 3, 9, i, i + 1 );
          gtk_widget_show (button);
        }

      if( i == 2 )
        {
          button = gtk_button_new_with_label (_(" Full Round "));
          mmf_button = new min_max_full_buttons_t( i, MMF_FULL_ROUND,
                                                   this );
          gtk_signal_connect( GTK_OBJECT( button ), "clicked",
                              (GtkSignalFunc) StartTournamentWindow::set_entry,
                              (gpointer) mmf_button );
          gtk_table_attach_defaults( GTK_TABLE( button_table ), button, 3, 9, i, i + 1 );
          gtk_widget_show( button );
        }

      button = gtk_button_new_with_label (_(" Max "));
      mmf_button = new min_max_full_buttons_t( i, MMF_MAX, this );
      gtk_signal_connect (GTK_OBJECT (button), "clicked",
                          (GtkSignalFunc) StartTournamentWindow::set_entry,
                          (gpointer) mmf_button );
      gtk_table_attach_defaults( GTK_TABLE( button_table ), button, 9 - add, 12, i, i + 1 );
      gtk_widget_show (button);
    }

  gtk_table_set_row_spacings( GTK_TABLE( description_table ) , 5 );
  gtk_table_set_col_spacings( GTK_TABLE( description_table ) , 5 );
  gtk_table_set_row_spacings( GTK_TABLE( entry_table ) , 5 );
  gtk_table_set_col_spacings( GTK_TABLE( entry_table ) , 5 );
  gtk_table_set_row_spacings( GTK_TABLE( button_table ) , 5 );
  gtk_table_set_col_spacings( GTK_TABLE( button_table ) , 5 );
  gtk_widget_show( description_table );
  gtk_widget_show( entry_table );
  gtk_widget_show( button_table );

  GtkWidget* vsep = gtk_vseparator_new();
  gtk_box_pack_start( GTK_BOX( hbox2 ), vsep, FALSE, FALSE, 0 );
  gtk_widget_show( vsep );

  GtkWidget* vbox2 = gtk_vbox_new( FALSE, 10 );
  gtk_box_pack_start( GTK_BOX(hbox2), vbox2, TRUE, FALSE, 0);
  gtk_widget_show (vbox2);

  {
    char * button_labels[] = { _(" Load tournament "), _(" Save tournament ") };
    for(int i=0;i<2;i++)
      {
        GtkWidget* button = gtk_button_new_with_label( button_labels[i] );
        if(i==0)
          gtk_signal_connect( GTK_OBJECT( button ), "clicked",
                              (GtkSignalFunc) StartTournamentWindow::load_tournament_selected,
                              (gpointer) this );          
        if(i==1)
          gtk_signal_connect( GTK_OBJECT( button ), "clicked",
                              (GtkSignalFunc) StartTournamentWindow::save_tournament_selected,
                              (gpointer) this );
        gtk_box_pack_start( GTK_BOX(vbox2), button, TRUE, FALSE, 0);
        gtk_widget_show( button );
      }
  }  

  vbox2 = gtk_vbox_new( FALSE, 10 );
  gtk_box_pack_end( GTK_BOX(hbox2), vbox2, TRUE, FALSE, 0);
  gtk_widget_show (vbox2);

  {
    char * button_labels[] = { _(" Start "), _(" Cancel ") };
    for(int i=0;i<2;i++)
      {
        GtkWidget* button = gtk_button_new_with_label( button_labels[i] );
        if(i==0)
          gtk_signal_connect( GTK_OBJECT( button ), "clicked",
                              (GtkSignalFunc) StartTournamentWindow::start,
                              (gpointer) this );          
        if(i==1)
          gtk_signal_connect( GTK_OBJECT( button ), "clicked",
                              (GtkSignalFunc) StartTournamentWindow::cancel_new_tournament,
                              (gpointer) this );
        gtk_box_pack_start( GTK_BOX(vbox2), button, TRUE, FALSE, 0);
        gtk_widget_show( button );
      }
  }  

  load_tournament_file( the_opts.get_s( OPTION_TMP_RTB_DIR ) +
                        tmp_tournament_file, false );
  gtk_widget_show( window_p );
  
}

StartTournamentWindow::~StartTournamentWindow()
{
  gtk_widget_destroy( window_p );
  list<start_tournament_info_t*>::const_iterator li;
  for(li = selected_robot_tournament.begin(); li != selected_robot_tournament.end(); ++li)
    delete *li;
  for(li = selected_robot_directory.begin(); li != selected_robot_directory.end(); ++li)
    delete *li;
  for(li = selected_arena_tournament.begin(); li != selected_arena_tournament.end(); ++li)
    delete *li;
  for(li = selected_arena_directory.begin(); li != selected_arena_directory.end(); ++li)
    delete *li;
}

void
StartTournamentWindow::add_clist( GtkWidget* clist, GtkWidget* box )
{
  int all_clists_width=150;
  int all_clists_height= 150;

  GtkObject* hadj = gtk_adjustment_new( 0.0, 0.0, 100.0, 1.0, 1.0, 1.0 );
  GtkObject* vadj = gtk_adjustment_new( 0.0, 0.0, 100.0, 1.0, 1.0, 1.0 );
  GtkWidget* scrolled_win =
    gtk_scrolled_window_new( GTK_ADJUSTMENT ( hadj ),
                             GTK_ADJUSTMENT ( vadj ) );
  gtk_scrolled_window_set_policy( GTK_SCROLLED_WINDOW( scrolled_win ),
                                  GTK_POLICY_AUTOMATIC,
                                  GTK_POLICY_AUTOMATIC );
  gtk_box_pack_start( GTK_BOX( box ), scrolled_win, TRUE, TRUE, 5 );
  gtk_widget_show( scrolled_win );

  gtk_clist_set_selection_mode( GTK_CLIST( clist ),
                                GTK_SELECTION_MULTIPLE );
  gtk_clist_set_column_width( GTK_CLIST( clist ), 0, 90 );
  gtk_clist_set_column_justification( GTK_CLIST( clist ),
                                      0, GTK_JUSTIFY_LEFT);
  gtk_clist_column_titles_passive( GTK_CLIST( clist ) );
  gtk_signal_connect( GTK_OBJECT( clist ), "select_row",
                      (GtkSignalFunc) StartTournamentWindow::selection_made, this );
  gtk_signal_connect( GTK_OBJECT( clist ), "unselect_row",
                      (GtkSignalFunc) StartTournamentWindow::selection_made, this );

  gtk_clist_set_shadow_type( GTK_CLIST( clist ),
                             GTK_SHADOW_IN );
  gtk_widget_set_usize( clist, all_clists_width,
                        all_clists_height );
  gtk_container_add( GTK_CONTAINER( scrolled_win ),
                     clist );

  GtkStyle* clist_style = gtk_rc_get_style(window_p);
  if( clist_style == NULL )
    clist_style = gtk_style_new();
  else
  clist_style = gtk_style_copy(clist_style);
  clist_style->base[GTK_STATE_NORMAL] = *(the_gui.get_bg_gdk_colour_p());
  clist_style->base[GTK_STATE_ACTIVE] = make_gdk_colour( 0xffffff );
  clist_style->bg[GTK_STATE_SELECTED] = make_gdk_colour( 0xf0d2b4 );
  clist_style->fg[GTK_STATE_SELECTED] = *(the_gui.get_fg_gdk_colour_p());
  gtk_widget_set_style( clist, clist_style );
  gtk_widget_show( clist );
}

void
StartTournamentWindow::delete_event_occured( GtkWidget* widget,
                                             GdkEvent* event,
                                             class StartTournamentWindow* stw_p )
{
  the_gui.close_starttournamentwindow();
}

void
StartTournamentWindow::cancel_new_tournament( GtkWidget* widget,
                                              class StartTournamentWindow* stw_p )
{
  the_gui.close_starttournamentwindow();
}

void
StartTournamentWindow::load_tournament_selected( GtkWidget* widget,
                                                 class StartTournamentWindow* stw_p )
{
  if( stw_p->get_filesel() != NULL )
    return;

  GtkWidget* fs = gtk_file_selection_new( _("Choose a tournament file to load") );
  gtk_signal_connect( GTK_OBJECT( fs ), "destroy",
                      (GtkSignalFunc) StartTournamentWindow::destroy_filesel,
                      (gpointer) stw_p );
  gtk_signal_connect
    ( GTK_OBJECT( GTK_FILE_SELECTION( fs )->cancel_button ), "clicked",
      (GtkSignalFunc) StartTournamentWindow::destroy_filesel,
      (gpointer) stw_p );
  gtk_signal_connect
    ( GTK_OBJECT( GTK_FILE_SELECTION( fs )->ok_button ), "clicked",
      (GtkSignalFunc) StartTournamentWindow::load_file_selected,
      (gpointer) stw_p );
  gtk_widget_show( fs );
  stw_p->set_filesel( fs );
}

void
StartTournamentWindow::load_file_selected( GtkWidget* widget,
                                           class StartTournamentWindow* stw_p )
{
  stw_p->load_tournament_file
    ( string( gtk_file_selection_get_filename
              ( GTK_FILE_SELECTION( stw_p->get_filesel() ) ) ),
      true );
  destroy_filesel( stw_p->get_filesel(), stw_p );
}

void
StartTournamentWindow::save_tournament_selected( GtkWidget* widget,
                                                 class StartTournamentWindow* stw_p )
{
  if( stw_p->get_filesel() != NULL )
    return;

  GtkWidget* fs = gtk_file_selection_new( _("Choose file to save tournament into") );
  gtk_signal_connect( GTK_OBJECT( fs ), "destroy",
                      (GtkSignalFunc) StartTournamentWindow::destroy_filesel,
                      (gpointer) stw_p );
  gtk_signal_connect
    ( GTK_OBJECT( GTK_FILE_SELECTION( fs )->cancel_button ), "clicked",
      (GtkSignalFunc) StartTournamentWindow::destroy_filesel,
      (gpointer) stw_p );
  gtk_signal_connect
    ( GTK_OBJECT( GTK_FILE_SELECTION( fs )->ok_button ), "clicked",
      (GtkSignalFunc) StartTournamentWindow::save_file_selected,
      (gpointer) stw_p );
  gtk_widget_show( fs );
  stw_p->set_filesel( fs );
}

void
StartTournamentWindow::save_file_selected( GtkWidget* widget,
                                           class StartTournamentWindow* stw_p )
{
  stw_p->save_tournament_file
    ( string( gtk_file_selection_get_filename
             ( GTK_FILE_SELECTION( stw_p->get_filesel() ) ) ),
      true, true );
  destroy_filesel( stw_p->get_filesel(), stw_p );
}

void
StartTournamentWindow::destroy_filesel( GtkWidget* widget,
                                        class StartTournamentWindow* stw_p )
{
  gtk_widget_destroy( stw_p->get_filesel() );
  stw_p->set_filesel( NULL );
}

void
StartTournamentWindow::load_tournament_file( const string& full_filename,
                                             bool display_fail_message )
{
  if(!parse_tournament_file( full_filename,
                             (StartTournamentFunction)
                             StartTournamentWindow::new_tournament_from_tournament_file,
                             this, false ) && display_fail_message )
    {
      string error_msg( _("Error in specified tournament file.") );
      list<string> button_list;
      button_list.push_back( string( _(" Ok ") ) );
      string info_text = _("Tournament could not be loaded.") + string(1, '\n')
        + error_msg;
      Dialog( info_text, button_list, 
              (DialogFunction) StartTournamentWindow::dummy_result );
    }
}

void
StartTournamentWindow::new_tournament_from_tournament_file
( const list<start_tournament_info_t*>& robotfilename_list, 
  const list<start_tournament_info_t*>& arenafilename_list, 
  const int robots_p_game, const int games_p_sequence, const int n_o_sequences,
  StartTournamentWindow* stw_p )
{
  stw_p->new_tournament( robotfilename_list, arenafilename_list, robots_p_game,
                         games_p_sequence, n_o_sequences );
}

void
StartTournamentWindow::
new_tournament( const list<start_tournament_info_t*>& robotfilename_list, 
                const list<start_tournament_info_t*>& arenafilename_list, 
                const int robots_p_game, 
                const int games_p_sequence, 
                const int n_o_sequences )
{
 try
  {
  for( int i = 0; i<2; i++ )
    {
      GtkWidget* tour_clist;
      list<start_tournament_info_t*>* tour_list;
      bool robot;
      if( i == 0 )
        {
          tour_list = &selected_robot_tournament;
          tour_clist = robots_in_tournament_clist;
          robot = true;
        }
      else
        {
          tour_list = &selected_arena_tournament;
          tour_clist = arenas_in_tournament_clist;
          robot = false;
        }

      gtk_clist_clear( GTK_CLIST( tour_clist ) );
      list<start_tournament_info_t*>::const_iterator li2;
      for(li2 = tour_list->begin(); li2 != tour_list->end(); ++li2)
        delete *li2;
      tour_list->clear();

      list<start_tournament_info_t*>::const_iterator li;
      for( robot ? li = robotfilename_list.begin() : li = arenafilename_list.begin(); (!robot || li != robotfilename_list.end()) && (robot || li != arenafilename_list.end()); ++li)
        {
          start_tournament_info_t* info = *li;

          char* lst[] = { "" };

          int row = gtk_clist_append( GTK_CLIST( tour_clist ), lst );
          gtk_clist_set_foreground( GTK_CLIST( tour_clist ), row, 
                                    the_gui.get_fg_gdk_colour_p());
          gtk_clist_set_background( GTK_CLIST( tour_clist ), row, 
                                    the_gui.get_bg_gdk_colour_p());

      
          string fname = info->filename;
          fname = fname.substr(fname.rfind( '/', fname.size() ) + 1);
      
          gtk_clist_set_text( GTK_CLIST( tour_clist ),
                              row, 0, fname.c_str() );
          info->selected = false;
          info->row = row;
          tour_list->push_back( new start_tournament_info_t
                                  ( info->row, false, info->filename, info->directory ) );
        }
    }
   }
  catch(exception& e)
    {
      Error(true, e.what(), "StartTournamentWindow::new_tournament");
    }
  ostringstream robots_p_game_str, games_p_sequence_str, n_o_sequences_str;
  robots_p_game_str << robots_p_game;
  games_p_sequence_str << games_p_sequence;
  n_o_sequences_str << n_o_sequences;
  gtk_entry_set_text( GTK_ENTRY( entries[1] ), robots_p_game_str.str().c_str() );
  gtk_entry_set_text( GTK_ENTRY( entries[0] ), games_p_sequence_str.str().c_str() );
  gtk_entry_set_text( GTK_ENTRY( entries[2] ), n_o_sequences_str.str().c_str() );
}

void
StartTournamentWindow::save_tournament_file( const string& full_filename,
                                             bool display_file_fail_message,
                                             bool display_tour_fail_message )
{
  int value[3];
  int robot_number = get_selected_robot_tournament()->size();
  istringstream string2number;

  for( int i = 0; i < 3; i++ )
    {
      int min_value;
      int max_value;

      if(i != 1)
        max_value = 10000;
      else
        {
          max_value = min_rtb( the_opts.get_l( OPTION_MAX_ROBOTS_ALLOWED ),robot_number);
        }
      if(i != 1)
        min_value = 1;
      else
        min_value = 2;

      string2number.clear();
      string2number.str(gtk_entry_get_text( GTK_ENTRY( get_entries()[i] ) ));
      int tmp;
      string2number >> tmp;
      value[i] = tmp;

      value[i] = min_rtb( max_value, value[i] );
      value[i] = max_rtb( min_value, value[i] );
    }

  ofstream file(full_filename.c_str(), ios::out);
  if( robot_number > 1 && file &&
      !( selected_arena_tournament.empty() ) )
    {
      file << "Robots: " << endl;
      list<start_tournament_info_t*>::const_iterator li;
      for( li = selected_robot_tournament.begin(); li != selected_robot_tournament.end(); ++li )
        file << (*li)->filename << " " << endl;

      file << "Arenas: " << endl;
      for( li = selected_arena_tournament.begin(); li != selected_arena_tournament.end(); ++li )
        file << (*li)->filename << " " << endl;

      file << "Robots/Sequence: " << value[1] << endl;
      file << "Games/Sequence: " << value[0] << endl;
      file << "Sequences: " << value[2] << endl;
    }
  else if( display_file_fail_message || display_tour_fail_message )
    {
      string error_msg( "" );
      if( display_tour_fail_message )
        {
          if( robot_number <= 1 )
            error_msg += string(1, '\n') + _("There are too few robots in the tournament.");
          if( selected_arena_tournament.empty() )
            error_msg += string(1, '\n') + _("There are no arenas in the tournament.");
        }
      if( display_file_fail_message && !file )
        error_msg += string(1, '\n') + _("Could not open file.");

      if( error_msg != "" )
        {
          list<string> button_list;
          button_list.push_back( string( _(" Ok ") ) );
          string info_text = string( _("Tournament could not be saved.") ) + error_msg;
          Dialog( info_text, button_list, 
                  (DialogFunction) StartTournamentWindow::dummy_result );
        }
    }
}

void
StartTournamentWindow::set_entry( GtkWidget* widget,
                                  struct min_max_full_buttons_t* mmf_p )
{
  switch( mmf_p->type )
    {
    case MMF_MIN:
      {
        if( mmf_p->entry != 1 )
          gtk_entry_set_text( GTK_ENTRY( mmf_p->stw_p->get_entries()[mmf_p->entry] ), "1" );
        else
          gtk_entry_set_text( GTK_ENTRY( mmf_p->stw_p->get_entries()[mmf_p->entry] ), "2" );
      }
      break;
    case MMF_MAX:
      {
        if( mmf_p->entry != 1 )
          gtk_entry_set_text( GTK_ENTRY( mmf_p->stw_p->get_entries()[mmf_p->entry] ), "9999" );
        else
          {
            int number_of_robots = mmf_p->stw_p->
              get_selected_robot_tournament()->size();
            if( number_of_robots > the_opts.get_l( OPTION_MAX_ROBOTS_ALLOWED ) )
              number_of_robots = the_opts.get_l( OPTION_MAX_ROBOTS_ALLOWED );
            ostringstream number_of_robots_str;
            number_of_robots_str << number_of_robots;
            gtk_entry_set_text
              ( GTK_ENTRY( mmf_p->stw_p->get_entries()[mmf_p->entry] ),
                number_of_robots_str.str().c_str() );
          }
      }
      break;
    case MMF_FULL_ROUND:
      {
        int number_of_robots = mmf_p->stw_p->
          get_selected_robot_tournament()->size();

        istringstream string2number(gtk_entry_get_text
                   ( GTK_ENTRY( mmf_p->stw_p->get_entries()[1] ) ));
        int robots_per_sequence;
        string2number >> robots_per_sequence;

        if( number_of_robots < robots_per_sequence )
          robots_per_sequence = number_of_robots;
        if( robots_per_sequence > the_opts.get_l( OPTION_MAX_ROBOTS_ALLOWED ) )
          robots_per_sequence = the_opts.get_l( OPTION_MAX_ROBOTS_ALLOWED );
        if( robots_per_sequence < 2 )
          robots_per_sequence = 2;
        ostringstream number2string;
        number2string << min ( 9999, binomial( number_of_robots,
                                          robots_per_sequence) );
        gtk_entry_set_text
          ( GTK_ENTRY( mmf_p->stw_p->get_entries()[mmf_p->entry] ),
            number2string.str().c_str() );
      }
      break;
    case MMF_ALL_ARENAS:
      {
        int number_of_arenas = mmf_p->stw_p->
          get_selected_arena_tournament()->size();

        if( number_of_arenas > the_opts.get_l( OPTION_MAX_ROBOTS_ALLOWED ) )
          number_of_arenas = the_opts.get_l( OPTION_MAX_ROBOTS_ALLOWED );
        if( number_of_arenas < 1 )
          number_of_arenas = 1;
        ostringstream number_of_arenas_str;
        number_of_arenas_str << number_of_arenas;
        gtk_entry_set_text
          ( GTK_ENTRY( mmf_p->stw_p->get_entries()[mmf_p->entry] ),
            number_of_arenas_str.str().c_str() );
      }
      break;
    }
}

void
StartTournamentWindow::start( GtkWidget* widget,
                              class StartTournamentWindow* stw_p )
{
  if( stw_p->get_tournament_started_flag() == true )
    return;

  if( the_arena_controller.is_started() )
    if( the_arena.get_state() != NOT_STARTED && the_arena.get_state() != FINISHED )
      return;

  int value[3];
  int robot_number = stw_p->get_selected_robot_tournament()->size();
  istringstream string2number;

  for( int i = 0; i < 3; i++ )
    {
      int min_value;
      int max_value;

      if(i != 1)
        max_value = 10000;
      else
        {
          max_value = min_rtb(the_opts.get_l( OPTION_MAX_ROBOTS_ALLOWED ),robot_number);
        }
      if(i != 1)
        min_value = 1;
      else
        min_value = 2;

      string2number.clear();
      string2number.str(gtk_entry_get_text( GTK_ENTRY( stw_p->get_entries()[i] ) ));
      int tmp;
      string2number >> tmp;
      value[i] = tmp;

      value[i] = min_rtb( max_value, value[i] );
      value[i] = max_rtb( min_value, value[i] );
    }
  if( robot_number > 1 &&
      !( stw_p->get_selected_arena_tournament()->empty() ) )
    {
      stw_p->set_tournament_started_flag( true );
      the_arena_controller.start_realtime_arena();
      realtime_arena.start_tournament( *( stw_p->get_selected_robot_tournament() ),
                                       *( stw_p->get_selected_arena_tournament() ),
                                       value[1], value[0], value[2] );

      // create the tmp rtb dir if it exists and save the current tournament there
      create_tmp_rtb_dir();
      stw_p->save_tournament_file( the_opts.get_s( OPTION_TMP_RTB_DIR ) +
                                   tmp_tournament_file, false, false );

      // close down StartTournamentWindow
      the_gui.close_starttournamentwindow();
    }
  else
    {
      string error_msg( "" );
      if( robot_number <= 1 )
        error_msg += string(1,'\n') + _("There must be at least two robots in the tournament.");
      if( stw_p->selected_arena_tournament.empty() )
        error_msg += string(1,'\n') + _("There are no arenas in the tournament.");

      if( error_msg != "" )
        {
          list<string> button_list;
          button_list.push_back( string( _(" Ok ") ) );
          string info_text = _("Tournament could not be started.") + error_msg;
          Dialog( info_text, button_list, 
                  (DialogFunction) StartTournamentWindow::dummy_result );
        }
    }
}

void
StartTournamentWindow::button_selected( GtkWidget* widget,
                                        struct select_buttons_t* button )
{
  switch( button->button_nr )
    {
    case START_TORUNAMENT_REMOVE:
      button->stw_p->remove_all_selected( button->robot );
      break;
    case START_TORUNAMENT_SELECT_ALL_TOURNAMENT:
      button->stw_p->change_all_selection( button->robot,false,true );
      break;
    case START_TORUNAMENT_UNSELECT_ALL_TOURNAMENT:
      button->stw_p->change_all_selection( button->robot,false,false );
      break;
    case START_TORUNAMENT_ADD:
      button->stw_p->add_all_selected( button->robot );
      break;
    case START_TORUNAMENT_SELECT_ALL_DIRECTORY:
      button->stw_p->change_all_selection( button->robot,true,true );
      break;
    case START_TORUNAMENT_UNSELECT_ALL_DIRECTORY:
      button->stw_p->change_all_selection( button->robot,true,false );
      break;
    }
}

void
StartTournamentWindow::change_all_selection( const bool robots,
                                             const bool dir,
                                             const bool all )
{
  list<start_tournament_info_t*>::const_iterator li;
  list<start_tournament_info_t*>* info_list = NULL;
  GtkWidget * clist = NULL;


  if( robots && dir )
    {
      info_list = get_selected_robot_directory();
      clist = get_robots_in_directory_clist();
    }
  else if(robots && !dir)
    {
      info_list = get_selected_robot_tournament();
      clist = get_robots_in_tournament_clist();
    }
  else if(!robots && dir)
    {
      info_list = get_selected_arena_directory();
      clist = get_arenas_in_directory_clist();
    }
  else if(!robots && !dir)
    {
      info_list = get_selected_arena_tournament();
      clist = get_arenas_in_tournament_clist();
    }
  for( li = info_list->begin(); li != info_list->end(); ++li )
    {
      start_tournament_info_t* info_p = *li;
      if( all )
        {
          info_p->selected = true;
          gtk_clist_select_row( GTK_CLIST( clist ), info_p->row, 0);
        }
      else
        {
          info_p->selected = false;
          gtk_clist_unselect_row( GTK_CLIST( clist ), info_p->row, 0);
        }
    }
}

void
StartTournamentWindow::add_all_selected( const bool robots )
{
  list<start_tournament_info_t*>::const_iterator li;
  list<start_tournament_info_t*>* info_dir_list;
  list<start_tournament_info_t*>* info_tourn_list;
  GtkWidget * clist_tourn;

  if(robots)
    {
      info_dir_list = get_selected_robot_directory();
      info_tourn_list = get_selected_robot_tournament();
      clist_tourn = get_robots_in_tournament_clist();
    }
  else
    {
      info_dir_list = get_selected_arena_directory();
      info_tourn_list = get_selected_arena_tournament();
      clist_tourn = get_arenas_in_tournament_clist();
    }

  for( li = info_dir_list->begin(); li != info_dir_list->end() ; ++li )
    {
      start_tournament_info_t* info_dir_p = *li;
      if( info_dir_p->selected )
        {
          char * list[] = { "" };
          
          int row = gtk_clist_append( GTK_CLIST( clist_tourn ), list );
          gtk_clist_set_foreground( GTK_CLIST( clist_tourn ), row,
                                    the_gui.get_fg_gdk_colour_p() );
          gtk_clist_set_background( GTK_CLIST( clist_tourn ), row,
                                    the_gui.get_bg_gdk_colour_p() );


          gtk_clist_set_text( GTK_CLIST( clist_tourn ), row, 0,
                              info_dir_p->filename.c_str()/*.non_const_chars()*/ );
      
          start_tournament_info_t* info_tourn_p;
          string full_filename;
          if(robots)
            full_filename = info_dir_p->directory + info_dir_p->filename;
          else
            full_filename = info_dir_p->directory + info_dir_p->filename;

          info_tourn_p = new start_tournament_info_t
            ( row, false, full_filename.c_str(), info_dir_p->directory );
          info_tourn_list->push_back( info_tourn_p );
        }
    }
}

void
StartTournamentWindow::remove_all_selected( const bool robots )
{
  list<start_tournament_info_t*>::iterator li;
  list<start_tournament_info_t*>::const_iterator li2;
  list<start_tournament_info_t*>* info_dir_list;
  GtkWidget * clist_tourn;

  if(robots)
    {
      info_dir_list = get_selected_robot_tournament();
      clist_tourn = get_robots_in_tournament_clist();
    }
  else
    {
      info_dir_list = get_selected_arena_tournament();
      clist_tourn = get_arenas_in_tournament_clist();
    }

  for( li = info_dir_list->begin() ; li != info_dir_list->end(); )
    {
      start_tournament_info_t* info_p = *li;

      if( info_p->selected )
        {
          gtk_clist_remove(GTK_CLIST(clist_tourn), info_p->row);
          
          for( li2 = info_dir_list->begin(); li2 != info_dir_list->end(); ++li2 )
            {
              start_tournament_info_t* info2_p = *li2;
              if(info2_p->row > info_p->row)
                info2_p->row = info2_p->row - 1;
            }
          delete *li;
          li = info_dir_list->erase(li);
        }
      else
        ++li;
    }
}

void
StartTournamentWindow::selection_made( GtkWidget* clist, gint row,
                                       gint column, GdkEventButton *event,
                                       class StartTournamentWindow* stw_p )
{
  if( event == NULL )
    return;

  list<start_tournament_info_t*>* info_list = NULL;

  if(clist == stw_p->get_robots_in_tournament_clist() )
    info_list = stw_p->get_selected_robot_tournament();
  else if(clist == stw_p->get_robots_in_directory_clist() )
    info_list = stw_p->get_selected_robot_directory();
  else if(clist == stw_p->get_arenas_in_tournament_clist() )
    info_list = stw_p->get_selected_arena_tournament();
  else if(clist == stw_p->get_arenas_in_directory_clist() )
    info_list = stw_p->get_selected_arena_directory();

  start_tournament_info_t* info_p;
  info_p = stw_p->find_row_in_clist( row, info_list );
  if( info_p->selected )
    info_p->selected = false;
  else 
    info_p->selected = true;
}

start_tournament_info_t*
StartTournamentWindow::find_row_in_clist( const int row,
                                          list<start_tournament_info_t*>* info_list )
{
  list<start_tournament_info_t*>::const_iterator li;
  for( li = info_list->begin(); li != info_list->end() ; ++li )
    {
      start_tournament_info_t* info_p = *li;
      if( info_p->row == row )
        return info_p;
    }
  return NULL;
}
