#include "word.h"

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


// strlength() - Computes the length of a string.
//     word: The pointer of the length string.
// Returns the length of the string as a uint32_t.
uint32_t strlength(const char* const word) {
    uint32_t length = 0;

    while (word[length] != '\0') {
        length++;
    }

    return length;
}


// strcopy() - Copies a string and pastes it to a specified buffer.
//     destination: The destination buffer.
//     source: The pointer to the source string that will be copied.
void strcopy(char* const destination, const char* const source) {
    uint32_t int32 = 0;

    while (source[int32] != '\0') {
        destination[int32] = source[int32];
        int32++;
    }

    destination[int32] = '\0';
}


// init_word() - Initiates a Word structure.
//     word: The pointer to the Word structure.
void init_word(struct Word *word) {
    word->word = NULL;
    word->offset = 0;
}


// update_word() - Updates a Word structure with new data.
//     word: The pointer to the Word structure.
//     wrd: The pointer to the string with new data.
//     offset: The offset value to set in the Word structure.
void update_word(struct Word *word, const char* const wrd, uint16_t offset) {
    if (word->word != NULL) {
        free(word->word);
        word->word = NULL;
    }
    
    uint32_t length = strlength(wrd);
    word->word = (char*)malloc((length + 1) * sizeof(char));

    if (word->word == NULL) {
        fprintf(stderr, "Memory allocation failed in update_word\n");
        exit(1);
    }

    strcopy(word->word, wrd);
    word->offset = offset;
}


// display_word() - Outputs a Word structure's contents.
//     out: The pointer to an output file where Word will be displayed.
//     word: The Word structure that will be displayed.
void display_word(FILE* out, struct Word word) {
    if (word.word != NULL) {
        fprintf(out, "%-20s @ Offset: %02x\n", word.word, word.offset);
    }
    else {
        fprintf(out, "(null word) @ Offset: %02x\n", word.offset);
    }
}


// free_word() - Frees the memory allocated for a Word structure.
//     word: The pointer to the Word structure.
void free_word(struct Word word) {
    if (word.word != NULL) {
        free(word.word);
        word.word = NULL;
    }
}


// init_list() - Initializes a Word_List.
//     word_list: The pointer to the Word_List structure.
//     size: The size of the Word_List.
void init_list(struct Word_List* word_list, uint16_t size) {
    word_list->words = (struct Word*)malloc(size * sizeof(struct Word));

    if (word_list->words == NULL) {
        exit(1);
    }

    word_list->size = size;
    word_list->count = 0;
}


// add_word() - Adds a Word to a Word_List structure and resizes if necessary.
//     word_list: The pointer to the Word_List structure.
//     word: The Word structure that will be added.
void add_word(struct Word_List* word_list, struct Word word) {
    if (word_list->count == word_list->size) {
        word_list->size *= 2;
        word_list->words = (struct Word*)realloc(word_list->words, word_list->size * sizeof(struct Word));

        if (word_list->words == NULL) {
            exit(1);
        }
    }

    word_list->words[word_list->count].word = (char*)malloc((strlength(word.word) + 1) * sizeof(char));

    if (word_list->words[word_list->count].word == NULL) {
        exit(1);
    }

    strcopy(word_list->words[word_list->count].word, word.word);
    word_list->words[word_list->count].offset = word.offset;
    word_list->count++;
}


// get_word_at_index() - Gets a Word from a Word_List at an index.
//     word_list: The pointer to the Word_List structure.
//     index: The index of the Word to get.
// Returns the Word at the specified index.
struct Word get_word_at_index(struct Word_List word_list, uint16_t index) {
    struct Word result;
    init_word(&result);

    if (index < word_list.count) {
        result.word = (char*)malloc((strlength(word_list.words[index].word) + 1) * sizeof(char));
        if (result.word == NULL) {
            exit(1);
        }
        strcopy(result.word, word_list.words[index].word);
        result.offset = word_list.words[index].offset;
    }
    else {
        update_word(&result, "Word Not Found", 0);
    }

    return result;
}


// free_list() - Frees the memory allocated for a Word_List structure.
//     word_list: The pointer to the Word_List structure.
void free_list(struct Word_List word_list) {
    for (uint16_t i = 0; i < word_list.count; i++) {

        if (word_list.words[i].word != NULL) {
            free(word_list.words[i].word);
            word_list.words[i].word = NULL;
        }
    }

    if (word_list.words != NULL) {
        free(word_list.words);
        word_list.words = NULL;
    }
}


// display_words() - Outputs all words of a Word_List structure.
//     out: The pointer to an output file where words will be displayed.
//     word_list: The pointer to the Word_List structure.
void display_words(FILE* out, struct Word_List word_list) {
    for (uint16_t i = 0; i < word_list.count; i++) {
        display_word(out, word_list.words[i]);
    }
}