SRC = src
BIN = bin

# List sources manually to exclude sim.cpp and simmulti.cpp
SOURCE = det.cpp histo.cpp CAENd5202.cpp fiber.cpp
OBJECT = $(patsubst %, $(BIN)/%, $(notdir $(SOURCE:.cpp=.o)))
HEADER = CAENd5202.h fiber.h
LIBSRC = $(patsubst %, $(SRC)/%, $(HEADER))

CC = g++
LINKOPTION = $(shell root-config --libs --cflags)
INCLUDE = -I$(shell root-config --incdir)
CFLAGS = -c -w -std=c++14 $(INCLUDE)

sort : $(BIN)/sort.o $(OBJECT) libSFA.cxx
	@echo "Linking..."
	$(CC) -o$@ $(filter-out %.cxx, $^) $(LINKOPTION) $(filter %.cxx, $^)
	@echo "Finished"

$(BIN)/%.o : $(SRC)/%.cpp
	@echo "Compiling..."
	$(CC) $(CFLAGS) -c $< -o$@

libSFA.cxx : $(LIBSRC) $(SRC)/LinkDef.h
	rootcling $@ $^

clean :
	rm -f $(BIN)/*.o
	rm -f *.cxx
	rm -f *.pcm
