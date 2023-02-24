CC = gcc
CFLAGS = -Wall -std=c99
PROG = ballz

ALLEGRO = -lallegro_image -lallegro_primitives -lallegro_dialog -lallegro_ttf -lallegro_font -lallegro_audio -lallegro_acodec -lallegro
LFLAGS = $(ALLEGRO) -lm

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

all: purge $(PROG)

$(PROG): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

clean:
	rm -rf *.o

purge: clean
	rm -rf $(PROG)
