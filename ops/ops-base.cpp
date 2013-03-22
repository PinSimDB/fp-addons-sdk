// std
#include <stdio.h>
#include <stdint.h>
#include <cstring>
#include <iostream>
#include <sstream>
using namespace std;

// libops
#include "ops-base.h"
#include "ops-log.h"

#include "minilzo.h"
#define HEAP_ALLOC(var,size) lzo_align_t __LZO_MMODEL var [ ((size) + (sizeof(lzo_align_t) - 1)) / sizeof(lzo_align_t) ]

static HEAP_ALLOC(lzo_wrkmem,LZO1X_1_MEM_COMPRESS);


namespace ops
{

	bool init() {
		// init lzo
		if (lzo_init() != LZO_E_OK) {
			ERROR << "internal error - lzo_init() failed !!!\n";
			ERROR << "(this usually indicates a compiler bug - try recompiling\nwithout optimizations, and enable `-DLZO_DEBUG' for diagnostics)\n";
			return false;
		}
		DEBUG << "lzo initialized.";

		DEBUG << "libops initialized.";
		return true;
	}

	void shutdown() {
		DEBUG << "libops shutdown.";
	}


	RawData::RawData() : len(0), data(NULL) {
	}

	RawData::RawData(uint32_t initLen, bool zeroMemory) : len(initLen) {
		if (len > 0) {
			data = new uint8_t[len];

			if (zeroMemory) {
				memset(data, 0, len);
			}
		} else {
			data = NULL;
		}
	}

	RawData::RawData(uint32_t srcLen, const uint8_t * srcData) : len(srcLen) {
		if (len > 0) {
			data = new uint8_t[len];
			memcpy(data, srcData, len);
		} else {
			data = NULL;
		}
	}

	RawData::RawData(const RawData & rd) {
		len = rd.len;
		if (len > 0) {
			data = new uint8_t[len];
			memcpy(data, rd.data, len);
		} else {
			data = NULL;
		}
	}

	RawData & RawData::operator=(const RawData & rd) {
		if (this != &rd) {  // Check for self-assignment
			if (data != NULL) {
				delete [] data;
				data = NULL;
			}
			len=rd.len;
			if (len > 0) {
				data = new uint8_t[len];
				memcpy(data, rd.data, len);
			} else {
				data = NULL;
			}
		}
		return *this;
	}

	RawData::~RawData() {
		if (data != NULL) {
			delete [] data;
		}
	}

 bool RawData::ispacked()
 {
   if( len < 4 )
     return false;
   if (data[0] == 'z' && data[1] == 'L' && data[2] == 'Z' && data[3] == 'O')
     return true;
   else
     return false;
 }


    RawData * RawData::packLZO()
    {
        if (ispacked()) {
            // already packed
			return new ops::RawData(len, data);
        }
        else
        {
            ops::RawData * result = new ops::RawData();
            // TODO : free not used memory ?
            result->data = new uint8_t[8 + len * 2];
            result->len = 0;
            int r = lzo1x_1_compress(data, len, &result->data[8], (lzo_uint*)&result->len, lzo_wrkmem);
            result->data[0]='z';
            result->data[1]='L';
            result->data[2]='Z';
            result->data[3]='O';
            *((uint32_t*)(&result->data[4])) = len;
            result->len += 8;
            if (r == LZO_E_OK)
            {
                return result;
            } else {
                return NULL;
            }
        }
    }

	RawData * RawData::unpackLZO()
	{
        if (ispacked()) {
            // Decompress if 'zLZO'
			ops::RawData * value = new ops::RawData();
            uint32_t rawLen;
			value->len =  *((uint32_t *) &data[4]);
			value->data = new uint8_t[value->len];
            // TODO cast, int
			int r = lzo1x_decompress( (const unsigned char *) &data[8], len - 8, (unsigned char *)value->data, (lzo_uint*)&rawLen, NULL);
			if (r != LZO_E_OK || rawLen != value->len) {
                // this should NEVER happen
				ERROR << " internal error - decompression failed : " << r;
				delete [] value->data;
				value->len = 0;
				value->data = NULL;
			}
			return value;
        }
        else
        {
            // Plain copy
			return new ops::RawData(len, data);
        }
	}


} // namespace ops
