CXX=g++
CXXFLAGS=-std=c++14 -Wall -g
EXEC=raiinet
FILES= controller.cc game.cc main.cc textdisplay.cc graphicdisplay.cc window.cc link.cc board.cc player.cc ability.cc abilityFactory.cc download.cc firewall.cc linkboost.cc polarize.cc scan.cc
OBJECTS=${FILES:.cc=.o}

${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC} -lX11

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC}
