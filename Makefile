
CC=g++ -std=c++11
CCFLAGS += -l SDL2 -l SDL2_image -l SDL2_ttf -l SDL2_mixer

all: a.out

a.out: main.o rsdl.o ball.o game.o team.o player.o field.o playermove.o scoreboard.o
	$(CC) main.o rsdl.o ball.o game.o team.o player.o field.o playermove.o scoreboard.o $(CCFLAGS) -o soccerStars.out

main.o: main.cpp main.hpp ball.hpp game.hpp field.hpp team.hpp playermove.hpp
	$(CC) -c main.cpp main.hpp ball.hpp game.hpp field.hpp team.hpp playermove.hpp

game.o: game.cpp game.hpp ball.hpp game.hpp field.hpp team.hpp scoreboard.hpp
	$(CC) -c game.cpp game.hpp ball.hpp game.hpp field.hpp team.hpp scoreboard.hpp

team.o: team.cpp team.hpp player.hpp
	$(CC) -c team.cpp team.hpp player.hpp

player.o: player.cpp player.hpp field.hpp 
	$(CC) -c player.cpp player.hpp field.hpp

ball.o: ball.cpp ball.hpp playermove.hpp field.hpp scoreboard.hpp
	$(CC) -c ball.cpp ball.hpp playermove.hpp field.hpp scoreboard.hpp

field.o: field.cpp field.hpp
	$(CC) -c field.cpp field.hpp

playermove.o: playermove.cpp playermove.hpp player.hpp team.hpp
	$(CC) -c playermove.cpp playermove.hpp player.hpp team.hpp

scoreboard.o: scoreboard.cpp scoreboard.hpp
	$(CC) -c scoreboard.cpp scoreboard.hpp

rsdl.o: RSDL-master/src/rsdl.cpp
	$(CC) -c RSDL-master/src/rsdl.cpp 

.PHONY: clean
clean:
	rm *.o
	rm a.out




