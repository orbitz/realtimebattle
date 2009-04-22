robotexecdir = $(RTB_DIR)/Robots
INCLUDES = -I$(top_srcdir)/include -I$(top_srcdir)/intl

robotexec_PROGRAMS = rotate_and_fire_signal.robot \
                     rotate_and_fire_blocking.robot \
                     rotate_and_fire_select.robot

rotate_and_fire_signal_robot_SOURCES = rotate_and_fire.cc rotate_and_fire.h \
                                       rotate_and_fire_signal.cc

rotate_and_fire_blocking_robot_SOURCES = rotate_and_fire.cc rotate_and_fire.h \
                                       rotate_and_fire_blocking.cc

rotate_and_fire_select_robot_SOURCES = rotate_and_fire.cc rotate_and_fire.h \
                                       rotate_and_fire_select.cc

rafdir = $(RTB_DIR)/Robots/rotate_and_fire
raf_DATA = $(rotate_and_fire_signal_robot_SOURCES) \
           $(rotate_and_fire_blocking_robot_SOURCES) \
           $(rotate_and_fire_select_robot_SOURCES) \
	   Makefile.am

LDADD = $(LIBINTL)
