SRC = src
BIN = bin

#list source manually to exclude sim.cpp and simmulti.cpp
SOURCE = det.cpp histo.cpp CAENd5202.cpp
OBJECT = $(patsubst %, $(BIN)/%, $(notdir $(SOURCE:.cpp=.o)))

CC = g++
CFLAGS= -c -w -std=c++17 -I$(shell root-config --incdir)
LINKOPTION = $(shell root-config --libs) 

sort : $(BIN)/sort.o $(OBJECT)
	@echo "Linking..."
	$(CC) -o $@ $^ $(LINKOPTION)
	@echo "Finished"

$(BIN)/%.o : $(SRC)/%.cpp
	@echo "Compiling..."
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(BIN)/*.o 



