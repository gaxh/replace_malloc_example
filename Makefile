
CFLAGS = -g -Wall -Werror -D_GNU_SOURCE
CFILES = replace_malloc.c
LDFLAGS = -ldl

replace_malloc: $(CFILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f replace_malloc

