#ifndef _NEW_H_
#define _NEW_H_

#include <mm/malloc.h>

namespace std
{

  inline void bad_alloc(const char *what = NULL)
  {
    if (what == NULL) {
      //panic::panic("Bad alloc.");
    }
    //panic::panic(what);
  }

}

inline void * operator new(size_t n)
{
  void * p = malloc(n);
  //printf("[new] %d bytes, pointer = 0x%x\n", n, (int) p);
  if (p == NULL) {
    std::bad_alloc();
  }
  return p;
}

inline void * operator new [](size_t n)
{
  void * p = malloc(n);
  //printf("[new] %d bytes, pointer = 0x%x\n", n, (int) p);
  if (p == NULL) {
    std::bad_alloc();
  }
  return p;
}

inline void operator delete(void * p)
{
  //printf("[delete] pointer = 0x%x\n", (int) p);
  return free(p);
}

inline void operator delete [](void * p)
{
  //printf("[delete] pointer = 0x%x\n", (int) p);
  return free(p);
}

/* placement new. */
inline void * operator new(size_t, void * p)
{ 
  return p; 
}

inline void * operator new [](size_t, void * p)
{
  return p; 
}

inline void operator delete(void *, void *) { }

inline void operator delete [](void *, void *) { }

#endif