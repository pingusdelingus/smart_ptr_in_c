#include "smart_ptr.h"
#include "types.h"
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

/*
 * old
smart_ptr* CreateSmartPtr(u32 s)
{

    smart_ptr* p = (smart_ptr * ) mmap(NULL, sizeof(smart_ptr), PROT_READ |
PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS ,-1,0 ); if (p == MAP_FAILED){
    perror("failed memory mapping");
    return NULL;
  }

  p->data = (void *) mmap(NULL, s, PROT_READ | PROT_WRITE, MAP_PRIVATE |
MAP_ANONYMOUS ,-1,0 );

  if (p->data == MAP_FAILED){
    perror("failed memory mapping");
    munmap(p);
    return NULL;
  }

  p->size = s;
  p->refcount = 1;

  return p;
}// end of CreateSmartPtr
*/

/*
inline void sdestructor(smart_ptr* p)
{
  munmap(p->data, data->size);
  munmap(p, sizeof(p));
}
*/


void s_assign(void **dst, void *src)
{
    if (*dst == src) {
        return;
    }
    if (src) {
        incref(get_s(src));
    }

    if (*dst) {
        decref(dst);
    }

    *dst = src;
}

// called everytime variable leaves scope
inline void decref(void *p)
{
    printf("called decref \n");
    void *user_ptr = *(void **)p;

    if (!user_ptr) {
        return;
    }
    smart_ptr *start = get_s(user_ptr);
    printf("ref count is %zu \n", start->refcount);
    if ((--(start->refcount)) == 0) {
        printf("refcount is 0 \n");
        if (NULL != start->dtor) {
            printf("calling sdestructor\n");
            start->dtor(start);
        }
    }
}

// end of decref

inline static smart_ptr *get_s(void *ptr)
{
    if (!ptr) {
        return NULL;
    }

    return (smart_ptr *)((char *)ptr - offsetof(smart_ptr, data));
} // end of gets

__attribute__((malloc)) void *smalloc(u32 size, void (*dtor)(void *))
{

    // Allocate the header + the user's requested size
    smart_ptr *p = malloc(sizeof(smart_ptr) + size);
    if (!p) {
        return NULL;
    }
    p->num_bytes = size;
    p->dtor = dtor;
    p->refcount = 1;

    return (void *)p->data;
}

inline void sdestructor(void *p) { free(p); }

void sfree(smart_ptr *ptr)
{
    if (ptr == NULL) {
        return;
    }
    smart_ptr *meta = get_s(ptr);

} // end of sfree

inline void incref(smart_ptr *ptr) { ptr->refcount++; }

inline void s_cpy(void *this, void *that)
{
    smart_ptr *in = get_s(this);
    smart_ptr *out = get_s(that);

    memcpy(out, in, in->num_bytes);
}

typedef struct {
    f32 x;
    f32 y;
} vec2;

int main(void)
{
    smart smart_ptr *my_s_ptr = smalloc(sizeof(vec2), &sdestructor);
    printf("made smart ptr \n");

    smart smart_ptr *p2 = NULL;

    set(p2, my_s_ptr);

    printf("Pointers linked. Refcount: %zu\n", get_s(p2)->refcount);


    return 0;
}
