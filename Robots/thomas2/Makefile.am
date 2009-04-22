if NO_GRAPHICS
else
robotexecdir = $(RTB_DIR)/Robots
INCLUDES = \
	$(GTK_CFLAGS)\
	-I$(top_srcdir)/include
thomas2_robot_LDADD =  \
	$(GTK_LIBS)
robotexec_PROGRAMS = thomas2.robot
thomas2_robot_SOURCES = thomas2.cc
endif
thomas2dir = $(RTB_DIR)/Robots/thomas2
thomas2_DATA = thomas2.cc Makefile.am 

