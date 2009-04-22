#! /bin/sh
# Argument one: name of the robot wrapper script in the form <Strategy>-<RobotName>-<Team>.robot
# <Strategy>, <RobotName> and <Team> must not contain hyphens, any kind of slashes, the at symbol or the ampersand
# Argument two: <rtb-team home>
# <rtb home> must not contain the at symbol, the ampersand or the character "|"
# This script will create a file named <robot name>.robot that assumes that /tmp/<robot name>.log is the only log file used
# Argument three: Template file can contain the following sequences, that will be replaced:
# @STRATEGY@, @ROBOTNAME@, @TEAM@, @RTBHOME@
# If the third argument is not present, take the included HERE document configuration template 
# Will create file called like argument one

strategy=`echo "$1" | sed -ne 's/^\([^-\\\/@&]\{1,\}\)-\([^-\\\/@&]\{1,\}\)-\([^-\\\/@&]\{1,\}\)\.robot$/\1/p'`
name=`echo "$1" | sed -ne 's/^\([^-\\\/@&]\{1,\}\)-\([^-\\\/@&]\{1,\}\)-\([^-\\\/@&]\{1,\}\)\.robot$/\2/p'`
team=`echo "$1" | sed -ne 's/^\([^-\\\/@&]\{1,\}\)-\([^-\\\/@&]\{1,\}\)-\([^-\\\/@&]\{1,\}\)\.robot$/\3/p'`

rtbhome=`echo "$2" | sed -ne 's/[&|@]/FAULT/p'`
if test ! x$rtbhome = "x"; then
	echo "<rtb home> contained illegal characters (e. g. &, |, @): $2." 
	exit 1
fi

if test -z "$strategy" -o -z "$name" -o -z "$team"; then
	echo "Robot wrapper script name was not valid (contained hyphens or some kind of slashes, the at symbol or the ampersand or does not follow the scheme <Strategy>-<RobotName>-<Team>.robot): $1."
	exit 1
else
	echo "Should generate wrapper script for robot $name playing in $team with strategy $strategy."
fi

if test -z "$3"; then
#echo "No template file specified, take included HERE document configuration template ..." 
cat <<__EOF| sed -ne "s/@STRATEGY@/$strategy/g; s/@ROBOTNAME@/$name/g; s/@TEAM@/$team/g; s|@RTBHOME@|$2|g; w $1"
#! /bin/sh
#mkdir -p /tmp/@TEAM@/
#rm -f "/tmp/@TEAM@/@STRATEGY@-@ROBOTNAME@-@TEAM@.log"
exec "@RTBHOME@/rtb-team" "@RTBHOME@/@STRATEGY@-@ROBOTNAME@-@TEAM@.conf"
#"@RTBHOME@/rtb-team" "@RTBHOME@/@STRATEGY@-@ROBOTNAME@-@TEAM@.conf"
__EOF
else
	if test -f "$3" -a -r "$3" -a -s "$3"; then
		cat <"$3"| sed -ne "s/@STRATEGY@/$strategy/g; s/@ROBOTNAME@/$name/g; s/@TEAM@/$team/g; s|@RTBHOME@|$2|g; w $1"
	else
		echo "Template file $3 was not readable, not existent or is empty."
		exit 1
	fi
fi
#echo "Wrapper script file generation completed successfully."
exit 0
