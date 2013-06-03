%.a : %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -S -o $@ $<

CFILES=primes.c
CFLAGS=-Ofast

primes: $(subst .c,.o,$(CFILES))
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^

assemble: $(subst .c,.a,$(CFILES))

clean:
	rm -f primes $(subst .c,.o,$(CFILES))
