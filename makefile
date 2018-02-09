TARGET = terpi
LIBS = -lm -lwiringPi -lcurl
CC = g++
CFLAGS = -g -Wall

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.cpp, %.o, $(wildcard *.cpp)) $(patsubst %.cpp, %.o, $(wildcard **/*.cpp)) $(patsubst %.c, %.o, $(wildcard *.c)) $(patsubst %.c, %.o, $(wildcard **/*.c))
HEADERS = $(wildcard *.hpp) $(wildcard **/*.hpp) $(wildcard *.h) $(wildcard **/*.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)
