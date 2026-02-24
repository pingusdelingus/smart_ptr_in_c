#include "types.h"
#include <stdio.h>
#include <stdarg.h>


typedef struct {
    void (*dtor)(void *);
    u16 refcount;
    void *ptr;
}smart_ptr ;


static smart_ptr *get_s(void *ptr);
// constructor
void* smalloc(u32 size);

// destructor
void sfree(smart_ptr* ptr);

void sdestructor(smart_ptr* p)
//accessor 



// wtf is this garbage
typedef void (*variadic_func_ptr)(const char*, ...);



// move semantics on cleanup
void decref(smart_ptr* ptr);
void incref(smart_ptr* ptr);
smart_ptr* move(smart_ptr* ptr, variadic_func_ptr);


void variadic_func(const char* format, ...) ;


#define smart __attribute__((cleanup(decref)))


// assignment operator
#define set(dst, src) do { \
    void *temp_src = (src);    \
    if (temp_src) incref(temp_src); \
    if (dst) decref(dst);      \
    (dst) = temp_src;          \
} while (0)

