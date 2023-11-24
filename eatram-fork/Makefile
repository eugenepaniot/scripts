CC	= gcc
CFLAGS	= -O2
LDFLAGS	=

all:	eat

eat: eat.o
	@echo "=== $@ ==="
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)

eat.o : eat.c
	@echo "=== making object: $@ ==="
	$(CC) $(LDFLAGS) $(CFLAGS) -c $< -o $@

clean:
	@echo "=== $@ ==="
	@rm -fv *.o *~
	@rm -fv eat
