#ifndef __DS2_MALLOC_H__
#define __DS2_MALLOC_H__

#ifdef __cplusplus
extern "C" {
#endif

extern void heapInit(unsigned int start, unsigned int end);

extern void* Drv_alloc(unsigned int nbytes);
extern void* Drv_memalign(unsigned int align, unsigned int nbytes);
extern void Drv_deAlloc(void* address);
extern void* Drv_realloc(void* address, unsigned int nbytes);
extern void* Drv_calloc(unsigned int nmem, unsigned int size);

//compatibility for some libs free(NULL) this is a dirty & lazy fix for more codes
//note: ISO C free(NULL) is unsafe, only C++ delete NULL is safe
//maybe it's not good to spoil programmers?
static inline void Drv_safeDealloc(void* address)
{
	if(address)
		Drv_deAlloc(address);
}

#ifdef __cplusplus
}
#endif

#define malloc		Drv_alloc
#define memalign	Drv_memalign
#define calloc		Drv_calloc
#define realloc		Drv_realloc
#define free		Drv_safeDealloc

#ifdef __cplusplus
#include <stdio.h>
inline void* operator new ( size_t s ) { return malloc( s ); }
inline void* operator new[] ( size_t s ) { return malloc( s ); }
inline void operator delete ( void* p ) { free( p ); }
inline void operator delete[] ( void* p ) { free( p ); }
#endif

#endif //__DS2_MALLOC_H__
