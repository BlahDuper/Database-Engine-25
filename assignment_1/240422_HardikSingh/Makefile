# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Target executable
TARGET = btree

# Source files
SRCS = operations.c
OBJS = $(SRCS:.c=.o)

# Build rule
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Clean rule
clean:
	rm -f $(OBJS) $(TARGET)
