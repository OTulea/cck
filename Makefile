CXX = g++
CXXFLAGS = -std=c++17 -Wall -MMD -g -Werror=vla
EXEC = cc3k+
OBJECTS = main.o generate.o cell.o controller.o display.o world.o character.o gameObject.o gold.o player.o dragon.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
