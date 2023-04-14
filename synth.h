#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sndfile.h>

#define SAMPLE_RATE 44100

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

void write_wav(float* buffer, int n){
    SNDFILE *file;
    SF_INFO info;

    // Open the output file for writing
    info.samplerate = SAMPLE_RATE;
    info.channels = 1;
    info.format = SF_FORMAT_WAV | SF_FORMAT_FLOAT;
    file = sf_open("sine.wav", SFM_WRITE, &info);
    if (!file) {
        printf("Error opening output file\n");
        exit(1);
    }

    // Write the samples to the output file
    sf_write_float(file, buffer, n);

    // Close the output file
    sf_close(file);
}