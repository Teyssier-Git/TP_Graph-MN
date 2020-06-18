all: test_graphe

test_graphe: test_graphe.o graphe.o io_graphe.o fap.o pile.o file.o
	gcc -o test_graphe test_graphe.o graphe.o io_graphe.o fap.o pile.o file.o

graphe.o: graphe.c graphe.h file.h pile.h fap.h
	gcc -Wall -c graphe.c

fap.o: fap.c fap.h
	gcc -Wall -c fap.c

pile.o: pile.c pile.h
	gcc -Wall -c pile.c

file.o: file.c file.h
	gcc -Wall -c file.c

io_graphe.o: io_graphe.c graphe.h
	gcc -Wall -c io_graphe.c


test_graphe.o: test_graphe.c graphe.h file.h pile.h fap.h
	gcc -Wall -c test_graphe.c

clean:
	rm -f *.o test_graphe  *~ data/*~
