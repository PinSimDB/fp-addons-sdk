// std

#include <stdio.h>
#include <io.h>
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

#include <sys/stat.h>
#include <sys/types.h>

// libops
#include "ops-fp.h"
#include "ops-log.h"
#include "ops-files.h"

std::string getPath(std::string &outdir, ops::fp::ChunkString * nameChunk, std::string ext)
{
    std::string path = nameChunk->value;
    std::transform(path.begin(), path.end(), path.begin(), ::tolower);
    return std::string(outdir + "/" + path + ext);
}

std::string getPath(std::string &outdir, ops::fp::ChunkString * nameChunk, ops::fp::ChunkString * pathChunk)
{
    std::string ext = pathChunk->value.substr(pathChunk->value.find_last_of(".") + 1);
    std::string path = nameChunk->value;
    std::transform(path.begin(), path.end(), path.begin(), ::tolower);
    return std::string(outdir + "/" + path + "." + ext);
}

void writeRawDataChunk (ops::fp::FPBaseHandler * handler, std::string path, ops::fp::ChunkRawData * dataChunk)
{
    std::ofstream file;
    file.open(path.c_str(), std::ios::binary|std::ios::out);

    ops::RawData * data = handler->getRawData(dataChunk->value.data, dataChunk->value.len, true);
    file.write((const char*)data->data, data->len);
    file.close();

    std::cout << "\twriting " << path << std::endl;
}


void grabFPM(ops::files::ConfigManager &config, std::string &filename, std::string &outdir)
{
	INFO << "starting .fpm dump";

    // FPM grab config
    uint32_t cfg_preview = config.intValue("fpm", "grab_preview");
    uint32_t cfg_primary = config.intValue("fpm", "grab_primary_model");
    uint32_t cfg_secondary = config.intValue("fpm", "grab_secondary_model");
    uint32_t cfg_mask = config.intValue("fpm", "grab_mask_model");
    uint32_t cfg_reflection = config.intValue("fpm", "grab_reflection_model");


	ops::fp::FPMHandler * handler = new ops::fp::FPMHandler();
	ops::fp::ChunkChunkList * chunk = (ops::fp::ChunkChunkList*)handler->flexLoad(filename);

    ops::fp::ChunkString * nameChunk = (ops::fp::ChunkString*)handler->getChunkByLabel((ops::fp::ChunkChunkList*)chunk, "name");

    // Preview
    if (cfg_preview == 1) {
        ops::fp::ChunkString * pathChunk = (ops::fp::ChunkString*)handler->getChunkByLabel((ops::fp::ChunkChunkList*)chunk, "preview_path");
        ops::fp::ChunkRawData * dataChunk = (ops::fp::ChunkRawData*)handler->getChunkByLabel((ops::fp::ChunkChunkList*)chunk, "preview_data");
        if (pathChunk != NULL && dataChunk != NULL)
        {
            std::string ext = pathChunk->value.substr(pathChunk->value.find_last_of(".") + 1);
            writeRawDataChunk(handler, getPath(outdir, nameChunk, ".bmp"), dataChunk);
        }
    }

    if (cfg_primary == 1) {
        ops::fp::ChunkString * pathChunk = (ops::fp::ChunkString*)handler->getChunkByLabel((ops::fp::ChunkChunkList*)chunk, "primary_model_path");
        ops::fp::ChunkRawData * dataChunk = (ops::fp::ChunkRawData*)handler->getChunkByLabel((ops::fp::ChunkChunkList*)chunk, "primary_model_data");
        if (pathChunk != NULL && dataChunk != NULL)
        {
            std::string ext = pathChunk->value.substr(pathChunk->value.find_last_of(".") + 1);
            writeRawDataChunk(handler, getPath(outdir, nameChunk, ".ms3d"), dataChunk);
        }
    }

    if (cfg_secondary == 1) {
        ops::fp::ChunkString * pathChunk = (ops::fp::ChunkString*)handler->getChunkByLabel((ops::fp::ChunkChunkList*)chunk, "secondary_model_path");
        ops::fp::ChunkRawData * dataChunk = (ops::fp::ChunkRawData*)handler->getChunkByLabel((ops::fp::ChunkChunkList*)chunk, "secondary_model_data");
        if (pathChunk != NULL && dataChunk != NULL)
        {
            std::string ext = pathChunk->value.substr(pathChunk->value.find_last_of(".") + 1);
            writeRawDataChunk(handler, getPath(outdir, nameChunk, "_secondary.ms3d"), dataChunk);
        }
    }

    if (cfg_mask == 1) {
        ops::fp::ChunkString * pathChunk = (ops::fp::ChunkString*)handler->getChunkByLabel((ops::fp::ChunkChunkList*)chunk, "mask_model_path");
        ops::fp::ChunkRawData * dataChunk = (ops::fp::ChunkRawData*)handler->getChunkByLabel((ops::fp::ChunkChunkList*)chunk, "mask_model_data");
        if (pathChunk != NULL && dataChunk != NULL)
        {
            std::string ext = pathChunk->value.substr(pathChunk->value.find_last_of(".") + 1);
            writeRawDataChunk(handler, getPath(outdir, nameChunk, "_mask.ms3d"), dataChunk);
        }
    }

    if (cfg_reflection == 1) {
        ops::fp::ChunkString * pathChunk = (ops::fp::ChunkString*)handler->getChunkByLabel((ops::fp::ChunkChunkList*)chunk, "reflection_model_path");
        ops::fp::ChunkRawData * dataChunk = (ops::fp::ChunkRawData*)handler->getChunkByLabel((ops::fp::ChunkChunkList*)chunk, "reflection_model_data");
        if (pathChunk != NULL && dataChunk != NULL)
        {
            std::string ext = pathChunk->value.substr(pathChunk->value.find_last_of(".") + 1);
            writeRawDataChunk(handler, getPath(outdir, nameChunk, "_reflection.ms3d"), dataChunk);
        }
    }

	delete chunk;
	delete handler;
    INFO << "done";
}

void grabFPT(ops::files::ConfigManager &config, std::string &filename, std::string &outdir)
{
	INFO << "starting fpt dump";

    uint32_t cfg_image = config.intValue("fpt", "grab_image");
    uint32_t cfg_sound = config.intValue("fpt", "grab_sound");
    uint32_t cfg_dmdfont = config.intValue("fpt", "grab_dmdfont");
    uint32_t cfg_pinmodel = config.intValue("fpt", "grab_fpm");
    uint32_t cfg_script = config.intValue("fpt", "grab_script");

	ops::fp::FPTHandler * handler = new ops::fp::FPTHandler();
	ops::fp::ChunkChunkList * chunk = (ops::fp::ChunkChunkList*)handler->flexLoad(filename, true);

    if (cfg_image == 1) {
        ops::fp::ChunkChunkList * resources = handler->getChunksByLabel(chunk, "image");
        for (uint32_t i=0; i<resources->value.size(); i++)
        {
            ops::fp::ChunkString * nameChunk = (ops::fp::ChunkString*)handler->getChunkByLabel((ops::fp::ChunkChunkList *)resources->value[i], "name");
            ops::fp::ChunkString * pathChunk = (ops::fp::ChunkString*)handler->getChunkByLabel((ops::fp::ChunkChunkList *)resources->value[i], "path");
            ops::fp::ChunkRawData * dataChunk = (ops::fp::ChunkRawData*)handler->getChunkByLabel((ops::fp::ChunkChunkList *)resources->value[i], "data");
            if (pathChunk != NULL && dataChunk != NULL)
            {
                writeRawDataChunk(handler, getPath(outdir, nameChunk, pathChunk), dataChunk);
            }
        }
    }

    if (cfg_sound == 1) {
        ops::fp::ChunkChunkList * resources = handler->getChunksByLabel(chunk, "sound");
        for (uint32_t i=0; i<resources->value.size(); i++)
        {
            ops::fp::ChunkString * nameChunk = (ops::fp::ChunkString*)handler->getChunkByLabel((ops::fp::ChunkChunkList *)resources->value[i], "name");
            ops::fp::ChunkString * pathChunk = (ops::fp::ChunkString*)handler->getChunkByLabel((ops::fp::ChunkChunkList *)resources->value[i], "path");
            ops::fp::ChunkRawData * dataChunk = (ops::fp::ChunkRawData*)handler->getChunkByLabel((ops::fp::ChunkChunkList *)resources->value[i], "data");
            if (pathChunk != NULL && dataChunk != NULL)
            {
                writeRawDataChunk(handler, getPath(outdir, nameChunk, pathChunk), dataChunk);
            }
        }
    }

    if (cfg_sound == 1) {
        ops::fp::ChunkChunkList * resources = handler->getChunksByLabel(chunk, "music");
        for (uint32_t i=0; i<resources->value.size(); i++)
        {
            ops::fp::ChunkString * nameChunk = (ops::fp::ChunkString*)handler->getChunkByLabel((ops::fp::ChunkChunkList *)resources->value[i], "name");
            ops::fp::ChunkString * pathChunk = (ops::fp::ChunkString*)handler->getChunkByLabel((ops::fp::ChunkChunkList *)resources->value[i], "path");
            ops::fp::ChunkRawData * dataChunk = (ops::fp::ChunkRawData*)handler->getChunkByLabel((ops::fp::ChunkChunkList *)resources->value[i], "data");
            if (pathChunk != NULL && dataChunk != NULL)
            {
                writeRawDataChunk(handler, getPath(outdir, nameChunk, pathChunk), dataChunk);
            }
        }
    }

    if (cfg_dmdfont == 1) {
        ops::fp::ChunkChunkList * resources = handler->getChunksByLabel(chunk, "dmdfont");
        for (uint32_t i=0; i<resources->value.size(); i++)
        {
            ops::fp::ChunkString * nameChunk = (ops::fp::ChunkString*)handler->getChunkByLabel((ops::fp::ChunkChunkList *)resources->value[i], "name");
            ops::fp::ChunkString * pathChunk = (ops::fp::ChunkString*)handler->getChunkByLabel((ops::fp::ChunkChunkList *)resources->value[i], "path");
            ops::fp::ChunkRawData * dataChunk = (ops::fp::ChunkRawData*)handler->getChunkByLabel((ops::fp::ChunkChunkList *)resources->value[i], "data");
            if (pathChunk != NULL && dataChunk != NULL)
            {
                writeRawDataChunk(handler, getPath(outdir, nameChunk, pathChunk), dataChunk);
            }
        }
    }

    if (cfg_pinmodel == 1) {
        ops::fp::ChunkChunkList * resources = handler->getChunksByLabel(chunk, "pinmodel");
        for (uint32_t i=0; i<resources->value.size(); i++)
        {
            ops::fp::ChunkString * nameChunk = (ops::fp::ChunkString*)handler->getChunkByLabel((ops::fp::ChunkChunkList *)resources->value[i], "name");
            ops::fp::ChunkInt * linkedChunk = (ops::fp::ChunkInt*)handler->getChunkByLabel((ops::fp::ChunkChunkList *)resources->value[i], "linked");

            if (linkedChunk->value == 0)
            {
                ops::fp::ChunkRawData * pinmodelChunk = (ops::fp::ChunkRawData*)handler->getChunkByLabel((ops::fp::ChunkChunkList *)resources->value[i], "pinmodel_raw");
                std::string path = getPath(outdir, nameChunk, ".fpm");

                // recreate fpm
                std::cout << "\twriting " << path << std::endl;
                ops::msole::FileWriter * writer = new ops::msole::FileWriter();
                writer->open(path);
                writer->mkdir("PinModel");
                writer->writeAll("PinModel/ModelData", &pinmodelChunk->value);
                writer->close();

                // grab fpm content
                std::string outdirFPM(path + ".grab");
                std::replace(outdirFPM.begin(), outdirFPM.end(), '\\', '/');
                if (_mkdir(outdirFPM.c_str()) != 0)
                {
                    ERROR << "can't create out directory : " << outdirFPM << " errno=" << errno << " " << strerror(errno);
                }
                grabFPM(config, path, outdirFPM);
            }
        }
    }


    if (cfg_script == 1) {
        // TODO : embedded script

        ops::fp::ChunkChunkList * tableDataChunk = (ops::fp::ChunkChunkList *) handler->getChunkByLabel(chunk, "table_data");
        //handler->dumpChunks(std::cout, tableDataChunk, 0, false, true, 128);
        ops::fp::ChunkRawData * scriptChunk = (ops::fp::ChunkRawData *)handler->getChunkByLabel(tableDataChunk, "script");
        //handler->dumpChunks(std::cout, scriptChunk, 0, false, true, 128);
        if (scriptChunk != NULL)
        {
            std::string path(outdir + "/script.vbs");
            std::ofstream file;
            file.open(path.c_str(), std::ios::binary|std::ios::out);

            // Another glitch in FP file format ....
            // Add zLZO header
            ops::RawData reData(0, (const uint8_t *)NULL);
            reData.len = scriptChunk->value.len + 4;
            reData.data = new uint8_t[reData.len];
            reData.data[0] = 'z';
            reData.data[1] = 'L';
            reData.data[2] = 'Z';
            reData.data[3] = 'O';
            memcpy(&reData.data[4], scriptChunk->value.data, scriptChunk->value.len);

            ops::RawData * data = handler->getRawData(reData.data, reData.len, true);
            file.write((const char*)data->data, data->len);
            file.close();

            std::cout << "\twriting " << path << std::endl;
        }
    }

	delete chunk;
    delete handler;
    INFO << "done";
}

void grabFPL(ops::files::ConfigManager &config, std::string &filename, std::string &outdir)
{
	INFO << "starting fpl grab";

    uint32_t cfg_image = config.intValue("fpl", "grab_image");
    uint32_t cfg_sound = config.intValue("fpl", "grab_sound");
    uint32_t cfg_dmdfont = config.intValue("fpl", "grab_dmdfont");
    uint32_t cfg_pinmodel = config.intValue("fpl", "grab_fpm");
    uint32_t cfg_script = config.intValue("fpl", "grab_script");

	ops::fp::FPLHandler * handler = new ops::fp::FPLHandler();
	ops::fp::ChunkChunkList * chunk = (ops::fp::ChunkChunkList *)handler->flexLoad(filename);
    for (uint32_t i=0; i<chunk->value.size(); i++)
    {
        ops::fp::ChunkGeneric * resChunk = chunk->value[i];

        ops::fp::ChunkString * nameChunk = (ops::fp::ChunkString*)handler->getChunkByLabel((ops::fp::ChunkChunkList*)resChunk, "name");
        ops::fp::ChunkString * pathChunk = (ops::fp::ChunkString*)handler->getChunkByLabel((ops::fp::ChunkChunkList*)resChunk, "path");
        ops::fp::ChunkRawData * dataChunk = (ops::fp::ChunkRawData*)handler->getChunkByLabel((ops::fp::ChunkChunkList*)resChunk, "data");
        std::string path = getPath(outdir, nameChunk, pathChunk);
        std::string ext = path.substr(path.find_last_of(".") + 1);

        if ( (ext == "bmp" || ext == "tga" || ext == "jpg") && cfg_image == 1) {
            writeRawDataChunk(handler,  path, dataChunk);
        }
        if ( (ext == "wav" || ext == "ogg" ) && cfg_sound == 1) {
            writeRawDataChunk(handler,  path, dataChunk);
        }
        if ( (ext == "dmdf" ) && cfg_dmdfont == 1) {
            writeRawDataChunk(handler,  path, dataChunk);
        }
        if ( (ext == "vbs" ) && cfg_script == 1) {
            writeRawDataChunk(handler,  path, dataChunk);
        }
        if( ext == "fpm" && cfg_pinmodel == 1) {
            writeRawDataChunk(handler,  path, dataChunk);
            std::string outdirFPM(path + ".grab");
            std::replace(outdirFPM.begin(), outdirFPM.end(), '\\', '/');
            if (_mkdir(outdirFPM.c_str()) != 0)
            {
                ERROR << "can't create out directory : " << outdirFPM << " errno=" << errno << " " << strerror(errno);
            }
            grabFPM(config, path, outdirFPM);
        }
	}
	delete chunk;
    delete handler;
    INFO << "done";
}

int main (int argc, char *argv[])
{
	// check args
	if (argc != 2 && argc != 3)
	{
		std::cout << "usage : " << argv [0] << " inputfile [outdir]" << std::endl;
		std::cout << "\tgrab all resources Future Pinball 1.9.1 files" << std::endl;
		std::cout << "\t\tinputfile\t.fpm/.fpt/.fpl file" << std::endl;
		std::cout << "\t\toutdir\toptionnal outdir file" << std::endl;
		return 1;
	}
    std::string inputfile(argv[1]);
    std::replace(inputfile.begin(), inputfile.end(), '\\', '/');


    // get output dir
    std::string fileWOpath = "";
    if (inputfile.find_last_of("/") != std::string::npos)
    {
        fileWOpath = inputfile.substr(inputfile.find_last_of("/")+1);
    }
    else
    {
        fileWOpath = inputfile;
    }

    // read config file
    ops::files::ConfigManager config;
    ops::files::Path configPath(argv[0]);
    std::string configFile = configPath.basenameWOExt() + ".cfg";
    config.addStdFile(configFile.c_str());


    std::string outdir(fileWOpath + ".grab");
    if (argc == 3)
    {
        outdir = std::string(argv[2]) + "/" + outdir;
        std::replace(outdir.begin(), outdir.end(), '\\', '/');
    }
    if (_mkdir(outdir.c_str()) != 0)
    {
        ERROR << "can't create out directory : " << outdir << " errno=" << errno << " " << strerror(errno);
        return 1;
    }
    std::cout << "exporting files to " << outdir << std::endl;



	// libops init
	ops::Log::level() = ops::logERROR;
	if (!ops::init()) {
		return 1;
	}
	if (!ops::msole::init()) {
		return 1;
	}

	// do the grab, given extension file
    std::string fileExt = "";
    if (inputfile.find_last_of(".") != std::string::npos)
    {
        fileExt = inputfile.substr(inputfile.find_last_of(".")+1);
    }

    if (fileExt == "fpm")
    {
        grabFPM(config, inputfile, outdir);
    }
    else if (fileExt == "fpt")
    {
        grabFPT(config, inputfile, outdir);
    }
    else if (fileExt == "fpl")
    {
        grabFPL(config, inputfile, outdir);
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
