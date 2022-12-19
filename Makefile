main: q1a1.out
	./q1a1.out
	rm -f *.out *.o

q1.out: q1a1.c
	gcc q1a1.c -o q1a1.out