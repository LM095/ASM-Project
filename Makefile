CC=gcc
CFLAGS=-m32
LFLAGS=-m32
SOURCES=controller.c
OBJECTS = controller.o
EXECUTABLE=controller

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) -o $(EXECUTABLE)

controller.o : controller.c
	$(CC) $(CFLAGS) -c controller.c

clean:
	@rm *.o $(EXECUTABLE)
