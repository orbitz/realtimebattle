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

#ifndef __OPTIONS_WINDOW__
#define __OPTIONS_WINDOW__

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef NO_GRAPHICS

struct _GtkWidget;
typedef struct _GtkWidget GtkWidget;
union _GdkEvent;
typedef union _GdkEvent GdkEvent;
typedef void* gpointer;
template<class T> struct option_info_t;

struct entry_t;
class Options;

class OptionsWindow
{
private:
  struct button_t
  {
    string label;
    bool used;
    GtkSignalFunc func;
    gpointer data;
  };

public:
  OptionsWindow                   ( const int default_width  = -1,
                                    const int default_height = -1,
                                    const int default_x_pos  = -1,
                                    const int default_y_pos  = -1 );
  ~OptionsWindow                  ();

  void add_option_to_notebook     ( GtkWidget* description_table,
                                    GtkWidget* entry_table,
                                    GtkWidget* button_table,
                                    int row, string description,
                                    GtkWidget* entry, string entry_text,
                                    entry_t* info, button_t* buttons );
  void update_all_gtk_entries     ();
  void set_all_options            ();

  static void default_opts        ( GtkWidget* widget,
                                    class OptionsWindow* optionswindow_p );
  static void load                ( GtkWidget* widget,
                                    class OptionsWindow* optionswindow_p );
  static void save                ( GtkWidget* widget,
                                    class OptionsWindow* optionswindow_p );
  static void save_def            ( GtkWidget* widget,
                                    class OptionsWindow* optionswindow_p );
  static void apply               ( GtkWidget* widget,
                                    class OptionsWindow* optionswindow_p );
  static void ok                  ( GtkWidget* widget,
                                    class OptionsWindow* optionswindow_p );
  static void cancel              ( GtkWidget* widget,
                                    class OptionsWindow* optionswindow_p );
  static void delete_event_occured( GtkWidget* widget, GdkEvent* event,
                                    class OptionsWindow* optionswindow_p );

  static void load_options        ( GtkWidget* widget,
                                    class OptionsWindow* optionswindow_p );
  static void save_options        ( GtkWidget* widget,
                                    class OptionsWindow* optionswindow_p );
  static void destroy_filesel     ( GtkWidget* widget,
                                    class OptionsWindow* optionswindow_p );

  static void grab_windows        ( GtkWidget* widget,
                                    class OptionsWindow* optionswindow_p );

  static void double_min          ( GtkWidget* widget,
                                    option_info_t<double>* option );
  static void double_def          ( GtkWidget* widget,
                                    option_info_t<double>* option );
  static void double_max          ( GtkWidget* widget,
                                    option_info_t<double>* option );
  static void long_min            ( GtkWidget* widget,
                                    option_info_t<long>* option );
  static void long_def            ( GtkWidget* widget,
                                    option_info_t<long>* option );
  static void long_max            ( GtkWidget* widget,
                                    option_info_t<long>* option );
  static void string_def          ( GtkWidget* widget,
                                    option_info_t<string>* option );

  GtkWidget* get_window_p         () { return window_p; }

private:
  GtkWidget* get_filesel          () { return filesel; }
  void set_filesel                ( GtkWidget* fs ) { filesel = fs; } 

  GtkWidget* window_p;
  GtkWidget* filesel;
};

#endif 

#endif
