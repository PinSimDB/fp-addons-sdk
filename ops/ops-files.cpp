// std
#include <stdio.h>
#include <memory.h>

#include <sys/types.h>
#include <dirent.h>


#include <iostream>
#include <string>
#include <algorithm>

// ops
#include "ops-base.h"
#include "ops-log.h"
#include "ops-files.h"
#include "ops-tools.h"



namespace ops
{
namespace files
{

Path::Path()
{
    value="";
}

Path::Path(const char * initvalue)
{
    value = initvalue;
    normalize();
}

Path::Path(const std::string &initvalue)
{
    value = initvalue;
    normalize();
}

Path::Path(const Path & path)
{
    value = path.value;
}

Path::~Path()
{
}

Path & Path::operator=(const char * path)
{
    value = path;
    normalize();
    return *this;
}

Path & Path::operator=(const std::string & path)
{
    value = path;
    normalize();
    return *this;
}

Path & Path::operator=(const Path & path)
{
    if (this != &path) // Check for self-assignment
    {
        value = path.value;
    }
    return *this;
}

/*
bool Path::exists()
{
    return false;
}


bool Path::isFile()
{
    return false;
}

bool Path::isDirectory()
{
    return false;
}
*/


std::string Path::dirname()
{
    if (value.find_last_of("/") != std::string::npos)
    {
        return value.substr(0, value.find_last_of("/"));
    }
    else
        return "";
}

std::string Path::basename()
{
    if (value.find_last_of("/") != std::string::npos)
    {
        return value.substr(value.find_last_of("/")+1);
    }
    else return value;
}

std::string Path::basenameWOExt()
{
    std::string tmp(value);
    if (value.find_last_of("/") != std::string::npos)
    {
        tmp = value.substr(value.find_last_of("/")+1);

    }
    if (tmp.find_last_of(".") != std::string::npos)
    {
        tmp = tmp.substr(0, tmp.find_last_of("."));
    }
    return tmp;
}


std::string Path::getFileExt()
{
    std::string fileExt = "";
    if (value.find_last_of(".") != std::string::npos)
    {
        fileExt = value.substr(value.find_last_of(".")+1);
    }
    return fileExt;
}


std::vector<std::string> Path::list()
{
    DIR *dp;
    std::vector<std::string> files;
    struct dirent *dirp;
    if((dp  = opendir(value.c_str())) == NULL)
    {
        ERROR << "can't list files in directory (" << errno << ") : " << value;
        return files;
    }

    while ((dirp = readdir(dp)) != NULL)
    {
        std::string newFile(dirp->d_name);
        if ( !(newFile == ".") && !(newFile == "..") )
        {
            files.push_back(newFile);
        }
    }
    closedir(dp);
    return files;
}

bool Path::makedir()
{
    if (mkdir(value.c_str()) != 0)
    {
        ERROR << "can't create out directory : " << value << " errno=" << errno << " " << strerror(errno);
        return false;
    }
    return true;
}

bool Path::rm()
{
    if (remove(value.c_str()) != 0)
    {
        ERROR << "can't remove file : " << value << " errno=" << errno << " " << strerror(errno);
        return false;
    }
    return true;
}

void Path::normalize()
{
    // replace \ with /
    std::replace(value.begin(), value.end(), '\\', '/');
    // remove trailing /
    // TODO
}

//-------------------------------------------------------------------------------------------------------

// TODO : revoir les sorties res

uint32_t AbsFile::writeInt(const uint32_t & value) {
    return write(&value, sizeof(uint32_t));
}

uint32_t AbsFile::writeFloat(const float & value) {
    return write(&value, sizeof(float));
}

uint32_t AbsFile::writeRawData(const ops::RawData & value) {
    write(&value.len, sizeof(uint32_t));
    return write(value.data, value.len);
}

uint32_t AbsFile::writeString(const std::string & value) {
    uint32_t len = value.size();
    write(&len, sizeof(uint32_t));
    return write(value.c_str(), len);
}

uint32_t AbsFile::readInt(uint32_t * value) {
    return read(value, sizeof(uint32_t));
}

uint32_t AbsFile::readFloat(float * value) {
    return read(value, sizeof(float));
}

uint32_t AbsFile::readRawData(ops::RawData * value) {
    read(&value->len, sizeof(uint32_t));
    value->data = new uint8_t[value->len];
    return read(value->data, value->len);
}

uint32_t AbsFile::readString(std::string * value) {
    uint32_t len = 0;
    read(&len, sizeof(uint32_t));
    uint8_t * buffer = new uint8_t[len + 1];
    read(buffer, len);
    buffer[len] = 0;
    value->assign((const char*)buffer);
    return true;
}

//-------------------------------------------------------------------------------------------------------

StdFile::StdFile(ops::files::Path filePath, const char * mode)
{
    stream = fopen(filePath.value.c_str(), mode);
    if (stream == NULL)
    {
        ERROR << "can't open " << filePath.value;
    }
    desc = filePath.value;
}


StdFile::StdFile(const char * filename, const char * mode)
{
    stream = fopen(filename, mode);
    if (stream == NULL)
    {
        ERROR << "can't open " << filename;
    }
    desc = filename;
}

StdFile::~StdFile()
{
    if ((stream != NULL))
        close();
}

std::string StdFile::toString()
{
    return desc;
}

bool StdFile::is_open()
{
    return (stream != NULL);
}


bool StdFile::is_eof()
{
    return (feof(stream) != 0);
}


bool StdFile::close()
{
    if (stream == NULL)
    {
        ERROR << "stream not opened";
        return false;
    }
    if (fclose(stream) == EOF)
    {
        ERROR << "can't close stream not opened";
        return false;
    }
    stream = NULL;
    return true;
}

bool StdFile::seek(int32_t offset, uint32_t origin)
{
    if (stream == NULL)
    {
        ERROR << "stream not opened";
        return false;
    }
    int res = fseek(stream, offset, origin);
    if (res != 0)
    {
        ERROR << "can't seek to specified position";
        return false;
    }
    return true;
}

int32_t StdFile::tell ()
{
    if (stream == NULL)
    {
        ERROR << "stream not opened";
        return -1;
    }
    int32_t res = ftell(stream);
    if (res == -1)
    {
        ERROR << "an error occur";
        return -1;
    }
    return res;
}

uint32_t StdFile::read(void * ptr, size_t count)
{
    if (stream == NULL)
    {
        ERROR << "stream not opened";
        return -1;
    }
    uint32_t res = fread(ptr, 1, count, stream);
    if (res != count)
    {
        ERROR << "read error : " << ferror(stream) << ", feof : " << feof(stream);
    }
    return res;
}


uint32_t StdFile::write(const void * ptr, uint32_t count)
{
    if (stream == NULL)
    {
        ERROR << "stream not opened";
        return -1;
    }
    uint32_t res = fwrite(ptr, 1, count, stream);
    if (res != count)
    {
        ERROR << "write error : " << ferror(stream) << ", feof : " << feof(stream);
    }
    return res;
}


std::string StdFile::readLine()
{
    if (stream == NULL)
    {
        ERROR << "stream not opened";
        return "ERROR";
    }
    char buffer[4096];
    char * res = fgets(buffer, 4096, stream);
    if (res == NULL)
    {
        ERROR << "can't read chars";
        return "ERROR";
    }
    if(res[strlen(res) - 1] == '\n')
        res[strlen(res) - 1] = 0;
    return std::string(res);
}

//-------------------------------------------------------------------------------------------------------

#define MEMFILE_INITEXTEND 128*1024

MemFile::MemFile(uint8_t * initPtr, uint32_t initSize, bool allowExtend, std::string initDesc) :
    startPtr(initPtr),
    size(initSize), reservedSize(initSize),
    desc(initDesc)
{
    currentPtr = startPtr;
    if (allowExtend)
    {
        extendSize = MEMFILE_INITEXTEND;
    }
    else
    {
        extendSize = 0;
    }
}

MemFile::MemFile(uint32_t initReservedSize, bool allowExtend, std::string initDesc) :
    size(0), reservedSize(initReservedSize),
    desc(initDesc)
{
    startPtr = new uint8_t[initReservedSize];
    currentPtr = startPtr;
    if (allowExtend)
    {
        extendSize = MEMFILE_INITEXTEND;
    }
    else
    {
        extendSize = 0;
    }
}

MemFile::~MemFile()
{
}

std::string MemFile::toString()
{
    return desc;
}

bool MemFile::is_open()
{
    return (startPtr != NULL);
}

bool MemFile::is_eof()
{
    return (currentPtr >= startPtr + size);
}

bool MemFile::close()
{
    if (startPtr == NULL)
    {
        ERROR << "stream not opened";
        return false;
    }
    startPtr = NULL;
    currentPtr = NULL;
    size = 0;
    reservedSize = 0;
    extendSize = MEMFILE_INITEXTEND;
    return true;
}



bool MemFile::seek(int32_t offset, uint32_t origin)
{
    if (startPtr == NULL)
    {
        ERROR << "stream not opened";
        return false;
    }
    uint8_t* newPtr = currentPtr;
    switch (origin)
    {
    case SEEK_SET :
        newPtr = startPtr + offset;
        break;
    case SEEK_CUR :
        newPtr = currentPtr + offset;
        break;
    case SEEK_END :
        newPtr = startPtr + size + offset;
        break;
    }
    if (newPtr < startPtr)
    {
        ERROR << "out of bound";
        return false;
    }
    if (newPtr >= startPtr + size)
    {
        ERROR << "out of bound";
        return false;
    }
    currentPtr = newPtr;
    return true;
}

int32_t MemFile::tell ()
{
    if (startPtr == NULL)
    {
        ERROR << "stream not opened";
        return -1;
    }
    if (currentPtr >= startPtr + size)
    {
        return -1;
    }
    return currentPtr - startPtr;
}

uint32_t MemFile::read(void * dst, uint32_t count)
{
    if (startPtr == NULL)
    {
        ERROR << "stream not opened";
        return -1;
    }
    uint32_t realCount = count;
    if (currentPtr + count > startPtr + size)
    {
        realCount = startPtr + size - currentPtr;
    }
    memcpy(dst, currentPtr, realCount);
    currentPtr +=  realCount;
    return realCount;
}


uint32_t MemFile::write(const void * src, uint32_t count)
{
    if (startPtr == NULL)
    {
        ERROR << "stream not opened";
        return -1;
    }

    // check if size is enough
    uint32_t realCount = count;
    if (currentPtr + count > startPtr + size)
    {
        if (extendSize > 0)
        {
            // if allow extend
            uint32_t neededReversedSize = reservedSize;
            while (currentPtr + count > startPtr + neededReversedSize)
            {
                neededReversedSize += extendSize;
                extendSize = extendSize * 2;
            }
            if (neededReversedSize != reservedSize)
            {
                // previous reservedSize is not enough, resize
                uint32_t offset = currentPtr - startPtr;
                uint8_t * oldPtr = startPtr;

                reservedSize = neededReversedSize;
                startPtr = new uint8_t[neededReversedSize];
                currentPtr = startPtr + offset;
                memcpy(startPtr, oldPtr, size);
                delete [] oldPtr;
            }
        }
        else
        {
            // truncate data
            realCount = startPtr + size - currentPtr;
        }
        size += realCount;
    }

    memcpy(currentPtr, src, realCount);
    currentPtr +=  realCount;
    return realCount;
}


std::string MemFile::readLine()
{
    if (startPtr == NULL)
    {
        ERROR << "MemFile::readLine() : stream not opened";
        return "ERROR";
    }
    uint8_t * ptr = currentPtr;
    uint32_t len = 0;
    while (currentPtr + len < startPtr + size && (currentPtr[len] != 0x0D && currentPtr[len] != 0x0A) )
    {
        len ++;
    }
    currentPtr += len;
    if(currentPtr + len < startPtr + size && (currentPtr[0] == 0x0D || currentPtr[0] == 0x0A))
    {
        currentPtr++;
    }
    if(currentPtr + len < startPtr + size && (currentPtr[0] == 0x0D || currentPtr[0] == 0x0A))
    {
        currentPtr++;
    }
    return std::string((const char *)ptr, len);
}


//-------------------------------------------------------------------------------------------------------

// TODO from lvr :)
// - optim map clé/data avec data struct valeur et num de fichier.
//   garder uniquement la valeur de la clé trouver dans le dernier fichier.
//   garder la liste des fichiers dans un vector separé, prévoir une méthode pour connaitre le fichier source d'une clé
// - code retour des addXXXFile
// - méthode addFile qui prend un AbsFile (renommer le parse et le mettre public ?).
//   Si on a un file qui vient d'ailleurs, ou si on a déjà ouvert le packed file, pour éviter de le rouvrir deux fois (prend du temps)
// - faire un boolValue() : true si =1, false sinon
// - j'aime pas bien le nom ini en fait, car pas vraiment un fichier .ini (plus ou moins normé, avec des sections, etc ...). CfgHandler ?


ConfigManager::ConfigManager()
{
}

ConfigManager::~ConfigManager()
{
}

bool ConfigManager::addPackedFile(const char * file, const char * iniFile)
{
    ops::files::Path inputPath(file);
    ops::files::Path filePath(iniFile);
    ops::files::LZOFile lzoFile(inputPath.value, LZOFILE_MODE_READ);
    //TODO faire wrapper pour la partie suivante
    ops::RawData * data = lzoFile.readFile(filePath.value);
    if (data->len == 0)
    {
        std::cout << "can't open file : " << iniFile << std::endl;
        return false;
    }
    ops::files::MemFile inFile(data->data, data->len, false, "test.cfg");

    bool success = parse(inFile);

    inFile.close();

    delete data;
    lzoFile.close();

    return success;
}

bool ConfigManager::addStdFile(const ops::files::Path &path) {
    ops::files::StdFile inFile(path, "rt");

    bool success = parse(inFile);

    inFile.close();

    return success;
}

bool ConfigManager::addStdFile(const char * file)
{
    ops::files::Path inputPath(file);
    ops::files::StdFile inFile(inputPath, "rt");

    bool success = parse(inFile);

    inFile.close();

    return success;
}

//TODO : pas fait de trim
bool ConfigManager::parse(ops::files::AbsFile &inFile)
{
    if (!inFile.is_open())
    {
        ERROR << "can't open file : " << inFile.toString() << std::endl;
        return false;
    }

    uint32_t fileNumber = files.size();
    files.push_back(inFile.toString());

    while (!inFile.is_eof())
    {
        std::string line = inFile.readLine();
        if (!line.empty())
        {
            //commented lines with ";" and "#" are ignored
            if (line.at(0) != ';' && line.at(0) != '#')
            {
                std::vector<std::string> keyvaluepair = ops::tools::split(line, '=');
                if (keyvaluepair.size() == 2)
                {
                    //TODO je ne sais pas si le remplacement d'une variable par un autre détruit la précédente
                    //(valeur surchargée par un autre fichier)
                    ConfigValue value;
                    value.value = keyvaluepair[1];
                    value.fileNumber = fileNumber;
                    data[keyvaluepair[0]] = value;
                    //std::cout << "adding [" << keyvaluepair[0] << "] - [" << keyvaluepair[1] << "]" << std::endl;
                }
            }
        }
    }
}

std::string ConfigManager::stringValue(const char * section, const char * key)
{
    std::string completeKey = section;
    completeKey += ".";
    completeKey += key;

    std::string out = "";
    std::map<std::string, ConfigValue>::iterator it;
    it = data.find(completeKey);
    if (it != data.end())
    {
        ConfigValue value = data[completeKey];
        out = value.value;
    }
    return out;
}

void ConfigManager::dump()
{
    std::cout << "Dumping key values" << std::endl;

    std::map<std::string, ConfigValue>::iterator it;
    for (it = data.begin() ; it != data.end(); it++ )
    {
        ConfigValue value = it->second;
        std::cout << "[" << it->first << "] = [" << value.value << "], found in file " << files[value.fileNumber] << std::endl;
    }
}

uint32_t ConfigManager::intValue(const char * section, const char * key)
{
    std::string s = stringValue(section, key);
    return atoi(s.c_str());
}

double ConfigManager::doubleValue(const char * section, const char * key)
{
    std::string s = stringValue(section, key);
    return atof(s.c_str());
}

bool ConfigManager::boolValue(const char * section, const char * key)
{
    return (intValue(section, key) == 1);
}

//-------------------------------------------------------------------------------------------------------


char  LZOFILE_HEADER_ID[] = "OPSZ";
uint32_t LZOFILE_HEADER_VERSION = 1;


LZOFile::LZOFile(ops::files::Path &initPath, uint32_t initMode) {
    path = initPath.value;
    mode = initMode;
    init();
}

LZOFile::LZOFile(std::string &initPath, uint32_t initMode)
{
    path = initPath;
    mode = initMode;
    init();
}

LZOFile::~LZOFile()
{
    if (stream != NULL)
    {
        close();
    }
}


void LZOFile::init() {
if (mode == LZOFILE_MODE_READ)
    {
        // LZOFILE_MODE_READ
        stream = new StdFile(path.c_str(), "rb");
        if (stream->is_open())
        {
            // Check header
            char id[4];
            uint32_t version=0;
            stream->read(id, 4);
            stream->read(&version, sizeof(version));
            if (strncmp(id, LZOFILE_HEADER_ID, 4) != 0 || version != LZOFILE_HEADER_VERSION)
            {
                // Not a valid LZOFile
                ERROR << "not a valid LZOFile : " << path;
                stream = NULL;
            }
            else
            {
                // jump to index start
                currentIndex = 8;
                uint32_t fileSize;
                do
                {
                    stream->read(&fileSize, sizeof(fileSize));
                    if (fileSize != 0)
                    {
                        stream->seek(fileSize, SEEK_CUR);
                    }
                    currentIndex += sizeof(fileSize);
                    currentIndex += fileSize;
                }
                while (fileSize != 0);
                // read index
                uint32_t pathLen;
                do
                {
                    stream->read(&pathLen, sizeof(pathLen));
                    if (pathLen != 0)
                    {
                        char buffer[pathLen+1];
                        stream->read(buffer, pathLen);
                        buffer[pathLen]=0;
                        LZOFileIndex newIndex;
                        newIndex.filePath = buffer;
                        stream->read(&newIndex.fileSize, sizeof(newIndex.fileSize));
                        stream->read(&newIndex.compressedSize, sizeof(newIndex.compressedSize));
                        stream->read(&newIndex.index, sizeof(newIndex.index));
                        filesIndex.push_back(newIndex);
                    }
                }
                while (pathLen != 0);
            }
        }
        else
        {
            ERROR << "can't open LZOFile : " << path;
            stream = NULL;
        }
    }
    else
    {
        // LZOFILE_MODE_WRITE
        stream = new StdFile(path.c_str(), "wb");
        if (stream->is_open())
        {
            stream->write(LZOFILE_HEADER_ID, 4);
            stream->write(&LZOFILE_HEADER_VERSION, sizeof(LZOFILE_HEADER_VERSION));
            currentIndex = 8;
        }
        else
        {
            ERROR << "can't open LZOFile : " << path;
            stream = NULL;
        }
    }
}

bool LZOFile::is_open()
{
    return (stream != NULL);
}

bool LZOFile::close()
{
    if (stream == NULL)
    {
        ERROR << "stream already close";
        return false;
    }
    if (mode == LZOFILE_MODE_WRITE)
    {
        // Write index
        uint32_t indexStartId = 0;
        stream->write(&indexStartId, sizeof(indexStartId));
        for (uint32_t i=0; i<filesIndex.size(); i++)
        {
            uint32_t fileNameLen = filesIndex[i].filePath.size();
            stream->write(&fileNameLen, sizeof(fileNameLen));
            stream->write(filesIndex[i].filePath.c_str(), fileNameLen);
            stream->write(&filesIndex[i].fileSize, sizeof(filesIndex[i].fileSize));
            stream->write(&filesIndex[i].compressedSize, sizeof(filesIndex[i].compressedSize));
            stream->write(&filesIndex[i].index, sizeof(filesIndex[i].index));
        }
        uint32_t indexStopId = 0;
        stream->write(&indexStopId, sizeof(indexStopId));
    }

    stream->close();
    delete stream;
    stream = NULL;
    return true;
}

bool LZOFile::addFile(const char * filePath, ops::RawData &data) {
    return addFile(std::string(filePath), data);
}

bool LZOFile::addFile(const std::string &filePath, ops::RawData &data)
{
    if (stream == NULL)
    {
        ERROR << "stream already close";
        return false;
    }
    if (mode == LZOFILE_MODE_READ)
    {
        ERROR << "stream not open in write file";
        return false;
    }
    if (data.len == 0)
    {
        ERROR << "can't add empty file : " << filePath;
        return false;
    }

    ops::RawData * packedData = data.packLZO();

    LZOFileIndex newIndex;
    newIndex.filePath = filePath;
    newIndex.fileSize = data.len;
    newIndex.compressedSize = packedData->len;
    newIndex.index = currentIndex;
    filesIndex.push_back(newIndex);

    stream->write(&packedData->len, sizeof(packedData->len));
    stream->write(packedData->data, packedData->len);

    currentIndex += sizeof(packedData->len);
    currentIndex += packedData->len;

    delete packedData;

    return true;
}

void LZOFile::dump(std::ostream &outstream)
{
    if (stream == NULL)
    {
        ERROR << "stream already close";
        return;
    }
    outstream << "path : " << path << std::endl;
    outstream << "mode : " << mode << std::endl;
    for (uint32_t i=0; i<filesIndex.size(); i++)
    {
        /*
        outstream << "\t" << filesIndex[i].filePath << std::endl;
        outstream << "\t\tsize :" << filesIndex[i].fileSize << std::endl;
        outstream << "\t\tcompressedSize :" << filesIndex[i].compressedSize << std::endl;
        outstream << "\t\tindex :" << filesIndex[i].index << std::endl;
        */
        outstream << "\t" << filesIndex[i].filePath << " : " << filesIndex[i].fileSize << "B (" << filesIndex[i].compressedSize << "B) ratio=" << (filesIndex[i].compressedSize*100 / filesIndex[i].fileSize) << "%" << std::endl;
    }
}

ops::RawData * LZOFile::readFile(const char * filePath) {
    return readFile(std::string(filePath));
}

ops::RawData * LZOFile::readFile(const std::string &filePath)
{
    if (stream == NULL)
    {
        ERROR << "stream already close";
        return NULL;
    }
    if (mode == LZOFILE_MODE_WRITE)
    {
        ERROR << "stream not open in read file";
        return NULL;
    }

    // Search in index
    uint32_t index = 0;
    for (uint32_t i=0; i<filesIndex.size(); i++)
    {
        if (filesIndex[i].filePath == filePath)
        {
            index = filesIndex[i].index;
            break;
        }
    }
    if (index == 0)
    {
        ERROR << "can't find stream file " << filePath;
        return NULL;
    }


    stream->seek(index, SEEK_SET);
    ops::RawData * packedData = new ops::RawData();
    stream->read(&packedData->len, sizeof(packedData->len));
    packedData->data = new uint8_t[packedData->len];
    stream->read(packedData->data, packedData->len);

    ops::RawData * data = packedData->unpackLZO();
    delete packedData;

    return data;
}


} // namespace files
} // namespace ops
