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

// gsf
#include <gsf/gsf-utils.h>

#include <gsf/gsf-input-stdio.h>
#include <gsf/gsf-infile.h>
#include <gsf/gsf-infile-stdio.h>
#include <gsf/gsf-infile-msole.h>

#include <gsf/gsf-output-stdio.h>
#include <gsf/gsf-outfile.h>
#include <gsf/gsf-outfile-stdio.h>
#include <gsf/gsf-outfile-msole.h>


namespace ops
{
namespace msole
{

// TODO partial file read as FileWriter

class FileReaderI {
private:
	GsfInfile * baseInfile;
	std::map<std::string, GsfInput*> openDirList;
	void listVisitor(GsfInput * input, std::vector<std::string> * result, std::string partialInfile, std::vector<GsfInput *> *visited );

public:
	uint32_t open(std::string filepath);
	std::vector<std::string> * list();
		ops::RawData * readAll(std::string inpath);
	uint32_t close();

};



uint32_t FileReaderI::open(string filepath) {
	GError * err = NULL;

	// open file
	GsfInput * input = gsf_input_stdio_new (filepath.c_str(), &err);

	if (input == NULL) {
		ERROR << "ops::msole::FileReader::open() : can't open " + filepath + " : " + err->message;
		g_error_free (err);
		return RET_ERR;
	}
	baseInfile = gsf_infile_msole_new (input, &err);
	g_object_unref (G_OBJECT (input));
	if (baseInfile == NULL) {
		ERROR << "ops::msole::FileReader::open() : " + filepath + " is not a OLE file : " + err->message;
		g_error_free (err);
		return RET_ERR;
	}

	// create map for keeping open directories
	openDirList["/"] = GSF_INPUT(baseInfile);

	DEBUG << "ops::msole::FileReader::open() : " + filepath + " opened";

	return RET_OK;
}


uint32_t FileReaderI::close() {
	if (baseInfile == NULL) {
		WARNING << "ops::msole::FileReader::close() : file already closed";
		return RET_ERR;
	}

	// close base file
	g_object_unref (G_OBJECT (baseInfile));
	baseInfile = NULL;

	// clear directories map
	openDirList.clear();

	DEBUG << "ops::msole::FileReader::close() : file closed";

	return RET_OK;
}




void FileReaderI::listVisitor(GsfInput * input, vector<string> * result, string partialInfile, vector<GsfInput *> * visited)
{
	// TODO : closed file !!!
	if (GSF_IS_INFILE (input) && gsf_infile_num_children (GSF_INFILE (input)) > 0)
                    {
		for (int32_t i = 0 ; i < gsf_infile_num_children (GSF_INFILE (input)) ; i++)
                                        {
			GsfInput * child = gsf_infile_child_by_index (GSF_INFILE (input), i);
			string infileName = gsf_input_name(child);
			result->push_back(partialInfile + infileName);
                                                            visited->push_back( child );
			listVisitor(child, result, partialInfile + infileName + "/", visited);
		}
	}
}

vector<string> * FileReaderI::list() {

	if (baseInfile == NULL) {
		ERROR << "ops::msole::FileReader::list() : file not opened";
	   return NULL;
	}

	GsfInput * input = GSF_INPUT(baseInfile);
	vector<string> * result = new vector<string>();
	vector<GsfInput *> * visited = new vector<GsfInput*>();
	listVisitor(input, result, "", visited);

                    std::vector <GsfInput*>::iterator it;
                    for( it = visited->begin(); it != visited->end(); ++it )
                    {
                      g_object_unref (G_OBJECT (*it));
                    }
                    delete visited;


	return result;
}


ops::RawData * FileReaderI::readAll(string inpath) {

	if (baseInfile == NULL) {
		ERROR << "ops::msole::FileReader::readAll() : file not opened";
		return NULL;
	}

	// TODO : keep open directories like FileWriter does for faster multiple access !!

	// split inpath
	vector<string> pathElements = ops::tools::split(inpath, '/');

	// go into subdirs
	size_t currentPathIndex = 0;
	GsfInput * input = GSF_INPUT(baseInfile);
	for (; input != NULL && currentPathIndex < pathElements.size(); currentPathIndex++) {
		GsfInput * lastInput = input;
		input = gsf_infile_child_by_name (GSF_INFILE(lastInput), pathElements[currentPathIndex].c_str());
		if (lastInput != GSF_INPUT(baseInfile)) {
			g_object_unref (G_OBJECT (lastInput));
		}
	}
	if (input == NULL) {
		ERROR << "ops::msole::FileReader::readAll() : can't find " + inpath;
		return NULL;
	}

	// load data
	DEBUG << "ops::msole::FileReader::readAll() : " + inpath + " loaded";
	ops::RawData * result = new ops::RawData();
	result->len = (uint32_t) gsf_input_size (input);
	if (result->len  > 0) {
	   // TODO check remining length if not loadable in one time
		result->data = new uint8_t[result->len];
		// TODO test read result
		memcpy(result->data, gsf_input_read (input, result->len, NULL), result->len);
	}
	if (input != GSF_INPUT(baseInfile)) {
		g_object_unref (G_OBJECT (input));
	}

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
