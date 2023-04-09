#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sndfile.h>

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


int main() {
    int n = ceil(SAMPLE_RATE * DURATION);

    float freqs[7] = {440.0f, 493.88f, 523.25f, 587.33f, 659.26f, 698.46f, 783.99f};
    int n_notes = sizeof(freqs) / sizeof(float);
    float buffer[n*n_notes];
    for (int f=0; f < n_notes; f++){
        float phaseIncrement = 2.0f * M_PI / (SAMPLE_RATE / freqs[f]);
        float phase = 0.0f;

        // Generate the sine wave
        for (int i = n*f; i < n*(f+1); i++) {
            buffer[i] = AMP * sinf(phase);
            phase += phaseIncrement;
        }
    }

    write_wav(buffer, n*n_notes);

    return 0;
}



