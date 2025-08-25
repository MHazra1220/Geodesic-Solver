main.sh: main.o world.o particle.o camera.o
	g++ -o main.sh main.o world.o particle.o camera.o -fopenmp -O1

main.o: main.cpp
	g++ -c main.cpp -O1

world.o: world.h world.cpp
	g++ -c world.cpp -O1

particle.o: particle.h particle.cpp
	g++ -c particle.cpp -O1

camera.o: camera.h camera.cpp
	g++ -c camera.cpp -fopenmp -O1

clean:
	rm main.o world.o particle.o camera.o
