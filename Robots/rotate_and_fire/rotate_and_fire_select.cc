
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
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

#include "rotate_and_fire.h"

// This version of rotate_and_fire uses 'select' to check 
// for new messages. 

int 
main(int argc, char * argv[])
{
  class RotateAndFire raf_obj("Rotate&Fire-select", "66ccdd");


  // STDIN block (otherwise, we won't get complete messages from the server) and signals are not used

  raf_obj.robot_option( USE_NON_BLOCKING, false );
  //  raf_obj.robot_option( SIGNAL, SIGUSR1 );

  // Make a set of file descriptor to check for.
  // In this case only STDIN
  
  fd_set file_descriptor_set;
  FD_ZERO( &file_descriptor_set );
  FD_SET( STDIN_FILENO, &file_descriptor_set );  



  while( !raf_obj.is_quitting() )
    {
      raf_obj.check_messages();

      // Here you can do what you like, when finished ...



      // ... wait for the next message
      select( FD_SETSIZE, &file_descriptor_set, NULL, NULL, NULL );
    }

  return( EXIT_SUCCESS );
}
