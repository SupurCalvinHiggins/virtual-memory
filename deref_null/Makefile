CC = g++
CCFLAGS = -Wall -g

SOURCE = main.c
EXECUTABLE = main

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCE)
	$(CC) $(CCFLAGS) -o $@ $<

clean:
	rm -f $(EXECUTABLE)
