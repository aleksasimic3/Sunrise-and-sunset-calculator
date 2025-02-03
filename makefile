CC = g++
CFLAGS = -Wall -Iinclude

SOURCE = src/
INCLUDE = include/
BUILD = build/
BINARY = bin/

$(BINARY)sunrise.exe: $(BUILD)main.o $(BUILD)sunrise.o
	$(CC) -o $(BINARY)sunrise.exe $(BUILD)main.o $(BUILD)sunrise.o $(CFLAGS)

$(BUILD)main.o: $(SOURCE)main.cpp $(INCLUDE)sunrise.h
	$(CC) -c -o $(BUILD)main.o $(SOURCE)main.cpp $(CFLAGS)

$(BUILD)sunrise.o: $(SOURCE)sunrise.cpp $(INCLUDE)sunrise.h
	$(CC) -c -o $(BUILD)sunrise.o $(SOURCE)sunrise.cpp $(CFLAGS)