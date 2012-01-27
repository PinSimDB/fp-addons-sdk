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
#include "ops-log.h"
#include "ops-fp.h"
#include "ops-files.h"

void dumpFPM(ops::files::ConfigManager &config, std::string &filename, std::ostream &outstream)
{
	INFO << "starting .fpm dump";

    uint32_t cfg_id = config.intValue("fpm", "show_id");
    uint32_t cfg_type = config.intValue("fpm", "show_type");
    uint32_t cfg_rawLen = config.intValue("fpm", "rawdata_len");

    bool showId = (cfg_id == 1);
    bool showType = (cfg_type == 1);

	ops::fp::FPMHandler * handler = new ops::fp::FPMHandler();
	ops::fp::ChunkGeneric * chunks = handler->flexLoad(filename);
	handler->dumpChunks(outstream, chunks, 0, showId, showType, cfg_rawLen);

	delete chunks;
	delete handler;
    INFO << "done";
}

void dumpFPT(ops::files::ConfigManager &config, std::string &filename, std::ostream &outstream)
{
	INFO << "starting fpt dump";

    uint32_t cfg_id = config.intValue("fpt", "show_id");
    uint32_t cfg_type = config.intValue("fpt", "show_type");
    uint32_t cfg_rawLen = config.intValue("fpt", "rawdata_len");
    bool showId = (cfg_id == 1);
    bool showType = (cfg_type == 1);

	ops::fp::FPTHandler * handler = new ops::fp::FPTHandler();
	ops::fp::ChunkGeneric * chunks = handler->flexLoad(filename);
	handler->dumpChunks(outstream, chunks, 0, showId, showType, cfg_rawLen);


	delete chunks;
    delete handler;
    INFO << "done";
}

void dumpFPL(ops::files::ConfigManager &config, std::string &filename, std::ostream &outstream)
{
	INFO << "starting fpl dump";

    uint32_t cfg_id = config.intValue("fpl", "show_id");
    uint32_t cfg_type = config.intValue("fpl", "show_type");
    uint32_t cfg_rawLen = config.intValue("fpl", "rawdata_len");
    bool showId = (cfg_id == 1);
    bool showType = (cfg_type == 1);

	ops::fp::FPLHandler * handler = new ops::fp::FPLHandler();
	ops::fp::ChunkGeneric * chunks = handler->flexLoad(filename);
	handler->dumpChunks(outstream, chunks, 0, showId, showType, cfg_rawLen);

	delete chunks;
    delete handler;
    INFO << "done";
}

int main (int argc, char *argv[])
{
	// check args
	if (argc != 2 && argc != 3)
	{
		std::cout << "usage : " << argv [0] << " inputfile [dumpfile]" << std::endl;
		std::cout << "\tdump Future Pinball 1.9.1 files" << std::endl;
		std::cout << "\t\tinputfile\t.fpm/.fpt/.fpl file" << std::endl;
		std::cout << "\t\tdumpfile\toptionnal output file" << std::endl;
		return 1;
	}
    std::string inputfile = argv[1];
    std::replace(inputfile.begin(), inputfile.end(), '\\', '/');

    // get output
    std::ostream * outstream = NULL;
    if (argc == 3)
    {
        std::ofstream * outfilestream = new std::ofstream(argv[2]);
        if (!outfilestream->is_open())
        {
            ERROR << "can't open outfile : " << argv[2];
            return 1;
        }
        outstream = outfilestream;
    }
    else
    {
        outstream = &std::cout;
    }

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
    (*outstream) << "DUMP of " << inputfile << std::endl;
    if (fileExt == "fpm")
    {
        dumpFPM(config, inputfile, *outstream);
    }
    else if (fileExt == "fpt")
    {
        dumpFPT(config, inputfile, *outstream);
    }
    else if (fileExt == "fpl")
    {
        dumpFPL(config, inputfile, *outstream);
    }
    else
    {
        ERROR << "not a Future Pinball file";
    }


    // close output
    if (argc == 3)
    {
        ((std::ofstream*)outstream)->close();
        delete ((std::ofstream*)outstream);
    }

	// libops shutdown
	ops::msole::shutdown();
	ops::shutdown();

	return 0;
}
