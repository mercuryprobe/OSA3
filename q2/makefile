main: p1Soc.out p2Soc.out
	rm -r /tmp/socketFile
	./p1Soc.out & ./p2Soc.out

p1Soc.out: p1Soc.c
	gcc p1Soc.c -o p1Soc.out

p2Soc.out: p2Soc.c
	gcc p2Soc.c -o p2Soc.out