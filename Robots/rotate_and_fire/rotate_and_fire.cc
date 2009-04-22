
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#ifdef TIME_WITH_SYS_TIME 
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

#ifdef HAVE_LOCALE_H
# include <locale.h>
#endif

#include "IntlDefs.h"
#include "rotate_and_fire.h"

namespace std { }
using namespace std;

RotateAndFire::RotateAndFire(const char* name, const char* colour)
{
  robot_name = new char[strlen(name) + 1];
  strcpy(robot_name, name);

  robot_colour = new char[strlen(colour) + 1];
  strcpy(robot_colour, colour);

  quitting = false;

  robot_rotate = 0.53;
  acceleration = 0.54;
  brake_value = 0.0;

  // Get random seed from the clock
  // timeval current_time;
  // gettimeofday(&current_time, NULL);
  // srand(current_time.tv_usec);

  // We want to know when RotateTo and RotateAmount finishes
  robot_option( SEND_ROTATION_REACHED, 1 );
}

RotateAndFire::~RotateAndFire()
{
  delete [] robot_name;
  delete [] robot_colour;
}

// Function for sending messages from robot. 
// They do nothing but write the command and its arguments to cout.
void
RotateAndFire::robot_option( const int option, const int value )
{
  cout << "RobotOption " << option << " " << value << endl;
}

void
RotateAndFire::name( const char* name )
{
  cout << "Name " << name << endl;
}

void
RotateAndFire::colour( const char* home, const char* away )
{
  cout << "Colour " << home << " " << away << endl;
}

void
RotateAndFire::rotate( const int what, const double vel )
{
  cout << "Rotate " << what << " " << vel << endl;
}

void
RotateAndFire::rotate_to( const int what, const double vel,
                          const double angle )
{
  cout << "RotateTo " << what << " " << vel << " " << angle << endl;
}

void
RotateAndFire::rotate_amount( const int what, const double vel,
                              const double angle )
{
  cout << "RotateAmount " << what << " " << vel << " " << angle << endl;
}

void
RotateAndFire::sweep( const int what, const double vel,
                      const double left, const double right )
{
  cout << "Sweep " << what << " " << vel << " "
       << left << " " << right << endl;
}

void
RotateAndFire::accelerate( const double amount )
{
  cout << "Accelerate " << amount << endl;
}

void
RotateAndFire::brake( const double amount )
{
  cout << "Brake " << amount << endl;
}

void
RotateAndFire::shoot( const double energy )
{
  cout << "Shoot " << energy << endl;
}

void
RotateAndFire::print( const char* message )
{
  cout << "Print " << message << endl;
}

void
RotateAndFire::debug( const char* message )
{
  cout << "Debug " << message << endl;
}

void
RotateAndFire::debug_line( const double start_angle,
                           const double start_radius,
                           const double end_angle,
                           const double end_radius )
{
  cout << "DebugLine " << start_angle << " " << start_radius
       << " " << end_angle << " " << end_radius << endl;
}

void
RotateAndFire::debug_circle( const double center_angle,
                             const double center_radius,
                             const double circle_radius )
{
  cout << "DebugCircle " << center_angle << " " << center_radius 
       << " " << circle_radius << endl;
}

// Here are all functions that is called whenever a message of
// corresponding type is recieved.

// Send name and colour if it is the first sequence.
void
RotateAndFire::initialize( const int first_seq )
{
  if( first_seq == 1 )
    {
      name( robot_name );
      colour( robot_colour, robot_colour );
    }
}

// The name the robot got in the game.
void
RotateAndFire::your_name( const char* prev_name )
{
}

// The colour the robot got in the game.
void
RotateAndFire::your_colour( const char* colour )
{
}

// This function is called when a game option is changed and
// the robot's copy of this option will change.
void
RotateAndFire::game_option( const int option, const double value )
{
  switch( option )
    {
    case ROBOT_MAX_ROTATE:
      robot_max_rotate = value;
      break;
    case ROBOT_CANNON_MAX_ROTATE:
      robot_cannon_max_rotate = value;
      break;
    case ROBOT_RADAR_MAX_ROTATE:
      robot_radar_max_rotate = value;
      break;
    case ROBOT_MAX_ACCELERATION:
      robot_max_acceleration = value;
      break;
    case ROBOT_MIN_ACCELERATION:
      robot_min_acceleration = value;
      break;
    case ROBOT_START_ENERGY:
      robot_start_energy = value;
      break;
    case ROBOT_MAX_ENERGY:
      robot_max_energy = value;
      break;
    case ROBOT_ENERGY_LEVELS:
      robot_energy_levels = value;
      break;
    case SHOT_SPEED:
      shot_speed = value;
      break;
    case SHOT_MIN_ENERGY:
      shot_min_energy = value;
      break;
    case SHOT_MAX_ENERGY:
      shot_max_energy = value;
      break;
    case SHOT_ENERGY_INCREASE_SPEED:
      shot_energy_increase_speed = value;
      break;
    case TIMEOUT:
      timeout = value;
      break;
    case DEBUG_LEVEL:
      debug_level = value;
      break;
    case SEND_ROBOT_COORDINATES:
      send_robot_coordinates = (int)rint(value);
      break;
    }
}

// Reset the robot when a new game starts. Send initial rotate and
// accelerate commands.
void
RotateAndFire::game_starts()
{
  robot_rotate = 0.53;
  acceleration = 0.54;
  brake_value = 0.0;

  rotate_allowed = true;
  shots_hit = 0;
  last_shot_hit_time = -2.0;
  current_time = 0;

  radar_and_cannon_rotate =
    robot_cannon_max_rotate - fabs(robot_rotate) - robot_rotate;
  rotate( 1, robot_rotate );
  rotate( 6, radar_and_cannon_rotate );
  accelerate( acceleration );
}

// Radar info when no object is seen, should never happen.
void
RotateAndFire::radar_noobject( const double dist, const double angle )
{
}

// Radar info when robot is seen.
// Lock cannon and radar on the seen robot,
// and if it is near enough, begin to brake. Shoot at it.
void
RotateAndFire::radar_robot( const double dist, const double angle )
{
  if( radar_and_cannon_rotate != - robot_rotate )
    {
      radar_and_cannon_rotate = - robot_rotate;
      rotate( 6, radar_and_cannon_rotate );
    }

  if( dist < 2 && acceleration != 0.0 )
    {
      acceleration = 0.0;
      accelerate( acceleration );
      brake_value = 1.0;
      brake( brake_value );
      if( debug_level >= 1 )
        cout << "Debug "<< _("Brakes locked: Robot found and at distance")
             << " " << dist << endl;
    }
  shoot( 2 );
}

// Radar info when a shot is seen.
void
RotateAndFire::radar_shot( const double dist, const double angle )
{
  if( radar_and_cannon_rotate !=
      robot_cannon_max_rotate - fabs(robot_rotate) - robot_rotate )
    {
      radar_and_cannon_rotate =
        robot_cannon_max_rotate - fabs(robot_rotate) - robot_rotate;
      rotate( 6, radar_and_cannon_rotate );
    }
}

// Radar info when a wall is seen.
// If a wall is near in the robot direction, begin to brake,
// else have normal acceleration.
void
RotateAndFire::radar_wall( const double dist, const double angle )
{
  if( radar_and_cannon_rotate !=
      robot_cannon_max_rotate - fabs(robot_rotate) - robot_rotate )
    {
      radar_and_cannon_rotate =
        robot_cannon_max_rotate - fabs(robot_rotate) - robot_rotate;
      rotate( 6, radar_and_cannon_rotate );
    }

  double old_acc = acceleration;

  double mod_angle = fmod( angle, 2 * M_PI );

  if( mod_angle < 1 * M_PI / 3 || mod_angle > 5 * M_PI / 3 )
    {
      if( dist < 1.0 )
        {
          acceleration = 0.0;
          brake_value = 1.0;
        }
      else
        {
          acceleration = 1.0;
          brake_value = 0.0;
        }
    }

  if( old_acc != acceleration )
    {
      accelerate( acceleration );
      brake( brake_value );
      if( debug_level >= 4 )
        {
          cout << "Debug " << _("Acceleration") << ": " << acceleration << endl;
          cout << "Debug " << _("Brake") << ": " << brake_value << endl;
        }
      if( debug_level == 5 )
        cout << "Debug " << _("Modular angle, Angle") << ": "
             << mod_angle << ", " << angle << endl;
    }
}

// Radar info when a cookie is seen.
void
RotateAndFire::radar_cookie( const double dist, const double angle )
{
  if( radar_and_cannon_rotate !=
      robot_cannon_max_rotate - fabs(robot_rotate) - robot_rotate )
    {
      radar_and_cannon_rotate =
        robot_cannon_max_rotate - fabs(robot_rotate) - robot_rotate;
      rotate( 6, radar_and_cannon_rotate );
    }
  if( debug_level >= 2 )
    {
      cout << "Debug " << _("Cookie found at") << " ("
           << current_x_coordinate + dist*cos(angle + current_robot_angle)
           << "," << current_y_coordinate + dist*sin(angle + current_robot_angle)
           << ")" << endl;
      cout << "Debug " << _("I am at") << " "
           << " (" << current_x_coordinate << "," << current_y_coordinate
           << ") " << _("at angle") << " " << current_robot_angle << endl;
    }
}

// Radar info when a mine is seen.
void
RotateAndFire::radar_mine( const double dist, const double angle )
{
  if( radar_and_cannon_rotate !=
      robot_cannon_max_rotate - fabs(robot_rotate) - robot_rotate )
    {
      radar_and_cannon_rotate =
        robot_cannon_max_rotate - fabs(robot_rotate) - robot_rotate;
      rotate( 6, radar_and_cannon_rotate );
    }
  shoot( shot_min_energy );
  if( debug_level >= 2 )
    {
      cout << "Debug " << _("Mine found at") << " ("
           << current_x_coordinate + dist*cos(angle + current_robot_angle)
           << "," << current_y_coordinate + dist*sin(angle + current_robot_angle)
           << ")" << endl;
      cout << "Debug " << _("I am at") << " "
           << " (" << current_x_coordinate << "," << current_y_coordinate
           << ") " << _("at angle") << " " << current_robot_angle << endl;
    }
}


// Information about the position of the robot. Coordinates are given either absolute,
// relative to the starting position, or not at all, depending on the option
// SEND_ROBOT_COORDINATES.
void
RotateAndFire::coordinates( const double x, const double y, const double rotation)
{
  if( send_robot_coordinates == 0 )
    return;

  current_x_coordinate = x;
  current_y_coordinate = y;
  current_robot_angle = rotation;
}

// Get information about time, speed and cannon_angle
// Update current time and check how long time has passed since
// we were last hit by a shot.
void
RotateAndFire::info( const double time, const double speed,
                     const double cannon_angle )
{
  current_time = time;

  if( current_time - last_shot_hit_time > 2.0 )
    shots_hit = 0;
}

// Information about seen robots
void
RotateAndFire::robot_info( const double energy, int enemy )
{
}

// Check if rotation has been reached.
// If it is the robot. Begin normal acceleration and rotation
void
RotateAndFire::rotation_reached( const int what )
{
  if( what & 1 == 1 )
    {
      rotate_allowed = true;
      acceleration_change_allowed = true;
      int direction = rand() / (RAND_MAX/2);
      if( direction == 0 ) direction = -1;
      robot_rotate = 0.53 * direction;
      acceleration = 1.0;
      accelerate( acceleration );
    }
}

// Our energy level.
void
RotateAndFire::energy( const double energylevel )
{
}

// How many robots is left in the game.
void
RotateAndFire::robots_left( const int number_of_robots )
{
  number_of_robots_left = number_of_robots;
}

// We have collided with no object. Should not happen.
void
RotateAndFire::collision_noobject( const double angle )
{
}

// We have collided with another robot.
void
RotateAndFire::collision_robot( const double angle )
{
}

// We have collided with a shot.
// Set last_shot_hit_time and increase shots_hit.
// If shots_hit is too high, begin fleeing acceleration and rotation.

void
RotateAndFire::collision_shot( const double angle )
{
  shots_hit++;
  last_shot_hit_time = current_time;
  if( debug_level == 5 )
    cout << "Debug Number of shots hit " << shots_hit << endl;

  if( shots_hit > 5 )
    {
      int direction = rand() / (RAND_MAX/2);
      if( direction == 0 ) direction = -1;
      robot_rotate = robot_max_rotate * direction;
      rotate_amount( 1, robot_rotate, M_PI / 2 );
      rotate_allowed = false;
      acceleration = robot_max_acceleration;
      accelerate( acceleration );
      acceleration_change_allowed = false;
      if( debug_level >= 3 )
        {
          cout << "Debug " << _("New rotation") << ": " << robot_rotate << endl;
          cout << "Debug " << _("New acceleration") << ": " << acceleration << endl;
        }
      shots_hit = 0;
      print( _("Hit too many times! Flee!") );
    }
}

// We have collided with a wall.
void
RotateAndFire::collision_wall( const double angle )
{
}

// We have collided with a cookie.
void
RotateAndFire::collision_cookie( const double angle )
{
  print( _("Cookie eaten!") );
}

// We have collided with a mine.
void
RotateAndFire::collision_mine( const double angle )
{
  print( _("Oh no! A mine") );
}

// We have received a warning message.
// Print the message.
void
RotateAndFire::warning( const int type, const char* message )
{
  char full_message[200];

  switch( type )
    {
    case UNKNOWN_MESSAGE:
      strcpy( full_message, _("Unknown message: ") );
      break;
    case PROCESS_TIME_LOW:
      strcpy( full_message, _("Process time low: ") );
      break;
    case MESSAGE_SENT_IN_ILLEGAL_STATE:
      strcpy( full_message, _("Message sent in illegal state: ") );
      break;
    case UNKNOWN_OPTION:
      strcpy( full_message, _("Unknown option: ") );
      break;
    case OBSOLETE_KEYWORD:
      strcpy( full_message, _("Obsolete Keyword: ") );
      break;
    }

  strcat( full_message, message );
  print( full_message );
}

// Unfortunately we have died.
void
RotateAndFire::dead()
{
}

// The game has finished
void
RotateAndFire::game_finishes()
{
}

// You have been ordered to exit the robot
void
RotateAndFire::exit_robot()
{
  print( _("Shutting down and leaving") );
  quitting = true;
}

// This function is called before checking messages.
// Every hundred time, change the direction of the rotation.
void
RotateAndFire::pre_checking_messages()
{
  if(rand() < (RAND_MAX/200) && rotate_allowed )
    {
      robot_rotate = -robot_rotate;
      rotate( 1, robot_rotate );
      if( debug_level >= 2 )
        cout << "Debug " << _("New") << " robot_rotate : " << robot_rotate << endl;
    }
}

// This function handles all the message checking and
// calls the appropriate function.
// This function is called whenever a SIGUSR1 (decided in the
// robotoption sent to the server) signal is received.
void
RotateAndFire::check_messages( )
{
  quitting = false;

  char msg_name[81];
  message_to_robot_type msg_t;

  cin.clear();
  while( !cin.eof() && !quitting )
    {
      pre_checking_messages();

      cin >> msg_name;
      msg_t = name2msg_to_robot_type(msg_name);

      switch(msg_t)
        {
        case INITIALIZE:
          {
            int init;
            cin >> init;
            initialize( init );
          }
          break;
        case YOUR_NAME:
          {
            char name[81];
            cin >> name;
            your_name( name );
          }
          break;
        case YOUR_COLOUR:
          {
            char col[81];
            cin >> col;
            your_colour( col );
          }
          break;
        case GAME_OPTION:
          {
            int nr;
            double value;
            cin >> nr >> value;
            game_option( nr, value );
          }
          break;
        case GAME_STARTS:
          game_starts();
          break;
        case RADAR:
          {
            double dist, angle;
            int object;

            cin >> dist >> object >> angle;
            switch(object)
              {
              case NOOBJECT:
                radar_noobject( dist, angle );
                break;
              case ROBOT:
                radar_robot( dist, angle );
                break;
              case WALL:
                radar_wall( dist, angle );
                break;
              case SHOT:
                radar_shot( dist, angle );
                break;
              case COOKIE:
                radar_cookie( dist, angle );
                break;
              case MINE:
                radar_mine( dist, angle );
                break;
              default:
                cout << "Print" << _("Unknown Object seen!") << endl;
                break;
              }
          }
          break;          
        case COORDINATES:
          {
            double x, y, rotation;
            cin >> x >> y >> rotation;
            coordinates( x, y, rotation );
          }
          break;
        case INFO:
          {
            double time, speed, cannon_angle;
            cin >> time >> speed >> cannon_angle;
            info( time, speed, cannon_angle );
          }
          break;
        case ROBOT_INFO:
          {
            double energy;
            int enemy;
            cin >> energy >> enemy;
            robot_info( energy, enemy );
          }
          break;
        case ROTATION_REACHED:
          {
            int what;
            cin >> what;
            rotation_reached( what );
          }
          break;
        case ENERGY:
          {
            double en;
            cin >> en;
            energy( en );
          }
          break;
        case ROBOTS_LEFT:
          {
            int nr;
            cin >> nr;
            robots_left( nr );
          }
          break;
        case COLLISION:
          {
            int object;
            double angle;

            cin >> object >> angle;
            switch( object )
              {
              case NOOBJECT:
                collision_noobject( angle );
                break;
              case ROBOT:
                collision_robot( angle );
                break;
              case WALL:
                collision_wall( angle );
                break;
              case SHOT:
                collision_shot( angle );
                break;
              case COOKIE:
                collision_cookie( angle );
                break;
              case MINE:
                collision_mine( angle );
                break;
              default:
                cout << "Print " << _("Collided with Unknown Object!") << endl;
                break;
              }
          }
          break;
        case WARNING:
          {
            int type;
            char text[81];
            cin >> type;
            cin.getline(text,80,'\n');
            warning( type, text );
          }
          break;
        case DEAD:
          dead();
          break;
        case GAME_FINISHES:
          game_finishes();
          break;
        case EXIT_ROBOT:
          exit_robot();
          break;
        default:
          break;
        }
    }
}

// A function to convert the messagename to an enum message_to_robot_type
message_to_robot_type
name2msg_to_robot_type(char* msg_name)
{
  for(int i=0; message_to_robot[i].msg[0] != '\0'; i++)
    {
      if( strcmp(message_to_robot[i].msg, msg_name) == 0 )
        return (message_to_robot_type)i;
    }
  return UNKNOWN_MESSAGE_TO_ROBOT;
}
