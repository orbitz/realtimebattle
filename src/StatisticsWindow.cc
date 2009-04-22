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

#include "StatisticsWindow.h"
#include "IntlDefs.h"
#include "Gui.h"
#include "ArenaController.h"
#include "Various.h"
#include "Robot.h"

#include <sstream>
#include <iomanip>
using namespace std;

//extern class Gui the_gui;

StatisticsWindow::StatisticsWindow( const int default_width,
                                    const int default_height,
                                    const int default_x_pos,
                                    const int default_y_pos )
{
  type = STAT_TYPE_TOTAL;
  looking_at_nr = 1;
  filesel = NULL;

  // The window widget

  window_p = gtk_window_new( GTK_WINDOW_TOPLEVEL );
  gtk_widget_set_name( window_p, "RTB Statistics" );

  gtk_window_set_title( GTK_WINDOW( window_p ), _("Statistics") );

  gtk_container_border_width( GTK_CONTAINER( window_p ), 12 );

  if( default_width != -1 && default_height != -1 )
    {
      gtk_window_set_default_size( GTK_WINDOW( window_p ),
                                   default_width, default_height );
      gtk_widget_set_usize( window_p , 364, 130 );
    }
  if( default_x_pos != -1 && default_y_pos != -1 )
    gtk_widget_set_uposition( window_p, default_x_pos, default_y_pos );

  gtk_signal_connect( GTK_OBJECT( window_p ), "delete_event",
                      (GtkSignalFunc) StatisticsWindow::delete_event_occured,
                      (gpointer) this );

  // Main box

  GtkWidget* vbox = gtk_vbox_new( FALSE, 10 );
  gtk_container_add( GTK_CONTAINER( window_p ) ,vbox );
  gtk_widget_show( vbox );

  // Buttons for displaying different types of statistics

  GtkWidget* hbox = gtk_hbox_new( FALSE, 10 );
  gtk_box_pack_start( GTK_BOX( vbox ), hbox, FALSE, FALSE, 0 );
  gtk_widget_show( hbox );

  {
    struct button_t { string label; GtkSignalFunc func; gpointer data; };

    struct button_t buttons[] = {
      { string( _(" Close ") ), (GtkSignalFunc) StatisticsWindow::exit,
        (gpointer) this },
      { string( _(" Save Statistics ") ), (GtkSignalFunc) StatisticsWindow::save,
        (gpointer) this },
      { string( _(" Total ") ), (GtkSignalFunc) StatisticsWindow::change_table_type,
        (gpointer) new change_type_data_t( STAT_TYPE_TOTAL, this ) },
      { string( _(" Sequence Total ") ), (GtkSignalFunc) StatisticsWindow::change_table_type,
        (gpointer) new change_type_data_t( STAT_TYPE_SEQUENCE, this ) },
      { string( " " ) + string( _("Game") ) + string( " " ),
        (GtkSignalFunc) StatisticsWindow::change_table_type,
        (gpointer) new change_type_data_t( STAT_TYPE_GAME, this ) },
      { string( _(" Robot ") ), (GtkSignalFunc) StatisticsWindow::change_table_type,
        (gpointer) new change_type_data_t( STAT_TYPE_ROBOT, this ) } };

    GtkWidget* button_w;
    for( int i=0; i<6; i++ )
      {
        button_w = gtk_button_new_with_label( buttons[i].label.c_str() );
        gtk_signal_connect( GTK_OBJECT( button_w ), "clicked",
                            buttons[i].func, buttons[i].data );
        gtk_box_pack_start( GTK_BOX( hbox ), button_w, TRUE, TRUE, 0 );
        gtk_widget_show( button_w );
      } 
  }

  gtk_widget_show( window_p );

  hbox = gtk_hbox_new( FALSE, 10 );
  gtk_box_pack_start( GTK_BOX( vbox ), hbox, FALSE, FALSE, 0 );
  gtk_widget_show( hbox );

  {
    char* first_xpm[13] =
    { "13 10 2 1",
      "       c None",
      "x      c #000000000000",
      "xx         xx",
      "xx       xxxx",  
      "xx     xxxxxx",
      "xx   xxxxxxxx",
      "xx  xxxxxxxxx",
      "xx  xxxxxxxxx",
      "xx   xxxxxxxx",
      "xx     xxxxxx",
      "xx       xxxx",
      "xx         xx" };
    char* prev_xpm[13] =
    { "9 10 2 1",
      "       c None",
      "x      c #000000000000",
      "       xx",
      "     xxxx",  
      "   xxxxxx",
      " xxxxxxxx",
      "xxxxxxxxx",
      "xxxxxxxxx",
      " xxxxxxxx",
      "   xxxxxx",
      "     xxxx",
      "       xx" };
    char* next_xpm[13] =
    { "9 10 2 1",
      "       c None",
      "x      c #000000000000",
      "xx       ",
      "xxxx     ",  
      "xxxxxx   ",
      "xxxxxxxx ",
      "xxxxxxxxx",
      "xxxxxxxxx",
      "xxxxxxxx ",
      "xxxxxx   ",
      "xxxx     ",
      "xx       " };
    char* last_xpm[13] =
    { "13 10 2 1",
      "       c None",
      "x      c #000000000000",
      "xx         xx",
      "xxxx       xx",  
      "xxxxxx     xx",
      "xxxxxxxx   xx",
      "xxxxxxxxx  xx",
      "xxxxxxxxx  xx",
      "xxxxxxxx   xx",
      "xxxxxx     xx",
      "xxxx       xx",
      "xx         xx" };

    struct button_t
    { char** xpm; GtkSignalFunc func; gpointer data; int pack; };

    struct button_t buttons[] = {
      { first_xpm, (GtkSignalFunc) StatisticsWindow::change_stats_viewed,
        (gpointer) new change_stat_data_t( -1, true, this ), FALSE },
      { prev_xpm, (GtkSignalFunc) StatisticsWindow::change_stats_viewed,
        (gpointer) new change_stat_data_t( -1, false, this ), FALSE },
      { NULL, (GtkSignalFunc) StatisticsWindow::add_the_statistics_to_clist,
        (gpointer) this, TRUE },
      { next_xpm, (GtkSignalFunc) StatisticsWindow::change_stats_viewed,
        (gpointer) new change_stat_data_t( 1, false, this ), FALSE },
      { last_xpm, (GtkSignalFunc) StatisticsWindow::change_stats_viewed,
        (gpointer) new change_stat_data_t( 1, true, this ), FALSE } };

    for( int i=0; i<5; i++ )
      {
        GtkWidget* button_w = gtk_button_new();
        if( buttons[i].xpm != NULL )
          {
            GdkPixmap* pixmap;
            GdkBitmap* bitmap_mask;

            pixmap = gdk_pixmap_create_from_xpm_d( window_p->window,
                                                   &bitmap_mask,
                                                   &(window_p->style->black),
                                                   buttons[i].xpm );
            GtkWidget* pixmap_widget = gtk_pixmap_new( pixmap, bitmap_mask );
            gtk_widget_show( pixmap_widget );
            gtk_container_add( GTK_CONTAINER( button_w ), pixmap_widget );
            gtk_widget_set_usize( button_w, 24, 20 );
          }
        else
          {
            title_button = button_w;
            title_button_hbox = NULL;
          }
        gtk_signal_connect( GTK_OBJECT( button_w ), "clicked",
                            buttons[i].func, buttons[i].data );
        gtk_box_pack_start( GTK_BOX( hbox ), button_w,
                            buttons[i].pack, buttons[i].pack, 0 );
        gtk_widget_show( button_w );
      }
  }

  GtkObject* hadj = gtk_adjustment_new( 0.0, 0.0, 100.0, 1.0, 1.0, 1.0 );
  GtkObject* vadj = gtk_adjustment_new( 0.0, 0.0, 100.0, 1.0, 1.0, 1.0 );
  GtkWidget* scrolled_win = gtk_scrolled_window_new( GTK_ADJUSTMENT( hadj ),
                                                     GTK_ADJUSTMENT( vadj ) );
  gtk_scrolled_window_set_policy( GTK_SCROLLED_WINDOW( scrolled_win ),
                                  GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC );
  gtk_box_pack_start( GTK_BOX( vbox ), scrolled_win, TRUE, TRUE, 0);
  gtk_widget_show( scrolled_win );

  char * titles[7] = { "",_("Name"), _("Position"), _("Points"),
                       _("Games"), _("Survival Time"), _("Total Points") };
  clist = gtk_clist_new_with_titles(7, titles );
  gtk_clist_set_selection_mode( GTK_CLIST( clist ), GTK_SELECTION_BROWSE );
  gtk_clist_set_column_width( GTK_CLIST( clist ), 0, 5 );
  gtk_clist_set_column_width( GTK_CLIST( clist ), 1, 120 );
  gtk_clist_set_column_width( GTK_CLIST( clist ), 2, 45 );
  gtk_clist_set_column_width( GTK_CLIST( clist ), 3, 35 );
  gtk_clist_set_column_width( GTK_CLIST( clist ), 4, 45 );
  gtk_clist_set_column_width( GTK_CLIST( clist ), 5, 75 );
  gtk_clist_set_column_width( GTK_CLIST( clist ), 6, 60 );
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
  gtk_clist_set_column_justification( GTK_CLIST( clist ), 6,
                                      GTK_JUSTIFY_RIGHT );
  gtk_clist_column_title_passive( GTK_CLIST( clist ), 0 );
  gtk_signal_connect( GTK_OBJECT( clist ), "select_row",
                      (GtkSignalFunc) row_selected, this );

  gtk_clist_set_column_resizeable( GTK_CLIST( clist ), 0, FALSE );
  gtk_clist_set_column_max_width( GTK_CLIST( clist ), 0, 5 );

  gtk_clist_set_shadow_type( GTK_CLIST( clist ), GTK_SHADOW_IN );
  gtk_clist_set_compare_func( GTK_CLIST( clist ), float_compare );
  gtk_clist_set_sort_column( GTK_CLIST( clist ), 6 );
  gtk_clist_set_sort_type( GTK_CLIST( clist ), GTK_SORT_DESCENDING );

  for( int i=2; i <=6; i++ )
    gtk_clist_set_column_auto_resize( GTK_CLIST( clist ), i, TRUE );

  gtk_signal_connect( GTK_OBJECT( clist ), "click_column",
                      (GtkSignalFunc) change_sorting_in_clist,
                      (gpointer) this );
  gtk_container_add( GTK_CONTAINER( scrolled_win ), clist );

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

  add_the_statistics_to_clist( clist, this );
}

StatisticsWindow::~StatisticsWindow()
{
  gtk_widget_destroy( window_p );
}

void
StatisticsWindow::delete_event_occured( GtkWidget* widget, GdkEvent* event,
                                        class StatisticsWindow* sw_p )
{
  the_gui.close_statisticswindow();
}

void
StatisticsWindow::exit( GtkWidget* widget, class StatisticsWindow* sw_p )
{
  the_gui.close_statisticswindow();
}

void
StatisticsWindow::save( GtkWidget* widget, class StatisticsWindow* sw_p )
{
  if( sw_p->get_filesel() != NULL )
    return;

  GtkWidget* fs =
    gtk_file_selection_new( _("Choose a statistics file to save") );
  gtk_signal_connect( GTK_OBJECT( fs ), "destroy",
                      (GtkSignalFunc) StatisticsWindow::destroy_filesel,
                      (gpointer) sw_p );
  gtk_signal_connect
    ( GTK_OBJECT( GTK_FILE_SELECTION( fs )->cancel_button ), "clicked",
      (GtkSignalFunc) StatisticsWindow::destroy_filesel,
      (gpointer) sw_p );
  gtk_signal_connect
    ( GTK_OBJECT( GTK_FILE_SELECTION( fs )->ok_button ), "clicked",
      (GtkSignalFunc) StatisticsWindow::save_stats,
      (gpointer) sw_p );
  gtk_widget_show( fs );
  sw_p->set_filesel( fs );
}

void
StatisticsWindow::save_stats( GtkWidget* widget,
                              class StatisticsWindow* sw_p )
{
  the_arena.save_statistics_to_file
    ( gtk_file_selection_get_filename
      ( GTK_FILE_SELECTION( sw_p->get_filesel() ) ) );
  destroy_filesel( sw_p->get_filesel(), sw_p );
}

void
StatisticsWindow::destroy_filesel( GtkWidget* widget,
                                   class StatisticsWindow* sw_p )
{
  gtk_widget_destroy( sw_p->get_filesel() );
  sw_p->set_filesel( NULL );
}

void
StatisticsWindow::change_table_type( GtkWidget* widget,
                                     struct change_type_data_t* info_p )
{
  GtkWidget* clist = info_p->sw_p->get_clist();
  stat_types sw_type = info_p->sw_p->get_type();
  int number = info_p->sw_p->get_looking_at_nr();

  gtk_clist_freeze( GTK_CLIST( clist ) );

  if( info_p->type != sw_type )
    {
      if( info_p->type == STAT_TYPE_TOTAL || info_p->type == STAT_TYPE_SEQUENCE )
        {
          gtk_clist_set_column_visibility( GTK_CLIST( clist ),4, TRUE );
        }
      else
        {
          gtk_clist_set_column_visibility ( GTK_CLIST( clist ),4,FALSE );
        }
      if(sw_type == STAT_TYPE_ROBOT && info_p->type != STAT_TYPE_ROBOT)
        {
          gtk_clist_set_column_resizeable( GTK_CLIST( clist ), 0, FALSE );
          gtk_clist_set_column_max_width( GTK_CLIST( clist ), 0, 5 );

          gtk_clist_set_column_auto_resize( GTK_CLIST( clist ), 0, FALSE );
          gtk_clist_set_column_auto_resize( GTK_CLIST( clist ), 1, FALSE );
          gtk_clist_set_column_title( GTK_CLIST( clist ), 0, "" );
          gtk_clist_set_column_title( GTK_CLIST( clist ), 1, _("Name") );
          gtk_clist_column_title_passive( GTK_CLIST( clist ), 0 );
          gtk_clist_set_column_width( GTK_CLIST( clist ), 0, 5 );
          gtk_clist_set_column_width( GTK_CLIST( clist ), 1, 120 );
          gtk_clist_set_column_justification( GTK_CLIST( clist ), 0,
                                              GTK_JUSTIFY_CENTER );
          gtk_clist_set_column_justification( GTK_CLIST( clist ), 1,
                                              GTK_JUSTIFY_LEFT );
        }
      if(sw_type != STAT_TYPE_ROBOT && info_p->type == STAT_TYPE_ROBOT)
        {
          gtk_clist_set_column_resizeable( GTK_CLIST( clist ), 0, TRUE );
          gtk_clist_set_column_max_width( GTK_CLIST( clist ), 0, 10000 );

          gtk_clist_set_column_auto_resize( GTK_CLIST( clist ), 0, TRUE );
          gtk_clist_set_column_auto_resize( GTK_CLIST( clist ), 1, TRUE );
          gtk_clist_set_column_title( GTK_CLIST( clist ), 0, _("Seq") );
          gtk_clist_set_column_title( GTK_CLIST( clist ), 1, _("Game") );
          gtk_clist_column_title_active( GTK_CLIST( clist ), 0 );
          gtk_clist_set_column_width( GTK_CLIST( clist ), 0, 40 );
          gtk_clist_set_column_width( GTK_CLIST( clist ), 1, 45 );
          gtk_clist_set_column_justification( GTK_CLIST( clist ), 0,
                                              GTK_JUSTIFY_RIGHT );
          gtk_clist_set_column_justification( GTK_CLIST( clist ), 1,
                                              GTK_JUSTIFY_RIGHT );
        }
      info_p->sw_p->set_type( info_p->type );

      switch( info_p->type )
        {
        case STAT_TYPE_TOTAL:
          change_sorting_in_clist( GTK_CLIST( clist ), 6, info_p->sw_p );
          break;
        case STAT_TYPE_SEQUENCE:
          change_sorting_in_clist( GTK_CLIST( clist ), 6, info_p->sw_p );
          number = the_arena.get_sequence_nr();
          break;
        case STAT_TYPE_GAME:
          change_sorting_in_clist( GTK_CLIST( clist ), 3, info_p->sw_p );
          number = ( the_arena.get_sequence_nr() - 1 ) *
            the_arena.get_games_per_sequence() + the_arena.get_game_nr();
          break;
        case STAT_TYPE_ROBOT:
          change_sorting_in_clist( GTK_CLIST( clist ), 0, info_p->sw_p );
          number = 1;
          break;
        }
      info_p->sw_p->set_looking_at_nr( number );
    }

  add_the_statistics_to_clist( clist, info_p->sw_p );
  gtk_clist_thaw( GTK_CLIST( clist ) );
}

void
StatisticsWindow::change_stats_viewed( GtkWidget* widget,
                                       struct change_stat_data_t* info_p )
{
  GtkWidget* clist = info_p->sw_p->get_clist();
  stat_types sw_type = info_p->sw_p->get_type();
  int number = info_p->sw_p->get_looking_at_nr();

  int game = the_arena.get_game_nr();
  int max_nr = -1;

  switch( sw_type )
    {
    case STAT_TYPE_TOTAL:
      max_nr = 1;
      break;
    case STAT_TYPE_SEQUENCE:
      max_nr = the_arena.get_sequence_nr();
      break;
    case STAT_TYPE_GAME:
      max_nr = ( the_arena.get_sequence_nr() - 1 ) *
        the_arena.get_games_per_sequence() + game;
      break;
    case STAT_TYPE_ROBOT:
      max_nr = the_arena.get_all_robots_in_tournament()->size();
      break;
    }

  if( info_p->extreme_change && max_nr > 0 )
    {
      if( info_p->change < 0 )
        number = abs_rtb( info_p->change );
      else if( info_p->change > 0 )
        number = max_rtb( max_nr - info_p->change + 1, 1 );
    }
  else
    {
      number += info_p->change;
      number = min_rtb( number, max_nr );
      number = max_rtb( number, 1 );
    }

  info_p->sw_p->set_looking_at_nr( number );
  add_the_statistics_to_clist( clist, info_p->sw_p );
}

void
StatisticsWindow::make_title_button()
{
  if( title_button_hbox != NULL )
    gtk_widget_destroy( title_button_hbox );
  title_button_hbox = gtk_hbox_new( FALSE, 10 );
  gtk_container_add( GTK_CONTAINER( title_button ), title_button_hbox );
  gtk_widget_show( title_button_hbox );

  string title;
  switch( type )
    {
    case STAT_TYPE_TOTAL:
      title = _(" Grand Total ");
      break;
    case STAT_TYPE_SEQUENCE:
      {
        ostringstream number2string;
        number2string << looking_at_nr;
        title = string( _(" Sequence ") ) + number2string.str();
      }
      break;
    case STAT_TYPE_GAME:
      {
        int gps = the_arena.get_games_per_sequence();
        int game = looking_at_nr % gps;
        int sequence = (looking_at_nr / gps ) + 1;
        if( game == 0 )
          {
            game = gps;
            sequence--;
          }
        ostringstream sequence_str, game_str;
        sequence_str << sequence;
        game_str << game;
        title = string( _(" Sequence: ") ) + sequence_str.str() + "  " +
          _("Game") + ": " + game_str.str();
      }
      break;
    case STAT_TYPE_ROBOT:
      {
        Robot* robot_p;

        int i=0;
        list<Robot*>::iterator li;
        for( li = the_arena.get_all_robots_in_tournament()->begin(); li != the_arena.get_all_robots_in_tournament()->end(); ++li )
          {
            i++;
            robot_p = *li;
            if( looking_at_nr == i )
              {
                GdkPixmap* col_pixmap;
                GdkBitmap* bitmap_mask;
                robot_p->get_stat_pixmap(window_p->window, col_pixmap, bitmap_mask);
                GtkWidget* pixmap_w = gtk_pixmap_new( col_pixmap, bitmap_mask );
                gtk_box_pack_start( GTK_BOX( title_button_hbox ),
                                    pixmap_w, FALSE, FALSE, 0 );
                gtk_widget_show( pixmap_w );
                title = robot_p->get_robot_name();
              }
          }
      }
      break;
    }

  GtkWidget* label = gtk_label_new( title.c_str() );
  gtk_box_pack_start( GTK_BOX( title_button_hbox ), label, TRUE, TRUE, 0 );
  gtk_widget_show( label );
}

void
StatisticsWindow::add_new_row( Robot* robot_p, stat_t average_stat,
                               int games_played )
{
  char* empty_list[] = { "", "", "", "", "", "", "", "" };
  int row = gtk_clist_append( GTK_CLIST( clist ), empty_list );
  gtk_clist_set_foreground( GTK_CLIST( clist ), row,
                            the_gui.get_fg_gdk_colour_p() );
  gtk_clist_set_background( GTK_CLIST( clist ), row,
                            the_gui.get_bg_gdk_colour_p() );

  if( type == STAT_TYPE_GAME ||
      type == STAT_TYPE_SEQUENCE ||
      type == STAT_TYPE_TOTAL)
    {
      GdkPixmap * colour_pixmap;
      GdkBitmap * bitmap_mask;

      robot_p->get_stat_pixmap( window_p->window,
                                colour_pixmap, bitmap_mask );

      gtk_clist_set_pixmap( GTK_CLIST( clist ), row, 0,
                            colour_pixmap, bitmap_mask );
      gtk_clist_set_text( GTK_CLIST( clist ), row, 1,
                          robot_p->get_robot_name().c_str()/*non_const_chars()*/ );
    }

  if( type == STAT_TYPE_ROBOT)
    {
      ostringstream sequence_nr_str, game_nr_str;
      sequence_nr_str << average_stat.sequence_nr;
      game_nr_str << average_stat.game_nr;
      gtk_clist_set_text( GTK_CLIST( clist ), row, 0,
                          sequence_nr_str.str().c_str() );
      gtk_clist_set_text( GTK_CLIST( clist ), row, 1,
                          game_nr_str.str().c_str() );
    }

  ostringstream position_str;
  position_str << average_stat.position;
  gtk_clist_set_text( GTK_CLIST( clist ), row, 2,
                      position_str.str().c_str() );

  ostringstream points_str;
  if( type == STAT_TYPE_SEQUENCE ||
      type == STAT_TYPE_TOTAL)
    points_str << setiosflags(ios::fixed) << setprecision(2) << average_stat.points;
  else
    points_str << average_stat.points;

  gtk_clist_set_text( GTK_CLIST( clist ), row, 3, points_str.str().c_str() );

  if( type == STAT_TYPE_TOTAL ||
      type == STAT_TYPE_SEQUENCE )
  {
      ostringstream games_played_str;
      games_played_str << games_played;
      gtk_clist_set_text( GTK_CLIST( clist ), row, 4,
                          games_played_str.str().c_str() );
  }
  else
      gtk_clist_set_text( GTK_CLIST( clist ), row, 4,
                          string( "" ).c_str() );

  ostringstream time_survived_str;
  time_survived_str << setiosflags(ios::fixed) << setprecision(2) << average_stat.time_survived;
  gtk_clist_set_text( GTK_CLIST( clist ), row, 5, time_survived_str.str().c_str() );

  ostringstream total_points_str;
  total_points_str << average_stat.total_points;
  gtk_clist_set_text( GTK_CLIST( clist ), row, 6,
                      total_points_str.str().c_str() );
}

void
StatisticsWindow::add_the_statistics_to_clist( GtkWidget* widget,
                                               class StatisticsWindow* sw_p )
{
  Robot* robot_p = NULL;
  stat_t* stat_p = NULL;
  list<Robot*>::iterator li;
  list<stat_t*>::const_iterator stat_li;

  GtkWidget* clist = sw_p->get_clist();
  stat_types sw_type = sw_p->get_type();
  int number = sw_p->get_looking_at_nr();

  sw_p->make_title_button();

  gtk_clist_freeze( GTK_CLIST( clist ) );
  gtk_clist_clear( GTK_CLIST( clist ) );


  switch( sw_type )
    {
    case STAT_TYPE_TOTAL:
    case STAT_TYPE_SEQUENCE:
      {
        int number_of_robots = the_arena.get_number_of_robots();

        double points[number_of_robots];
        int position[number_of_robots];

        int robot_nr = -1;

        for( li = the_arena.get_all_robots_in_tournament()->begin(); li != the_arena.get_all_robots_in_tournament()->end(); ++li )
          {
            robot_nr++;
            robot_p = *li;
            points[robot_nr] = 0;
            for(stat_li = robot_p->get_statistics()->begin();
                stat_li != robot_p->get_statistics()->end(); ++stat_li)
              {
                stat_p = *stat_li;
                if( ( sw_type == STAT_TYPE_SEQUENCE &&
                      stat_p->sequence_nr == number ) ||
                    sw_type == STAT_TYPE_TOTAL )
                  points[robot_nr] += (*stat_li)->points;
              }
          }

        for(int i = 0;i<number_of_robots;i++)
          {
            int temp_pos = 1;
            for(int j = 0;j<number_of_robots;j++)
              if(points[j] > points[i])
                temp_pos++;
            position[i] = temp_pos;
          }

        robot_nr = -1;
        for( li = the_arena.get_all_robots_in_tournament()->begin(); li != the_arena.get_all_robots_in_tournament()->end(); ++li )
          {
            robot_nr++;
            robot_p = *li;
            stat_t average_stat(0,0,0,0.0,0.0,0.0);
            int number_of_stat_found = 0;
            for(stat_li = robot_p->get_statistics()->begin();
                stat_li != robot_p->get_statistics()->end(); ++stat_li)
              {
                stat_p = *stat_li;
                if( ( sw_type == STAT_TYPE_SEQUENCE &&
                      stat_p->sequence_nr == number ) ||
                    sw_type == STAT_TYPE_TOTAL )
                  {
                    number_of_stat_found++;
                    average_stat.points += stat_p->points;
                    average_stat.time_survived += stat_p->time_survived;
                    average_stat.total_points += stat_p->points;
                  }
              }

            if( number_of_stat_found > 0 )
              {
                average_stat.position = position[robot_nr];
                average_stat.points /= number_of_stat_found;
                average_stat.time_survived /= number_of_stat_found;
                sw_p->add_new_row( robot_p, average_stat, number_of_stat_found );
              }
          }
      }
      break;
    case STAT_TYPE_GAME:
      {
        int gps = the_arena.get_games_per_sequence();
        int game = number % gps;
        int sequence = ( number / gps ) + 1;
        if( game == 0 )
          {
            game = gps;
            sequence--;
          }

        for( li = the_arena.get_all_robots_in_tournament()->begin(); li != the_arena.get_all_robots_in_tournament()->end(); ++li )
          {
            robot_p = *li;

            for(stat_li = robot_p->get_statistics()->begin();
                stat_li != robot_p->get_statistics()->end(); ++stat_li)
              {
                stat_p = *stat_li;
                if(stat_p->sequence_nr == sequence &&
                   stat_p->game_nr == game)
                  sw_p->add_new_row( robot_p, *stat_p, -1 );
              }
          }
      }
      break;
    case STAT_TYPE_ROBOT:
      {
        int i=0;

        for( li = the_arena.get_all_robots_in_tournament()->begin(); li != the_arena.get_all_robots_in_tournament()->end(); ++li )
          {
            i++;
            robot_p = *li;
            if( i == number )
              for(stat_li = robot_p->get_statistics()->begin();
                stat_li != robot_p->get_statistics()->end(); ++stat_li)
                {
                  stat_p = *stat_li;
                  sw_p->add_new_row( robot_p, *stat_p, -1 );
                }
          }
      }
      break;
    }

  gtk_clist_sort( GTK_CLIST( clist ) );
  gtk_clist_thaw( GTK_CLIST( clist ) );
}

void
StatisticsWindow::change_sorting_in_clist( GtkCList* clist, gint column,
                                           class StatisticsWindow* sw_p )
{
  stat_types sw_type = sw_p->get_type();
  
  gtk_clist_freeze( clist );
  switch(column)
    {
    case 0:
      if( sw_type == STAT_TYPE_ROBOT )
        {
          gtk_clist_set_compare_func( clist, float_compare );
          gtk_clist_set_sort_column( clist, column );
          gtk_clist_set_sort_type( clist, GTK_SORT_ASCENDING );
          gtk_clist_sort( clist );
        }
      break;
    case 1:
      if( sw_type != STAT_TYPE_ROBOT )
        {
          gtk_clist_set_compare_func( clist, string_case_insensitive_compare );
          gtk_clist_set_sort_column( clist, column );
          gtk_clist_set_sort_type( clist, GTK_SORT_ASCENDING );
          gtk_clist_sort( clist );
        }
      else
        {
          gtk_clist_set_compare_func( clist, float_compare );
          gtk_clist_set_sort_column( clist, column );
          gtk_clist_set_sort_type( clist, GTK_SORT_ASCENDING );
          gtk_clist_sort( clist );
        }
      break;
    case 2:
      gtk_clist_set_compare_func( clist, float_compare );
      gtk_clist_set_sort_column( clist, column );
      gtk_clist_set_sort_type( clist, GTK_SORT_ASCENDING );
      gtk_clist_sort( clist );
      break;
    case 3:
    case 5:
    case 6:
      gtk_clist_set_compare_func( clist, float_compare );
      gtk_clist_set_sort_column( clist, column );
      gtk_clist_set_sort_type( clist, GTK_SORT_DESCENDING );
      gtk_clist_sort( clist );
      break;
    case 4:
      if( sw_type == STAT_TYPE_TOTAL || sw_type == STAT_TYPE_SEQUENCE )
        {
          gtk_clist_set_compare_func( clist, float_compare );
          gtk_clist_set_sort_column( clist, column );
          gtk_clist_set_sort_type( clist, GTK_SORT_ASCENDING );
          gtk_clist_sort( clist );
        }
      break;
    }
  add_the_statistics_to_clist( GTK_WIDGET( clist ), sw_p );
  gtk_clist_thaw( clist );
}

void
StatisticsWindow::row_selected( GtkWidget* clist, gint row,
                                gint column, GdkEventButton *event,
                                class StatisticsWindow* sw_p )
{
  stat_types sw_type = sw_p->get_type();

  if( column == 1 && sw_type != STAT_TYPE_ROBOT )
    {
      gtk_clist_freeze( GTK_CLIST( clist ) );

      gchar* clist_text;

      gtk_clist_get_text( GTK_CLIST( clist ), row, column, &clist_text );
      string robot_name;
      if( clist_text != NULL )
        robot_name = clist_text;

      list<Robot*>::iterator li;
      Robot* robot_p = NULL;
      int counter = 0;
      bool found_robot = false;
      for( li = the_arena.get_all_robots_in_tournament()->begin(); 
        (li != the_arena.get_all_robots_in_tournament()->end()) && (!found_robot); ++li )
        {
          counter--;
          robot_p = *li;
          if( robot_p->get_robot_name() == robot_name )
            {
              found_robot = true;
              change_type_data_t type_data( STAT_TYPE_ROBOT, sw_p );
              change_table_type( clist, &type_data );

              change_stat_data_t stat_data( counter, true, sw_p );
              change_stats_viewed( clist, &stat_data );
            }
        }
      gtk_clist_thaw( GTK_CLIST( clist ) );
    }
}
