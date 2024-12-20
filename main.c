#include <stdio.h>
#include <stdlib.h>
#include "unscramble.h"
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


// The main function that will be run.
int main(int argc, char* argv[]) {

    // Validates the number of command-line arguments (there should be 3).
    if (argc != 3) {
        // If there aren't three command-line arguments...
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    const char* input_file = argv[1];
    const char* output_file = argv[2];

    // Opens and parses the input file.
    FILE* input = fopen(input_file, "r");
    if (!input) {
        // If the input file is inaccessible...
        fprintf(stderr, "Error: Unable to open input file '%s'.\n", input_file);
        return 1;
    }

    size_t size; // Size of the binary data.
    char filename[256]; // Name of binary data.
    char mode[16]; // Determines if data is clear or fuzzy.

    // Reads the input file for size, name, and clear/fuzzy.
    if (fscanf(input, "%zu\n%255s\n%15s", &size, filename, mode) != 3) {
        fprintf(stderr, "Error: Input file is not formatted correctly.\n");
        fclose(input);
        return 1;
    }
    fclose(input);

    // Read binary data from the data file.
    uint8_t* data = read_binary_file(filename, size);

    // Initializes the word list that will store the unscrambled words.
    struct Word_List word_list;
    init_list(&word_list, 10);

    // Unscrambles based on the mode (clear or fuzzy).
    // Uses clear unscrambling if first character is "c" (for clear).
    if (mode[0] == 'c') {
        unscramble_clear(data, &word_list);
    }
    // Uses fuzzy unscrambling if first character is "f" (for fuzzy).
    else if (mode[0] == 'f') {
        unscramble_fuzzy(data, &word_list);
    }
    else {
        // If first character is neither "c" or "f"...
        fprintf(stderr, "Error: Invalid mode '%s'. Expected 'clear' or 'fuzzy'.\n", mode);
        free(data);
        free_list(word_list);
        return 1;
    }

    // Writes the unscrambled words to the output file.
    FILE* output = fopen(output_file, "w");
    if (!output) {
        fprintf(stderr, "Error: Unable to open output file '%s'.\n", output_file);
        free(data);
        free_list(word_list);
        return 1;
    }

    // The output.
    fprintf(output, "Bytes in file:\t%zu\n", size);
    fprintf(output, "Name of file:\t%s\n", filename);
    fprintf(output, "Clear|Fuzzy:\t%s\n", mode);
    display_words(output, word_list); // Outputs all unscrambled words and offsets.

    // Frees resources and closes output file.
    free(data);
    free_list(word_list);
    fclose(output);

    return 0;
}