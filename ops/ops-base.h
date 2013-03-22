#ifndef __OPS_BASE_H_INCLUDED
#define __OPS_BASE_H_INCLUDED

// std
#include <stdint.h>
#include <string>
#include <sstream>


namespace ops
{
	const uint32_t RET_OK = 0;
	const uint32_t RET_ERR = 1;

	// init functions
	bool init();
	void shutdown();

	// raw data
	class RawData {
	public :
		uint32_t len;
		uint8_t * data;
		RawData();
		RawData(uint32_t initLen, bool zeroMemory);
		RawData(uint32_t srcLen, const uint8_t * srcData);
		RawData(const RawData & rd);
		RawData & operator=(const RawData & rd);
		~RawData();

   bool ispacked();
		RawData * packLZO();
		RawData * unpackLZO();
	};



	class BaseFile {
	public:
		std::string path;
	};


} // namespace ops


#endif // __OPS_BASE_H_INCLUDED

