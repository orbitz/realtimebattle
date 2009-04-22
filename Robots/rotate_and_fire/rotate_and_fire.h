
#ifndef __ROTATE_AND_FIRE__
#define __ROTATE_AND_FIRE__

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "Messagetypes.h"


message_to_robot_type name2msg_to_robot_type(char* msg_name);

class RotateAndFire
{
public:
  // The constructor and destructor.
  RotateAndFire               (const char* name, const char* colour);
  ~RotateAndFire              ();

  // These functions are equivalent to messages sent from the robot
  void robot_option           ( const int option_nr, const int value );
  void name                   ( const char* name );
  void colour                 ( const char* home, const char* away );
  void rotate                 ( const int what, const double vel );
  void rotate_to              ( const int what, const double vel,
                                const double angle );
  void rotate_amount          ( const int what, const double vel,
                                const double angle );
  void sweep                  ( const int what, const double vel,
                                const double left, const double right );
  void accelerate             ( const double amount );
  void brake                  ( const double amount );
  void shoot                  ( const double energy );
  void print                  ( const char* message );
  void debug                  ( const char* message );
  void debug_line             ( const double start_angle,
                                const double start_radius,
                                const double end_angle,
                                const double end_radius );
  void debug_circle           ( const double center_angle,
                                const double center_radius,
                                const double circle_radius );

  // These functions are called when the robot gets a message
  // of the corresponding type. There are several radar and
  // collision functions. These functions corresponds to each
  // object that the robot has collided with or found with the radar.
  void initialize             ( const int first_seq );
  void your_name              ( const char* prev_name );
  void your_colour            ( const char* colour );
  void game_option            ( const int option, const double value );
  void game_starts            ();
  void radar_noobject         ( const double dist, const double angle );
  void radar_robot            ( const double dist, const double angle );
  void radar_shot             ( const double dist, const double angle );
  void radar_wall             ( const double dist, const double angle );
  void radar_cookie           ( const double dist, const double angle );
  void radar_mine             ( const double dist, const double angle );
  void coordinates            ( const double x, const double y, const double rotation);
  void info                   ( const double time, const double speed,
                                const double cannon_angle );
  void robot_info             ( const double energy, int enemy );
  void rotation_reached       ( const int what );
  void energy                 ( const double energylevel );
  void robots_left            ( const int number_of_robots );
  void collision_noobject     ( const double angle );
  void collision_robot        ( const double angle );
  void collision_shot         ( const double angle );
  void collision_wall         ( const double angle );
  void collision_cookie       ( const double angle );
  void collision_mine         ( const double angle );
  void warning                ( const int type, const char* message );
  void dead                   ();
  void game_finishes          ();
  void exit_robot             ();

  // This function is called before checking messages.
  void pre_checking_messages  ();

  // This function goes through all messages and calls the
  // corresponding function whenever a message is recieved.
  void check_messages  ( );


  // Tells if robot is about to quit
  bool is_quitting () { return quitting; }

protected:

  // These variables contains the current values of the robot rotation,
  // acceleration and brake.
  double radar_and_cannon_rotate;
  double robot_rotate;
  double acceleration;
  double brake_value;

  // Is set if th robot gets the 'EXIT_ROBOT' message
  bool quitting;

  // The robot name and colour are set in the constructor
  char* robot_name;
  char* robot_colour;

  // Boolean variables to control if it is allowed to change robot
  // rotation and acceleration.
  bool rotate_allowed;
  bool acceleration_change_allowed;

  // Variables that controls how frequent the robot has been hit by shots.
  int shots_hit;
  double last_shot_hit_time;
  int number_of_robots_left;

  // The current time is stored in this variable.
  double current_time;

  // If we get coordinates, these are stored here.
  double current_x_coordinate;
  double current_y_coordinate;
  double current_robot_angle;

  // All game options are stored in these variables.
  double robot_max_rotate;
  double robot_cannon_max_rotate;
  double robot_radar_max_rotate;
  double robot_max_acceleration;
  double robot_min_acceleration;
  double robot_start_energy;
  double robot_max_energy;
  double robot_energy_levels;
  double shot_speed;
  double shot_min_energy;
  double shot_max_energy;
  double shot_energy_increase_speed;
  double timeout;
  double debug_level;
  int send_robot_coordinates;
};

#endif 
