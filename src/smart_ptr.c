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
    //  printf("called decref \n");
    void *user_ptr = *(void **)p;

    if (!user_ptr) {
        return;
    }
    smart_ptr *start = get_s(user_ptr);
    //   printf("ref count is %zu \n", start->refcount);
    if ((--(start->refcount)) == 0) {
        //        printf("refcount is 0 \n");
        if (NULL != start->dtor) {
            //            printf("calling sdestructor\n");
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

__attribute__((malloc)) void *smalloc(u32 size, void (*dtor)(void *),
                                      b8 iszeroed)
{
    smart_ptr *p;
    // Allocate the header + the user's requested size
    if (iszeroed) {
        p = calloc(1, (sizeof(smart_ptr) + size));
    } else {

        p = malloc(sizeof(smart_ptr) + size);
    }
    if (!p) {
        return NULL;
    }
    p->num_bytes = size;
    p->dtor = dtor;
    p->refcount = 1;

    return (void *)p->data;
}

inline void sdes(void *p) { free(p); }

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

int dot(vec2 *a, vec2 *b) { return (a->x * b->x) + (a->y * b->y); }

void printvec(vec2 *that) { printf("x:%f, y:%f \n", that->x, that->y); }

int main(void)
{
    // initialize a smart_ptr with zeroe'd mem
    smart(vec_ptr, vec2, &sdes, ZEROED_MEM);
    vec_ptr->x = 3;
    vec_ptr->y = 9;


    smart(v2, vec2, &sdes, ZEROED_MEM); // initialize a smart_ptr with
    v2->x = 4;
    v2->y = 1;


    printf("v1: \n");
    printvec(vec_ptr);
    printf("v2: \n");
    printvec(v2);

    printf("dot product is : %d\n", dot(vec_ptr, v2));


    for (int i = 0; i < 100000; i++) {
        smart(v8, vec2, &sdes, ZEROED_MEM);
    }


    return 0;
}
