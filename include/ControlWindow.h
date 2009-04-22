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

#ifndef __CONTROL_WINDOW__
#define __CONTROL_WINDOW__

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef NO_GRAPHICS

#include <string>
using namespace std;

struct _GtkWidget;
typedef struct _GtkWidget GtkWidget;
union _GdkEvent;
struct _GtkAdjustment;
typedef struct _GtkAdjustment GtkAdjustment;
typedef union _GdkEvent GdkEvent;
typedef void* gpointer;

struct dialog_result_t;

class ControlWindow
{
public:
  enum displayed_t { NO_WIDGETS, DEBUG_WIDGETS, REPLAY_WIDGETS };

  ControlWindow                    ( const int default_width  = -1,
                                     const int default_height = -1,
                                     const int default_x_pos  = -1,
                                     const int default_y_pos  = -1 );
  ~ControlWindow                   ();

  void set_window_title            ( const string& text );
  void remove_replay_widgets       ();
  void clear_extra_widgets         ();
  void display_debug_widgets       ();
  void display_replay_widgets      ();

  static void delete_event_occured ( GtkWidget* widget, GdkEvent* event,
                                     class ControlWindow* cw_p );
  static void quit_rtb             ( GtkWidget* widget,
                                     class ControlWindow* cw_p );
  static void pause                ( GtkWidget* widget,
                                     class ControlWindow* cw_p );
  static void step                 ( GtkWidget* widget, gpointer data );
  static void end_game             ( GtkWidget* widget, gpointer data );
  static void kill_robot           ( GtkWidget* widget, gpointer data );
  static void change_debug_level   ( GtkAdjustment *adj,
                                     class ControlWindow* cw_p );
  static void end_clicked          ( GtkWidget* widget, gpointer data ); 
  static void end_tournament       ( int result );
  static void options_clicked      ( GtkWidget* widget,
                                     class ControlWindow* cw_p );
  static void statistics_clicked   ( GtkWidget* widget,
                                     class ControlWindow* cw_p );
  static void new_tournament       ( GtkWidget* widget,
                                     class ControlWindow* cw_p );
  static void replay_tournament    ( GtkWidget* widget,
                                     class ControlWindow* cw_p );
  static void arena_window_toggle  ( GtkWidget* widget,
                                     class ControlWindow* cw_p );
  static void message_window_toggle( GtkWidget* widget,
                                     class ControlWindow* cw_p );
  static void score_window_toggle  ( GtkWidget* widget,
                                     class ControlWindow* cw_p );
  static void replay               ( GtkWidget* widget,
                                     class ControlWindow* cw_p );
  static void rewind_pressed       ( GtkWidget* widget,
                                     class ControlWindow* cw_p );
  static void rewind_released      ( GtkWidget* widget,
                                     class ControlWindow* cw_p );
  static void fast_forward_pressed ( GtkWidget* widget,
                                     class ControlWindow* cw_p );
  static void fast_forward_released( GtkWidget* widget,
                                     class ControlWindow* cw_p );
  static void step_forward         ( GtkWidget* widget,
                                     class ControlWindow* cw_p );
  static void step_backward        ( GtkWidget* widget,
                                     class ControlWindow* cw_p );
  static void next_game            ( GtkWidget* widget,
                                     class ControlWindow* cw_p );
  static void prev_game            ( GtkWidget* widget,
                                     class ControlWindow* cw_p );
  static void next_seq             ( GtkWidget* widget,
                                     class ControlWindow* cw_p );
  static void prev_seq             ( GtkWidget* widget,
                                     class ControlWindow* cw_p );
  static void dummy                ( GtkWidget* widget,
                                     class ControlWindow* cw_p ) {}
  static void change_current_replay_time( GtkAdjustment *adj,
                                          class ControlWindow* cw_p );
  static void destroy_filesel      ( GtkWidget* widget,
                                     class ControlWindow* cw_p );
  static void kill_and_open_filesel( int result );
  void open_replay_filesel         ();

  displayed_t get_displayed        () { return displayed; }
  GtkWidget* get_filesel           () { return filesel; }
  GtkWidget* get_window_p          () { return window_p; }

  bool is_arenawindow_checked      ();
  bool is_messagewindow_checked    ();
  bool is_scorewindow_checked      ();

  GtkWidget* get_show_arena_menu_item() { return show_arena_menu_item; }
  GtkWidget* get_show_message_menu_item() { return show_message_menu_item; }
  GtkWidget* get_show_score_menu_item() { return show_score_menu_item; }

  void set_filesel                 ( GtkWidget* fs ) { filesel = fs; }

  void set_progress_time           ( const double time );
  void change_time_limitations     ();

private:

  GtkWidget* window_p;
  GtkWidget* debug_level;
  GtkWidget* filesel;

  GtkWidget* window_hbox;
  GtkWidget* vseparator;
  GtkWidget* extra_vbox;

  GtkWidget* show_arena_menu_item;
  GtkWidget* show_message_menu_item;
  GtkWidget* show_score_menu_item;

  GtkWidget* time_control;
  GtkAdjustment* current_replay_time_adjustment;

  displayed_t displayed;
};

#endif 

#endif
