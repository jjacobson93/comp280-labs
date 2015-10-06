CC = gcc
CFLAGS = -g -Wall -Wextra -std=c99
# LDFLAGS = -lm

TARGET = gol

all: $(TARGET)

gol: gol.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) -r $(TARGET) *.o *.dSYM
