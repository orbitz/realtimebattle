loggingpriority=10 # because twenty robots will cause a lot of logging messages
frameworkdir=$(ROBOTDIR)/rtb-team-framework
noinst_SCRIPTS= $(top_srcdir)/createrobot.sh $(top_srcdir)/createrobotconf.sh

brotfrucht_confs=\
Brotfrucht-Roggenbrot-Brotteam.conf\
Brotfrucht-Weizenbrot-Brotteam.conf\
Brotfrucht-Mischbrot-Brotteam.conf\
Brotfrucht-Gerstenbrot-Brotteam.conf\
Brotfrucht-Toastbrot-Brotteam.conf\
Brotfrucht-Erdbeere-Fruchtteam.conf\
Brotfrucht-Himbeere-Fruchtteam.conf\
Brotfrucht-Blaubeere-Fruchtteam.conf\
Brotfrucht-Heidelbeere-Fruchtteam.conf\
Brotfrucht-Stachelbeere-Fruchtteam.conf\
Columbus-Cortez-Spain.conf\
Columbus-Vespucci-Spain.conf\
Columbus-Pizarro-Spain.conf\
Columbus-Cabral-Portugal.conf\
Columbus-Zarco-Portugal.conf\
Columbus-Vasco_da_Gama-Portugal.conf\
empty-lamer-TeamLazy.conf\
empty-fool-TeamLazy.conf\
empty-idiot-TeamLazy.conf\
Cobra-Mamba-CobraLike.conf\
Cobra-Cobra-CobraLike.conf\
Cobra-Krait-CobraLike.conf\
Cobra-CoralSnake-CobraLike.conf\
Cobra-TigerSnake-CobraLike.conf\
Cobra-Anaconda-AnacondaLike.conf\
Cobra-Boa-AnacondaLike.conf\
Cobra-Python-AnacondaLike.conf\
Cobra-Viper-AnacondaLike.conf\
Cobra-Adder-AnacondaLike.conf

CLEANFILES=$(brotfrucht_confs) $(robot_SCRIPTS)

framework_DATA=$(brotfrucht_confs)

robotdir=$(ROBOTDIR)
robot_SCRIPTS=\
Brotfrucht-Roggenbrot-Brotteam.robot\
Brotfrucht-Weizenbrot-Brotteam.robot\
Brotfrucht-Mischbrot-Brotteam.robot\
Brotfrucht-Gerstenbrot-Brotteam.robot\
Brotfrucht-Toastbrot-Brotteam.robot\
Brotfrucht-Erdbeere-Fruchtteam.robot\
Brotfrucht-Himbeere-Fruchtteam.robot\
Brotfrucht-Blaubeere-Fruchtteam.robot\
Brotfrucht-Heidelbeere-Fruchtteam.robot\
Brotfrucht-Stachelbeere-Fruchtteam.robot\
Columbus-Cortez-Spain.robot\
Columbus-Vespucci-Spain.robot\
Columbus-Pizarro-Spain.robot\
Columbus-Cabral-Portugal.robot\
Columbus-Zarco-Portugal.robot\
Columbus-Vasco_da_Gama-Portugal.robot\
empty-lamer-TeamLazy.robot\
empty-fool-TeamLazy.robot\
empty-idiot-TeamLazy.robot\
Cobra-Mamba-CobraLike.robot\
Cobra-Cobra-CobraLike.robot\
Cobra-Krait-CobraLike.robot\
Cobra-CoralSnake-CobraLike.robot\
Cobra-TigerSnake-CobraLike.robot\
Cobra-Anaconda-AnacondaLike.robot\
Cobra-Boa-AnacondaLike.robot\
Cobra-Python-AnacondaLike.robot\
Cobra-Viper-AnacondaLike.robot\
Cobra-Adder-AnacondaLike.robot

%.robot: $(top_srcdir)/createrobot.sh
	"$(top_srcdir)/createrobot.sh" "$@" "$(frameworkdir)"

%.conf: $(top_srcdir)/createrobotconf.sh
	"$(top_srcdir)/createrobotconf.sh" "$@" $(loggingpriority)
