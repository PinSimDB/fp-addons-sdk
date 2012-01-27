#ifndef __OPS_FILES_H_INCLUDED
#define __OPS_FILES_H_INCLUDED

// std
#include <stdio.h>
#include <stdint.h>

// stl
#include <string>
#include <vector>
#include <map>

// ops
#include "ops-base.h"

namespace ops
{
namespace files
{

// http://www.cplusplus.com/reference/clibrary/cstdio/

class Path
{
public:
    std::string value;

    Path();
    Path(const char * initvalue);
    Path(const std::string &initvalue);
    Path(const Path & path);
    ~Path();

    Path & operator=(const char * path);
    Path & operator=(const std::string & path);
    Path & operator=(const Path & path);

    //bool exists();
    //bool isFile();
    //bool isDirectory();
    std::string dirname();
    std::string basename();
    std::string basenameWOExt();
    std::string getFileExt();
    bool makedir();
    std::vector<std::string> list();
    bool rm();


private:
    void normalize();

};

// Only binary functions
class AbsFile
{
public:
    virtual bool close() = 0;
    virtual bool is_open() = 0;
    virtual bool is_eof() = 0;
    virtual std::string toString() = 0;
    // TODO : flush
    // TODO : reopen

    // Binary I/O functions
    virtual bool seek(int32_t offset, uint32_t origin) = 0;
    virtual int32_t tell() = 0;
    // TODO : rewind
    virtual uint32_t read(void * ptr, uint32_t count) = 0;
    virtual uint32_t write(const void * ptr, uint32_t count) = 0;

    // helper read/write func
    uint32_t writeInt(const uint32_t & value);
    uint32_t writeFloat(const float & value);
    uint32_t writeRawData(const ops::RawData & value);
    uint32_t writeString(const std::string & value);

    uint32_t readInt(uint32_t * value);
    uint32_t readFloat(float * value);
    uint32_t readRawData(ops::RawData * value);
    uint32_t readString(std::string * value);

    // Text I/O functions
    virtual std::string readLine() = 0;

};




class StdFile : public AbsFile
{
public:
    StdFile(ops::files::Path filePath, const char * mode);
    StdFile(const char * filename, const char * mode);
    ~StdFile();

    std::string toString();

    bool is_open();
    bool close();
    bool is_eof();
    // TODO : flush
    // TODO : reopen

    // Binary I/O functions
    bool seek(int32_t offset, uint32_t origin);
    int32_t tell();
    // TODO : rewind
    uint32_t read(void * ptr, uint32_t count);
    uint32_t write(const void * ptr, uint32_t count);



    // Text I/O functions
    std::string readLine();

private:
    FILE * stream;
    std::string desc;
};


class MemFile : public AbsFile
{
public:
    MemFile(uint8_t * initPtr, uint32_t initSize, bool allowExtend, std::string initDesc = "");
    MemFile(uint32_t initSize, bool allowExtend, std::string initDesc = "");
    ~MemFile();

    std::string toString();

    bool is_open();
    bool close();
    bool is_eof();
    // TODO : flush
    // TODO : reopen

    // Binary I/O functions
    bool seek(int32_t offset, uint32_t origin);
    int32_t tell ();
    // TODO : rewind
    uint32_t read(void * dst, uint32_t count);
    uint32_t write(const void * src, uint32_t count);

    // Text I/O functions
    std::string readLine();

    // Specific members
    uint8_t * startPtr;
    uint32_t size;


private:
    uint8_t * currentPtr;
    uint32_t reservedSize;
    uint32_t extendSize;
    std::string desc;
};



typedef struct
{
    std::string value;
    uint32_t fileNumber;
} ConfigValue;

class ConfigManager
{
public:
    ConfigManager();
    ~ConfigManager();
    bool addPackedFile(const char * file, const char * iniFile);
    bool addStdFile(const char * file);
    bool addStdFile(const ops::files::Path &path);

    std::string stringValue(const char * section, const char * key);
    uint32_t intValue(const char * section, const char * key);
    double doubleValue(const char * section, const char * key);
    bool boolValue(const char * section, const char * key);
    void dump();
    bool parse(ops::files::AbsFile &inFile);
private:
    std::map<std::string, ConfigValue> data;
    std::vector<std::string> files;
};





#define LZOFILE_MODE_READ 1
#define LZOFILE_MODE_WRITE 2

struct LZOFileIndex
{
    std::string filePath;
    uint32_t fileSize;
    uint32_t compressedSize;
    uint32_t index;
};

class LZOFile
{
public:
    std::string path;
    uint32_t mode;
    std::vector<LZOFileIndex> filesIndex;

    LZOFile(std::string &initPath, uint32_t initMode);
    LZOFile(ops::files::Path &initPath, uint32_t initMode);
    ~LZOFile();

    bool close();
    bool is_open();

    // write functions
    bool addFile(const char * filePath, ops::RawData &data);
    bool addFile(const std::string &filePath, ops::RawData &data);

    // read functions
    ops::RawData * readFile(const char *filePath);
    ops::RawData * readFile(const std::string &filePath);

    // misc.
    //TODO : void listFiles();
    void dump(std::ostream &outstream);


private:
    ops::files::StdFile * stream;
    uint32_t currentIndex;

    void init();
};


} // namespace files
} // namespace ops


#endif // __OPS_FILES_H_INCLUDED

