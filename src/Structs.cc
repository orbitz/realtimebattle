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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifndef NO_GRAPHICS

#include "Structs.h"
#include "ArenaController.h"
#include "ArenaRealTime.h"

pixmap_t::~pixmap_t()
{
  if( pixmap != NULL )
    {
      //      gdk_pixmap_unref(pixmap);
      //      gdk_bitmap_unref(bitmap);
      //TODO: check if window is open before unrefing
    }
}

void
pixmap_t::set_pixmap(GdkColor& col, GdkWindow* win)
{
  if( pixmap != NULL )
    {
      gdk_pixmap_unref(pixmap);
      gdk_bitmap_unref(bitmap);
    }
  
  gchar square_bits[] = {
    0xff, 0x3f, 0xff, 0x3f, 0xff, 0x3f, 0xff, 0x3f, 0xff, 0x3f, 0xff, 0x3f,
    0xff, 0x3f, 0xff, 0x3f, 0xff, 0x3f, 0xff, 0x3f, 0xff, 0x3f, 0xff, 0x3f,
    0xff, 0x3f, 0xff, 0x3f, 0xff, 0x3f, 0xff, 0x3f};

  pixmap = gdk_pixmap_create_from_data( win, square_bits, 16, 16, -1, &col, 
                                        the_gui.get_bg_gdk_colour_p() );

  bitmap = gdk_bitmap_create_from_data( win, square_bits, 16, 16 );
}

void
pixmap_t::get_pixmap(GdkColor& col, GdkWindow* win, GdkPixmap*& pixm, GdkBitmap*& bitm )
{
  if( win != window && pixmap != NULL )
    {
      gdk_pixmap_unref(pixmap);
      pixmap = NULL;
      gdk_bitmap_unref(bitmap);
      bitmap = NULL;
    }
  if( pixmap == NULL ) set_pixmap(col, win);
  
  pixm = pixmap;
  bitm = bitmap;
}

#endif
