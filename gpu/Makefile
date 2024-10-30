LIBOBJS = lib.o

all: main

%.o : %.s
	as $< -g -o $@

lib.a: $(LIBOBJS)
	ar -cvq lib.a $(LIBOBJS)

main: main.c gpu_lib.c lib.a
	gcc -o main main.c gpu_lib.c lib.a
clean:
	rm -f main lib.a $(LIBOBJS)
