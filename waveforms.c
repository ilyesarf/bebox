#include <stdio.h>
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

void sine(int n, int f, struct Note* note, float freq, float* buffer){
    float period = 1.0f / freq;
    float phaseIncrement = 2.0f * M_PI / (SAMPLE_RATE / freq);
    float phase = 0.0f;
    float amp = 0.0f;

    // Generate the sine wave
    for (int i = n*f; i < n*(f+1); i++) {
        amp = sinf(phase);
        buffer[i] += amp * adsr(period, note->attack, note->decay, note->sustain, note->release);
        phase += phaseIncrement;
        while (phase > 2.0f * M_PI) {
            phase -= 2.0f * M_PI;
        }
    }
}

void sawtooth(int n, int f, struct Note* note, float freq, float* buffer){
    float period = 1.0f / freq;
    float phaseIncrement = 2.0f / (SAMPLE_RATE / freq);
    float phase = 0.0f;
    float amp = 0.0f;

    // generate sawtooth waveform
    for (int i = n*f; i < n*(f+1); i++) {
        amp = fmodf(phase, 1.0f);
        buffer[i] += amp * adsr(period, note->attack, note->decay, note->sustain, note->release);
        phase += phaseIncrement;
        while (phase > 1.0f) {
            phase -= 1.0f;
        }
    }
}

void square(int n, int f, struct Note* note, float freq, float* buffer){
    float period = 1.0f / freq;
    float phaseIncrement = 2.0f * M_PI / (SAMPLE_RATE / freq);
    float phase = 0.0f;
    float dutyCycle = 0.5f; // 50% duty cycle by default
    
    // Check if duty cycle is specified in note data
    if (note->dutyCycle > 0 && note->dutyCycle < 1) {
        dutyCycle = note->dutyCycle;
    }

    // Generate the square wave
    for (int i = n*f; i < n*(f+1); i++) {
        float value = sinf(phase);
        if (value > dutyCycle) {
            buffer[i] += 1.0f * adsr(period, note->attack, note->decay, note->sustain, note->release);
        } else {
            buffer[i] += -1.0f * adsr(period, note->attack, note->decay, note->sustain, note->release);
        }

        phase += phaseIncrement;

        while (phase > 2.0f * M_PI) {
            phase -= 2.0f * M_PI;
        }
    }
}

void triangle(int n, int f, struct Note* note, float freq, float* buffer) {
    float period = 1.0f / freq;
    float phaseIncrement = 2 * freq / SAMPLE_RATE;
    float phase = 0.0f;
    float amp = 0.8f;

    for (int i = n*f; i < n*(f+1); i++) {
        float sample;
        if (phase <= 0.5) {
            sample = 4 * amp * phase - amp;
        } else {
            sample = 3 * amp - 4 * amp * (phase - 0.5);
        }

        sample *= adsr(period, note->attack, note->decay, note->sustain, note->release);
        buffer[i] += sample;

        phase += phaseIncrement;
        
        while (phase > 1.0) {
            phase -= 1.0;
        }
    }
}



