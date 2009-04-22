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

#include <math.h>
#include <iostream>

using namespace std;

//#include "Gui.h"
#include "Shape.h"
#include "Various.h"
#include "Options.h"

extern class Options the_opts;

Shape::Shape()
{
  set_colour(the_opts.get_l(OPTION_FOREGROUND_COLOUR));
}

Shape::Shape(int long colour)
{
  set_colour(colour);
}

void
Shape::set_colour(long int colour)
{
  rgb_colour = colour;
#ifndef NO_GRAPHICS
  gdk_colour = make_gdk_colour( colour );
#endif
}

#ifndef NO_GRAPHICS

void
Shape::set_colour(const GdkColor& colour)
{
  gdk_colour = colour;
  rgb_colour = gdk2hex_colour( colour );
}

#endif
