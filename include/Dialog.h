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

#ifndef __DIALOG__
#define __DIALOG__

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef NO_GRAPHICS

#include <list>
using namespace std;

struct _GtkWidget;
typedef struct _GtkWidget GtkWidget;
union _GdkEvent;
typedef union  _GdkEvent  GdkEvent;
typedef int gint;
typedef void (*DialogFunction)(int);

class Dialog
{
public:
  struct result_t
  { 
    result_t                        ( const int r, const DialogFunction f,
                                      GtkWidget* w ) :
      result(r), function(f), widget(w) {}
    int result;
    DialogFunction function;
    GtkWidget* widget;
  };

  Dialog                            ( const string& info,
                                      const list<string>& button_labels,
                                      const DialogFunction func,
                                      const gchar* window_title = "RealTimeBattle" );
  ~Dialog                           () {}

  static void generate_result       ( GtkWidget* widget, result_t* result_p );

private:

  GtkWidget* window_p;
};

#endif 

#endif 
