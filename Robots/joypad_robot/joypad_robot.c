/**************************************************
 joypad_robot.c -- Copyright(c) 1999 Jiro Sekiba <jir@hello.to>
 This program is destributed under the GPL.
 **************************************************/
#include<stdio.h>
#include<fcntl.h>
#include<sys/time.h>
#include<sys/types.h>
#include<unistd.h>

#define __USE_GNU /* for M_PIl etc */
#include<math.h>

#include<linux/joystick.h>

#include <Messagetypes.h>
/* if you can't compile, comment out prev line and uncomment next line */
/* #include<Messagetypes.h> */

double GAME_OPT[13];
double rotate_angle = M_PIl/36; /* 5 deg */
int joy_fd = -1;


void initialize(int num)
{
  if(num == 1)
  {
    puts("Name Joybot");
    puts("Colour B2DD32 32DDBs");
    
    fflush(stdout);
  }
}

void gameoption(int type, double value)
{
  GAME_OPT[type] = value;
}

void debug(char *msg)
{
  printf("Debug %s\n",msg);
}

void right(void)
{
  printf("RotateAmount 1 %f -%f\n",GAME_OPT[ROBOT_MAX_ROTATE],rotate_angle);
}

void left(void)
{
  printf("RotateAmount 1 %f %f\n",GAME_OPT[ROBOT_MAX_ROTATE],rotate_angle);
  
}

void shoot(void)
{
  printf("Shoot %f",GAME_OPT[SHOT_MAX_ENERGY]);
}

void accel_on(void)
{
  printf("Accelerate %f\n",GAME_OPT[ROBOT_MAX_ACCELERATION]/2);
}

void accel_off(void)
{
  printf("Accelerate 0.0\n");
}

void accel_back(void)
{
  printf("Accelerate %f\n",GAME_OPT[ROBOT_MIN_ACCELERATION]);
}

void brake_on(void)
{
  puts("Brake 1.0");
}

void brake_off(void)
{
  puts("Brake 0.0");
}

void idle(void)
{
      /* do something for idle time */
}

void main_loop(void)
{
  struct timeval tv;
  fd_set rfds;
  int retval;
  struct js_event event;

  char buf[2048];
  int type;
  double value;
  int num;

      /* Wait up to one seconds. */
  tv.tv_sec = 1;
  tv.tv_usec = 0;

  while(1)
  {
    FD_ZERO(&rfds);
    FD_SET(0,&rfds);
    FD_SET(joy_fd,&rfds);
    retval = select(6,&rfds,NULL,NULL,&tv);

    if(FD_ISSET(joy_fd,&rfds))
    {
      read(joy_fd, &event, sizeof(struct js_event));
      switch(event.type)
      {

        case JS_EVENT_BUTTON:
            switch(event.number)
            {
              case 0: /* accel */
                  if(event.value) /* pressed */
                      accel_on();
                  else  /* released */
                      accel_off();
                  break;
              case 1: /* brake */
                  if(event.value) /* pressed */
                      brake_on();
                  else/* released */
                      brake_off();
                  break;
              case 2: /* shoot */
                  if(event.value) /* pressed */
                      shoot();
                  break;
              case 3:  /* back */
                  if(event.value) /* pressed */
                      accel_back();
                  else /* released */
                      accel_off();
                  break;
              case 4:
                      /* do somthing if your joypad has button 5 */
                  break;
              case 5:
                      /* do somthing if your joypad has button 6 */
                  break;
              default:
                  break; /* do nothing */
                      
            }
            break;
        case JS_EVENT_AXIS:
                /* AXIS and calibration depend on your joystick.
                   You should hack for you environment. */
            switch(event.number)
            {
              case 0: /* left/right*/
                  if(event.value > 1000)  /* right */
                  {
                    right();
                  }
                  else if(event.value < 100) /* left */
                  {
                    left();
                  }
                  break;
              case 1: /* up/down */
                  if(event.value > 1000) /* down */
                  {
                          /* do if you want to do something when push down */
                  }
                  else if( event.value < 100 ) /* up */
                  {
                          /* do if you want to do something when push up */
                  }
                  break;
              default:
                      /* do nothing */
		  break;
            }
            break;
        default:
            break;
      }
    }
    else if( FD_ISSET(0,&rfds) ) /* message from RTB */
    {
      fgets(buf,2048,stdin);
      if(strncmp(buf,"Initialize",10) == 0 )
      {
        sscanf(buf,"Initialize %i",&num);
        initialize(num);
      }
      else if(strncmp(buf,"GameOption",10) == 0 )
      {
        sscanf(buf,"GameOption %i %lf",&type,&value);
        gameoption(type,value);

      }
      else if(strncmp(buf,"Dead",4) == 0 )
      {
        break;
      }
    }
    else
    {
      idle();
    }
    fflush(stdout);
  }
}

int open_joystick(char *name)
{
  joy_fd = open(name, O_RDONLY);
  if(joy_fd < 0)
  {
    fprintf(stderr,"Can't open file %s\n",name);
    return(1);
  }
  return(0);
}

int main(void)
{
  if(open_joystick("/dev/js0"))
	  if (open_joystick("/dev/input/js0")) {
  			printf("RobotOption %i 1\n",USE_NON_BLOCKING);
    			puts("Name Joybot");
			puts("Colour B2DD32 32DDBs");
  			fflush(stdout);
    			fprintf(stderr,"Exiting joystick bot ...\n");
			sleep(3);
    			puts("Print Could not open the joystick device, exiting ...");
			return -1;
	 }

  printf("RobotOption %i 1\n",USE_NON_BLOCKING);
  fflush(stdout);
  main_loop();
  
}
