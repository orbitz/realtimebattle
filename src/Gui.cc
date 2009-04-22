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

#include "Gui.h"
#include "IntlDefs.h"
#include "ArenaController.h"
#include "ArenaWindow.h"
#include "MessageWindow.h"
#include "ScoreWindow.h"
#include "StatisticsWindow.h"
#include "StartTournamentWindow.h"
#include "Dialog.h"
#include "Options.h"

#include <string>
using namespace std;

Gui::Gui()
{
  arenawindow_p = NULL;
  messagewindow_p = NULL;
  scorewindow_p = NULL;
  statisticswindow_p = NULL;
  starttournamentwindow_p = NULL;
}

void
Gui::set_colours()
{  
  bg_rgb_colour = the_opts.get_l( OPTION_BACKGROUND_COLOUR );
  fg_rgb_colour = the_opts.get_l( OPTION_FOREGROUND_COLOUR );
  rtb_message_rgb_colour = the_opts.get_l( OPTION_RTB_MESSAGE_COLOUR );

  bg_gdk_colour = make_gdk_colour( bg_rgb_colour );
  fg_gdk_colour = make_gdk_colour( fg_rgb_colour );
  rtb_message_gdk_colour = make_gdk_colour( rtb_message_rgb_colour );
}

void
Gui::open_arenawindow()
{
  if( NULL == arenawindow_p )
    arenawindow_p = 
      new ArenaWindow( the_opts.get_l( OPTION_ARENA_WINDOW_SIZE_X ),
                       the_opts.get_l( OPTION_ARENA_WINDOW_SIZE_Y ),
                       the_opts.get_l( OPTION_ARENA_WINDOW_POS_X  ),
                       the_opts.get_l( OPTION_ARENA_WINDOW_POS_Y  ) );
}

void
Gui::close_arenawindow()
{
  if( NULL != arenawindow_p )
    {
      delete arenawindow_p;
      arenawindow_p = NULL;
    }
}

void
Gui::open_messagewindow()
{
  if( NULL == messagewindow_p )
    messagewindow_p = 
      new MessageWindow( the_opts.get_l( OPTION_MESSAGE_WINDOW_SIZE_X ),
                         the_opts.get_l( OPTION_MESSAGE_WINDOW_SIZE_Y ),
                         the_opts.get_l( OPTION_MESSAGE_WINDOW_POS_X ),
                         the_opts.get_l( OPTION_MESSAGE_WINDOW_POS_Y ) );
}

void
Gui::close_messagewindow()
{
  if( NULL != messagewindow_p )
    {
      delete messagewindow_p;
      messagewindow_p = NULL;
    }
}

void
Gui::open_scorewindow()
{
  if( NULL == scorewindow_p )
    scorewindow_p = 
      new ScoreWindow( the_opts.get_l( OPTION_SCORE_WINDOW_SIZE_X ),
                       the_opts.get_l( OPTION_SCORE_WINDOW_SIZE_Y ),
                       the_opts.get_l( OPTION_SCORE_WINDOW_POS_X ),
                       the_opts.get_l( OPTION_SCORE_WINDOW_POS_Y ) );
}

void
Gui::close_scorewindow()
{
  if( NULL != scorewindow_p )
    {
      delete scorewindow_p;
      scorewindow_p = NULL;
    }
}

void
Gui::open_statisticswindow()
{
  if( the_arena_controller.is_started() )
    if( NULL == statisticswindow_p && the_arena.get_state() != NOT_STARTED )
      statisticswindow_p = 
        new StatisticsWindow( the_opts.get_l( OPTION_STATISTICS_WINDOW_SIZE_X ),
                              the_opts.get_l( OPTION_STATISTICS_WINDOW_SIZE_Y ),
                              -1, -1 );
}

void
Gui::close_statisticswindow()
{
  if( NULL != statisticswindow_p )
    {
      delete statisticswindow_p;
      statisticswindow_p = NULL;
    }
}

void
Gui::open_starttournamentwindow()
{
  bool open = false;
  if( NULL == starttournamentwindow_p )
    {
      if( the_arena_controller.is_started() )
        if( the_arena.get_state() != NOT_STARTED &&
            the_arena.get_state() != FINISHED )
          {
            string info_text( _("This action will kill the current tournament.\n"
                                 "Do you want to do that?"));
            list<string> string_list;
            string_list.push_back( string( "Yes" ) );
            string_list.push_back( string( "No"  ) );
            Dialog( info_text, string_list,
                    (DialogFunction) Gui::kill_and_start_new_tournament );
          }
        else
          open = true;
      else
        open = true;
    }

  if( open )
    starttournamentwindow_p = 
      new StartTournamentWindow( -1, -1, -1, -1 );
}

void
Gui::kill_and_start_new_tournament( int result )
{
  if( the_arena_controller.is_started() && result == 1 )
    {
      the_arena.interrupt_tournament();
      the_gui.open_starttournamentwindow();
    }
}

void
Gui::close_starttournamentwindow()
{
  if( NULL != starttournamentwindow_p )
    {
      delete starttournamentwindow_p;
      starttournamentwindow_p = NULL;
    }
}
