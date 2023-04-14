#include <math.h>
#include "waveforms.h"

float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

float adsr(float t, float attack, float decay, float sustain, float release) {
    if (t < attack) {
        return lerp(0.0f, 1.0f, t / attack);
    } else if (t < attack + decay) {
        return lerp(1.0f, sustain, (t - attack) / decay);
    } else if (t < attack + decay + sustain) {
        return sustain;
    } else {
        return lerp(sustain, 0.0f, (t - attack - decay - sustain) / release);
    }
}

void sine(int n, int f, struct Note* note, int pitch, float* buffer){
    float freq = (note->freq)/pow(2, (4-pitch));
    float period = 1.0f / freq;
    float phaseIncrement = 2.0f * M_PI / (SAMPLE_RATE / freq);
    float phase = 0.0f;
    float amp = 0.0f;

    // Generate the sine wave
    for (int i = n*f; i < n*(f+1); i++) {
        amp = sinf(phase);
        buffer[i] = amp * adsr(period, note->attack, note->decay, note->sustain, note->release);
        phase += phaseIncrement;
    }
}

void sawtooth(int n, int f, struct Note* note, int pitch, float* buffer) {
    float freq = (note->freq) / pow(2, (4 - pitch));
    float period = 1.0f / freq;
    float phaseIncrement = 2.0f / (SAMPLE_RATE / freq);
    float phase = 0.0f;
    float amp = 0.0f;

    // generate sawtooth waveform
    for (int i = n*f; i < n*(f+1); i++) {
        amp = fmodf(phase, 1.0f);
        buffer[i] = amp * adsr(period, note->attack, note->decay, note->sustain, note->release);
        phase += phaseIncrement;
    }
}