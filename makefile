main.sh: main.o world.o particle.o
	g++ -o main.sh main.o world.o particle.o -fopenmp

main.o: main.cpp world.o particle.o
	g++ -c main.cpp -fopenmp

world.o: world.h world.cpp particle.h particle.cpp
	g++ -c world.cpp

particle.o: particle.h particle.cpp
	g++ -c particle.cpp

clean:
	rm main.sh main.o world.o particle.o
