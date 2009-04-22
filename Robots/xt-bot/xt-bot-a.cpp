/****************************************************************************
xt-bot-a.cpp
Einfacher Roboter in C++. Er verfolgt seine Gegner faehrt nicht gegen Waende,
sammelt Keckse und Versucht die Minen zu zerschiessen, allerdings hat er
leichte Ladehemmungen.

Copyright (C) 2001 xt

changed by Falko Menge (20. Nov 2004):
I fixed the bug of the self-termination after the first game.
Now it resets itsself after every game.


This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of
the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

****************************************************************************/

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <signal.h>
#include <unistd.h>
#include <Messagetypes.h>
using std::cout;
using std::cin;
using std::endl;
void message(int);
class bot
 {
  public:
   int Initialize;
   char YourName[80];
   int YourColour;
   int GameOption_Optionsnummer;
   double GameOption_value;
   double Radar_Entfernung;
   int Radar_typ;
   double Radar_Winkel;
   double Info_Zeit;
   double Info_Geschwindigkeit;
   double Info_Kanonen_Winkel;
   double RobotInfo_Energie_Level;
   int RobotInfo_Team_Mitglied;
   int RotationReached_wer;
   double Energy_Level;
   int RobotsLeft_anz;
   int Collision_typ;
   double Collision_winkel;
   int Warning_typ;
   char Warning_string[80];

   double fahr_winkel;
 //public:

   bot(){ fahr_winkel = 0.0; }
   void init();
   void options();
   void nachricht(); 
   void debug(char *);
   void print(char *);
   void beschleunigen(double);
   void rotateamount(int, double, double);
   void sweep(int,double,double,double);
 }; 

void bot::sweep(int was, double velocty, double left, double right)
 {
  cout<<"Sweep "<<was<<" "<<velocty<<" "<<left<<" "<<right<<endl;
 }
void bot::rotateamount(int was, double velocity, double winkel)
 {
  cout<<"RotateAmount "<<was<<" "<<velocity<<" "<<winkel<<endl;
 }

void bot::beschleunigen(double i)
 {
  cout<<"Accelerate "<<i<<endl;
 }

void bot::print(char *a)
 {
  cout<<"Print "<<a<<endl;
 } 

void bot::options()
 {
  cout<<"RobotOption "<<USE_NON_BLOCKING<<" "<<1<<endl;
  cout<<"RobotOption "<<SIGNAL<<" "<<SIGUSR1<<endl;
 }

void bot::init()
 {
  cout<<"Colour 00FF00 00FF00"<<endl;
  cout<<"Name xt2-bot A"<<endl;
 }

void bot::debug(char *str)
 {
  cout<<"Debug "<<str<<endl;
 }

void bot::nachricht()
 {
  char typ[81];
  
  cin.clear();
  while( !cin.eof())
  {  
    cin>>typ;
  
    if(!strcmp(typ,"Initialize"))
    cin>>Initialize;

    else if(!strcmp(typ,"YourName"))
      cin>>YourName;

    else if(!strcmp(typ,"GameOption"))
      cin>>GameOption_Optionsnummer>>GameOption_value;
      // that line seems to be wrong because there is more than one option

    else if(!strcmp(typ,"GameStarts"))
      ;
    else if(!strcmp(typ,"Radar"))
     {
      cin>>Radar_Entfernung>>Radar_typ>>Radar_Winkel;

      if(Radar_typ == 0)
      {
      if(Radar_Winkel < -0.3||Radar_Winkel > 0.3)
       {
         if(Radar_Winkel > 0)
          {
          if(Radar_Entfernung > 6)
             cout<<"RotateAmount "<<1<<" "<<7<<" "<<0.4<<endl;
          else
              cout<<"RotateAmount "<<1<<" "<<7<<" "<<0.7<<endl;
           }
         else
          {
           if(Radar_Entfernung >6)
             cout<<"RotateAmount "<<1<<" "<<7<<" "<<-0.4<<endl;
           else
             cout<<"RotateAmount "<<1<<" "<<7<<" "<<-0.7<<endl;
          }
        }
       if(Radar_Winkel > -0.4&&Radar_Winkel<0.4)
         {
          cout<<"Debug gegner! winkel:"<<Radar_Winkel<<" entfernung: "
             <<Radar_Entfernung<<endl;
          cout<<"Shoot 30"<<endl;
         }
      }

      else if(Radar_typ == 2)
       {
         if(Radar_Winkel > -0.4 && Radar_Winkel < 0.4
            && Radar_Entfernung < 7) //wand seite
            cout<<"RotateAmount "<<1<<" "<<7<<" "<<0.1<<endl;
         if(Radar_Winkel > -0.4 && Radar_Winkel < 0.4
            && Radar_Entfernung < 2) // wand frontal
            cout<<"RotateAmount "<<1<<" "<<7<<" "<<0.6<<endl;
         if(Radar_Winkel > -0.1 && Radar_Winkel < 0.2
            && Radar_Entfernung < 0.01) //stehe an der wand
            cout<<"RotateAmount "<<1<<" "<<7<<" "<<1<<endl,beschleunigen(1);
        }
      else if(Radar_typ == 4&&Radar_Winkel > -0.3&&Radar_Winkel < 0.3 )
       {
        cout<<"Shoot 0.5"<<endl;
       }
      else if(Radar_typ == 3)
       {
        cout<<"Debug oh lecker ich sehe einen kecks!"<<endl;
        if(Radar_Winkel > 0.3)
          cout<<"RotateAmount "<<1<<" "<<7<<" "<<0.9<<endl;
        else if(Radar_Winkel < -0.3)
           cout<<"RotateAmount "<<1<<" "<<7<<" "<<-0.9<<endl;
        beschleunigen(0.5);
       }
     }
    else if(!strcmp(typ,"Info"))
     {
      cin>>Info_Zeit;
      cin>>Info_Geschwindigkeit;
      cin>>Info_Kanonen_Winkel;
     }
   else if(!strcmp(typ,"RobotInfo"))
    {
     cin>>RobotInfo_Energie_Level>>RobotInfo_Team_Mitglied;
     cout<<"Debug der gegner hat noch "<<RobotInfo_Energie_Level<<endl;
    }
   else if(!strcmp(typ,"RotionReached"))
     cin>>RotationReached_wer;

   else if(!strcmp(typ,"Energy"))
    {
     cin>>Energy_Level;
     //cout<<"Debug ich habe noch:"<<Energy_Level<<" lebenspunkte"<<endl;
    }

   else if(!strcmp(typ,"RobotsLeft"))
     cin>>RobotsLeft_anz;

   else if(!strcmp(typ,"Collision"))
    {
     cin>>Collision_typ;
     cin>>Collision_winkel;
    }

   else if(!strcmp(typ,"Warning"))
    {
     cin>>Warning_typ;
     cin>>Warning_string[80];

     cout<<"Debug "<<Warning_string<<endl;
    }
   else if(!strcmp(typ,""))
    ;
   else if(!strcmp(typ,"GameFinishes"))
    {
     //exit(0);
     // reset for the next round
     Radar_Entfernung        = 0.0;
     Radar_typ               = -1;
     Radar_Winkel            = 0.0;
     Info_Zeit               = 0.0;
     Info_Geschwindigkeit    = 0.0;
     Info_Kanonen_Winkel     = 0.0;
     RobotInfo_Energie_Level = 0.0;
     RobotInfo_Team_Mitglied = 0;
     RotationReached_wer     = 0;
     Energy_Level            = 0.0;
     RobotsLeft_anz          = 0;
     Collision_typ           = -1;
     Collision_winkel        = 0;
     Warning_typ             = 0;
     fahr_winkel             = 0.0;
    }
   else
     cout<<"Debug unbekanntes dingens ("<<typ<<") "<<endl;
  }//while
 }//nachricht

bot xt;
int main()
 {
  double i;
  signal(SIGUSR1, message);

  sigset_t usr1set;
  sigemptyset(&usr1set);
  sigaddset(&usr1set, SIGUSR1);
  sigprocmask(SIG_UNBLOCK, &usr1set, NULL);

  xt.init();
  xt.options();
  xt.print("los gehts!");

  //cout<<"RotateAmount "<<2<<" "<<-2.5<<endl;//kanone nachfuehren.

  while(1)
   {
     xt.sweep(4,14,-1,1);
     //xt.sweep(2,7,-0.2,0.2);

     xt.beschleunigen(0.4);
     //xt.rotateamount(1,3,1);

    sleep(1);
  }

  return(0);
 }

void message(int sig)
 {
   signal(SIGUSR1,SIG_IGN);
   xt.nachricht();
   signal(SIGUSR1, message);
}
