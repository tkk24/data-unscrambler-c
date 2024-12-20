#ifndef UNSCRAMBLE_H
#define UNSCRAMBLE_H

#include "word.h"
#include <stdint.h>
#include <stdlib.h>

// read_binary_file() - Reads a binary file and returns its contents in an allocated buffer.
//     filename: The pointer to the string containing the file to open.
//     size: The size of the file in bytes.
// Returns a pointer to a buffer containing file's contents.
uint8_t* read_binary_file(const char* filename, size_t size);

// unscramble_clear() - Parses binary data with scrambled word records, extracts them, and adds them to a Word_List structure.
//     data: The pointer to the binary data.
//     word_list: The pointer to the Word_List.
void unscramble_clear(uint8_t* data, struct Word_List* word_list);

// flip_nybbles() - Helper function that reverses the higher and lower nybbles of an 8-bit byte.
//     byte: The byte whose nybbles will be flipped.
// Returns a new byte with the nybbles of the orignal byte flipped. For example, if the byte was
// 0000 1111, then the new byte returned would be 1111 0000.
unsigned char flip_nybbles(unsigned char byte);

// unscramble_fuzzy() - Parses binary data with fuzzed word records, decodes and adds them to a Word_List structure.
//     data: The pointer to the fuzzed binary data.
//     word_list: The pointer to the Word_List.
void unscramble_fuzzy(uint8_t* data, struct Word_List* word_list);

#endif // UNSCRAMBLE_H
