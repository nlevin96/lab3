#include "toy_stdio.h"


int main(int argc, char *argv[]) {
   int chars_printed=0;
   chars_printed=toy_printf("Hex unsigned: %x\n", -1);
   toy_printf("chars_printed: %d\n",chars_printed);
   toy_printf("Octal unsigned: %o\n", -1);
   toy_printf("Unsigned value: %u\n", 15);
   toy_printf("Unsigned value: %u\n", -15);
   toy_printf("Unsigned value: %d\n", -15);
   int integers_array[] = {1,-2,3,4,5};
   char * strings_array[] = {"This", "is", "array", "of", "strings"};
   int array_size = 5;
   chars_printed=toy_printf("Print array of integers: %Ad\n", integers_array, array_size);
   toy_printf("chars_printed: %d\n",chars_printed);
   toy_printf("Print array of strings: %As\n", strings_array, array_size);
   toy_printf("Non-padded string: %s\n", "str");
   toy_printf("Right-padded string: %6s\n", "str");
   toy_printf("Left-added string: %-6s\n", "str");
   toy_printf("With numeric placeholders: %05d\n", -1);
   toy_printf("With numeric placeholders: %010d\n", -122);
   toy_printf("5 bin value: %b\n", 5);

   
}