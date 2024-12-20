#include "unscramble.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


// On my honor:
//
// - I have not discussed the C language code in my program with
// anyone other than my instructor or the teaching assistants
// assigned to this course.
//
// - I have not used C language code obtained from another student,
// or any other unauthorized source, either modified or unmodified.
//
// - If any C language code or documentation used in my program
// was obtained from an allowed source, such as a text book or course
// notes, that has been clearly noted with a proper citation in
// the comments of my program.
//
// Timothy Kim


// read_binary_file() - Reads a binary file and returns its contents in an allocated buffer.
//     filename: The pointer to the string containing the file to open.
//     size: The size of the file in bytes.
// Returns a pointer to a buffer containing file's contents.
uint8_t* read_binary_file(const char* filename, size_t size) {

    FILE* binary_file = fopen(filename, "rb");
    // If file doesn't exist...
    if (!binary_file) {
        fprintf(stderr, "Failed to open file %s\n", filename);
        exit(1);
    }

    uint8_t* data = (uint8_t*)malloc(size);
    // If memory cannot be allocated properly...
    if (!data) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(binary_file);
        exit(1);
    }

    size_t num_bytes = fread(data, 1, size, binary_file);
    // If bytes read don't match from the input...
    if (num_bytes != size) {
        fprintf(stderr, "Error: Expected to read %zu bytes, but only read %zu bytes.\n", size, num_bytes);
        free(data);
        exit(1);
    }

    fclose(binary_file);
    return data;
}


// unscramble_clear() - Parses binary data with scrambled word records, extracts them, and adds them to a Word_List structure.
//     data: The pointer to the binary data.
//     word_list: The pointer to the Word_List.
void unscramble_clear(uint8_t* data, struct Word_List* word_list) {

    // Reads the number of words from the binary data.
    uint16_t num_words = *((uint16_t*)data); // Number of words in the data.
    uint16_t offset = *((uint16_t*)(data + 2)); // Offset of the first word.

    // Iterates through each word record.
    for (int i = 0; i < num_words; i++) {

        // Extracts the data for the current word.
        uint8_t word_len = *(data + offset); // The word length.
        uint16_t next_offset = *((uint16_t*)(data + offset + 1)); // The offset to next word.

        // Allocates memory for the extracted word.
        char* word = (char*)calloc((word_len - 2), sizeof(char));
        // If memory allocation fails...
        if (!word) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }

        // Unscrambles the word by copying characters.
        for (int j = 0; j < word_len - 3; j++) {
            *(word + j) = *(data + offset + 3 + j); 
        }
        *(word + (word_len - 3)) = '\0';

        // Adds the unscrambled word to Word_List.
        struct Word new_word;
        init_word(&new_word);
        update_word(&new_word, word, offset + 3); // Updates Word with the extracted word and the actual offset.
        add_word(word_list, new_word);

        // Frees previous word data and moves to next word.
        free(word);
        free_word(new_word);
        offset = next_offset;
    }
}


// flip_nybbles() - Helper function that reverses the higher and lower nybbles of an 8-bit byte.
//     byte: The byte whose nybbles will be flipped.
// Returns a new byte with the nybbles of the orignal byte flipped. For example, if the byte was
// 0000 1111, then the new byte returned would be 1111 0000.
unsigned char flip_nybbles(unsigned char byte) {
    unsigned char higher_nybble = (byte & 0xF0) >> 4; // Shifts down the first four bits.
    unsigned char lower_nybble = (byte & 0x0F) << 4; // Shift up the last four bits.
    return higher_nybble | lower_nybble; // Combine the nybbles together.
}


// unscramble_fuzzy() - Parses binary data with fuzzed word records, decodes and adds them to a Word_List structure.
//     data: The pointer to the fuzzed binary data.
//     word_list: The pointer to the Word_List.
void unscramble_fuzzy(uint8_t* data, struct Word_List* word_list) {

    // Reads the number of words from the binary data.
    uint16_t num_words = *((uint16_t*)data);
    uint16_t offset = sizeof(uint16_t);
    offset = *((uint16_t*)(data + offset)); // The offset to the first word record.

    // Iterates through each fuzzed word.
    for (int i = 0; i < num_words; i++) {
        uint8_t word_len = *(data + offset); // The fuzzed word length.

        // Allocates memory for the word.
        char* word = (char*)calloc((word_len), sizeof(char));
        if (!word) {
            // If memory allocation fails...
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }

        // Uses helper function to flip nybbles and decode word length.
        uint8_t flipped_len = flip_nybbles(word_len);

        // Extracts the word by applying XOR with flipped_word_len.
        for (int j = 0; j < word_len - 3; j++) {
            *(word + j) = *(data + offset + j + 3) ^ flipped_len; // Where the XOR happens.
        }
        *(word + (word_len - 3)) = '\0';

        // Unfuzzes the offset for the next word record.
        uint8_t first_byte = *(word);
        uint16_t mask = ((first_byte) << 8) | first_byte;
        uint16_t next_offset = mask ^ *((uint16_t*)(data + offset + 1));

        // Add the unscrambled word to Word_List.
        struct Word new_word;
        init_word(&new_word);
        update_word(&new_word, word, offset + 3);  // Updates Word with the extracted word and the actual offset.
        add_word(word_list, new_word);

        // Frees previous word data and moves to next word.
        free(word);
        free_word(new_word);
        offset = next_offset;
    }
}
