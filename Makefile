CC = clang
CFLAGS = -Wall -g
LDFLAGS = -lm -lsndfile

SOURCES = synth.c waveforms.c
OBJECTS = $(SOURCES:.c=.o)

.PHONY: all clean

all: synth

synth: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) synth



