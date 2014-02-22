DFLAG = -g
OBJS = Substrate.o SpatialNode.o SpatialConnection.o CPPNInputs.o HyperNeat.o main.o
OUTPUT = bin/HyperNeat

OS = $(shell uname -s)
ECHO = @


all: $(OBJS)
		@mkdir -p bin
		@echo "Linking $(OBJS) to $(OUTPUT)"
		$(ECHO)$(CXX) $(OBJS) -o $(OUTPUT)

%.o: %.cpp
		@echo "Compiling $< to $@"
		$(ECHO)$(CXX) $(DFLAG) -c $< -o $@

%.o: %.c
		@echo "Compiling $< to $@" 
		$(ECHO)$(CC) $(DFLAG) -c $< -o $@

clean:
		@rm -f $(OBJS) bin/HyperNeat
