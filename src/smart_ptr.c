#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <sys/mman.h>
#include "smart_ptr.h"
#include <stdarg.h>


smart_ptr* CreateSmartPtr(u32 s)
{
  
    smart_ptr* p = (smart_ptr * ) mmap(NULL, sizeof(smart_ptr), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS ,-1,0 ); 
    if (p == MAP_FAILED){
    perror("failed memory mapping");
    return NULL;
  }

  p->data = (void *) mmap(NULL, s, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS ,-1,0 ); 
  
  if (p->data == MAP_FAILED){
    perror("failed memory mapping");
    munmap(p);
    return NULL;
  }

  p->size = s; 
  p->refcount = 1;

  return p;
}// end of CreateSmartPtr


inline void sdestructor(smart_ptr* p)
{
  munmap(p->data, data->size);
  munmap(p, sizeof(p));
}


// called everytime variable leaves scope
inline void decref(smart_ptr* ptr ) 
{
  ptr->refcount--;
  
  if (refcount == 0 ) { 
      (ptr);
  }
}// end of decref


inline static smart_ptr *get_s(void *ptr) 
{
    return ptr - sizeof (smart_ptr) + 1;
}// end of gets


__attribute__((malloc))
void *smalloc(size_t size, void (*dtor)(void *)) 
{
    smart_ptr *meta = malloc(sizeof ( smart_ptr) );
    
    *meta = (smart_ptr) {
        .dtor = dtor,
        .refcount = 1,
        .ptr  = malloc(size) // add refcount padding
    };
    return meta->ptr;
}// end of smalloc

inline void* sdestructor(void * p)
{
  free(p->ptr);
  free(p);
}


void sfree(void *ptr) 
{
    if (ptr == NULL)
        return;
    smart_ptr *meta = get_s(ptr);
    assert(ptr == meta->ptr); // ptr will be a pointer returned by smalloc
    
}// end of sfree


inline void incref(smart_ptr* ptr ) 
{
  ptr->refcount++;
}


inline smart_ptr* move(smart_ptr* ptr,..)
{
  if (ptr) incref(ptr);
  return ptr;
}// smart_ptr* 


void my_variadic_function(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(format, args); 
    va_end(args);
}


int main(void)

{
  
  smart_ptr* ptr = CreateSmartPtr(sizeof(vec2));

  

  return 0;
}
