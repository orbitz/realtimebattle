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

#ifndef __MESSAGE_WINDOW__
#define __MESSAGE_WINDOW__

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <iostream>
using namespace std;

struct _GtkWidget;
typedef struct _GtkWidget GtkWidget;
union _GdkEvent;
typedef union _GdkEvent GdkEvent;

class MessageWindow
{
public:
  MessageWindow                  ( const int default_width  = -1,
                                   const int default_height = -1,
                                   const int default_x_pos  = -1,
                                   const int default_y_pos  = -1 );
  ~MessageWindow                 ();

  void set_window_title          ();
  void add_message               ( const string& name_of_messager, 
                                   const string& message );

  static gboolean hide_window        ( GtkWidget* widget, GdkEvent* event,
                                   class MessageWindow* messagewindow_p );
  static void show_window        ( GtkWidget* widget,
                                   class MessageWindow* messagewindow_p );

  // warning Do not use the widget variable. It may be NULL.
  static void clear_clist        ( GtkWidget* widget,
                                   class MessageWindow* messagewindow_p );
  static void show_one_robot     ( GtkWidget* widget,
                                   class MessageWindow* messagewindow_p );
  static void show_all           ( GtkWidget* widget,
                                   class MessageWindow* messagewindow_p );

  void freeze_clist              ();
  void thaw_clist                ();

  void set_viewed_robot          ( class Robot* robot_p );
  void set_window_shown          ( bool win_shown );

  GtkWidget* get_window_p        () { return window_p; }
  bool is_window_shown           () { return window_shown; }
  GtkWidget* get_clist           () { return clist; }

private:

  GtkWidget* window_p;
  GtkWidget* clist;
  Robot* viewed_robot;

  GtkStyle* rtb_message_row_style;
  GtkStyle* robot_message_row_style;

  bool window_shown;
};

#endif
