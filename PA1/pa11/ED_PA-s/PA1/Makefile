CC = gcc
CFLAGS = -Wall -Iinclude
SRC_DIR = src
BIN_DIR = bin
OBJ_DIR = obj

all: $(BIN_DIR)/pa1.out

$(BIN_DIR)/pa1.out: $(OBJ_DIR)/evalenum.o
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/evalenum.o: $(SRC_DIR)/evalenum.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/pa1.out