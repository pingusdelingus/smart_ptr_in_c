#include "types.h"
#include <stdalign.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#ifndef SMART_PTR_HEADER

#define SMART_PTR_HEADER
#define ZEROED_MEM 1
#define UNZEROD_MEM 0

typedef struct {
    void (*dtor)(void *);
    size_t num_bytes;
    size_t refcount;
    alignas(max_align_t) char data[];
} smart_ptr;

static smart_ptr *get_s(void *ptr);
// constructor
void *smalloc(u32 size, void (*f)(void *), b8 iszeroed);

// destructor
void sfree(smart_ptr *ptr);

void sdes(void *p);
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



#define smart(name, atype, dstrtr, iszeroed)                                   \
    __attribute__((cleanup(decref))) atype *name =                             \
        smalloc(sizeof(atype), (dstrtr), (iszeroed))



#define move(src, dst)                                                         \
    do {                                                                       \
        dst = src;                                                             \
        src = NULL;                                                            \
    } while (0)


// assignment operator
#define set(dst, src) s_assign((void **)&(dst), (src))

#endif // end of SMART_PTR_HEADER
