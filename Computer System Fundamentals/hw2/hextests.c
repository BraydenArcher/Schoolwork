// Unit tests for hex functions
// These tests should work for both your C implementations and your
// assembly language implementations

#include <stdio.h>
#include <stdlib.h>
#include "tctest.h"
#include "hexfuncs.h"

// test fixture object
typedef struct {
  char test_data_1[16];
  char test_data_2[23];
  char test_data_3[23];
  char test_data_4[110];
} TestObjs;

// setup function (to create the test fixture)
TestObjs *setup(void) {
  TestObjs *objs = malloc(sizeof(TestObjs));
  strcpy(objs->test_data_1, "Hello, world!\n");
  strcpy(objs->test_data_2, "This is a test string\n");
  strcpy(objs->test_data_3, "This\nis\na\ntest\nstring\n");
  strcpy(objs->test_data_4, "This is a\nvery long test stri\nng with\nrandom new lines\ninserted to\ntry and\n break my not\n so flawless\nprogram");
  return objs;
}

// cleanup function (to destroy the test fixture)
void cleanup(TestObjs *objs) {
  free(objs);
}

// Prototypes for test functions
void testFormatOffset(TestObjs *objs);
void testFormatByteAsHex(TestObjs *objs);
void testHexToPrintable(TestObjs *objs);

int main(int argc, char **argv) {
  if (argc > 1) {
    tctest_testname_to_execute = argv[1];
  }

  TEST_INIT();

  TEST(testFormatOffset);
  TEST(testFormatByteAsHex);
  TEST(testHexToPrintable);

  TEST_FINI();

  return 0;
}

void testFormatOffset(TestObjs *objs) {
  (void) objs; // suppress warning about unused parameter
  char buf[16];
  hex_format_offset(1L, buf);
  ASSERT(0 == strcmp(buf, "00000001"));

  hex_format_offset(2, buf);
  ASSERT(0 == strcmp(buf, "00000002"));

  hex_format_offset(4, buf);
  ASSERT(0 == strcmp(buf, "00000004"));

  hex_format_offset(27, buf);
  ASSERT(0 == strcmp(buf, "0000001b"));

  hex_format_offset(48, buf);
  ASSERT(0 == strcmp(buf, "00000030"));

  hex_format_offset(590, buf);
  ASSERT(0 == strcmp(buf, "0000024e"));

  hex_format_offset(800, buf);
  ASSERT(0 == strcmp(buf, "00000320"));

  hex_format_offset(880, buf);
  ASSERT(0 == strcmp(buf, "00000370"));

  hex_format_offset(3242, buf);
  ASSERT(0 == strcmp(buf, "00000caa"));

  hex_format_offset(0, buf);
  ASSERT(0 == strcmp(buf, "00000000"));
}

void testFormatByteAsHex(TestObjs *objs) {
  char buf[16];
  hex_format_byte_as_hex(objs->test_data_1[0], buf);
  ASSERT(0 == strcmp(buf, "48"));

  hex_format_byte_as_hex(objs->test_data_2[21], buf);
  ASSERT(0 == strcmp(buf, "0a"));

  hex_format_byte_as_hex(objs->test_data_3[11], buf);
  ASSERT(0 == strcmp(buf, "65"));

  hex_format_byte_as_hex(objs->test_data_4[76], buf);
  ASSERT(0 == strcmp(buf, "62"));

  hex_format_byte_as_hex(objs->test_data_4[9], buf);
  ASSERT(0 == strcmp(buf, "0a"));

  hex_format_byte_as_hex(objs->test_data_4[10], buf);
  ASSERT(0 == strcmp(buf, "76"));

  hex_format_byte_as_hex(objs->test_data_4[11], buf);
  ASSERT(0 == strcmp(buf, "65"));

  hex_format_byte_as_hex(objs->test_data_4[12], buf);
  ASSERT(0 == strcmp(buf, "72"));

  hex_format_byte_as_hex(objs->test_data_4[13], buf);
  ASSERT(0 == strcmp(buf, "79"));

  hex_format_byte_as_hex(objs->test_data_4[14], buf);
  ASSERT(0 == strcmp(buf, "20"));

}

void testHexToPrintable(TestObjs *objs) {
  ASSERT('H' == hex_to_printable(objs->test_data_1[0]));
  ASSERT('.' == hex_to_printable(objs->test_data_1[13]));

  ASSERT('.' == hex_to_printable(objs->test_data_2[21]));
  ASSERT(' ' == hex_to_printable(objs->test_data_2[4]));

  ASSERT('e' == hex_to_printable(objs->test_data_3[11]));
  ASSERT('g' == hex_to_printable(objs->test_data_3[20]));

  ASSERT('.' == hex_to_printable(objs->test_data_4[9]));
  ASSERT('b' == hex_to_printable(objs->test_data_4[76]));

  ASSERT('v' == hex_to_printable(objs->test_data_4[10]));
  ASSERT('e' == hex_to_printable(objs->test_data_4[11]));

  ASSERT('r' == hex_to_printable(objs->test_data_4[12]));
  ASSERT('y' == hex_to_printable(objs->test_data_4[13]));

  ASSERT(' ' == hex_to_printable(objs->test_data_4[14]));
  ASSERT('l' == hex_to_printable(objs->test_data_4[15]));

  ASSERT('o' == hex_to_printable(objs->test_data_4[16]));
  ASSERT('n' == hex_to_printable(objs->test_data_4[17]));

  ASSERT('g' == hex_to_printable(objs->test_data_4[18]));
  ASSERT(' ' == hex_to_printable(objs->test_data_4[19]));
}
