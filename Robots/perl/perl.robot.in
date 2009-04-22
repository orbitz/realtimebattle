#! @PERL_SHEBANG@

use strict;
use IO::Handle;

srand;

sub UNKNOWN_MESSAGE()               	{0}
sub PROCESS_TIME_LOW()              	{1}
sub MESSAGE_SENT_IN_ILLEGAL_STATE() 	{2}
sub UNKNOWN_OPTION()                	{3}
sub OBSOLETE_KEYWORD()              	{4}

sub ROBOT_MAX_ROTATE()		{0}
sub ROBOT_CANNON_MAX_ROTATE()		{1}
sub ROBOT_RADAR_MAX_ROTATE()		{2}
sub ROBOT_MAX_ACCELERATION()		{3}
sub ROBOT_MIN_ACCELERATION()		{4}
sub ROBOT_START_ENERGY()		{5}
sub ROBOT_MAX_ENERGY()		{6}
sub ROBOT_ENERGY_LEVELS()		{7}
sub SHOT_SPEED()			{8}
sub SHOT_MIN_ENERGY()			{9}
sub SHOT_MAX_ENERGY()			{10}
sub SHOT_ENERGY_INCREASE_SPEED()	{11}
sub TIMEOUT()				{12}
sub DEBUG_LEVEL()			{13}

sub SIGNAL()				{2}
sub SEND_SIGNAL()			{0}
sub SEND_ROTATION_REACHED()		{1}

sub NOOBJECT()			{-1}
sub ROBOT()				{0}
sub SHOT()				{1}
sub WALL()				{2}
sub COOKIE()				{3}
sub MINE()				{4}
sub LAST_OBJECT_TYPE()		{5}

##################################################################
# CMDs
sub RobotOption($$){
  my ($nr, $value) = @_;
  print "RobotOption $nr $value\n";
}

sub Name($) {
  my $name = shift;
  print "Name $name\n";
}

sub Colour($) {
  my $color = shift;
  print "Colour $color\n";
}
sub Color($) { Colour(@_) }

sub Rotate($$) {
  my $what = shift; # 1 - robot, 2 - cannon, 4 - radar
  my $v    = shift; # velocity
  print "Rotate $what $v\n";
}

sub RotateTo($$$) {
  my ($what, $v, $end) = @_;
  print "RotateTo $what $v $end\n";
}

sub RotateAmount($$$) {
  my ($what, $v, $angl) = @_;
  print "RotateAmount $what $v $angl\n";
}

sub Sweep($$$$) {
  my ($what, $v, $la, $ra) = @_;
  print "Sweep $what $v $la $ra\n";
}

sub Accelerate($) {
  my $value = shift;
  print "Accelerate $value\n";
}

sub Brake($) {
  my $portion = shift;
  print "Brake $portion\n";
}

sub Shoot($) {
  my $energy = shift;
  print "Shoot $energy\n";
}

sub Print(@) {
  my $txt = join(" ",@_);
  print "Print $txt\n";
}

sub Debug($) {
  my $txt = shift;
  print "Debug $txt\n";
}

sub DebugLine($$$$) {
  my ($a1, $r1, $a2, $r2) = @_;
  print "DebugLine $a1 $r1 $a2 $r2\n";
}

sub DebugCircle($$$) {
  my ($a,$r1,$r2) = @_;
  print "DebugCircle $a $r1 $r2\n";
}

# Objects: Robot=0, Shot=1, Wall=2, Cookie=3, Mine=4, LAST=5
my $NAME = "Perl v0.0";
my $COLOR= "00ff00 ff0000";
my $QUIT = 0;
my %OPTION;
my %RDATA = (Energy => 100, RobotsLeft => 0);

open(LOG,">>/tmp/perl.robot.log") || die "Kann Logfile nicht öffnen: $!";
LOG->autoflush(1);
STDOUT->autoflush(1);
STDERR->autoflush(1);

RobotOption(SIGNAL,10); # Get Signals

# Signalhandler
$SIG{USR1} = \&check_message;
$SIG{USR2} = 'IGNORE';
$SIG{HUP}  = \&quit_bot;

print LOG "Starting Roboter\n";

while(not $QUIT) {
  &run_robot
}

print LOG "Jetzt ist aber Ende\a\n";
exit;
###################################################################

sub run_robot() {
    sleep(1);
}

sub quit_bot() {
  $QUIT = 1;
  run_robot();
}

sub check_message() {
  my $signame = shift;
  my @input = <STDIN>;
  foreach my $msg (@input) {
    chomp($msg);              # entferne \n
    if ($msg) {&parse($msg)}
  }
  run_robot();
}

sub parse($) {
  my $input = shift;
  my ($cmd, @arg) = split(" ",$input);
  if    ($cmd eq "Initialize") {Initialize($arg[0])}
  elsif ($cmd eq "YourName")   {YourName(@arg)}
  elsif ($cmd eq "YourColour") {YourColour(@arg)}
  elsif ($cmd eq "GameOption") {GameOption(@arg)}
  elsif ($cmd eq "GameStarts") {GameStarts()}
  elsif ($cmd eq "Radar")      {Radar(@arg)}
  elsif ($cmd eq "Info")       {Info(@arg)}
  elsif ($cmd eq "RobotInfo")  {RobotInfo(@arg)}
  elsif ($cmd eq "RotationReached") {RotationReached($arg[0])}
  elsif ($cmd eq "Energy")     {Energy($arg[0])}
  elsif ($cmd eq "RobotsLeft") {RobotsLeft($arg[0])}
  elsif ($cmd eq "Collision")  {Collision(@arg)}
  elsif ($cmd eq "Warning")    {Warning(@arg)}
  elsif ($cmd eq "Dead")       {Dead()}
  elsif ($cmd eq "GameFinishes") {GameFinishes()}
  elsif ($cmd eq "ExitRobot")  {ExitRobot()}
  else {print LOG "Unknown Command $input\n";
#        print "Print Unknown Command $input\n"
       }
}


####################################################
# Message:
sub Initialize($) {
  my $arg = shift;
  if ($arg ==1) {
    print LOG "Initialising\n";
    Name($NAME);
    Colour($COLOR);
    Print("Ahh, awaken again...");
  } else {
    # warte auf YourName und Colour
  }
}

sub YourName(@) {
  $NAME = join(" ",@_);
  Print("My name is now $NAME");
}

sub YourColour(@) {
  $COLOR = join(" ",@_);
  Print("My colo(u)r is now $COLOR");
}

sub GameOption(@){
  my @arg = @_;
  my ($optionnr,$value) = split(" ",@arg);
  $OPTION{$optionnr} = $value;
}

sub GameStarts() {
  Print("Let's go dancing...");
  dump_options();
  tac_move();
}

sub Radar($$$) {
  my $dist = shift;
  my $obj  = shift;
  my $angle= shift;
#  print LOG "Radar: $dist, $obj, $angle\n";
  if ($obj == 0) {tac_fire($angle)}
  if ($obj == 2 or $obj == 4) {tac_wall($dist)}
  print LOG "Gotch: $obj\n" if ($obj == 5);
}

sub Info($$$) {
  my $time  = shift;
  my $speed = shift;
  my $cannon= shift; # angle
}

sub RobotInfo($$) {
  my $energy = shift;
  my $team   = shift;
}

sub RotationReached($) {
  my $reached = shift;
}

sub Energy($) {
  $RDATA{'Energy'} = shift;
#  print LOG "Energy: $RDATA{'Energy'}\n";
}

sub RobotsLeft($) {
  my $robots = shift;
  if ($RDATA{RobotsLeft}) {
    if ($robots < $RDATA{RobotsLeft}) {
      Print("Gotcha. Onother one's gone! :-)");
    }  
  }
  $RDATA{RobotsLeft} = $robots;
}

sub Collision($$) {
  my $obj  = shift;
  my $angle= shift;
  if    ($obj == 4) {Print("Boom! Hit by a mine")}
  elsif ($obj == 3) {Print("Yamm! Cookies Cookiiiies!")}  
}

sub Warning($$) {
  my $type = shift;
  my $msg = join(" ",@_);
  print LOG "WARN: ($type) $msg\n";
  Print("Someone's fucking me: ($type) $msg");
}

sub Dead() {
  Print("Fuck! Good Bye!");
}

sub GameFinishes() {
  Print("Good Bye!");
}

sub ExitRobot() {
  Print("10 9 8 7 6 5 4 3 2 1 ZERO and LIFT OFFFFFFFF");
  exit(0);
}


#########################################################################
# Tools
sub dump_options() {
  $OPTION{'ROBOT_MAX_ROTATE'}=12;
  print LOG "RobotMaxRotate(): ", $OPTION{ROBOT_MAX_ROTATE()},"\n";
  print LOG "RobotMaxRotate: ", $OPTION{ROBOT_MAX_ROTATE},"\n";
  print LOG "RobotCannonMaxRotate: ", $OPTION{ROBOT_CANNON_MAX_ROTATE()},"\n";
  print LOG "RobotMaxAcceleration: ", $OPTION{ROBOT_MAX_ACCELERATION()},"\n";
  print LOG "RobotMinAcceleration: ", $OPTION{ROBOT_MIN_ACCELERATION()},"\n";
  print LOG "RobotStartEnergy: ", $OPTION{ROBOT_START_ENERGY()},"\n";
  
}

##########################################################################
# Tactics
sub tac_fire($) {
  my $angle= shift;
  RotateTo(7, 2, $angle);
#  print LOG "Trying to Hit Boogie at $angle\n";
  Shoot(10);
}

sub tac_move() {
  my $speed = rand(1)+0.5;
  my $angle = rand(0.2)-0.4;
  Accelerate($speed);
  RotateAmount(7,$angle,360);
}

sub tac_wall() {
  my $dist = shift;
  if ($dist < 5) {
    Brake(1);
    RotateAmount(7,2.0,90);
  } else { tac_move }
}

