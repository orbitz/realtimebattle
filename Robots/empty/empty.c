#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*#include "../messagetypes.h"*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

int main(int argc, char * argv[])
{
  printf("RobotOption 0 0\n"); 
  printf("Name Empty\n");
  printf("Colour ee2299 aaffaa\n");
  fflush(stdout);

  sleep(50);

  return(EXIT_SUCCESS);
}
