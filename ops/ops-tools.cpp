// std
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <stdint.h>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

// ops
#include "ops-tools.h"



namespace ops
{
namespace tools
{


	std::vector<std::string> &split(const string &s, char delim, std::vector<std::string> &elems) {
		std::stringstream ss(s);
		std::string item;
		while(getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}

	std::vector<std::string> split(const string &s, char delim) {
		std::vector<std::string> elems;
		return split(s, delim, elems);
	}


void tab(FILE * handler, uint32_t level) {
	uint32_t i;
	for (i = 0 ; i < level ; i++) {
		fprintf (handler, "   ");
	}
}

void dumpHex(FILE * handler, uint32_t level, const char * data, uint32_t dataLen, uint32_t maxLen) {
	uint32_t i = 0;
	uint32_t j = 0;
	uint32_t len = dataLen;

	if (maxLen > 0 && len > maxLen) {
		len = maxLen;
	}

	while (i < len) {
		tab(handler, level - 1);
		j = 0;
		while (j<16) {
			if (j%4 == 0) {
				tab(handler, 1);
			}
			if (i+j < len) {
				uint8_t hex = (uint8_t)data[i+j];
				if (hex<16) {
					fprintf (handler, "0");
				}
				fprintf (handler, "%X ", hex);
			} else {
				fprintf (handler, "   ");
			}
			j++;
		}
		j = 0;
		while (j<16) {
			if (j%4 == 0) {
				fprintf (handler, " ");
			}
			if (i+j < len) {
				uint8_t hex = (uint8_t)data[i+j];
				if (hex<32 || hex>127) {
					hex = '.';
				}
				fprintf (handler, "%c", hex);
			} else {
				fprintf (handler, " ");
			}
			j++;
		}
		fprintf (handler, "\n");
		i+=16;
	}
	if (len != dataLen) {
		tab(handler, level);
		fprintf(handler, "...\n");
	}

}

void dumpHex(std::ostringstream & out, uint32_t tabs, const ops::RawData & data, uint32_t maxLen) {
	uint32_t i = 0;
	uint32_t j = 0;
	uint32_t len = data.len;

	if (maxLen > 0 && len > maxLen) {
		len = maxLen;
	}
	while (i < len) {
		for (j=0; j<tabs-1; j++) {
			out << "\t";
		}
		j = 0;
		while (j<16) {
			if (j%4 == 0) {
				out << "\t";
			}
			if (i+j < len) {
				uint32_t value = (uint8_t)data.data[i+j];
				out << std::hex << std::setw(2) << std::setfill('0') << value << " ";
			} else {
				out << "   ";
			}
			j++;
		}
		j = 0;
		while (j<16) {
			if (j%4 == 0) {
				out << " ";
			}
			if (i+j < len) {
				uint8_t value = (uint8_t)data.data[i+j];
				if (value<32 || value>127) {
					out << ".";
				} else {
					out << char(value);
				}
			} else {
				out << " ";
			}
			j++;
		}
		out << std::endl;
		i+=16;
	}
	if (len != data.len) {
		for (j=0; j<tabs; j++) {
			out << "\t";
		}
		out << "..." << std::endl;
	}

}


/*
MemFile::MemFile() {
    stream = NULL;
    startPtr = NULL;
    currentPtr = NULL;
}

MemFile::~MemFile() {

}

FILE * MemFile::fopen(uint8_t * ptr, size_t streamSize) {
    startPtr = ptr;
    currentPtr = ptr;
    size = streamSize;
    stream = new FILE();
    return stream;
}

uint32_t MemFile::fseek(FILE * streamHandle, uint32_t offset, uint32_t origin) {
    if (stream != NULL) {
        // TODO EOF verification
        switch (origin) {
        case SEEK_SET :
            currentPtr = startPtr + offset;
            break;
        case SEEK_CUR :
            currentPtr += offset;
            break;
        case SEEK_END :
            currentPtr = startPtr + size + offset;
            break;
        }

        return 0;
    } else {
        // TODO
        return 1;
    }
}

uint32_t MemFile::ftell (FILE * streamHandle) {
    if (stream != NULL) {
        // TODO EOF verification

        return currentPtr - startPtr;
    } else {
        // TODO
        return 0;
    }
}

uint32_t MemFile::fread(void * ptr, size_t sizet, size_t count, FILE * streamHandle) {
    if (stream != NULL) {
        // TODO EOF verification
        memcpy(ptr, currentPtr, sizet * count);

        currentPtr += sizet * count;
        return sizet * count;
    } else {
        // TODO
        return 0;
    }
}

uint32_t MemFile::fclose(FILE * streamHandle) {
    if (stream != NULL) {
        delete stream;
        stream = NULL;
        startPtr = NULL;
        currentPtr = NULL;
        return 0;
    } else {
        // TODO
        return 1;
    }
}
*/


} // namespace tools
} // namespace ops
