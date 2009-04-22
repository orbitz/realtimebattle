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

#ifndef __SCORE_WINDOW__
#define __SCORE_WINDOW__

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef NO_GRAPHICS

struct _GtkWidget;
typedef struct _GtkWidget GtkWidget;
union _GdkEvent;
typedef union _GdkEvent GdkEvent;
struct _GdkEventButton;
typedef struct _GdkEventButton GdkEventButton;
typedef int gint;
typedef void* gpointer;

class Robot;

class ScoreWindow
{
public:
  ScoreWindow                      ( const int default_width  = -1,
                                     const int default_height = -1,
                                     const int default_x_pos  = -1,
                                     const int default_y_pos  = -1 );
  ~ScoreWindow                     ();

  void set_window_title            ();
  void add_robots                  ();
  void update_robots                  ();
  
  static gboolean hide_window          ( GtkWidget* widget, GdkEvent* event,
                                     class ScoreWindow* scorewindow_p );
  static void show_window          ( GtkWidget* widget,
                                     class ScoreWindow* scorewindow_p );
  static void new_robot_selected   ( GtkWidget* clist,
                                     gint row, gint column,
                                     GdkEventButton *event,
                                     class ScoreWindow* scorewindow_p );

  GtkWidget* get_window_p          () { return window_p; }
  GtkWidget* get_clist             () { return clist; }
  Robot* get_selected_robot        () { return selected_robot; }
  bool is_window_shown             () { return window_shown; }
  void set_selected_robot          ( Robot* rp ) { selected_robot = rp; }
  void set_window_shown            ( bool r ) { window_shown = r; }

private:

  bool window_shown;

  GtkWidget* window_p;
  GtkWidget* clist;

  Robot* selected_robot;
};

#endif 

#endif
