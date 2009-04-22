#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

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

#include <math.h>
#include "Messagetypes.h"

using namespace std;



#define abs(x) ((x>0) ? (x) : -(x))

volatile double acceleration = 0.0;
volatile double robot_rotate = 0.0;
volatile double cannon_rotate = 0.0, radar_rotate = 0.0;
double tid, speed, radar_angle, cannon_angle, tid0, slumprotate = 1.0 ;
double maxspeed = 10.0; 
int robots_left = 20;
bool sweep = false, align_cro= true, align_cra = true, enemy = false;

volatile sig_atomic_t exit_robot = false;


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

void
check_messages(int sig)
{
  exit_robot = false;

  char msg_name[81];
  char text[81];
  message_to_robot_type msg_t;

  //timeval current_time;
  //gettimeofday(&current_time, NULL);
  //srand(current_time.tv_usec);

  cin.clear();
  while( !cin.eof() )
    {
      cin >> msg_name;
      msg_t = name2msg_to_robot_type(msg_name);
      switch(msg_t)
        {
        case INITIALIZE:
          int init;
          cin >> init ;
          if (init == 1)
            {
              cout << "Name Seek and Destroy" << endl;
              cout << "Colour dede11 de5500" << endl;
            }
          break;
        case GAME_STARTS:
          //          cout << "Rotate 1 " << robot_rotate << endl;
          cout << "Sweep 6 " << M_PI/2.0 << " " << -M_PI/2.0 << " " << M_PI/2.0 << endl;
          acceleration = 0.4;
          cout << "Accelerate " << acceleration << endl;
          break;
        case GAME_FINISHES:
          break;

        case INFO:
          cin >> tid >> speed  >> cannon_angle;
          if(rand() < (RAND_MAX/500))
            {
              slumprotate = 0.0;
              while (abs(slumprotate)<0.5)
                slumprotate = ((double)(rand())/(double)RAND_MAX-0.5)*2.0;
              // cout << "Print seek_and_destroy: slumptal = " << slumprotate << endl;
            }
          break;
          
        case ROBOTS_LEFT:
          cin >> robots_left;
          break;

        case RADAR:
          {
            double dist, energy;
            int object;

            cin >> dist >> object >> radar_angle;
            switch(object)
              {
              case ROBOT:
                enemy = true;
                energy = 20.0/(dist*dist+0.2)+0.5+2.0/double(robots_left);
                robot_rotate = M_PI/3.0;
                if (dist<3.0)
                  {
                    acceleration = 2.0;
                  }
                else
                  {
                    if (speed<maxspeed)
                      acceleration = 2.0;
                    else
                      acceleration = 0.0;
                  }
                cout << "RotateAmount 1 " << robot_rotate << " " << radar_angle << endl;
                cout << "RotateTo 6 " << -robot_rotate << " " << 0.0 << endl;
                cout << "Shoot " << energy << endl;
                cout << "Accelerate " << acceleration << endl;

                //cout << "Print kill" << endl;
                break;
              case WALL:
                {
                  if (enemy)
                    {
                      cout << "Sweep 6 " << M_PI/2.0 << " " << -M_PI/6.0 << " " << M_PI/6.0 << endl;
                      enemy = false;
                      slumprotate = -slumprotate;
                    }
                  if( dist < 1.0 )
                    {
                      cout << "Sweep 6 " << M_PI/2.0 << " " << -M_PI/2.0 << " " << M_PI/2.0 << endl;
                      if (speed > 0.1)
                        {
                          acceleration = 0.0;
                          robot_rotate = 0.0;
                          cout << "Brake 1" << endl;
                        }
                      else
                        {
                          robot_rotate = 5.0*slumprotate;
                          acceleration = 0.1;
                        }
                    }
                  else
                    {
                      if (dist<4.0)
                        {
                          if (speed > 2.0)
                            {
                              acceleration = 0.0 ;
                              robot_rotate = 2.0*slumprotate ;
                              cout << "Brake 0.7" << endl;
                            }
                          else
                            {
                              acceleration = 0.1;
                              robot_rotate = 1.0*slumprotate;
                            }
                        }
                      else
                        {
                          robot_rotate = 0.3*slumprotate;
                          if (speed<maxspeed)
                            acceleration = 2.0;
                          else
                            acceleration = 0.0;
                        }
                        
                    }
                    cout << "Rotate 1 " << robot_rotate << endl;
                    cout << "Accelerate " << acceleration << endl;
                    
                }
                break;
              case SHOT:
                if (speed < 1.0)
                  {
                    acceleration = 2.0;
                    robot_rotate = 1.0*slumprotate;
                  }
                else
                  robot_rotate = 3.0*slumprotate;
                cout << "Accelerate " << acceleration << endl;
                cout << "Rotate 1 " << robot_rotate << endl;
                //cout << "Print Help me!" << endl;
                //                cout << "Print A shot is approaching!" << endl;
                break;
              case COOKIE:
                robot_rotate = 0.0;
                if (speed<3.0)
                  acceleration = 2.0;
                else
                  acceleration = 0.0;
                cout << "Rotate 1 " << robot_rotate << endl;
                cout << "Accelerate " << acceleration << endl;
                
                //                cout << "Print Yummy! A cookie" << endl;
                break;

              case MINE:
                if (dist < 10)
                  {
                    acceleration = 0.0;
                    robot_rotate = 2.0*slumprotate;
                    if (speed > 0.4)
                      cout << "Brake 1" << endl;
                    cout << "Shoot 0.5" << endl;
                    cout << "Accelerate " << acceleration << endl;
                    cout << "Rotate 1 " << robot_rotate << endl;
                  }
                //cout << "Print Avoid this mine" << endl;
                break;
              }
          break;
          }

        case COLLISION:
          {
            int tmp;
            double energy_diff = 0, coll_angle;
            cin >> tmp >> coll_angle;
            switch(tmp)
              {
              case ROBOT: 
                if (abs(coll_angle)<M_PI/6.0 && energy_diff<5)
                  acceleration = 2.0;
                else
                  {
                    acceleration = -0.5;
                    robot_rotate = 5.0*slumprotate;
                  }
                cout << "Accelerate " << acceleration << endl; 
                cout << "Rotate 1 " << robot_rotate << endl;
                break;
              case SHOT:
                if (abs(coll_angle)<M_PI/4.0)
                  {
                    if(speed<3.0)
                      acceleration = 2.0;
                    else
                      acceleration = 0.0;
                    robot_rotate = 5.0*slumprotate;
                  }
                cout << "Rotate 1 " << robot_rotate << endl;
                cout << "Accelerate " << acceleration << endl; 
                break;
              case MINE: 
                cout << "Print Oh no! A mine!" << endl; 
                break;
              case COOKIE: 
                cout << "Print Cookie eaten!" << endl; 
                break;
              case WALL:
                acceleration = 0.0;
                cout << "Accelerate " << acceleration << endl; 
                break;
              }
            //if( enegry_diff != 0 ) cout << "Print energy changed by " << enegry_diff << endl;
          }
          break;
        case WARNING:
          cin.getline(text,80,'\n');
          cout << "Print Help, I got a yellow card!  " << text << endl;
          break;
        case EXIT_ROBOT:
          cout << "Print Shutting down and leaving" << endl;
          exit_robot = true;
          break;
        default:
          break;
        }
    }
  signal (sig, check_messages);
}

int 
main(int argc, char * argv[])
{
  sigset_t usr1set;

  //robot_rotate = 0.53;


  //check_messages(SIGUSR1);
  signal(SIGUSR1, check_messages);

  // libpthread seems to block USR1 sometimes for some reason
  sigemptyset(&usr1set);
  sigaddset(&usr1set, SIGUSR1);
  sigprocmask(SIG_UNBLOCK, &usr1set, NULL);
  

  cout << "RobotOption " << (int)SIGNAL << " " << (int)SIGUSR1 << endl;

  for(;;sleep(1))
    {
      if( exit_robot ) 
        {
          //          cerr << "Fire: Exiting normally!" << endl;
          return(EXIT_SUCCESS);
        }
    }
  return(EXIT_SUCCESS);
}
