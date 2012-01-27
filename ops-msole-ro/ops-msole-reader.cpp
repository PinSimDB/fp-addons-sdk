// std
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
using namespace std;

// ops-msole
#include "ops-msole.h"

// ops

#include "ops-log.h"
#include "ops-tools.h"

// pole
#include "pole.h"

namespace ops
{
namespace msole
{

class FileReaderI {
private:

    POLE::Storage* storage;
    void listVisitor(POLE::Storage* storage, vector<string> * result, string partialInfile);

public:
	uint32_t open(std::string filepath);
	std::vector<std::string> * list();
    ops::RawData * readAll(std::string inpath);
	uint32_t close();

};


uint32_t FileReaderI::open(string filepath) {
	// open file
	storage = new POLE::Storage( filepath.c_str() );
    storage->open();
	if (storage->result() != POLE::Storage::Ok )
    {
        ERROR << "ops::msole::FileReader::open() : can't open " + filepath;
        delete storage;
        storage = NULL;
        return RET_ERR;
    }

	DEBUG << "ops::msole::FileReader::open() : " + filepath + " opened";
	return RET_OK;
}


uint32_t FileReaderI::close() {
	if (storage == NULL) {
		WARNING << "ops::msole::FileReader::close() : file already closed";
		return RET_ERR;
	}

	// close storage
	delete storage;
	storage = NULL;

	DEBUG << "ops::msole::FileReader::close() : file closed";
	return RET_OK;
}




void FileReaderI::listVisitor(POLE::Storage* current, vector<string> * result, string partialInfile) {

    DEBUG << "ops::msole::FileReader::listVisitor() : " << partialInfile;

    std::list<std::string> entries;
    entries = current->entries( "/" + partialInfile );

    std::list<std::string>::iterator it;
    for( it = entries.begin(); it != entries.end(); ++it )
    {
        std::string name = *it;
        std::string fullname = partialInfile + name;
        result->push_back(fullname);
        if( current->isDirectory(fullname) )
        {
            listVisitor(current, result, fullname + "/" );
        }
    }
}


vector<string> * FileReaderI::list() {
	if (storage == NULL) {
		ERROR << "ops::msole::FileReader::list() : file not opened";
        return NULL;
	}

	vector<string> * result = new vector<string>();
	listVisitor(storage, result, "");

	return result;
}


ops::RawData * FileReaderI::readAll(string inpath) {
	if (storage == NULL) {
		ERROR << "ops::msole::FileReader::readAll() : file not opened";
        return NULL;
	}

    POLE::Stream* stream = new POLE::Stream( storage, "/" + inpath );
    if (!stream)
    {
		ERROR << "ops::msole::FileReader::readAll() : can't find " + inpath;
        return NULL;
    }
    if (stream->fail())
    {
		ERROR << "ops::msole::FileReader::readAll() : can't find " + inpath;
        return NULL;
    }


    DEBUG << "ops::msole::FileReader::readAll() : stream " << inpath << " opened";

    ops::RawData * result = new ops::RawData();
    result->len = (uint32_t)stream->size();
    DEBUG << "ops::msole::FileReader::readAll() : stream size : " << result->len ;


    if (result->len  > 0) {
	   	result->data = new uint8_t[result->len];
	   	uint32_t total = (uint32_t)stream->read( result->data, result->len );
        if (total != result->len)
        {
            ERROR << "ops::msole::FileReader::readAll() : bytes read (" << total << ") differ from stream size (" << result->len << ")";
            return NULL;
        }
	}

    delete stream;

    return result;
}



FileReader::FileReader() {
   _impl = new FileReaderI();
}


FileReader::~FileReader() {
	delete _impl;
}

uint32_t FileReader::open(string filepath) {
   return _impl->open(filepath);
}

uint32_t FileReader::close() {
   return _impl->close();
}

vector<string> * FileReader::list() {
	return _impl->list();
}

ops::RawData * FileReader::readAll(string inpath) {
        return _impl->readAll(inpath);
}



} // namespace msole
} // namespace ops
