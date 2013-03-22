// std
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <stdint.h>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>

// libops
#include "ops-fp.h"
#include "ops-log.h"
#include "ops-fp-writer.h"
#include "ops-files.h"
#include "ops-tools.h"

void patchFPT(ops::files::ConfigManager &config, std::string &inputFile, std::string &outputFile)
{
    // Can be configurable
/*
    uint32_t cfg_id = config.intValue("fpt", "show_id");
    uint32_t cfg_type = config.intValue("fpt", "show_type");
    uint32_t cfg_rawLen = config.intValue("fpt", "rawdata_len");
    bool showId = (cfg_id == 1);
    bool showType = (cfg_type == 1);
*/
	ops::fp::FPTWriter * handler = new ops::fp::FPTWriter();
	ops::fp::ChunkChunkList * chunks = (ops::fp::ChunkChunkList *)handler->flexLoad(inputFile);

    // Change chunks values :)

	// rebuild table mac (win32 only)
	#ifdef _WIN32
	ops::RawData data(16, true);
	handler->calcMAC(chunks, data.data, data.len);

	std::ostringstream out;
	ops::tools::dumpHex(out, 1, data, 16);
	std::cout << "COMPUTED MAC : " << std::endl;
	std::cout << out.str() << std::endl;
	#endif

    handler->flexSave(outputFile, chunks);

	delete chunks;
    delete handler;
    INFO << "done";
}

int main (int argc, char *argv[])
{
	// check args
	if (argc != 2)
	{
		std::cout << "usage : " << argv [0] << " inputfile" << std::endl;
		std::cout << "\tSample" << std::endl;
		std::cout << "\t\tinputfile\t.fp file" << std::endl;
		return 1;
	}
    std::string inputfile = argv[1];
    std::replace(inputfile.begin(), inputfile.end(), '\\', '/');


    std::string outputFile = inputfile + ".patch";



	// libops init
	ops::Log::level() = ops::logERROR;
	if (!ops::init()) {
		return 1;
	}
	if (!ops::msole::init()) {
		return 1;
	}


    // read config file
    ops::files::ConfigManager config;
    ops::files::Path configPath(argv[0]);
    std::string configFile = configPath.basenameWOExt() + ".cfg";
    config.addStdFile(configFile.c_str());

	// do the dump, given extension file
    std::string fileExt = "";
    if (inputfile.find_last_of(".") != std::string::npos)
    {
        fileExt = inputfile.substr(inputfile.find_last_of(".")+1);
    }

    if (fileExt == "fpt")
    {
        patchFPT(config, inputfile, outputFile);
    }
    else
    {
        ERROR << "not a Future Pinball file";
    }



	// libops shutdown
	ops::msole::shutdown();
	ops::shutdown();

	return 0;
}
