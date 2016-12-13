#ifndef __MEMORY_H__
#define __MEMORY_H__

extern void heapInit(unsigned int start, unsigned int end);

extern void* Drv_alloc(unsigned int nbytes);
extern void* Drv_memalign(unsigned int align, unsigned int nbytes);
extern void Drv_deAlloc(void* address);
extern void* Drv_realloc(void* address, unsigned int nbytes);
extern void* Drv_calloc(unsigned int nmem, unsigned int size);

#define ENABLE_MALLOC

#ifdef ENABLE_MALLOC
#define malloc		Drv_alloc
#define memalign	Drv_memalign
#define calloc		Drv_calloc
#define realloc		Drv_realloc
#define free		Drv_deAlloc
#endif

#endif //__MEMORY_H__
