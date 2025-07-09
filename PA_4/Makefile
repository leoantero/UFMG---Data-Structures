INCLUDE_DIR = include
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

CXX = g++
CXXFLAGS = -Wall -I$(INCLUDE_DIR) -std=c++11

SOURCES = $(SRC_DIR)/main.cpp $(SRC_DIR)/unionFind.cpp
OBJECTS = $(OBJ_DIR)/main.o $(OBJ_DIR)/unionFind.o

EXEC = $(BIN_DIR)/pa4.out

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(EXEC)


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean:
	rm -f $(OBJ_DIR)/*.o
	rm -f $(EXEC)
