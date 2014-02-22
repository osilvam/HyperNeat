EXECUTABLE = HyperNeat
VPATH = ./src ./headers ./objects
CC = g++ -O3
CFLAGS = -g -Wall -I./headers -I./objects -I./src

all: main.o  HyperNeat.o Substrate.o SpatialNode.o SpatialConnection.o CPPNInputs.o
	@echo "Linking main.o  HyperNeat.o Substrate.o SpatialNode.o SpatialConnection.o CPPNInputs.o to ./bin/HyperNeat"
	@$(CC) $(CFLAGS) ./objects/main.o ./objects/HyperNeat.o ./objects/Substrate.o ./objects/SpatialNode.o ./objects/SpatialConnection.o ./objects/CPPNInputs.o -o ./bin/HyperNeat

main.o: main.cpp	
	@echo "Compiling main.cpp to main.o"
	@$(CC) $(CFLAGS) -c ./src/main.cpp  -o ./objects/main.o

HyperNeat.o: HyperNeat.cpp
	@echo "Compiling HyperNeat.cpp to HyperNeat.o"
	@$(CC) $(CFLAGS) -c ./src/HyperNeat.cpp  -o ./objects/HyperNeat.o

Substrate.o: Substrate.cpp
	@echo "Compiling Substrate.cpp to Substrate.o"
	@$(CC) $(CFLAGS) -c ./src/Substrate.cpp  -o ./objects/Substrate.o

SpatialNode.o: SpatialNode.cpp
	@echo "Compiling SpatialNode.cpp to SpatialNode.o"
	@$(CC) $(CFLAGS) -c ./src/SpatialNode.cpp  -o ./objects/SpatialNode.o

SpatialConnection.o: SpatialConnection.cpp 
	@echo "Compiling SpatialConnection.cpp to SpatialConnection.o"
	@$(CC) $(CFLAGS) -c ./src/SpatialConnection.cpp -o ./objects/SpatialConnection.o

CPPNInputs.o: CPPNInputs.cpp 
	@echo "Compiling CPPNInputs.cpp to CPPNInputs.o"
	@$(CC) $(CFLAGS) -c ./src/CPPNInputs.cpp  -o ./objects/CPPNInputs.o

clean:
	@rm -f ./objects/*.o ./bin/*