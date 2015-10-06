CC = gcc
CFLAGS = -std=c99 -g -Wall -Wextra

TARGET = employee_db

LIB = readfile.o sort.o core.o

SRC = $(TARGET).c 

all: $(TARGET)

$(TARGET): $(SRC) $(LIB)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LIB)

$(LIB): readfile.c readfile.h sort.c sort.h core.c core.h
	$(CC) $(CFLAGS) -c readfile.c sort.c core.c

clean:
	$(RM) $(TARGET) $(LIB)
