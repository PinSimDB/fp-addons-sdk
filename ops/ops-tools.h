#ifndef __OPS_TOOLS_H_INCLUDED
#define __OPS_TOOLS_H_INCLUDED

// std
#include <stdio.h>
#include <stdint.h>
#include <vector>
#include <string>

// ops
#include "ops-base.h"

namespace ops
{
namespace tools
{

void tab(FILE * handler, uint32_t level);

void dumpHex(FILE * handler, uint32_t level, const char * data, uint32_t dataLen, uint32_t maxLen);

void dumpHex(std::ostringstream & out, uint32_t tabs, const ops::RawData & data, uint32_t maxLen);

std::vector<std::string> split(const std::string &s, char delim);


/*
class MemFile {

public:
		MemFile();
		~MemFile();

        FILE * fopen(uint8_t * ptr, size_t size);

        uint32_t fseek(FILE * stream, uint32_t offset, uint32_t origin);
        uint32_t ftell (FILE * stream);

        uint32_t fread(void * ptr, size_t size, size_t count, FILE * stream);

        uint32_t fclose(FILE * stream);

private:
        FILE * stream;
        uint8_t * startPtr;
        uint8_t * currentPtr;
        size_t size;

};
*/

} // namespace tools
} // namespace ops


#endif // __OPS_TOOLS_H_INCLUDED

