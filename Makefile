OBJECTS = main.o building.o ghost.o room.o hunter.o evidence.o sharedFunctions.o
HOST_CC = gcc -g
THREAD = -lpthread
EXEC = GhostHunters



all: $(OBJECTS)
	$(HOST_CC) -o $(EXEC) $(OBJECTS) $(THREAD)

main.o: main.c gameDefs.h
	$(HOST_CC) -c main.c

building.o: building.c gameDefs.h
	$(HOST_CC) -c building.c

ghost.o: ghost.c gameDefs.h
	$(HOST_CC) -c ghost.c

room.o: room.c gameDefs.h
	$(HOST_CC) -c room.c


hunter.o: hunter.c gameDefs.h
	$(HOST_CC) -c hunter.c

evidence.o: evidence.c gameDefs.h
	$(HOST_CC) -c evidence.c

sharedFunctions.o: sharedFunctions.c gameDefs.h
	$(HOST_CC) -c sharedFunctions.c

valgrind:
	valgrind --log-file="valgrindOutput.txt" --leak-check=full --track-origins=yes -s ./$(EXEC)

clean:
	rm -f *.o *~ $(EXEC)
