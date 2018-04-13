
#Makefile

OFILES=main.o glad/glad.o robot.o wall.o
CFILES=main.cpp glad/glad.c robot.cpp wall.cpp
CC=gcc
CFLAGS=

LIB= -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo  -lm -lstdc++ 

all : aspiracers

aspiracers : $(OFILES)
	$(CC) $(OFILES) -o $@ $(LIB)
	@rm -f *.o

	
clean : 
	@echo "CLEANING"
	@rm -f *.o
	
depend : 
	@echo "UPDATING DEPENDENCIES"
	@(sed '/^# DEPENDANCES/q' Makefile && \
	  $(CC) -MM $(CFLAGS) $(CFILES) | \
	  egrep -v "/usr/include" \
	 ) >Makefile.new
	@mv Makefile.new Makefile

# DEPENDANCES
main.o: main.cpp glad/glad.h glad/khrplatform.h robot.h cst.h wall.h
glad.o: glad/glad.c glad/glad.h glad/khrplatform.h
robot.o: robot.cpp robot.h cst.h
wall.o: wall.cpp wall.h robot.h cst.h
