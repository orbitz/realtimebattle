#! /bin/sh
# Argument one: name of the robot configuration file in the form <Strategy>-<RobotName>-<Team>.conf
# <Strategy>, <RobotName> and <Team> must not contain hyphens, any kind of slashes, the at symbol or the ampersand
# Argument two: loggging priority, if not set, assume logging priority three
# Argument three: Template file can contain the following sequences, that will be replaced:
# @STRATEGY@, @ROBOTNAME@, @TEAM@, @LOGPRIO@
# If the third argument is not present, take the included HERE document configuration template 
# Will create file called like argument one

strategy=`echo "$1" | sed -ne 's/^\([^-\\\/@&]\{1,\}\)-\([^-\\\/@&]\{1,\}\)-\([^-\\\/@&]\{1,\}\)\.conf$/\1/p'`
name=`echo "$1" | sed -ne 's/^\([^-\\\/@&]\{1,\}\)-\([^-\\\/@&]\{1,\}\)-\([^-\\\/@&]\{1,\}\)\.conf$/\2/p'`
team=`echo "$1" | sed -ne 's/^\([^-\\\/@&]\{1,\}\)-\([^-\\\/@&]\{1,\}\)-\([^-\\\/@&]\{1,\}\)\.conf$/\3/p'`
if test -z "$strategy" -o -z "$name" -o -z "$team"; then
	echo "Robot config file name was not valid (contained hyphens or some kind of slashes, the at symbol or the ampersand or does not follow the scheme <Strategy>-<RobotName>-<Team>.conf): $1."
	exit 1
else
	echo "Should generate configfile for robot $name playing in $team with strategy $strategy."
fi

logprio=`echo "$2" | sed -ne 's/^[0-9]\{1,\}$/&/p'`
if test -z "$logprio"; then
	if test -z "$2"; then
		echo "Logpriority not specified, assuming value 3."
		logprio="3"
	else
		echo "Logpriority was not a valid number but $2."
		exit 1
	fi
fi

if test -z "$3"; then
#echo "No template file specified, take included HERE document configuration template ..." 
cat <<__EOF| sed -ne "s/@STRATEGY@/$strategy/g; s/@ROBOTNAME@/$name/g; s/@TEAM@/$team/g; s/@LOGPRIO@/$logprio/g; w $1"
[Main]
#logDriverName="UnixFileLogDriver"
#logDriverParameters="file:/tmp/@TEAM@/@STRATEGY@-@ROBOTNAME@-@TEAM@.log"
#logDriverName="SyslogLogDriver"
#logDriverParameters="option: LOG_CONS|LOG_PID, ident: Realtimebattle, priority: LOG_INFO"
logDriverName="RTBLogDriver"
logDriverParameters="logCommand:Print"
logPriority="@LOGPRIO@"
RotationReachedValue="0"
RobotName="@ROBOTNAME@ Strategy: @STRATEGY@ Team: @TEAM@"
RobotHomeColour="FFFFFF"
RobotAwayColour="FFFFFF"

[IO]
ReadBufferSize="80000"
#DomainSocketName="/tmp/@TEAM@/@STRATEGY@-@TEAM@.socket"
DomainSocketName="/tmp/.@STRATEGY@-@TEAM@.socket"
#logDriverName="UnixFileLogDriver"
#logDriverParameters="file:/tmp/@TEAM@/@STRATEGY@-@ROBOTNAME@-@TEAM@.log"
#logDriverName="SyslogLogDriver"
#logDriverParameters="option: LOG_CONS|LOG_PID, ident: Realtimebattle, priority: LOG_INFO"
logDriverName="RTBLogDriver"
logDriverParameters="logCommand:Print"
logPriority="@LOGPRIO@"
#UseFcntl="false" # enable this for secure mode
UseFcntl="true"

[UnixServerCommunicator]
logPriority="@LOGPRIO@"
#logDriverName="UnixFileLogDriver"
#logDriverParameters="file:/tmp/@TEAM@/@STRATEGY@-@ROBOTNAME@-@TEAM@.log"
logDriverName="RTBLogDriver"
logDriverParameters="logCommand:Print"
#logDriverName="SyslogLogDriver"
#logDriverParameters="option: LOG_CONS|LOG_PID, ident: Realtimebattle, priority: LOG_INFO"

[UnixClientCommunicator]
#logPriority="@LOGPRIO@"
#logDriverName="UnixFileLogDriver"
#logDriverParameters="file:/tmp/@TEAM@/@STRATEGY@-@ROBOTNAME@-@TEAM@.log"
logDriverName="RTBLogDriver"
logDriverParameters="logCommand:Print"
#logDriverName="SyslogLogDriver"
#logDriverParameters="option: LOG_CONS|LOG_PID, ident: Realtimebattle, priority: LOG_INFO"

[ClientCoordinator]
#logDriverName="UnixFileLogDriver"
#logDriverParameters="file:/tmp/@TEAM@/@STRATEGY@-@ROBOTNAME@-@TEAM@.log"
logPriority="@LOGPRIO@"
logDriverName="RTBLogDriver"
logDriverParameters="logCommand:Print"
#logDriverName="SyslogLogDriver"
#logDriverParameters="option: LOG_CONS|LOG_PID, ident: Realtimebattle, priority: LOG_INFO"

[ServerCoordinator]
#logDriverName="UnixFileLogDriver"
#logDriverParameters="file:/tmp/@TEAM@/@STRATEGY@-@ROBOTNAME@-@TEAM@.log"
logPriority="@LOGPRIO@"
logDriverName="RTBLogDriver"
logDriverParameters="logCommand:Print"
#logDriverName="SyslogLogDriver"
#logDriverParameters="option: LOG_CONS|LOG_PID, ident: Realtimebattle, priority: LOG_INFO"

[ServerGameController]
#logDriverName="UnixFileLogDriver"
#logDriverParameters="file:/tmp/@TEAM@/@STRATEGY@-@ROBOTNAME@-@TEAM@.log"
logPriority="@LOGPRIO@"
logDriverName="RTBLogDriver"
logDriverParameters="logCommand:Print"
#logDriverName="SyslogLogDriver"
#logDriverParameters="option: LOG_CONS|LOG_PID, ident: Realtimebattle, priority: LOG_INFO"

[ClientGameController]
#logDriverName="UnixFileLogDriver"
#logDriverParameters="file:/tmp/@TEAM@/@STRATEGY@-@ROBOTNAME@-@TEAM@.log"
logPriority="@LOGPRIO@"
logDriverName="RTBLogDriver"
logDriverParameters="logCommand:Print"
#logDriverName="SyslogLogDriver"
#logDriverParameters="option: LOG_CONS|LOG_PID, ident: Realtimebattle, priority: LOG_INFO"

[Strategy]
StrategyName="@STRATEGY@"

[BrotFrucht]
# let the robot shot every second
ShootInterval="1"

[BrotfruchtServer]
#logDriverName="UnixFileLogDriver"
#logDriverParameters="file:/tmp/@TEAM@/@STRATEGY@-@ROBOTNAME@-@TEAM@.log"
logPriority="@LOGPRIO@"
logDriverName="RTBLogDriver"
logDriverParameters="logCommand:Print"
#logDriverName="SyslogLogDriver"
#logDriverParameters="option: LOG_CONS|LOG_PID, ident: Realtimebattle, priority: LOG_INFO"

[BrotfruchtClient]
#logDriverName="UnixFileLogDriver"
#logDriverParameters="file:/tmp/@TEAM@/@STRATEGY@-@ROBOTNAME@-@TEAM@.log"
logPriority="@LOGPRIO@"
logDriverName="RTBLogDriver"
logDriverParameters="logCommand:Print"
#logDriverName="SyslogLogDriver"
#logDriverParameters="option: LOG_CONS|LOG_PID, ident: Realtimebattle, priority: LOG_INFO"

[ColumbusServer]
#logDriverName="UnixFileLogDriver"
#logDriverParameters="file:/tmp/@TEAM@/@STRATEGY@-@ROBOTNAME@-@TEAM@.log"
logPriority="@LOGPRIO@"
logDriverName="RTBLogDriver"
logDriverParameters="logCommand:Print"
#logDriverName="SyslogLogDriver"
#logDriverParameters="option: LOG_CONS|LOG_PID, ident: Realtimebattle, priority: LOG_INFO"

[ColumbusClient]
logDriverName="UnixFileLogDriver"
logDriverParameters="file:/tmp/@TEAM@/@STRATEGY@-@ROBOTNAME@-@TEAM@.log"
logPriority="@LOGPRIO@"
logDriverName="RTBLogDriver"
logDriverParameters="logCommand:Print"
#logDriverName="SyslogLogDriver"
#logDriverParameters="option: LOG_CONS|LOG_PID, ident: Realtimebattle, priority: LOG_INFO"
__EOF
else
	if test -f "$3" -a -r "$3" -a -s "$3"; then
		cat <"$3" | sed -ne "s/@STRATEGY@/$strategy/g; s/@ROBOTNAME@/$name/g; s/@TEAM@/$team/g; s/@LOGPRIO@/$logprio/g; w $1"
	else
		echo "Template file $3 was not readable, not existent or is empty."
		exit 1
	fi
fi
#echo "Configuration file generation completed successfully."
exit 0
