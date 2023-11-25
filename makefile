# Compiler
CC = gcc

# Flags
CFLAGS = -Wall

# Debug flags
DEBUG_CFLAGS = -g

LIBS = -lSDL2 -lm

# Source files (all .c files in the current directory)
SRCS = $(wildcard *.c)

# Object files (corresponding .o files)
OBJS = $(SRCS:.c=.o)

# Output executables
TARGET = program
TARGET_DEBUG = program-debug

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

$(TARGET_DEBUG): $(OBJS)
	$(CC) $(CFLAGS) $(DEBUG_CFLAGS) -o $@ $(OBJS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(DEBUG_CFLAGS) -c $< $(LIBS)

clean:
	rm -f $(OBJS) $(TARGET) $(TARGET_DEBUG)

run: $(TARGET)
	./$(TARGET)
