target = example

example: example.o
	gcc -Wall -o $@ $<

example.o: example.c bopt.h
	gcc -Wall -o $@ -c $<

clean:
	rm -f example example.o
