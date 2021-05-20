// C implementation of hexdump functions

#include <unistd.h>  // this is the only system header file you may include!
#include "hexfuncs.h"

// TODO: add function implementations here
char convertBaseTentoHex (int n) {
    if (n < 10 && n >= 0) {
        char test = n + 48;
        return test;
    } else {
        switch (n) {
            case 10:
                return 'a';
            case 11:
                return 'b';
            case 12:
                return 'c';
            case 13:
                return 'd';
            case 14:
                return 'e';
            case 15:
                return 'f';
        }
    }
    return '0';
}

// Read up to 16 bytes from standard input into data_buf.
// Returns the number of characters read.
long hex_read(char data_buf[]) {
    long bytes = read(STDIN_FILENO, data_buf, 16); //Read in bytes and store in bytes long
    data_buf[bytes] = '\0'; //Add null termiantor right after the amount of input received
    return bytes; //Return number of bytes read
}
// Write given nul-terminated string to standard output.
void hex_write_string(const char s[]) {
    int counter = 0; //Counter to count
    while(s[counter] != '\0') {
        counter++; //Count while there is no null terminator
    }
    write(STDOUT_FILENO, s, counter); //Write however many bytes before the null terminator
}
// Format a long value as an offset string consisting of exactly 8
// hex digits.  The formatted offset is stored in sbuf, which must
// have enough room for a string of length 8.
void hex_format_offset(long offset, char sbuf[]) {
    long remainder = 0;
    int index = 7;
    for(int i = 0; i < 8; i++) { //Make the entire array into 0's
        sbuf[i] = '0';
    }
    while(offset != 0) { //Convert offset to hex
        remainder = offset % 16;
        offset /= 16;
        sbuf[index] = convertBaseTentoHex(remainder);
        index--;
    }
    sbuf[8] = '\0'; //Add null terminator
}
// Format a byte value (in the range 0-255) as string consisting
// of two hex digits.  The string is stored in sbuf.

//Use bitshfiting trickery to format last 4 and first 4 bytes as hex
void hex_format_byte_as_hex(long byteval, char sbuf[]) {
    int firstdigit = byteval >> 4; //Shift to get last 4 bits
    int seconddigit = byteval & 15; //bitwise AND to get first 4 bits

    if(firstdigit >= 0 && firstdigit <=9) {
        sbuf[0] = (char) firstdigit + 48;
    }
    else {
        sbuf[0] = (char) firstdigit + 87;
    }
    if(seconddigit >= 0 && seconddigit <=9) {
        sbuf[1] = (char) seconddigit + 48;
    }
    else {
        sbuf[1] = (char) seconddigit + 87;
    }

    /*sbuf[0] = convertBaseTentoHex(firstdigit); //Add bits to array accordingly
    sbuf[1] = convertBaseTentoHex(seconddigit);*/
    sbuf[2] = '\0'; //Null terminator added
}
// Convert a byte value (in the range 0-255) to a printable character
// value.  If byteval is already a printable character, it is returned
// unmodified.  If byteval is not a printable character, then the
// ASCII code for '.' should be returned.
long hex_to_printable(long byteval) {
    if(byteval > 126 || byteval < 32) { //Check if valid
        return 46; //If no return '.'
    }
    else {
        return byteval; //If valid return input
    }
}