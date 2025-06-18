#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>
#include <omp.h>

#define MAX_WORD_LENGTH 100
#define HASH_LENGTH 33

/**
 * Load words from a dictionary file into memory
 * Returns an array of strings and sets word_count to the number of words loaded
 */
char **load_words(const char *filename, uint64_t *word_count) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }

    char **words = NULL;
    char line[MAX_WORD_LENGTH];
    uint64_t count = 0;

    // Read each line from the file and store as a word
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;  // Remove newline character
        words = realloc(words, (count + 1) * sizeof(char *));
        words[count] = strdup(line);    // Duplicate string and store
        count++;
    }

    fclose(file);
    *word_count = count;
    return words;
}

/**
 * Reduction function: converts an MD5 hash back to a password format
 * Creates passwords in the format "word1-word2-number" from hash bytes
 */
void reduction_function(const unsigned char *hash, char **words, uint64_t num_words, char *result) {
    // Extract 8 bytes from the hash starting at position 8 to create a pseudo-random number
    uint64_t val = 0;
    memcpy(&val, hash + 8, sizeof(uint64_t));
    
    // Use the extracted value to select two words and a number
    uint64_t i1 = val % num_words;                                    // First word index
    uint64_t i2 = (val / num_words) % num_words;                     // Second word index
    uint64_t number = (val / (num_words * num_words)) % 1001;        // Number between 0-1000
    
    // Format the result as "word1-word2-number"
    snprintf(result, MAX_WORD_LENGTH, "%s-%s-%d", words[i1], words[i2], (int)number);
}

/**
 * Generate a rainbow table chain starting from a given password
 * Applies hash -> reduction function 't' times to create the chain
 */
void generate_chain(const char *start, char **words, uint64_t num_words, uint64_t t, char *out) {
    char current_password[MAX_WORD_LENGTH];
    strcpy(current_password, start);
    unsigned char hash[MD5_DIGEST_LENGTH];

    // Apply hash -> reduction 't' times to generate the chain
    for (uint64_t i = 0; i < t; i++) {
        MD5((unsigned char *)current_password, strlen(current_password), hash);  // Hash current password
        reduction_function(hash, words, num_words, current_password);            // Reduce hash to new password
    }

    // Return the final password (end of chain)
    strcpy(out, current_password);
}

/**
 * Build the rainbow table in parallel using OpenMP
 * Creates 'm' chains of length 't' and saves them to a file
 * Each line in the output file contains: start_password,end_password
 */
void build_rainbow_table_parallel(char **words, uint64_t num_words, uint64_t t, uint64_t m, const char *output_file) {
    FILE *file = fopen(output_file, "w");
    if (!file) {
        perror("Error opening output file");
        return;
    }

    // Set number of parallel threads
    omp_set_num_threads(12);

    // Generate 'm' chains in parallel
    #pragma omp parallel for
    for (uint64_t i = 0; i < m; i++) {
        // Generate a starting password based on the chain index
        // The starting password doesn't matter much - what matters is the end word of the chain
        char start[MAX_WORD_LENGTH];
        uint64_t i1 = i % num_words;
        uint64_t i2 = (i / num_words) % num_words;
        uint64_t number = i % 1001;
        snprintf(start, sizeof(start), "%s-%s-%d", words[i1], words[i2], (int)number);

        // Generate the complete chain from start to end
        char result[MAX_WORD_LENGTH];
        generate_chain(start, words, num_words, t, result);

        // Write the chain (start,end) to file - use critical section for thread safety
        #pragma omp critical
        {
            fprintf(file, "%s,%s\n", start, result);
        }
    }

    fclose(file);
}

int main() {
    const char *filename = "./dico.txt";
    uint64_t num_words;
    
    // Load dictionary words from file
    char **words = load_words(filename, &num_words);
    if (!words) {
        fprintf(stderr, "Error: Unable to load words from file\n");
        return 1;
    }

    // Rainbow table parameters
    uint64_t m = 50000000;  // Number of chains to generate
    uint64_t t = 1000;      // Length of each chain (number of hash->reduction steps)
    const char *output_file = "rainbow_table.txt";

    printf("Building rainbow table with %lu chains of length %lu...\n", m, t);
    printf("Total coverage: %lu hash operations\n", m * t);
    
    // Build the rainbow table
    build_rainbow_table_parallel(words, num_words, t, m, output_file);
    
    printf("Rainbow table saved to %s\n", output_file);

    // Cleanup allocated memory
    for (uint64_t i = 0; i < num_words; i++) {
        free(words[i]);
    }
    free(words);

    return 0;
}