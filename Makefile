CC=g++
#DBGFLAGS=-std=c++0x -Wall -fsanitize=address -g
DBGFLAGS=-std=c++14 -Wall -D_GLIBCXX_DEBUG -g
FLAGS=-Wconversion -Wall -Wextra -Wshadow -std=c++14 -O2 -g -fsanitize=address#-D_GLIBCXX_DEBUG


re: clean sim

#debug: main.cpp txtgen.cpp txtgen.h
#        $(CC) $(DBGFLAGS) main.cpp txtgen.cpp -o dbgen

sim: simulation.o pixel.o main.o 
	$(CC) $(FLAGS) main.o simulation.o pixel.o -o sim

main.o: main.cpp simulation.h pixel.h pixel.o simulation.o
	$(CC) $(FLAGS) -c main.cpp 
        
simulation.o: simulation.cpp simulation.h pixel.h
	$(CC) $(FLAGS) -c simulation.cpp 

pixel.o: pixel.cpp pixel.h simulation.h
	$(CC) $(FLAGS) -c pixel.cpp

clean: 
	rm -fd main.o simulation.o pixel.o sim
        
test: test.cpp
	rm -f test
	ls
	$(CC) $(FLAGS) test.cpp -o test

