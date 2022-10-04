all: a.out child1 child2

a.out: ./parent.c
	gcc ./parent.c
	
child1.o: ./child1.c
	gcc -c ./child1.c

child2.o: ./child2.c
	gcc -c ./child2.c

clean:
	rm -rf *.o a.out
