### Compiler & flags
CC=g++
CFLAGS=-g -c -O3 -I ./
### Directories
SRC_DIR=src
OBJ_DIR=build
HEAD_DIR=include

### Executable name
EXEC=TOV

### Sources (in src directory)
SOURCES=$(addprefix $(SRC_DIR)/, \
	main.cpp constants.cpp eos.cpp LSODA.cpp tov.cpp )

### Objects (in build directory)
OBJECTS=$(SOURCES:$(SRC_DIR)%.cpp=$(OBJ_DIR)%.o)

### Rules: #######################################

### General target (executable):
all: $(EXEC)

### How to make the executable:
$(EXEC): $(OBJECTS) 
	$(CC) $^ -o $@ -lm

### How to make every object:
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) $< -o $@ -lm

### How to clean up:
clean: 
	rm $(OBJECTS) $(EXEC)
