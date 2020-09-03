CC = gcc
FLAGS ?= -lm -lasound -lpthread

#Binary to be generated
OUT=music_player

SRCS := musicplayer.c

HEADERS := -I ../interface \
	-I ../inc

.PHONY: all

all: $(OUT)

$(OUT): $(SRCS)
	$(CC) -o $(OUT) $(HEADERS) $(SRCS) -L./../build -lmusic_player $(FLAGS)

.PHONY: clean

clean:
	rm $(OUT)
