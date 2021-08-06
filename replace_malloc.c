
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <unistd.h>
#include <dlfcn.h>

static void *dlsym_next( const char *name ) {
    return dlsym( RTLD_NEXT, name );
}

typedef void *(*MALLOC_FUNC_TYPE)(size_t);
typedef void (*FREE_FUNC_TYPE)(void *);

struct old_malloc {
    MALLOC_FUNC_TYPE malloc;
    FREE_FUNC_TYPE free;
};

static void old_malloc_init( struct old_malloc *om ) {
    om->malloc = dlsym_next("malloc");
    om->free = dlsym_next("free");
}

static struct old_malloc om;

#define t_malloc malloc
#define t_free free

static void unbuffered_fprintf( int fd, const char *fmt, ... ){
    static char buffer[10240];

    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    write(fd, buffer, strlen(buffer));
}

void *t_malloc( size_t sz ) {
    void *ret = om.malloc(sz);

    unbuffered_fprintf( 1, "malloc: sz=%zu, p=%p\n", sz, ret );
    
    return ret;
}

void t_free( void *p ) {
    om.free(p);

    unbuffered_fprintf( 1, "free: p=%p\n", p );
}

int main() {
    old_malloc_init(&om);

    for(int i = 0; i < 100; ++i)
    {
        int *mm = malloc( (i+1) * sizeof(mm[0]) );

        for(int j = 0; j < i+1; ++j)
        {
            mm[j] = j;
        }

        free(mm);
    }

    return 0;
}

