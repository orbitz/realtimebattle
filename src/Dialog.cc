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

#include "Dialog.h"
#include <string>
#include "IntlDefs.h"

//
// The value returned from Dialog is the order of button_labels,
// beginning with 1.
// If the delete_event is received, no value will be returned
//

Dialog::Dialog( const string& info, const list<string>& button_labels,
                const DialogFunction func, const gchar* window_title )
{
  // The window

  window_p = gtk_window_new( GTK_WINDOW_TOPLEVEL );
  gtk_window_set_title( GTK_WINDOW( window_p ), window_title );
  gtk_widget_set_name( window_p, "RTB Dialog" );
  gtk_window_set_policy( GTK_WINDOW( window_p ), FALSE, FALSE, FALSE );
  gtk_window_position( GTK_WINDOW( window_p ), GTK_WIN_POS_CENTER );
  gtk_container_border_width( GTK_CONTAINER( window_p ), 12 );
  gtk_signal_connect( GTK_OBJECT( window_p ), "delete_event",
                      (GtkSignalFunc) gtk_widget_destroy,
                      (gpointer) NULL );

  // Main box

  GtkWidget* vbox = gtk_vbox_new( FALSE, 10 );
  gtk_container_add( GTK_CONTAINER( window_p ), vbox );
  gtk_widget_show( vbox );

  // The info text

  GtkWidget* label = gtk_label_new( info.c_str() );
  gtk_box_pack_start( GTK_BOX( vbox ), label, TRUE, TRUE, 0 );
  gtk_widget_show( label );

  // The buttons

  GtkWidget* hbox = gtk_hbox_new( FALSE, 10 );
  gtk_box_pack_start( GTK_BOX( vbox ), hbox, TRUE, TRUE, 0 );
  gtk_widget_show( hbox );

  list<string>::const_iterator li;
  string str;
  GtkWidget* button_w;
  int list_nr = 0;
  for( li = button_labels.begin(); li != button_labels.end(); ++li )
    {
      list_nr++;
      str = *li;
      button_w = gtk_button_new_with_label( str.c_str() );
      gtk_signal_connect( GTK_OBJECT( button_w ), "clicked",
                          (GtkSignalFunc) Dialog::generate_result,
                          (gpointer) new result_t( list_nr, func, window_p ) );
      gtk_box_pack_start( GTK_BOX( hbox ), button_w, TRUE, TRUE, 0 );
      gtk_widget_show( button_w );
    }

  gtk_widget_show( window_p );
}

void
Dialog::generate_result( GtkWidget* widget, result_t* result_p )
{
  (*(result_p->function))(result_p->result);
  gtk_widget_destroy( result_p->widget );
}
