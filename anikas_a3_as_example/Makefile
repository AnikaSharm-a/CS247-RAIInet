CXX=g++
CXXFLAGS=-std=c++14 -Wall -g
EXEC=flood
FILES= cell.cc controller.cc game.cc main.cc textdisplay.cc view.cc graphicdisplay.cc window.cc
OBJECTS=${FILES:.cc=.o}

${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC} -lX11

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC}
