#include "types.h"
#include <stdalign.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    void (*dtor)(void *);
    size_t num_bytes;
    size_t refcount;
    alignas(max_align_t) char data[];
} smart_ptr;

static smart_ptr *get_s(void *ptr);
// constructor
void *smalloc(u32 size, void (*f)(void *));

// destructor
void sfree(smart_ptr *ptr);

void sdestructor(void *p);
// accessor

// wtf is this garbage
typedef void (*variadic_func_ptr)(const char *, ...);

// move semantics on cleanup
void decref(void *ptr);
void incref(smart_ptr *ptr);
// smart_ptr *move(smart_ptr *ptr);
void s_cpy(void *tis, void *that);
void variadic_func(const char *format, ...);
void s_assign(void **dst, void *src);
#define smart __attribute__((cleanup(decref)))

// assignment operator



#define move(src, dst)                                                         \
    do {                                                                       \
        dst = src;                                                             \
        src = NULL;                                                            \
    } while (0)


#define set(dst, src) s_assign((void **)&(dst), (src))
