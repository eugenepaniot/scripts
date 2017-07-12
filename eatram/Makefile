CC	= gcc -std=gnu99
CFLAGS	= -O2 -g -pipe -fpic -fexceptions -fstack-protector --param=ssp-buffer-size=4 -fno-strict-aliasing -fwrapv 
LDFLAGS	= -lrt -Wl,--as-needed -Wl,-z,relro  -Wl,--hash-style=gnu -rdynamic

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
