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

#ifndef __ARENA_WINDOW__
#define __ARENA_WINDOW__

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef NO_GRAPHICS

#include "Vector2D.h"

struct _GtkWidget;
typedef struct _GtkWidget GtkWidget;
union _GdkEvent;
typedef union _GdkEvent GdkEvent;
struct _GdkEventKey;
typedef struct _GdkEventKey GdkEventKey;
struct _GdkEventExpose;
typedef struct _GdkEventExpose GdkEventExpose;
struct _GdkColor;
typedef struct _GdkColor GdkColor;
typedef void* gpointer;
typedef int gint;


class ArenaWindow
{
public:
  ArenaWindow                      ( const int default_width  = -1,
                                     const int default_height = -1,
                                     const int default_x_pos  = -1,
                                     const int default_y_pos  = -1 );
  ~ArenaWindow                     ();

  void set_window_title            ();

  static gboolean hide_window          ( GtkWidget* widget, GdkEvent* event,
                                     class ArenaWindow* arenawindow_p );
  static void show_window          ( GtkWidget* widget,
                                     class ArenaWindow* arenawindow_p );
  static void no_zoom              ( GtkWidget* widget,
                                     class ArenaWindow* arenawindow_p );
  static void zoom_in              ( GtkWidget* widget,
                                     class ArenaWindow* arenawindow_p );
  static void zoom_out             ( GtkWidget* widget,
                                     class ArenaWindow* arenawindow_p );
  static gint redraw               ( GtkWidget* widget,
                                     GdkEventExpose* event,
                                     class ArenaWindow* arenawindow_p );
  static gint keyboard_handler     ( GtkWidget *widget, GdkEventKey *event,
                                     class ArenaWindow* arenawindow_p );


  inline int boundary2pixel_x      ( const double x );
  inline int boundary2pixel_y      ( const double y );


  void clear_area                  ();
  void draw_everything             ();
  void draw_moving_objects         ( const bool clear_objects_first );
  void drawing_area_scale_changed  ( const bool change_da_value = false );
  void draw_circle                 ( const Vector2D& center,
                                     const double radius,
                                     GdkColor& colour,
                                     const bool filled );
  void draw_arc                    ( const Vector2D& center, 
                                     const double inner_radius, const double outer_radius,
                                     const double angle1, const double angle2,
                                     GdkColor& colour );
  void draw_line                   ( const Vector2D& start,
                                     const Vector2D& direction,
                                     const double length, 
                                     const double thickness,
                                     GdkColor& colour );
  void draw_line                   ( const Vector2D& start,
                                     const Vector2D& direction,
                                     const double length, 
                                     GdkColor& colour );
  void draw_rectangle              ( const Vector2D& start,
                                     const Vector2D& end,
                                     GdkColor& colour,
                                     const bool filled );

  GtkWidget* get_drawing_area      () { return drawing_area; }
  GtkWidget* get_scrolled_window   () { return scrolled_window; }
  GtkWidget* get_window_p          () { return window_p; }
  bool is_window_shown             () { return window_shown; }
  void set_window_shown            ( bool win_shown );
  int get_zoom                     () { return zoom; }
  void set_zoom                    ( int z ) { zoom = z; }
  double get_drawing_area_scale    () { return drawing_area_scale; }

private:

  GtkWidget* window_p;
  GtkWidget* scrolled_window;
  GtkWidget* drawing_area;

  Vector2D scrolled_window_size;
  int zoom; // The zoom when pressed on one of the zoombuttons
  double drawing_area_scale;

  bool window_shown;
};

#include "ArenaController.h"

inline int
ArenaWindow::boundary2pixel_x( const double x )
{
  return (int)( ( x - the_arena.get_boundary()[0][0] ) * 
                drawing_area_scale + 0.5 );
}

inline int
ArenaWindow::boundary2pixel_y( const double y )
{
  return (int)( ( the_arena.get_boundary()[1][1] - y ) * 
                drawing_area_scale + 0.5 );
}

#endif 

#endif
