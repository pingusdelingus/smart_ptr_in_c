#include "smart_ptr.h"
#include <assert.h>
#include <stdarg.h>
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

// called everytime variable leaves scope
inline void decref(smart_ptr **p)
{
    (*p)->refcount--;

    if ((*p)->refcount == 0) {
        sdestructor(*p);
    }
}

// end of decref

inline static smart_ptr *get_s(smart_ptr *p) { return p->ptr; } // end of gets

__attribute__((malloc)) void *smalloc(u32 size, void (*dtor)(smart_ptr *))
{
    smart_ptr *p = malloc(sizeof(smart_ptr));

    *p = (smart_ptr){
        .dtor = dtor,
        .refcount = 1,
        .ptr = malloc(size) // add refcount padding
    };

    return p;
} // end of smalloc

inline void sdestructor(smart_ptr *p)
{
    free(p->ptr);
    free((smart_ptr *)p);
}

void sfree(smart_ptr *ptr)
{
    if (ptr == NULL) {
        return;
    }
    smart_ptr *meta = get_s(ptr);
    assert(ptr == meta->ptr); // ptr will be a pointer returned by smalloc

} // end of sfree

inline void incref(smart_ptr *ptr) { ptr->refcount++; }

inline smart_ptr *move(smart_ptr *ptr)
{
    if (ptr) {
        incref(ptr);
    }
    return ptr;
} // smart_ptr*

typedef struct {
    f32 x;
    f32 y;
} vec2;

int main(void)
{
    smart smart_ptr *my_s_ptr = smalloc(20, &sdestructor);
    return 0;
}
