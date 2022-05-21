default: build
build:
	gcc -c *.c 
	gcc -no-pie -o Filesystem.out *.o 
