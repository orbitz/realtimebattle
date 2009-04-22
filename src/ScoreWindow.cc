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

#include <gtk/gtk.h>

#include "ScoreWindow.h"
#include "IntlDefs.h"
#include "ControlWindow.h"
#include "ArenaController.h"
#include "Robot.h"
#include "Various.h"

#include <sstream>
using namespace std;

extern class ControlWindow* controlwindow_p;

//
// The constructor. The window will be set up here.
//
ScoreWindow::ScoreWindow( const int default_width,
                          const int default_height,
                          const int default_x_pos,
                          const int default_y_pos )
{
  window_p = gtk_window_new( GTK_WINDOW_TOPLEVEL );
  gtk_widget_set_name( window_p, "RTB Score" );

  set_window_title();

  gtk_container_border_width( GTK_CONTAINER( window_p ), 12 );

  if( default_width != -1 && default_height != -1 )
    {
      gtk_window_set_default_size( GTK_WINDOW( window_p ),
                                   default_width, default_height );
      gtk_widget_set_usize( window_p , 175, 80 );
    }
  if( default_x_pos != -1 && default_y_pos != -1 )
    gtk_widget_set_uposition( window_p, default_x_pos, default_y_pos );

  gtk_signal_connect( GTK_OBJECT( window_p ), "delete_event",
                      (GtkSignalFunc) ScoreWindow::hide_window,
                      (gpointer) this );

  GtkObject* hadj = gtk_adjustment_new ( 0.0, 0.0, 100.0, 1.0, 1.0, 1.0 );
  GtkObject* vadj = gtk_adjustment_new ( 0.0, 0.0, 100.0, 1.0, 1.0, 1.0 );
  GtkWidget* scrolled_win =
    gtk_scrolled_window_new( GTK_ADJUSTMENT ( hadj ),
                             GTK_ADJUSTMENT ( vadj ) );
  gtk_scrolled_window_set_policy ( GTK_SCROLLED_WINDOW ( scrolled_win ),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC );
  gtk_container_add( GTK_CONTAINER( window_p ), scrolled_win );
  gtk_widget_show ( scrolled_win );

  char * titles[6] = { "", _("Name"), _("Energy"), _("Place"),
                       _("Last"), _("Score") };
  clist = gtk_clist_new_with_titles( 6, titles );
  gtk_clist_set_selection_mode( GTK_CLIST( clist ), GTK_SELECTION_BROWSE );
  gtk_clist_set_column_width( GTK_CLIST( clist ), 0, 5 );
  gtk_clist_set_column_width( GTK_CLIST( clist ), 1, 120 );
  gtk_clist_set_column_width( GTK_CLIST( clist ), 2, 44 );
  gtk_clist_set_column_width( GTK_CLIST( clist ), 3, 38 );
  gtk_clist_set_column_width( GTK_CLIST( clist ), 4, 35 );
  gtk_clist_set_column_width( GTK_CLIST( clist ), 5, 45 );
  gtk_clist_set_column_justification( GTK_CLIST( clist ), 0,
                                      GTK_JUSTIFY_CENTER );
  gtk_clist_set_column_justification( GTK_CLIST( clist ), 1,
                                      GTK_JUSTIFY_LEFT );
  gtk_clist_set_column_justification( GTK_CLIST( clist ), 2,
                                      GTK_JUSTIFY_RIGHT );
  gtk_clist_set_column_justification( GTK_CLIST( clist ), 3,
                                      GTK_JUSTIFY_RIGHT );
  gtk_clist_set_column_justification( GTK_CLIST( clist ), 4,
                                      GTK_JUSTIFY_RIGHT );
  gtk_clist_set_column_justification( GTK_CLIST( clist ), 5,
                                      GTK_JUSTIFY_RIGHT );
  gtk_clist_column_titles_passive( GTK_CLIST( clist ) );
  gtk_signal_connect( GTK_OBJECT( clist ), "select_row",
                      (GtkSignalFunc) new_robot_selected, this );

  gtk_clist_set_column_resizeable( GTK_CLIST( clist ), 0, FALSE );
  gtk_clist_set_column_max_width( GTK_CLIST( clist ), 0, 5 );
  gtk_clist_set_shadow_type( GTK_CLIST( clist ), GTK_SHADOW_IN );
  gtk_container_add( GTK_CONTAINER( scrolled_win ), clist );

  for( int i=2; i <=5; i++ )
    gtk_clist_set_column_auto_resize( GTK_CLIST( clist ), i, TRUE );

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

  if( window_shown = ( controlwindow_p->is_scorewindow_checked() ) )
    gtk_widget_show_now( window_p );

  selected_robot = NULL;
}

//
// The destructor
//
ScoreWindow::~ScoreWindow()
{
  gtk_widget_destroy( window_p );
}

//
// Sets the window title in the form of:
// Score   Seq: 1 (2)  Game: 3 (4)  Time: 56
//
void
ScoreWindow::set_window_title()
{
  ostringstream sequence_nr_str, sequences_in_tournament_str, game_nr_str,
                games_per_sequence_str, total_time_str;
  sequence_nr_str << the_arena.get_sequence_nr();
  sequences_in_tournament_str << the_arena.get_sequences_in_tournament();
  game_nr_str << the_arena.get_game_nr();
  games_per_sequence_str << the_arena.get_games_per_sequence();
  total_time_str << (int)the_arena.get_total_time();
  string title = 
    string( _("Score") ) + "  " + 
    string( _(" Seq: ") ) + sequence_nr_str.str() + 
    " (" + sequences_in_tournament_str.str()  +

    ")  " + string( _("Game") ) + ": " + game_nr_str.str() +
    " (" + games_per_sequence_str.str() +

    ")  " + string( _("Time") ) + ": " + total_time_str.str();

  gtk_window_set_title( GTK_WINDOW( window_p ), title.c_str() );
}

//
// This function hides the window
// Warning: event can be NULL, do not use event!
//
gboolean
ScoreWindow::hide_window( GtkWidget* widget, GdkEvent* event,
                          class ScoreWindow* scorewindow_p )
{
  if( scorewindow_p->is_window_shown() )
    {
	  gtk_widget_hide( scorewindow_p->get_window_p() );
      
	  scorewindow_p->set_window_shown( false );
      if( controlwindow_p->is_scorewindow_checked() )
        {
          GtkWidget* menu_item = controlwindow_p->get_show_score_menu_item();
          gtk_check_menu_item_set_active( GTK_CHECK_MENU_ITEM( menu_item ), FALSE );
        }
    }
 return true;
}

//
// This function shows the window again
//
void
ScoreWindow::show_window( GtkWidget* widget,
                          class ScoreWindow* scorewindow_p )
{
  if( !scorewindow_p->is_window_shown() )
    {
      gtk_widget_show_now( scorewindow_p->get_window_p() );
      scorewindow_p->set_window_shown( true );
    }
}

//
// This function is called whenever the user selects a robot in
// the clist. Remebers this robot as the selected one.
//
void
ScoreWindow::new_robot_selected( GtkWidget * clist, gint row, gint column,
                                 GdkEventButton *event,
                                 class ScoreWindow* scorewindow_p )
{
  if( event == NULL ) return;

  Robot* robotp;
  
  list<Shape*>::iterator li;
  for( li = the_arena.get_object_lists()[ROBOT].begin(); li != the_arena.get_object_lists()[ROBOT].end(); ++li )
    {
      robotp = (Robot*)(*li);

      if( row == robotp->get_row_in_score_clist() )
        scorewindow_p->set_selected_robot( robotp );
    }
}

//
// This function updates all information about all robots in the sequence
//

void
ScoreWindow::update_robots()
{
  if( the_arena.get_game_nr() == 1 )
    {
      add_robots();
      return;
    }

  Robot* robot_p;
  list<Shape*>* object_lists = the_arena.get_object_lists();
  list<Shape*>::iterator li;
  
  for( li = object_lists[ROBOT].begin(); li != object_lists[ROBOT].end(); ++li )
    {
      robot_p = (Robot*)(*li);
      robot_p->reset_last_displayed();
      for( int i = 2; i <=5 ; i++ )
        gtk_clist_set_text( GTK_CLIST( clist ),
                            robot_p->get_row_in_score_clist(), i, "" );
      robot_p->display_score();
    }
}

// 
// This function resets score window and adds all robots in sequence
// to the score window. Sets the selected robot to the first one.
//
void
ScoreWindow::add_robots()
{
  gtk_clist_clear( GTK_CLIST( clist ) );
  Robot* robot_p;
  list<Shape*>* object_lists = the_arena.get_object_lists();
  list<Shape*>::iterator li;

  bool found_robot = false;

  for( li = object_lists[ROBOT].begin(); li != object_lists[ROBOT].end(); ++li )
    {
      robot_p = (Robot*)(*li);

      char* empty_list[] = { "", "", "", "", "", "" };
      int row = gtk_clist_append( GTK_CLIST( clist ), empty_list );

      gtk_clist_set_foreground( GTK_CLIST( clist ), row,
                                the_gui.get_fg_gdk_colour_p() );
      gtk_clist_set_background( GTK_CLIST( clist ), row,
                                the_gui.get_bg_gdk_colour_p() );

      robot_p->set_row_in_score_clist( row );

      GdkPixmap* colour_pixmap;
      GdkBitmap* bitmap_mask;
      robot_p->get_score_pixmap( window_p->window,
                                colour_pixmap, bitmap_mask );
      gtk_clist_set_pixmap( GTK_CLIST( clist ), row, 0,
                            colour_pixmap, bitmap_mask );
      gtk_clist_set_text( GTK_CLIST( clist ), row, 1,
                          robot_p->get_robot_name().c_str()/*.non_const_chars()*/ );
      gtk_clist_set_text( GTK_CLIST( clist ), row, 3, "" );
      robot_p->reset_last_displayed();
      robot_p->display_score();
      if( selected_robot == NULL && row == 0 )
        selected_robot = robot_p;
      if( selected_robot == robot_p )
        {
          found_robot = true;
          gtk_clist_select_row( GTK_CLIST( clist ), row, 1 );
        }
    }

  if( !found_robot )
    for( li = object_lists[ROBOT].begin(); li != object_lists[ROBOT].end(); ++li )
      {
        robot_p = (Robot*)(*li);
        if( robot_p->get_row_in_score_clist() == 0 )
          {
            selected_robot = robot_p;
            gtk_clist_select_row( GTK_CLIST( clist ), 0, 1 );
          }
      }
}
