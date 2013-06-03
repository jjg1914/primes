%.a : %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -S -o $@ $<

CFILES=primes.c
CFLAGS=

primes: $(subst .c,.o,$(CFILES))
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^

assemble: $(subst .c,.a,$(CFILES))
