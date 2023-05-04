# Target library
lib := libuthread.a
src := queue.c
obj := $(src:.c=.o)
all: $(lib)

## TODO: Phase 1

$(lib): $(obj)
	ar rcs $@ $^
%.o: %.c
	gcc -c -o $@ $<
clean:
	rm -f $(lib) $(obj)
