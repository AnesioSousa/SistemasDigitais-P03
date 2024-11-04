LIBOBJS = lib.o

all: main

%.o : %.s
	as $< -g -o $@

lib.a: $(LIBOBJS)
	ar -cvq lib.a $(LIBOBJS)

main: main2.c lib.a
	gcc -o main main2.c lib.a
clean:
	rm -f main lib.a $(LIBOBJS)
