# Author: j0lama

# COMPILER
CC = gcc

#FLAGS
CFLAGS = -c -g -Os -Wall -Wextra -pedantic

TARGET = demo

# PATHS
SOURCE_DIRECTORY = source/
INCLUDE_DIRECTORY = include/
OBJECT_DIRECTORY = objects/
BUILD_DIRECTORY = build/

CFILES = $(wildcard $(SOURCE_DIRECTORY)*.c)
OBJECTS = $(patsubst $(SOURCE_DIRECTORY)%.c, $(OBJECT_DIRECTORY)%.o, $(CFILES))

all: $(TARGET)

# EXEC CREATION
$(TARGET): $(OBJECT_DIRECTORY) $(OBJECTS)
	@echo "Linking objects..."
	@$(CC) $(OBJECTS) -o $@

$(OBJECT_DIRECTORY):
	@mkdir $(OBJECT_DIRECTORY)

# OBJECT CREATION
$(OBJECT_DIRECTORY)%.o: $(SOURCE_DIRECTORY)%.c
	@echo "Building $@..."
	@$(CC) -I $(INCLUDE_DIRECTORY) $(CFLAGS) $< -o $@ 

# COMMANDS
valgrind: 
	valgrind --leak-check=full --show-leak-kinds=all $(TARGET)

clean:
	@echo "Removing objects files"
	@rm -rf $(OBJECT_DIRECTORY) $(TARGET)