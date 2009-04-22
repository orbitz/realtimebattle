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

#ifndef __STATISTICS_WINDOW__
#define __STATISTICS_WINDOW__

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef NO_GRAPHICS

struct _GtkWidget;
typedef struct _GtkWidget GtkWidget;
union _GdkEvent;
typedef union _GdkEvent GdkEvent;
typedef void* gpointer;

class Robot;
struct stat_t;

class StatisticsWindow
{
public:
  enum stat_types
  {
    STAT_TYPE_TOTAL,
    STAT_TYPE_SEQUENCE,
    STAT_TYPE_GAME,
    STAT_TYPE_ROBOT
  };

  struct change_type_data_t
  {
    change_type_data_t               ( const stat_types t,
                                       class StatisticsWindow* p ) :
      type(t), sw_p(p) {}
    stat_types type;
    StatisticsWindow* sw_p;
  };

  struct change_stat_data_t
  {
    change_stat_data_t               ( const int c, const bool e,
                                       class StatisticsWindow* p ) :
      change(c), extreme_change(e), sw_p(p) {}
    int change;
    bool extreme_change;
    StatisticsWindow* sw_p;
  };

  StatisticsWindow                   ( const int default_width  = -1,
                                       const int default_height = -1,
                                       const int default_x_pos  = -1,
                                       const int default_y_pos  = -1 );
  ~StatisticsWindow                  ();

  void make_title_button             ();
  void add_new_row                   ( Robot* robot_p, stat_t average_stat,
                                       int games_played );

  static void delete_event_occured   ( GtkWidget* widget, GdkEvent* event,
                                       class StatisticsWindow* sw_p );
  static void exit                   ( GtkWidget* widget,
                                       class StatisticsWindow* sw_p );
  static void save                   ( GtkWidget* widget,
                                       class StatisticsWindow* sw_p );
  static void save_stats             ( GtkWidget* widget,
                                       class StatisticsWindow* sw_p );
  static void destroy_filesel        ( GtkWidget* widget,
                                       class StatisticsWindow* sw_p );
  static void change_table_type      ( GtkWidget* widget,
                                       struct change_type_data_t* info_p );
  static void change_stats_viewed    ( GtkWidget* widget,
                                       struct change_stat_data_t* info_p );
  static void add_the_statistics_to_clist( GtkWidget* widget,
                                           class StatisticsWindow* sw_p );
  static void change_sorting_in_clist( GtkCList *clist, gint column,
                                       class StatisticsWindow* sw_p );
  static void row_selected           ( GtkWidget* clist,
                                       gint row, gint column,
                                       GdkEventButton *event,
                                       class StatisticsWindow* sw_p );

  GtkWidget* get_window_p          () { return window_p; }
  GtkWidget* get_clist               () { return clist; }
  enum stat_types get_type           () { return type; }
  int get_looking_at_nr              () { return looking_at_nr; }
  void set_looking_at_nr              ( const int number )
    { looking_at_nr = number; }
  void set_type                      ( stat_types t ) { type = t; }

private:
  GtkWidget* get_filesel          () { return filesel; }
  void set_filesel                ( GtkWidget* fs ) { filesel = fs; } 

  GtkWidget* window_p;
  GtkWidget* clist;
  GtkWidget* title_button;
  GtkWidget* title_button_hbox;
  GtkWidget* filesel;

  stat_types type;
  int looking_at_nr;
};

#endif // NO_GRAPHICS

#endif // __STATISTICS_WINDOW__
