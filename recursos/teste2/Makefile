LIBOBJS = lib.o

all: main

main: main.c gpu_lib.c gpu_letters.c lib.a
	gcc -Wall main.c gpu_lib.c gpu_letters.c -o main lib.a -lintelfpgaup -lpthread -std=c99
		
%.o : %.s
	as $< -g -o $@

lib.a: $(LIBOBJS)
	ar -cvq lib.a $(LIBOBJS)

clean:
	rm -f main lib.a $(LIBOBJS)
