#ifndef __OPS_TABLE_H_INCLUDED
#define __OPS_TABLE_H_INCLUDED

// std
// ops
#include "ops-base.h"
#include "ops-files.h"


namespace ops
{


static char TABLE_FILE_EXT[] = "table\0";
static char LIBRARY_FILE_EXT[] = "library\0";
static char MODEL_FILE_EXT[] = "model\0";

static char TABLE_HEADER_ID[] = "OPST";
static uint32_t TABLE_HEADER_VERSION = 1;



struct Resource
{
    std::string name;
    std::string path;
    uint32_t type;
};


/*
TODO : remap type
1 : bmp
2 : jpg
4 : tga
9 : ogg
15 : dmdf
5 : model
*/

struct ImageResource : public Resource
{
    uint32_t transparentColor;
	uint32_t s3tcCompression;
	uint32_t disableFiltering;
};


struct SoundResource : public Resource
{
};

struct MusicResource : public Resource
{
};

struct ModelResource : public Resource
{
};

struct ScriptResource : public Resource
{
};

struct StringList
{
    std::string name;
    uint32_t nbValues;
    std::string * values;

    StringList();
    ~StringList();
};




struct LightList : public StringList
{
};

struct ImageList : public StringList
{
};


struct TableInfo
{
    std::string name;
    std::string authors;
    std::string releaseDate;
    std::string rules;
};


struct TableLoadingScreen
{
    std::string picture;
    uint32_t color;
};


struct TablePlayfield
{
    float width;
    float length;
    float frontGlassHeight;
    float rearGlassHeight;
    float slope;
    float centerLine;
    float flipperLine;

    std::string texture;
    uint32_t color;

    // editor
    uint32_t editorDisplayGrid;
    float editorGridSize;
    uint32_t editorDisplayTexture;
};

struct TableCabinet
{
    std::string texture;
    uint32_t woodColor;
    uint32_t buttonColor;
};

struct TableTranslite
{
    uint32_t width;
    uint32_t height;
    std::string texture;
    uint32_t color;
    uint32_t editorDisplayGrid;
    uint32_t editorGridSize;
    uint32_t editorDisplayTexture;
};

struct Position {
    float x;
    float y;
    float rotation;
    std::string surface;
    float offset;
};

struct ModelRenderOpt {
    std::string model;
    uint32_t color;
    std::string texture;
    std::string spheremap;
    uint32_t ornamental;
    uint32_t reflects_off_playfield;
    std::string playfield;
};

struct TableElement {
    std::string name;
    uint32_t type;
    uint32_t locked;
    uint32_t layer;
    Position position;
    ModelRenderOpt renderOpt;
};



class Table
{
public :
    // general
	TableInfo info;
    TableLoadingScreen loadingScreen;
    TablePlayfield playfield;
    TableCabinet cabinet;
    TableTranslite translite;

    // resources
    uint32_t nbLibraries;
    std::string * libraries;

    uint32_t nbImages;
    ImageResource * images;
    uint32_t nbSounds;
    SoundResource * sounds;
    uint32_t nbMusics;
    MusicResource * musics;
    uint32_t nbModels;
    ModelResource * models;
    uint32_t nbScripts;
    ScriptResource * scripts;

    // lists
    uint32_t nbImageLists;
    ImageList * imageLists;

    uint32_t nbLightLists;
    LightList * lightLists;

    // elements
    uint32_t nbElements;
    TableElement * elements;

    Table();
    ~Table();

    void dump(std::ostream &outstream);
    bool save(ops::files::AbsFile &outfile);
    bool load(ops::files::AbsFile &infile);
};


} // namespace ops

#endif // __OPS_TABLE_H_INCLUDED

