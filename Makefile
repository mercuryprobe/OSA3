main: q1.out
	./q1.out
	rm -f *.out *.o

q1.out: q1.c
	gcc q1.c -o q1.out