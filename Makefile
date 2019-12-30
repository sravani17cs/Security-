all: 
	gcc -o scopy scopy.c
run:
	./scopy perfile.ext destfile.txt
clean:
	rm *.o ./scopy
