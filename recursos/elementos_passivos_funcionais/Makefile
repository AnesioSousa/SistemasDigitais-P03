LIBOBJS = lib.o

all: main

main: main.c gpu_lib.c gpu_letters.c lib.a
	gcc -Wall main.c gpu_lib.c gpu_letters.c -o main lib.a -lintelfpgaup -lpthread -std=c99

lab: lab.c gpu_lib.c gpu_letters.c lib.a
	gcc -Wall lab.c gpu_lib.c gpu_letters.c -o lab lib.a -lintelfpgaup -lpthread

passive: elementos_passivos.c gpu_lib.c gpu_letters.c lib.a
	gcc -Wall elementos_passivos.c gpu_lib.c gpu_letters.c -o main lib.a -lintelfpgaup -lpthread -std=c99

%.o : %.s
	as $< -g -o $@

lib.a: $(LIBOBJS)
	ar -cvq lib.a $(LIBOBJS)

clean:
	rm -f main lab lib.a $(LIBOBJS)
