// C implementation of hexdump main function

#include "hexfuncs.h"  // this is the only header file which may be included!

int main(void) {
    char sbuf[17];
    int index = 0;
    sbuf[16] = '\0';
    while(1) {
        long bytes = hex_read(sbuf); //Get bytes read from user
        if(bytes == 0) {
            break;
        }
        char offset[9]; //Format a string to store offset
        hex_format_offset(index * 16, offset);
        hex_write_string(offset);
        hex_write_string(": "); //Print out offset to start
        char end[17];
        end[bytes] = '\0';
        for(int i = 0; i < bytes; i++) { //Loop to go through each byte and convert to hex
            char hex[3];
            hex_format_byte_as_hex((long) sbuf[i], hex);
            hex_write_string(hex);
            hex_write_string(" ");
            end[i] = hex_to_printable(sbuf[i]); //Store the original values here to print at end of line
        }
        if(bytes < 16) { //If the byte is less than 16 then print out spaces to compensate in the output
            for(int i = 0; i < 16-bytes; i++) {
                hex_write_string("   ");
            }
        }
        hex_write_string(" ");
        hex_write_string(end); //Print final string
        hex_write_string("\n");
        index++; //advance offset/index
    }
}
