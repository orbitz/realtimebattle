
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "rotate_and_fire.h"

#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

// This version of rotate_and_fire uses signals.


class RotateAndFire raf_obj("Rotate&Fire-signal", "aaaaff");


// We need a function to be called when a signal arrives
void
handle_signal(int signr)
{
  raf_obj.check_messages();

  // Call this function next time too
  signal(signr, handle_signal);
}


int 
main(int argc, char * argv[])
{
  // Use non blocking and signals

  raf_obj.robot_option( USE_NON_BLOCKING, true );
  raf_obj.robot_option( SIGNAL, SIGUSR1 );


  // Initialize signal function
  sigset_t usr1set;
  signal(SIGUSR1, handle_signal);


  // libpthread seems to block USR1 sometimes for some reason
  sigemptyset(&usr1set);
  sigaddset(&usr1set, SIGUSR1);
  sigprocmask(SIG_UNBLOCK, &usr1set, NULL);



  while( !raf_obj.is_quitting() )
    {
      // Here you can to whatever you like, but we have nothing 
      // to do...
      sleep(1);
    }
  return( EXIT_SUCCESS );
}
