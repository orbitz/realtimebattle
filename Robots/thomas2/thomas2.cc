/*
RealTimeBattle, a robot programming game for Unix
Copyright (C) 1998  Erik Ouchterlony and Ragnar Ouchterlony

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foundation,
Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <glib.h>
#include <math.h>
#include <Messagetypes.h>
#define abs(x) ((x>0) ? (x) : -(x))
#define sgn(x) ((x>0) ? 1 : -1)
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

volatile double acceleration = 0.0;
volatile double robot_rotate = 0.0;
volatile double cannon_rotate = 0.0, radar_rotate = 0.0;
double tid=0.0, radar_angle=0.0, prev_radar_angle, cannon_angle, tid0, slumprotate = 1.0 ;
double maxspeed =3.0, robotmaxrotate, cannonmaxrotate, radarmaxrotate, dist=0.0, prev_dist;
double robotstartenergy, robotmaxenergy, robotenergylevels, shotspeed, shotmaxenergy;
double  shotminenergy, shotenergyincreasespeed,timeout, rotend=0.0, broms=0.0;
double prev_speed, speed=0.0, prev_tid, energy, sweepleft, sweepright, prev_energy;
int robots_left = 20, rotating, enemy = 0, cookie = 0, flee = 0, wallcoll=0;
bool sweep = false, align_cro= true, align_cra = true;

volatile sig_atomic_t exit_robot = false;
static GTimer* timer;

using std::endl;
using std::cin;
using std::cout;

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

  gulong microsecs;
  g_timer_elapsed(timer, &microsecs);
  srand(microsecs);

  cin.clear();
  while( !cin.eof() )
    {
   	std::cin >> msg_name;
      msg_t = name2msg_to_robot_type(msg_name);
      switch(msg_t)
        {
        case INITIALIZE:
          int init;
          cin >> init ;
          if (init == 1)
            {
              cout << "Name Thomas2" << endl;
              cout << "Colour 993322  777777" << endl;
            }
          break;
        case GAME_STARTS:
          //          cout << "Rotate 1 " << robot_rotate << endl;
          cout << "Sweep 6 " << cannonmaxrotate << " " << -M_PI/2.0 << " " << M_PI/2.0 << endl;
          acceleration = 0.4;
          cout << "Accelerate " << acceleration << endl;
          break;
        case GAME_FINISHES:
          break;

        case GAME_OPTION:
          int optnr;
          double value;
          cin >> optnr >> value ;
          switch(optnr)
            {
            case ROBOT_MAX_ROTATE:
              robotmaxrotate = value;
              break;
            case ROBOT_CANNON_MAX_ROTATE:
              cannonmaxrotate = value;
              break;
            case ROBOT_RADAR_MAX_ROTATE:
              radarmaxrotate = value ;
              break;
            case ROBOT_START_ENERGY:
              robotstartenergy = value;
              break;
            case ROBOT_MAX_ENERGY:
              robotmaxenergy = value;
              break;
            case ROBOT_ENERGY_LEVELS:
              robotenergylevels = value;
              break;
            case SHOT_SPEED:
              shotspeed = value;
              break;
            case SHOT_MIN_ENERGY:
              shotminenergy = value;
              break;
            case SHOT_MAX_ENERGY:
              shotmaxenergy = value;
              break;
            case SHOT_ENERGY_INCREASE_SPEED:
              shotenergyincreasespeed = value;
              break;
            case TIMEOUT:
              timeout = value;
              break;
            }
          break;
/**        case SAVE_DATA:
          cout << "SaveDataFinished" << endl;
          break;
*/

        case INFO:
          prev_speed = speed;
          prev_tid = tid;
          cin >> tid >> speed  >> cannon_angle;
          break;

        case ENERGY:
          prev_energy=energy;
          cin >> energy;
          //          if (prev_energy != energy)
          //            cout << "Print Energiförlust: " << prev_energy-energy << endl;
          break;
          
        case ROBOTS_LEFT:
          cin >> robots_left;
          break;

        case RADAR:
          {
            double x1,x2,s,rot;
            int object;

            prev_dist = dist;
            prev_radar_angle = radar_angle;
            cin >> dist >> object >> radar_angle;
            switch(object)
              {
              case ROBOT:
                enemy = max(15-robots_left,1);
                broms=1.0;
                if (speed<maxspeed)
                  acceleration = 2.0;
                else
                  acceleration = 0.0;

                radar_angle=asin(sin(radar_angle));

                sweepleft=radar_angle-M_PI/20.0;
                sweepright=radar_angle+M_PI/20.0;
                if (abs(radar_angle)>M_PI/20.0)
                  {
                    cout << "RotateAmount 1 " << robotmaxrotate << " " << radar_angle << endl;
                    cout << "RotateAmount 6 " << robotmaxrotate << " " << -radar_angle << endl; 
                  }
                else
                  {
                        cout << "Sweep 6 " << cannonmaxrotate << " " << sweepleft << " " << sweepright << endl;
                  }
                cout << "Accelerate " << acceleration << endl;
                cout << "Shoot 2" << endl;
                //    cout << "Print Skjuter" << endl;
                cout << "Brake " << broms << endl;
                break;
              case WALL:
                if (flee>1)
                  flee--;
                else
                  {
                    if (flee==1)
                      {
                        sweepleft=-M_PI/3.0;
                        sweepright=M_PI/3.0;
                        flee--;
                      }
                    maxspeed=3.0;
                  }

                if (speed < maxspeed)
                  {
                    acceleration = 2.0;
                    broms = 0.0;
                  }
                else
                  {
                    acceleration = 0.0;
                    broms = 0.3;
                  }
                
                if (enemy>0)
                  {
                    enemy--;
                    if (abs(radar_angle)>M_PI/20.0)
                      {
                        acceleration=0.0;
                        broms=1.0;
                      }
                    else
                      {
                        sweepleft=sweepleft-M_PI/200.0;
                        sweepright=sweepright+M_PI/200.0;
                        cout << "Sweep 6 " << cannonmaxrotate << " " << sweepleft << " " << sweepright << endl;
                      }
                    //cout << "Debug enemy " << enemy << " radar " << radar_angle << endl;
                  }
                else
                  {
                    if (cookie>0)
                      {
                        cookie--;
                        sweepleft=sweepleft-M_PI/200.0;
                        sweepright=sweepright+M_PI/200.0;
                        if (abs(radar_angle)>M_PI/20.0)
                          {
                            acceleration=0.0;
                            broms=1.0;
                          }
                        else
                          {
                            sweepleft=sweepleft-M_PI/200.0;
                            sweepright=sweepright+M_PI/200.0;
                            cout << "Sweep 6 " << cannonmaxrotate << " " << sweepleft << " " << sweepright << endl;
                          }
                        // cout << "Debug cookie" << cookie << endl;
                      }
                    else
                      {
                        x1=abs(prev_dist * sin(prev_radar_angle));
                        x2=abs(dist * sin(radar_angle));

//                         if (wallcoll > 0)
//                           if (speed < 2.0)
//                             {
//                               if (dist > 2.0)
//                                 rot = sgn(radar_angle)*robotmaxrotate;
//                               else
//                                 rot = robotmaxrotate;
//                             }
//                           else
//                             wallcoll = 0;
//                         else
                          {
                            if (x1<=x2) 
                              rot = 0;
                            else
                              {
                                s = (prev_dist*cos(prev_radar_angle) - dist*cos(radar_angle)- (prev_speed+speed)/2.0*(tid-prev_tid))*
                                  (radar_angle-prev_radar_angle);
                                if (s>0)
                                  rot = robotmaxrotate;
                                else
                                  rot = -robotmaxrotate;
                              }
                          }

                            //cout << "Print " << s << endl;
                        if (((prev_dist/(dist+1e-7)>1.2) && (dist<2.0)) || ((dist<0.5) && (dist<prev_dist)))
                          {
                            if (radar_angle<prev_radar_angle)
                              {
                                sweepleft = prev_radar_angle;
                                sweepright = prev_radar_angle+M_PI/2.0;
                              }
                            else
                              {
                                sweepleft = prev_radar_angle-M_PI/2.0;
                                sweepright = prev_radar_angle;
                                  }
                          }
                        else
                          {
                            sweepleft=sweepleft-M_PI/200.0;
                            sweepright=sweepright+M_PI/200.0;
                          }
                      }
                    cout << "Rotate 1 " << rot << endl;
                    cout << "Sweep 6 " << cannonmaxrotate << " " << sweepleft << " " << sweepright << endl;
                    //cout << "Debug Rot " << rot << " Radar " << radar_angle << endl;
                    
                  }
                cout << "Accelerate " << acceleration << endl;
                cout << "Brake " << broms << endl;
                break;
              case SHOT:
                break;
              case COOKIE:
                cookie = 7;
                broms = 1.0;
                robot_rotate = 0.0;
                sweepleft=radar_angle-M_PI/20.0;
                sweepright=radar_angle+M_PI/20.0;
                if (speed<maxspeed)
                  acceleration = 2.0;
                else
                  acceleration = 0.0;

                radar_angle = asin(sin(radar_angle));
                cout << "RotateAmount 1 " << robotmaxrotate << " " << radar_angle << endl;
                cout << "Accelerate " << acceleration << endl;
                cout << "RotateAmount 6 " << robotmaxrotate << " " << -radar_angle << endl;
                cout << "Brake " << broms << endl;
                break;

              case MINE:
                if (dist < 5.0)
                  {
                    cout << "Shoot " << shotminenergy << endl;
                    //cout << "Print Skjuter" << endl;
                  }
                break;

/*              case EXPLOSION:
                cout << "Print Avoid! Explosion" << endl;
                break;
*/
              }
          break;
          }

        case COLLISION:
          {
            int tmp;
            double coll_angle;
            cin >> tmp >> coll_angle;
            switch(tmp)
              {
              case ROBOT: 
                enemy=0;
                break;
              case SHOT:
                //cout << "Print Skjuten!" << endl;
                flee = 10;
                enemy=0;
                maxspeed = 10.0;
                acceleration = 2.0;
                cout << "Accelerate " << acceleration << endl;
                sweepleft = coll_angle - M_PI/4.0;
                sweepright = coll_angle + M_PI/4.0;
                cout << "Sweep 6 " << cannonmaxrotate << " " << sweepleft << " " << sweepright << endl;
                break;
              case MINE: 
                cout << "Print Oh no! A mine!" << endl; 
                break;
              case COOKIE: 
                cout << "Print Cookie eaten!" << endl; 
                cookie = 0;
                break;
              case WALL:
                wallcoll=10;
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
//robot_rotate = 0.53;

  timer = g_timer_new();
  g_timer_reset(timer);
  srand(time(0));
  //check_messages(SIGUSR1);
  signal(SIGUSR1, check_messages);

  cout << "RobotOption " << SEND_SIGNAL << " " << true << endl;

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


