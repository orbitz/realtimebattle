robotexecdir = $(RTB_DIR)/Robots
INCLUDES = -I$(top_srcdir)/include
if linuxjoystickavailable
robotexec_PROGRAMS = joypad_robot.robot
endif
joypad_robot_robot_SOURCES = joypad_robot.c
joypaddir = $(RTB_DIR)/Robots/joypad_robot
joypad_DATA = $(joypad_robot_robot_SOURCES) Makefile.am README

