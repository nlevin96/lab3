#include "toy_stdio.h"


int main(int argc, char *argv[]) {
    
   toy_printf("Hex unsigned: %x\n", -1);
   toy_printf("Octal unsigned: %o\n", -1);
   toy_printf("Unsigned value: %u\n", 15);
   toy_printf("Unsigned value: %u\n", -15);
   toy_printf("Unsigned value: %d\n", -15);
   int integers_array[] = {1,-2,3,4,5};
   char * strings_array[] = {"This", "is", "array", "of", "strings"};
   int array_size = 5;
   int charsprinted = toy_printf("Print array of integers: %Ad\n", integers_array, array_size);
   toy_printf("charsprintes %d\n",charsprinted);
   toy_printf("Print array of strings: %As\n", strings_array, array_size);
   int check = toy_printf("my number %d\n",100);
   toy_printf("check num %d\n",check);
   toy_printf("Non-padded string: %s\n", "str");
   toy_printf("Right-padded string: %6s\n", "str");
   toy_printf("Left-added string: %-6s\n", "str");
   toy_printf("With numeric placeholders: %05d\n", -1);
   toy_printf("With numeric placeholders: %010d\n", -122);
   
   //original main[1]
   //int chars_printed = toy_printf("%x, %X\\n", 496351, 496351);
   //chars_printed += toy_printf("Welcome to \\c\\n");
   //chars_printed += toy_printf("Support for explicit\\N");
  //toy_printf("Printed %d chars\n", chars_printed);
   
   
   toy_printf("%b\n", 5);
   
}