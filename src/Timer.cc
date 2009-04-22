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

#include "Timer.h"

Timer::Timer()
{
  gettimeofday(&start, NULL);
}

void
Timer::reset()
{
  gettimeofday(&start, NULL);
}

double
Timer::get()
{
  timeval current_time;
  gettimeofday(&current_time, NULL);

  if (start.tv_usec > current_time.tv_usec)
    {
      current_time.tv_usec += 1000000;
      current_time.tv_sec--;
    }

  double elapsed;

  elapsed = ((double)( current_time.tv_usec - start.tv_usec )) / 1e6;
  elapsed += (double)( current_time.tv_sec  - start.tv_sec );

  return elapsed;
}
