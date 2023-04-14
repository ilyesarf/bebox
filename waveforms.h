#ifndef WAVEFORMS_H
#define WAVEFORMS_H

#define SAMPLE_RATE 44100

struct Note{
    char name;
    float freq;

    float attack;
    float decay;
    float sustain;
    float release;

    float dutyCycle;
};

float lerp(float a, float b, float t);

float adsr(float t, float attack, float decay, float sustain, float release); 

void sine(int n, int f, struct Note* note, int pitch, float* buffer);

void sawtooth(int n, int f, struct Note* note, int pitch, float* buffer);

void square(int n, int f, struct Note* note, int pitch, float* buffer);

void triangle(int n, int f, struct Note* note, int pitch, float* buffer);
#endif