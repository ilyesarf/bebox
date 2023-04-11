#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sndfile.h>

#define MAXBUFLEN 1000

#define SAMPLE_RATE 44100
#define AMP 0.3f
#define DURATION 1.0f

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

void sine(int n, int f, float freq, float* buffer){
    float phaseIncrement = 2.0f * M_PI / (SAMPLE_RATE / freq);
    float phase = 0.0f;

    // Generate the sine wave
    for (int i = n*f; i < n*(f+1); i++) {
        buffer[i] = AMP * sinf(phase);
        phase += phaseIncrement;
    }
}

int read_notes(char* fnotes){
    FILE *fp = fopen("notes.n", "r");
    char buffer[MAXBUFLEN + 1];

    if (fp == NULL){
        printf("File not found");
        exit(1);
    }

    int n_notes = 0;
    while (fscanf(fp, "%s", buffer) != EOF) {
        for (int j = 0; buffer[j] != '\0'; j++) {
            fnotes[n_notes++] = buffer[j];
        }
    }

    fclose(fp);

    return n_notes;
}

int main() {
    int n = ceil(SAMPLE_RATE * DURATION);

    char notes[7] = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};
    float freqs[7] = {440.0f, 493.88f, 523.25f, 587.33f, 659.26f, 698.46f, 783.99f};

    char fnotes[MAXBUFLEN + 1];
    int n_notes = read_notes(fnotes);

    float buffer[n*n_notes];
    for (int f=0; f < n_notes; f++){
        char fnote = fnotes[f];
        int id = -1;

        for (int i=0; i < sizeof(notes) / sizeof(char); i++){
            if (notes[i] == fnote){
                id = i;
                break;
            }
        }

        if (id == -1){
            printf("Note %c not found", fnote);
            exit(1);
        }

        sine(n, f, freqs[id], buffer);
    }

    write_wav(buffer, n*n_notes);

    return 0;
}



