all:
		gcc -Wall main.c -o main -lpthread -std=c99
			sudo ./main
clean:
	rm -f *.o main
