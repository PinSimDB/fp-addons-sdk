#ifndef __OPS_MSOLE_RW_H_INCLUDED
#define __OPS_MSOLE_RW_H_INCLUDED

// std
#include <stdint.h>
#include <string>
#include <vector>
#include <map>

// libops
#include "ops-base.h"

namespace ops
{
namespace msole
{

	// init functions
	bool init();
	void shutdown();


class FileReaderI;
class FileReader {

public:
	FileReader();
	~FileReader();

	uint32_t open(std::string filepath);
	std::vector<std::string> * list();
	ops::RawData * readAll(std::string inpath);
	uint32_t close();

private:
	FileReaderI * _impl;

};


class FileWriterI;
class FileWriter {

public:
	FileWriter();
	~FileWriter();

	uint32_t open(std::string filepath);
	uint32_t writeAll(std::string inpath, ops::RawData * data);
	uint32_t writeAll(std::string inpath, uint8_t * data, uint32_t len);
	uint32_t mkdir(std::string inpath);
	int32_t fileOpen(std::string inpath);
	uint32_t fileWrite(int32_t handler, ops::RawData * data);
	uint32_t fileWrite(int32_t handler, uint8_t * data, uint32_t len);
	uint32_t fileClose(int32_t handler);

	uint32_t close();

private:
	FileWriterI * _impl;
};


} // namespace msole
} // namespace ops

#endif // __OPS_MSOLE_RW_H_INCLUDED

