// std
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <map>
#include <string>
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



class FileWriterI {

private:
	GsfOutfile * baseOutfile;
	std::map<std::string, GsfOutput*> openDirList;
        std::vector<GsfOutput*> openFileHandler;

public:
	uint32_t open(std::string filepath);
        int32_t fileOpen(std::string inpath);
        uint32_t fileWrite(int32_t handler, uint8_t * data, uint32_t len);
        uint32_t fileClose(int32_t handler);
	uint32_t mkdir(std::string inpath);
	uint32_t close();
};





uint32_t FileWriterI::open(string filepath) {
	GError * err = NULL;

	// open file
	GsfOutput * output = gsf_output_stdio_new (filepath.c_str(), &err);
	if (output == NULL) {
		ERROR << "ops::msole::FileWriterI::open() : can't open " + filepath + " : " + err->message;
		g_error_free (err);
		return RET_ERR;
	}

	baseOutfile = gsf_outfile_msole_new (output);
	g_object_unref (G_OBJECT (output));
	if (baseOutfile == NULL) {
		ERROR << "ops::msole::FileWriterI::open() : can't create OLE file :" + filepath + " : " + err->message;
		g_error_free (err);
		return RET_ERR;
	}

	// create map for keeping open directories
	openDirList["/"] = GSF_OUTPUT(baseOutfile);

	DEBUG << "ops::msole::FileWriterI::open() : " + filepath + " opened";

	return RET_OK;
}


uint32_t FileWriterI::close() {
    if (baseOutfile == NULL) {
		WARNING << "ops::msole::FileWriterI::close() : file already closed";
		return RET_ERR;
    }

    // close all opened files.
    vector <GsfOutput*> :: iterator iter;
    for ( uint32_t i = 0 ; i < openFileHandler.size() ; i++) {
        GsfOutput * output = openFileHandler[i];
        if (output != NULL) {
            gsf_output_close (GSF_OUTPUT (output));
            g_object_unref (G_OBJECT (output));
        }
    }
    openFileHandler.clear();

    // close all opened directories.
    map <std::string, GsfOutput*> :: reverse_iterator rIter;
    for ( rIter = openDirList.rbegin( ) ; rIter != openDirList.rend( ) ; rIter++) {
        GsfOutput * output = openDirList[rIter->first];
        gsf_output_close (GSF_OUTPUT (output));
        g_object_unref (G_OBJECT (output));
    }
    openDirList.clear();

    baseOutfile = NULL;

	DEBUG << "ops::msole::FileWriterI::close() : file closed";

    return RET_OK;
}


int32_t FileWriterI::fileOpen(std::string inpath) {

    int32_t handler = -1;

    // check if writer is ready
    if (baseOutfile == NULL) {
	   ERROR << "ops::msole::FileWriterI::fileOpen() : file not opened";
       return RET_ERR;
    }

    // split inpath
    vector<string> pathElements = ops::tools::split(inpath, '/');

    // go into subdirs
    size_t currentPathIndex = 0;
    string partialInpath = "/";
    GsfOutput * output = GSF_OUTPUT(baseOutfile);
    for (; currentPathIndex < pathElements.size() ; currentPathIndex++) {
            if (currentPathIndex < pathElements.size() - 1) {
                    partialInpath += pathElements[currentPathIndex] + "/";
                    if (openDirList.find(partialInpath) != openDirList.end()) {
                            output = openDirList[partialInpath];
                    } else {
                       // create new child ans store it
                            output = gsf_outfile_new_child(GSF_OUTFILE(output), pathElements[currentPathIndex].c_str(), true);
                            openDirList[partialInpath] = output;
                    }
            } else {
                    // last path, so it's a plain file

                    output = gsf_outfile_new_child(GSF_OUTFILE(output), pathElements[currentPathIndex].c_str(), false);
                    // store file and return handler
                    handler = openFileHandler.size();
                    openFileHandler.push_back(output);
            }
    }

	DEBUG << "ops::msole::FileWriterI::fileOpen() : " + inpath + " opened";

    return handler;
}

uint32_t FileWriterI::fileWrite(int32_t handler, uint8_t * data, uint32_t len) {
    if (handler >=0 && handler < (int32_t)openFileHandler.size()) {
        GsfOutput * output = openFileHandler[handler];

        if (output->is_closed) {
			ERROR << "ops::msole::FileWriterI::fileWrite() : file already closed";
            return ops::RET_ERR;
        }

        if ( data != NULL && len > 0 ) {
                if (!gsf_output_write (output, len, (const guint8*)data)) {
						ERROR << "ops::msole::FileReader::fileWrite() : can't write in file ";
                        return RET_ERR;
                }
        }

		DEBUG << "ops::msole::FileWriterI::fileWrite() : data writed";

        return ops::RET_OK;
    } else {
		ERROR << "ops::msole::FileWriterI::fileWrite() : unknown handler";
        return ops::RET_ERR;
    }

}

uint32_t FileWriterI::fileClose(int32_t handler) {
    if (handler >=0 && handler < (int32_t)openFileHandler.size()) {
        GsfOutput * output = openFileHandler[handler];

        gsf_output_close (output);
        g_object_unref (G_OBJECT (output));

        openFileHandler[handler] = NULL;

		DEBUG << "ops::msole::FileWriterI::fileClose() : file closed";

        return ops::RET_OK;
    } else {
		ERROR << "ops::msole::FileWriterI::fileClose() : unknown handler";
        return ops::RET_ERR;
    }

}




uint32_t FileWriterI::mkdir(string inpath) {
    // check if writer is ready
    if (baseOutfile == NULL) {
			ERROR << "ops::msole::FileWriterI::writeAllImpl() : file not opened";
       return RET_ERR;
    }

    // split inpath
    vector<string> pathElements = ops::tools::split(inpath, '/');

    // go into subdirs
    size_t currentPathIndex = 0;
    string partialInpath = "/";
    GsfOutput * output = GSF_OUTPUT(baseOutfile);
    for (; currentPathIndex < pathElements.size() ; currentPathIndex++) {
        partialInpath += pathElements[currentPathIndex] + "/";
        if (openDirList.find(partialInpath) != openDirList.end()) {
                output = openDirList[partialInpath];
        } else {
           // create new child ans store it
                output = gsf_outfile_new_child(GSF_OUTFILE(output), pathElements[currentPathIndex].c_str(), true);
                openDirList[partialInpath] = output;
        }
    }

	DEBUG << "ops::msole::FileWriterI::writeAllImpl() : " + inpath + " created";

    return RET_OK;
}


FileWriter::FileWriter() {
   _impl = new FileWriterI();
}


FileWriter::~FileWriter() {
	delete _impl;
}

uint32_t FileWriter::open(string filepath) {
	return _impl->open(filepath);
}

uint32_t FileWriter::close() {
	return _impl->close();
}


uint32_t FileWriter::writeAll(string inpath, ops::RawData * data) {
    int32_t handler = _impl->fileOpen(inpath);
    if (handler == -1) {
        return ops::RET_ERR;
    } else {
        if (data != NULL) {
            _impl->fileWrite(handler, data->data, data->len);
        }
        return _impl->fileClose(handler);
    }
}

uint32_t FileWriter::writeAll(string inpath, uint8_t * data, uint32_t len) {
    int32_t handler = _impl->fileOpen(inpath);
    if (handler == -1) {
        return ops::RET_ERR;
    } else {
        _impl->fileWrite(handler, data, len);
        return _impl->fileClose(handler);
    }
}
int32_t FileWriter::fileOpen(std::string inpath) {
    return _impl->fileOpen(inpath);
}

uint32_t FileWriter::fileWrite(int32_t handler, ops::RawData * data) {
    return _impl->fileWrite(handler, data->data, data->len);
}

uint32_t FileWriter::fileWrite(int32_t handler, uint8_t * data, uint32_t len) {
    return _impl->fileWrite(handler, data, len);
}

uint32_t FileWriter::fileClose(int32_t handler) {
    return _impl->fileClose(handler);
}

uint32_t FileWriter::mkdir(string inpath) {
	return _impl->mkdir(inpath);
}

} // namespace msole
} // namespace ops
