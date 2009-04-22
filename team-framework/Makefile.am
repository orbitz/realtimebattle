AUTOMAKE_OPTIONS=dist-bzip2 dist-zip subdir-objects
ACLOCAL_AMFLAGS = -I buildinternal
SUBDIRS=conf
frameworkdir=$(ROBOTDIR)/rtb-team-framework
framework_PROGRAMS = rtb-team
noinst_SCRIPTS= createrobot.sh createrobotconf.sh
EXTRA_DIST=$(noinst_SCRIPTS)
rtb_team_LDADD= @ETR_SOCKET_LIBS@
rtb_team_SOURCES=\
	common.h \
	initcomponents.h \
	stdnamespace.h \
	exceptions/confignotloadedexception.h \
	exceptions/configurationparserexception.h \
	exceptions/exceptionsnamespace.h \
	exceptions/ioexception.h \
	exceptions/keynotfoundexception.h \
	exceptions/parserexception.h \
	exceptions/resourcenotfoundexception.h \
	exceptions/rtbexception.h \
	exceptions/serverispresentexception.h \
	exceptions/strategyexception.h \
	gamecontrol/clientcoordinator.h \
	gamecontrol/clientgamecontroller.h \
	gamecontrol/coordinator.h \
	gamecontrol/coordinatorcomposite.h \
	gamecontrol/coordinatorleaf.h \
	gamecontrol/gamecontroller.h \
	gamecontrol/gamecontrolnamespace.h \
	gamecontrol/gameoptionsrepository.h \
	gamecontrol/servercoordinator.h \
	gamecontrol/servergamecontroller.h \
	io/clientcommunicator.h \
	io/clientconnection.h \
	io/connection.h \
	io/iofactory.h \
	io/ionamespace.h \
	io/localclientconnection.h \
	io/remoteclientconnection.h \
	io/rtbconnection.h \
	io/servercommunicator.h \
	io/serverconnection.h \
	io/unixclientcommunicator.h \
	io/unixinstream.h \
	io/unixinstreambuf.h \
	io/unixiofactory.h \
	io/unixoutstream.h \
	io/unixoutstreambuf.h \
	io/unixremoteclientconnection.h \
	io/unixrtbconnection.h \
	io/unixservercommunicator.h \
	io/unixserverconnection.h \
	log/logdriver.h \
	log/logger.h \
	log/lognamespace.h \
	log/nulllogdriver.h \
	log/rtblogdriver.h \
	log/sysloglogdriver.h \
	log/unixfilelogdriver.h \
	parser/argument_type.h \
	parser/configurationparser.h \
	parser/game_option_type.h \
	parser/message_from_robot_type.h \
	parser/message_to_robot_type.h \
	parser/object_type.h \
	parser/parsernamespace.h \
	parser/robot_option_type.h \
	parser/rtbparser.h \
	parser/rtbparseresult.h \
	parser/warning_type.h \
	rtbglobal/masterresourcecontrol.h \
	rtbglobal/pmap.h \
	rtbglobal/pvector.h \
	rtbglobal/rtbglobalnamespace.h \
	rtbglobal/rtbinit.h \
	strategies/clientspecificrepository.h \
	strategies/clientstate.h \
	strategies/clientstatedata.h \
	strategies/internalserverspecificrepository.h \
	strategies/serverstate.h \
	strategies/serverstatedata.h \
	strategies/sharedserverspecificrepository.h \
	strategies/specificrepository.h \
	strategies/state.h \
	strategies/strategiesnamespace.h \
	strategies/strategyfactory.h \
	bots/brotfrucht/bfclientbasicstate.h \
	bots/brotfrucht/bfclientshoothalfstate.h \
	bots/brotfrucht/bfclientshoottwicestate.h \
	bots/brotfrucht/bfclientspecificrepository.h \
	bots/brotfrucht/bfclientstateenum.h \
	bots/brotfrucht/bffactory.h \
	bots/brotfrucht/bfinternalserverspecificrepository.h \
	bots/brotfrucht/bfserverbasicstate.h \
	bots/brotfrucht/bfservernorotatestate.h \
	bots/brotfrucht/bfserverrotatestate.h \
	bots/brotfrucht/bfserverstateenum.h \
	bots/brotfrucht/bfsharedserverspecificrepository.h \
	bots/brotfrucht/brotfruchtnamespace.h \
	bots/empty/empty_client_basicstate.h \
	bots/empty/empty_client_derivatedstate.h \
	bots/empty/empty_clientspecificrepository.h \
	bots/empty/empty_factory.h \
	bots/empty/empty_internalserverspecificrepository.h \
	bots/empty/empty_namespace.h \
	bots/empty/empty_server_basicstate.h \
	bots/empty/empty_server_derivatedstate.h \
	bots/empty/empty_sharedserverspecificrepository.h \
	initcomponents.cpp \
	initexceptions.cpp \
	main.cpp \
	exceptions/confignotloadedexception.cpp \
	exceptions/configurationparserexception.cpp \
	exceptions/ioexception.cpp \
	exceptions/keynotfoundexception.cpp \
	exceptions/parserexception.cpp \
	exceptions/resourcenotfoundexception.cpp \
	exceptions/rtbexception.cpp \
	exceptions/serverispresentexception.cpp \
	exceptions/strategyexception.cpp \
	gamecontrol/clientcoordinator.cpp \
	gamecontrol/clientgamecontroller.cpp \
	gamecontrol/coordinatorcomposite.cpp \
	gamecontrol/coordinatorleaf.cpp \
	gamecontrol/servercoordinator.cpp \
	gamecontrol/servergamecontroller.cpp \
	io/localclientconnection.cpp \
	io/unixclientcommunicator.cpp \
	io/unixinstream.cpp \
	io/unixinstreambuf.cpp \
	io/unixiofactory.cpp \
	io/unixoutstream.cpp \
	io/unixoutstreambuf.cpp \
	io/unixremoteclientconnection.cpp \
	io/unixrtbconnection.cpp \
	io/unixservercommunicator.cpp \
	io/unixserverconnection.cpp \
	log/logger.cpp \
	log/rtblogdriver.cpp \
	log/sysloglogdriver.cpp \
	log/unixfilelogdriver.cpp \
	parser/configurationparser.cpp \
	parser/rtbparser.cpp \
	rtbglobal/masterresourcecontrol.cpp \
	rtbglobal/rtbinit.cpp \
	strategies/clientstate.cpp \
	strategies/clientstatedata.cpp \
	strategies/serverstate.cpp \
	strategies/serverstatedata.cpp \
	bots/brotfrucht/bfclientbasicstate.cpp \
	bots/brotfrucht/bfclientshoothalfstate.cpp \
	bots/brotfrucht/bfclientshoottwicestate.cpp \
	bots/brotfrucht/bfclientspecificrepository.cpp \
	bots/brotfrucht/bffactory.cpp \
	bots/brotfrucht/bfinternalserverspecificrepository.cpp \
	bots/brotfrucht/bfserverbasicstate.cpp \
	bots/brotfrucht/bfservernorotatestate.cpp \
	bots/brotfrucht/bfserverrotatestate.cpp \
	bots/brotfrucht/bfsharedserverspecificrepository.cpp\
	bots/empty/empty_client_basicstate.cpp \
	bots/empty/empty_client_derivatedstate.cpp \
	bots/empty/empty_clientspecificrepository.cpp \
	bots/empty/empty_factory.cpp \
	bots/empty/empty_internalserverspecificrepository.cpp \
	bots/empty/empty_server_basicstate.cpp \
	bots/empty/empty_server_derivatedstate.cpp \
	bots/empty/empty_sharedserverspecificrepository.cpp \
	bots/columbus/columbusinternalserverspecificrepository.cpp \
	bots/columbus/columbusinternalserverspecificrepository.h \
	bots/columbus/columbusclientstateenum.h \
	bots/columbus/columbusserverstateenum.h \
	bots/columbus/columbusnamespace.h \
	bots/columbus/columbusserverattackstate.cpp \
	bots/columbus/columbusserverattackstate.h \
	bots/columbus/columbusserverbasestate.cpp \
	bots/columbus/columbusserverbasestate.h \
	bots/columbus/columbusserverdiscoverystate.cpp \
	bots/columbus/columbusserverdiscoverystate.h \
	bots/columbus/columbusserverescapestate.cpp \
	bots/columbus/columbusserverescapestate.h \
	bots/columbus/columbusserverinitstate.cpp \
	bots/columbus/columbusserverinitstate.h \
	bots/columbus/columbusserverstateenum.h \
	bots/columbus/columbusservertrackstate.cpp \
	bots/columbus/columbusservertrackstate.h \
	bots/columbus/columbusclientspecificrepository.cpp \
	bots/columbus/columbusclientspecificrepository.h \
	bots/columbus/columbusclientattackstate.cpp \
	bots/columbus/columbusclientattackstate.h \
	bots/columbus/columbusclientbasestate.cpp \
	bots/columbus/columbusclientbasestate.h \
	bots/columbus/columbusclientdrivestate.cpp \
	bots/columbus/columbusclientdrivestate.h \
	bots/columbus/columbussharedserverspecificrepository.cpp \
	bots/columbus/columbussharedserverspecificrepository.h \
	bots/columbus/columbusstrategyfactory.cpp \
	bots/columbus/columbusstrategyfactory.h \
	bots/general/teaminfo.cpp\
	bots/general/teaminfo.h\
	bots/general/enemiesinfo.cpp\
	bots/general/enemiesinfo.h\
	bots/general/generalnamespace.h\
	bots/general/maps/basemap.h\
	bots/general/maps/topographicmap.h\
	bots/general/maps/topographicmap_float.h\
	bots/general/maps/mapsnamespace.h\
	bots/general/maps/navigatemap.h\
	bots/general/maps/navigatemap.cpp\
	bots/general/maps/potentialmap.h\
	bots/general/maps/potentialmap.cpp\
	bots/cobra/cobra_client_basicstate.cpp\
	bots/cobra/cobra_client_basicstate.h\
	bots/cobra/cobra_clientspecificrepository.cpp\
	bots/cobra/cobra_clientspecificrepository.h\
	bots/cobra/cobra_factory.cpp\
	bots/cobra/cobra_factory.h\
	bots/cobra/cobra_internalserverspecificrepository.cpp\
	bots/cobra/cobra_internalserverspecificrepository.h\
	bots/cobra/cobra_namespace.h\
	bots/cobra/cobra_server_basicstate.cpp\
	bots/cobra/cobra_server_basicstate.h\
	bots/cobra/cobra_sharedserverspecificrepository.cpp\
	bots/cobra/cobra_sharedserverspecificrepository.h
