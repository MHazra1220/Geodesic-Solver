main.sh: main.o world.o particle.o camera.o
	g++ -o main.sh main.o world.o particle.o camera.o -fopenmp

main.o: main.cpp
	g++ -c main.cpp -fopenmp

world.o: world.h world.cpp
	g++ -c world.cpp

particle.o: particle.h particle.cpp
	g++ -c particle.cpp

camera.o: camera.h camera.cpp
	g++ -c camera.cpp

clean:
	rm main.o world.o particle.o camera.o
