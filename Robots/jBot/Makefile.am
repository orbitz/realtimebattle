jbotdir = $(RTB_DIR)/Robots/jBot
JAVAROOT=.
jbot_DATA= JBot_C.java  JBot.java  JBotMain.java  JBot_T.java Makefile.am jBot.robot.in
EXTRA_DIST= $(jbot_DATA)
if javacavailable
if javaavailable
robotexecdir = $(RTB_DIR)/Robots
robotexec_SCRIPTS = jBot.robot
jbot_JAVA = JBot_C.java  JBot.java  JBotMain.java  JBot_T.java
endif
endif
