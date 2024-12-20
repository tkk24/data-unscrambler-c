# data-unscrambler-c
A C project that unscramble and decode quotes from binary files, including methods such as XOR and flipping parts of bytes.

## Features
- Reads binary data files containing scrambled quotes.
- Reads files with clear and fuzzed decoding modes.
- Outputs unscrambled quotes along with word offsets.

## Project Structure
- **main.c**: Handles command-line arguments, reads input files, and orchestrates the unscrambling process.
- **unscramble.c**: Implements functions to decode clear and fuzzed binary data.
- **word.c**: Defines and manages `Word` and `Word_List` structures for storing and displaying decoded data.
- **headers**: Associated `.h` files for the C modules.

## Decoding Modes
### Clear Mode
In clear mode, the binary data contains word records in a straightforward format. The program:
1. Extracts the number of words and the offsets for each word record.
2. Reads the word length, characters, and the offset for the next word.
3. Unscrambles the words and stores them in a `Word_List` structure.

### Fuzzed Mode
In fuzzed mode, the binary data is obfuscated, requiring additional steps to decode. The program:
1. Reads the number of words and their offsets, just like in clear mode.
2. Applies two types of fuzzing techniques:
   - **Nybble Flipping**: The word length is fuzzed by flipping its higher and lower nybbles. For example:
     - If the byte is `0xAB`, the flipped version is `0xBA`.
     - This is achieved using bitwise operations: shifting the higher nybble right and the lower nybble left, then combining them with a bitwise OR.
   - **XOR Unfuzzing**: The characters in the word are XORed with the flipped word length to decode them. The program reverses this operation by applying XOR with the same value, a property of XOR that undoes the original operation.
3. Uses a two-byte mask derived from the first letter of each word to unfuzz the offsets for the next word record. This mask is created by duplicating the first letter into both bytes of the mask.
4. Reconstructs the original data and stores it in the `Word_List` structure.

### Example of XOR and Nybble Flipping
```c
// Nybble flipping example:
unsigned char flip_nybbles(unsigned char byte) {
    unsigned char higher_nybble = (byte & 0xF0) >> 4; // Extract and shift down higher nybble
    unsigned char lower_nybble = (byte & 0x0F) << 4; // Extract and shift up lower nybble
    return higher_nybble | lower_nybble; // Combine them
}

// XOR unfuzzing example:
char unfuzzed_char = fuzzed_char ^ flipped_word_length;
```



# Usage

## Running the Program
Compile the program using `gcc` or a similar C compiler:
```bash
gcc main.c unscramble.c word.c -o hex_unscrambler
```
Provide an input file specifying the binary file details and mode (clear or fuzzy):
```bash
./hex_unscrambler <input_file> <output_file>
```
The input file should have the following format:
1. Number of bytes in the file
2. Name of the binary file
3. Mode (clear or fuzzy)
```arduino
220
data-clear.bin
clear
```

## Output
The program writes the unscrambled words and their offsets to the specified output file.

Example Output:
```yaml
Bytes in file:   141
Name of file:    data-clear-2.bin
Clear|Fuzzy:     clear
Any                  @ Offset: 4c
fool                 @ Offset: 07
can                  @ Offset: 74
...
```
Attached to this repository are more examples of input and output files, and the binary file that is being read.
