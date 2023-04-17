#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "synth.h"
#include "waveforms.h"

#define MAXBUFLEN 1000
#define DURATION 0.5f

int read_notes(char** fnotes){
    FILE *fp = fopen("notes.n", "r");
    char line[MAXBUFLEN + 1];

    if (fp == NULL){
        printf("File not found");
        exit(1);
    }

    int n_notes;
    char space[] = " ";
    while (fgets(line, sizeof(line), fp)) {
        char** tokens = split(line, space, &n_notes);
        for (int i=0; i < n_notes; i++){
            strcpy(&fnotes[i], &tokens[i]);
        }
    }

    fclose(fp);

    return n_notes;
}

int get_waveforms(void (*waveforms[])(int, int, struct Note *, float, float *), int argc, char *argv[]){
    int n_waveforms = 0;
    
    for (int i = 1; i < argc; i++){
        char* wavefrm = argv[i];

        if (strcmp(wavefrm, "sine") == 0){
            waveforms[n_waveforms] = sine;
        } else if (strcmp(wavefrm, "tooth") == 0 || strcmp(wavefrm, "sawtooth") == 0){
            waveforms[n_waveforms] = sawtooth;
        } else if (strcmp(wavefrm, "sqr") == 0 || strcmp(wavefrm, "square") == 0){
            waveforms[n_waveforms] = square;
        } else if (strcmp(wavefrm, "tri") == 0 || strcmp(wavefrm, "triangle") == 0){
            waveforms[n_waveforms] = triangle;
        }else{
            printf("Undefined waveform: %s\n", wavefrm);
            exit(1);
        }
        n_waveforms++;
    }
    
    if (n_waveforms == 0){
        waveforms[0] = sine;
        n_waveforms = 1;
    }

    return n_waveforms;
}

void mix(void (*waveforms[])(int, int, struct Note *, float, float *), int n_waveforms, int n, int f, struct Note* note, float freq, float* buffer){
    //zero buffer
    for (int i=n*f; i < n*(f+1); i++){
        buffer[i] = 0;
    }


    if (note->name != '0'){
        for (int i=0; i < n_waveforms; i++){
            void (*waveform)(int, int, struct Note*, float, float*) = waveforms[i];
            waveform(n, f, note, freq, buffer);
        }
    }
}

int main(int argc, char *argv[]) {
    void (**waveforms)(int, int, struct Note *, float, float *) = NULL;
    waveforms = (void (**)(int, int, struct Note*, float, float*)) malloc((argc - 1) * sizeof(void (*)()));
    if (waveforms == NULL) {
        printf("Failed to allocate memory\n");
        exit(1);
    }

    int n_waveforms = get_waveforms(waveforms, argc, argv);

    int n = ceil(SAMPLE_RATE * DURATION);

    struct Note notes[8] = {
    {'C', 261.63f, 0.02, 0.2, 0.6, 0.3}, 
    {'D', 587.33f, 0.02, 0.2, 0.6, 0.3},  
    {'E', 329.63f, 0.02, 0.2, 0.6, 0.3}, 
    {'F', 349.23f, 0.02, 0.2, 0.6, 0.3}, 
    {'G', 392.00f, 0.02, 0.2, 0.6, 0.3},
    {'A', 440.0f, 0.02, 0.2, 0.6, 0.3}, 
    {'B', 493.88f, 0.02, 0.2, 0.6, 0.3},
    {'0'}};

    char* fnotes[MAXBUFLEN + 1];
    int n_notes = read_notes(fnotes);

    float buffer[n*n_notes];
    for (int f=0; f < n_notes; f++){
        char* fnote = fnotes[f];
        int sharp = 0;
        for (int i=0; i < strlen(fnote); i++){
            if (fnote[i] == '#'){
                sharp = 1;
                fnote[i] = fnote[i+1];
                fnote[i+1] = '\0';
            }
        }

        int pitch = 4;
        if (fnote[1] != '\0'){
            pitch = fnote[1] - '0';
        }

        int id = -1;

        for (int i=0; i < sizeof(notes) / sizeof(struct Note); i++){
            if (notes[i].name == fnote[0]){
                id = i;
                break;
            }
        }

        if (id == -1){
            printf("Note %c not found\n", *fnote);
            exit(1);
        }
        struct Note* note = &notes[id]; 
        float freq = ((note->freq) / pow(2, (4-pitch))) * pow(2, (sharp/12.0));
        
        mix(waveforms, n_waveforms, n, f, note, freq, buffer);
    }

    write_wav(buffer, n*n_notes);

    return 0;
}



