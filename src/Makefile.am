INCLUDES = \
	 -I$(top_srcdir)/include\
	 -I$(top_srcdir)/intl
	 

realtimebattle_LDADD = $(LIBINTL)
realtimebattle_CXXFLAGS =
realtimebattle_LDFLAGS = 

bin_PROGRAMS = realtimebattle

realtimebattle_SOURCES = \
	RealTimeBattle.cc\
	ArenaBase.cc\
	ArenaController.cc\
	ArenaRealTime.cc\
	ArenaReplay.cc\
	Broadcast.cc\
	Extras.cc\
	GeometricalObjects.cc\
	Options.cc\
	Robot.cc\
	Shape.cc\
	Shot.cc\
	Timer.cc\
	Various.cc\
	Vector2D.cc

if NO_GETOPT_LONG
realtimebattle_SOURCES += getopt.c
endif
if NO_GRAPHICS
realtimebattle_CXXFLAGS += \
	-DNO_GRAPHICS
else
realtimebattle_SOURCES += \
	Gui.cc\
	ArenaWindow.cc\
	ControlWindow.cc\
	Dialog.cc\
	MessageWindow.cc\
	OptionsWindow.cc\
	StatisticsWindow.cc\
	StartTournamentWindow.cc\
	Structs.cc\
	ScoreWindow.cc
	
INCLUDES += \
	$(GTK_CFLAGS)
	
realtimebattle_LDADD +=  \
	$(GTK_LIBS)
endif

if NO_NETWORK
realtimebattle_CXXFLAGS += \
	-DNO_NETWORK
else
realtimebattle_SOURCES += \
	ClientInterface.cc
realtimebattle_LDADD += @ETR_SOCKET_LIBS@
endif

