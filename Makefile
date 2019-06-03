# Author: j0lama

LIBNAME = libgc

# COMPILER
CC = gcc

#FLAGS
CFLAGS = -c -g -Os -Wall -Wextra -pedantic

TARGET = $(LIBNAME).a

# PATHS
SOURCE_DIRECTORY = source/
INCLUDE_DIRECTORY = include/
OBJECT_DIRECTORY = objects/
BUILD_DIRECTORY = build/

CFILES = $(wildcard $(SOURCE_DIRECTORY)*.c)
OBJECTS = $(patsubst $(SOURCE_DIRECTORY)%.c, $(OBJECT_DIRECTORY)%.o, $(CFILES))


ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif

all: $(OBJECT_DIRECTORY) $(TARGET)

$(TARGET): $(OBJECTS)
	@echo "Building static library..."
	@ar rcs $@ $^

$(OBJECT_DIRECTORY):
	@mkdir $(OBJECT_DIRECTORY)

$(OBJECT_DIRECTORY)%.o: $(SOURCE_DIRECTORY)%.c
	@echo "Building $@..."
	@$(CC) -I $(INCLUDE_DIRECTORY) $(CFLAGS) $< -o $@

.PHONY: install
install: $(TARGET)
	install -d $(DESTDIR)$(PREFIX)/lib/
	install -m 644 $(TARGET) $(DESTDIR)$(PREFIX)/lib/
	install -d $(DESTDIR)$(PREFIX)/include/
	install -m 644 $(INCLUDE_DIRECTORY)$(LIBNAME).h $(DESTDIR)$(PREFIX)/include/
	@echo "Installation done."

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/lib/$(TARGET)
	rm -f $(DESTDIR)$(PREFIX)/include/$(LIBNAME).h

.PHONY: clean
clean:
	@echo "Removing objects files"
	@rm -rf $(OBJECT_DIRECTORY) $(TARGET)