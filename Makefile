CC = gcc
CFLAG = `pkg-config --cflags gtk+-3.0 gmodule-2.0`
CFLAG_LIBS = `pkg-config --libs --cflags gtk+-3.0 gmodule-2.0`
DEBUG = -Wall -g -w

OBJECTS = main_window.o callback.o

.PHONY: clean

all: main_window

main_window: $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(DEBUG) $(CFLAG_LIBS)

main_window.o: main_window.c support.h
	$(CC) $(DEBUG) $(CFLAG_LIBS) -c $< -o $@

callback.o: callback.c support.h
	$(CC) $(DEBUG) $(CFLAG) -c $< -o $@

clean:
	rm -f *.o main_window .*.c.swm .*.c.swn .*.h.swn .*.h.swo .*.h.swp .*.c.swp
