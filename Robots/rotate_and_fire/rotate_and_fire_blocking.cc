
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "rotate_and_fire.h"

#include <stdlib.h>

// This version of rotate_and_fire uses blocking STDIN.

int 
main(int argc, char * argv[])
{
  class RotateAndFire raf_obj("Rotate&Fire-blocking", "9977dd");


  // STDIN blocks, signals are not used. 

  raf_obj.robot_option( USE_NON_BLOCKING, false );
  //  raf_obj.robot_option( SIGNAL, SIGUSR1 );



  // STDIN blocks, signals are not used. 

  raf_obj.robot_option( USE_NON_BLOCKING, false );
  //  raf_obj.robot_option( SIGNAL, SIGUSR1 );

  // STDIN will block instead of being eof when all messages have 
  // been read, therefore 'check_messages()' will not return until 
  // the robot shout quit. 
  raf_obj.check_messages();

  return( EXIT_SUCCESS );
}
