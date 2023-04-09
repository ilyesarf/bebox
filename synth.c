#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sndfile.h>

#define SAMPLE_RATE 44100
#define FREQ 440.0f
#define AMP 0.5f
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
    float phaseIncrement = 2.0f * M_PI / (SAMPLE_RATE / FREQ);
    float phase = 0.0f;
    float buffer[n];

    // Generate the sine wave
    for (int i = 0; i < n; i++) {
        buffer[i] = AMP * sinf(phase);
        phase += phaseIncrement;
    }

    write_wav(buffer, n);

    return 0;
}



