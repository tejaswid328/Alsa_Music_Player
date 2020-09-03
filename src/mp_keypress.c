#include <termios.h>
#include "music_player.h"

MP_INT32 kbhit(MP_VOID)
{
  struct termios oldt, newt;
  MP_INT32 oldf;
  MP_INT32 return_value = MP_TRUE;
 
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  if(fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK) >= 0)
  {   MP_INT32 ch;
      ch = getchar();
      tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  if( fcntl(STDIN_FILENO, F_SETFL, oldf) >= 0) 
  {
      if(ch != EOF)
      {
          ungetc(ch, stdin);
          return_value = MP_TRUE;
      }
      else
      {
          return_value = MP_FALSE;
      }
  }
  }
  return return_value;
}

