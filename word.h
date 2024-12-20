#ifndef WORD_H
#define WORD_H

#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

// struct Word - Represents a single word with an offset.
//     word: Pointer to the string representing the word.
//     offset: An uint16_t that stores the word's offset.
struct Word
{
	char* word;
	uint16_t offset;
};

// struct Word_List - Represents a collection of Word structs.
//     word: Pointer to the an array of Word structs.
//     size: An uint16_t that stores the size of the array.
//     size: An uint16_t that stores the number of Word structs in the array.
struct Word_List
{
	struct Word* words;
	uint16_t size;
	uint16_t count;
};

// strlength() - Computes the length of a string.
//     word: The pointer of the length string.
// Returns the length of the string as a uint32_t.
uint32_t strlength( const char* const word );

// strcopy() - Copies a string and pastes it to a specified buffer.
//     destination: The destination buffer.
//     source: The pointer to the source string that will be copied.
void strcopy( char* const destination, const char* const source );

// init_word() - Initiates a Word structure.
//     word: The pointer to the Word structure.
void init_word( struct Word *word );

// update_word() - Updates a Word structure with new data.
//     word: The pointer to the Word structure.
//     wrd: The pointer to the string with new data.
//     offset: The offset value to set in the Word structure.
void update_word( struct Word* word, const char* const wrd, uint16_t offset );

// display_word() - Outputs a Word structure's contents.
//     out: The pointer to an output file where Word will be displayed.
//     word: The Word structure that will be displayed.
void display_word( FILE* out, struct Word word );

// free_word() - Frees the memory allocated for a Word structure.
//     word: The pointer to the Word structure.
void free_word( struct Word word );

// init_list() - Initializes a Word_List.
//     word_list: The pointer to the Word_List structure.
//     size: The size of the Word_List.
void init_list( struct Word_List *word_list, uint16_t size );

// add_word() - Adds a Word to a Word_List structure and resizes if necessary.
//     word_list: The pointer to the Word_List structure.
//     word: The Word structure that will be added.
void add_word( struct Word_List *word_list, struct Word word );

// get_word_at_index() - Gets a Word from a Word_List at an index.
//     word_list: The pointer to the Word_List structure.
//     index: The index of the Word to get.
// Returns the Word at the specified index.
struct Word get_word_at_index( struct Word_List word_list, uint16_t index );

// free_list() - Frees the memory allocated for a Word_List structure.
//     word_list: The pointer to the Word_List structure.
void free_list( struct Word_List word_list );

// display_words() - Outputs all words of a Word_List structure.
//     out: The pointer to an output file where words will be displayed.
//     word_list: The pointer to the Word_List structure.
void display_words( FILE* out, struct Word_List word_list );

#endif
