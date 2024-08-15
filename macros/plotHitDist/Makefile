MAIN = ../..
SRC = $(MAIN)/src
BIN = $(MAIN)/bin

# List sources manually
SOURCE = CAENd5202.cpp fiber.cpp
OBJECT = $(patsubst %, $(BIN)/%, $(notdir $(SOURCE:.cpp=.o)))
HEADER = CAENd5202.h fiber.h
LIBSRC = $(patsubst %, $(SRC)/%, $(HEADER))

CC = g++
LINKOPTION = $(shell root-config --libs --cflags)
INCLUDE = -I$(shell root-config --incdir)
CFLAGS = -c -w -std=c++14 $(INCLUDE)

plot : plot.o $(OBJECT) $(MAIN)/libSFA.cxx
	@echo "Linking..."
	$(CC) -o$@ $(filter-out %.cxx, $^) $(LINKOPTION) $(filter %.cxx, $^)
	@echo "Finished"

plot.o : plot.cpp
	@echo "Compiling..."
	$(CC) $(CFLAGS) -c $< -o$@

clean :
	rm -f plot.o
	rm -f plot
