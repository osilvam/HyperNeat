EXECUTABLE = HyperNeat
VPATH = ./src ./headers ./objects
CC = g++ -O3
CFLAGS = -g -Wall -I./headers -I./objects -I./src

all: main.o  HyperNeat.o Substrate.o SpatialNode.o SpatialConnection.o CPPNInputs.o
	@$(CC) $(CFLAGS) ./objects/main.o ./objects/HyperNeat.o ./objects/Substrate.o ./objects/SpatialNode.o ./objects/SpatialConnection.o ./objects/CPPNInputs.o -o ./bin/HyperNeat

main.o: main.cpp
	@$(CC) $(CFLAGS) -c ./src/main.cpp  -o ./objects/main.o

HyperNeat.o: HyperNeat.cpp
	@$(CC) $(CFLAGS) -c ./src/HyperNeat.cpp  -o ./objects/HyperNeat.o

Substrate.o: Substrate.cpp
	@$(CC) $(CFLAGS) -c ./src/Substrate.cpp  -o ./objects/Substrate.o

SpatialNode.o: SpatialNode.cpp
	@$(CC) $(CFLAGS) -c ./src/SpatialNode.cpp  -o ./objects/SpatialNode.o

SpatialConnection.o: SpatialConnection.cpp 
	@$(CC) $(CFLAGS) -c ./src/SpatialConnection.cpp -o ./objects/SpatialConnection.o

CPPNInputs.o: CPPNInputs.cpp 
	@$(CC) $(CFLAGS) -c ./src/CPPNInputs.cpp  -o ./objects/CPPNInputs.o

clean:
	@rm -f ./objects/*.o ./bin/*