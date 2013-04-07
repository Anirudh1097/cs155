#include <stdio.h>

int main (){
  unsigned int write_byte = 0xbf;
  unsigned int already_written = 767;
  write_byte += 0x100;      /* write_byte is 0x17f now */
  already_written %= 0x100; /* already_written is 30 */
  /* afterwards padding is 97 (= 0x61) */
  unsigned int padding = (write_byte - already_written) % 0x100;
  if (padding < 10)
    padding += 0x100;

  printf("%d\n", padding);
}
