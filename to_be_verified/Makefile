CXX=g++
CXXFLAGS=-std=c++14 -Wall -g
EXEC=raiinet
FILES= controller.cc game.cc main.cc textdisplay.cc window.cc link.cc board.cc player.cc
OBJECTS=${FILES:.cc=.o}

${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC} -lX11

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC}
