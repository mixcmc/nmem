CC=g++

all: main.o Node.o Torus.o
	$(CC) -o test *.o 

%.o: %.cpp
	$(CC) -c $<
	
clean:
	rm -rf *.o