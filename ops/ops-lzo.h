#ifndef __OPS_LZO_H_INCLUDED
#define __OPS_LZO_H_INCLUDED

// lzo
#include "minilzo.h"
#define HEAP_ALLOC(var,size) lzo_align_t __LZO_MMODEL var [ ((size) + (sizeof(lzo_align_t) - 1)) / sizeof(lzo_align_t) ]

static HEAP_ALLOC(wrkmem,LZO1X_1_MEM_COMPRESS);

#endif // __OPS_LZO_H_INCLUDED

