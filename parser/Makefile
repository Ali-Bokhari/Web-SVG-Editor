CFLAGS = -Wall -g -std=c11
BIN =
INC = include/
SRC = src/
XML_PATH = /usr/include/libxml2

#UNAME Shell Variable
UNAME_S := $(shell uname -s)

# OS X Dependant Variables
ifeq ($(UNAME_S), Darwin)
    LIB := libsvgparse.dylib
# GNU/Linux Depedant Variables
else ifeq ($(UNAME_S), Linux)
    LIB := libsvgparse.so
endif

# Standard common makefile
parser: $(BIN)$(LIB)

$(BIN)$(LIB): $(BIN)SVGParser.o $(BIN)LinkedListAPI.o
	gcc -shared -o ../$(LIB) $(BIN)SVGParser.o $(BIN)LinkedListAPI.o -lxml2 -lm

$(BIN)SVGParser.o: $(SRC)SVGParser.c $(INC)LinkedListAPI.h $(INC)SVGParser.h
	gcc -c -fpic $(CFLAGS) -I$(XML_PATH) -I$(INC) $(SRC)SVGParser.c -o $(BIN)SVGParser.o

$(BIN)LinkedListAPI.o: $(SRC)LinkedListAPI.c $(INC)LinkedListAPI.h
	gcc -c -fpic $(CFLAGS) -I$(INC) $(SRC)LinkedListAPI.c -o $(BIN)LinkedListAPI.o

clean:
	rm $(BIN)*.o ../$(LIB)
