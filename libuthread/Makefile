# Target library
lib := libuthread.a
src := queue.c uthread.c context.c sem.c 
obj := $(src:.c=.o)
all: $(lib)

$(lib): $(obj)
	ar rcs $@ $^

%.o: %.c
	gcc -Wall -Wextra -Werror -c -o $@ $<

clean:
	rm -f $(lib) $(obj) *.o
