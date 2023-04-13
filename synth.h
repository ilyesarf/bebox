#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Note{
    char name;
    float freq;

    float attack;
    float decay;
    float sustain;
    float release;
};

char **split(char *str, char *delimiter, int *num_tokens) {
    // Allocate memory for the array of strings
    char **tokens = (char **)malloc(sizeof(char *));
    if (tokens == NULL) {
        fprintf(stderr, "Error: Unable to allocate memory\n");
        exit(EXIT_FAILURE);
    }

    // Split the string into tokens
    char* token = strtok(str, delimiter);
    int count = 0;
    while (token != NULL) {
        tokens[count] = token;
        count++;
        tokens = (char **)realloc(tokens, (count + 1) * sizeof(char *));
        if (tokens == NULL) {
            fprintf(stderr, "Error: Unable to allocate memory\n");
            exit(EXIT_FAILURE);
        }
        token = strtok(NULL, delimiter);
    }
    
    // Set the number of tokens
    *num_tokens = count;

    return tokens;
}