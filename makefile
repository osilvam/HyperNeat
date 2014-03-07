EXECUTABLE = HyperNeat
VPATH = ./src ./headers ./objects
CC = g++ -O3
CFLAGS = -g -Wall -I./headers -I./objects -I./src

all: main.o  HyperNeat.o Substrate.o SpatialNode.o CPPNInputs.o UserFunctions.o CPPN-NEAT.o function.o genetic_encoding.o
	@echo "Linking main.o  HyperNeat.o genetic_encoding.o Substrate.o SpatialNode.o function.o CPPNInputs.o UserFunctions.o CPPN-NEAT.o to ./bin/HyperNeat"
	@$(CC) $(CFLAGS) ./objects/main.o ./objects/genetic_encoding.o ./objects/function.o ./objects/HyperNeat.o ./objects/Substrate.o ./objects/SpatialNode.o ./objects/CPPN-NEAT.o ./objects/CPPNInputs.o ./objects/UserFunctions.o -o ./bin/HyperNeat

main.o: main.cpp 
	@echo "Compiling main.cpp to main.o"
	@$(CC) $(CFLAGS) -c ./src/main.cpp -o ./objects/main.o

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

UserFunctions.o: UserFunctions.cpp 
	@echo "Compiling UserFunctions.cpp to UserFunctions.o"
	@$(CC) $(CFLAGS) -c ./src/UserFunctions.cpp -o ./objects/UserFunctions.o

CPPN-NEAT.o: CPPN-NEAT.cpp
	@echo "Compiling CPPN-NEAT.cpp to CPPN-NEAT.o"
	@$(CC) $(CFLAGS) -c ./src/CPPN-NEAT.cpp -o ./objects/CPPN-NEAT.o

function.o: function.cpp
	@echo "Compiling function.cpp to function.o"
	@$(CC) $(CFLAGS) -c ./src/function.cpp -o ./objects/function.o

genetic_encoding.o: genetic_encoding.cpp
	@echo "Compiling genetic_encoding.cpp to genetic_encoding.o"
	@$(CC) $(CFLAGS) -c ./src/genetic_encoding.cpp -o ./objects/genetic_encoding.o

clean:
	@rm -f ./objects/*.o