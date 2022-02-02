all:
	gcc *.c -o main

run:
	./main

test:
	gcc test.c -o test 
	
runtest:
	./test