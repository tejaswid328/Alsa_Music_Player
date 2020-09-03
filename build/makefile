CC = gcc
FLAGS ?= -c
BDFLAGS ?= -shared
MFLAGS ?= -lm
CFLAGS ?= -g -Wall -fPIC -Wextra -lasound -lpthread

#library to be generated
OUT = libmusic_player.so

SRCS := $(wildcard ./../src/*.c)

HEADERS = -I ../interface/ \
	-I ../inc/
OBJS := $(patsubst ./../src/%,%,$(SRCS:.c=.o))
.PHONY: all
all: $(OUT)

$(OUT): $(OBJS)
	$(CC) $(BDFLAGS) -o $(OUT) $(OBJS) $(MFLAGS)

$(OBJS): $(SRCS)
	$(CC) $(FLAGS) $(CFLAGS) $(SRCS) $(HEADERS)

.PHONY: clean

clean:
	rm -f *.o *.so
