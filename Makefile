CC=g++
CFLAGS=-c -O3 -I ./
SRC_DIR=src
OBJ_DIR=build
HEAD_DIR=include

EXEC=TOV

SOURCES=$(addprefix $(SRC_DIR)/, \
	main.cpp constants.cpp eos.cpp LSODA.cpp tov.cpp )

OBJECTS=$(SOURCES:$(SRC_DIR)%.cpp=$(OBJ_DIR)%.o)

all: $(EXEC)

$(EXEC): $(OBJECTS) 
	$(CC) $^ -o $@ -lm

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) $< -o $@ -lm

clean: 
	rm $(OBJECTS) $(EXEC)