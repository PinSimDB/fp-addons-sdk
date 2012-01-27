// std
#include <stdint.h>
#include <string>
#include <fstream>
#include <string.h>

// ops
#include "ops-model.h"
#include "ops-table.h"
#include "ops-files.h"
#include "ops-log.h"

namespace ops
{



StringList::StringList()
{
    nbValues=0;
    values=NULL;
}

StringList::~StringList()
{
    if (values != NULL)
        delete [] values;
}

Table::Table()
{
    nbLibraries=0;
    libraries = NULL;

    nbImages=0;
    images=NULL;
    nbSounds=0;
    sounds=NULL;
    nbMusics=0;
    musics=NULL;
    nbModels=0;
    models=NULL;
    nbScripts=0;
    scripts=NULL;

    nbImageLists=0;
    imageLists=NULL;
    nbLightLists=0;
    lightLists=NULL;
}

Table::~Table()
{
    if (libraries != NULL)
        delete [] libraries;

    if (images != NULL)
        delete [] images;
    if (sounds != NULL)
        delete [] sounds;
    if (musics != NULL)
        delete [] musics;
    if (scripts != NULL)
        delete [] scripts;
    if (models != NULL)
        delete [] models;

    if (imageLists != NULL)
        delete [] imageLists;
    if (lightLists != NULL)
        delete [] lightLists;

    if (elements != NULL)
        delete [] elements;

}

void Table::dump(std::ostream &outstream)
{
    /*
    outstream << "info.name : " << info.name << std::endl;
    outstream << "info.authors : " << info.authors << std::endl;
    outstream << "info.releaseDate : " << info.releaseDate << std::endl;
    outstream << "info.rules : " << info.rules << std::endl;

    outstream << "loadingScreen.picture : " << loadingScreen.picture << std::endl;
    outstream << "loadingScreen.color : " << loadingScreen.color << std::endl;
    */

    outstream << "playfield.width : " << playfield.width << std::endl;
    outstream << "playfield.length : " << playfield.length << std::endl;
    outstream << "playfield.frontGlassHeight : " << playfield.frontGlassHeight << std::endl;
    outstream << "playfield.rearGlassHeight : " << playfield.rearGlassHeight << std::endl;
    outstream << "playfield.slope : " << playfield.slope << std::endl;
    outstream << "playfield.centerLine : " << playfield.centerLine << std::endl;
    outstream << "playfield.flipperLine : " << playfield.flipperLine << std::endl;
    outstream << "playfield.texture : " << playfield.texture << std::endl;
    outstream << "playfield.color : " << playfield.color << std::endl;
    outstream << "playfield.editorDisplayGrid : " << playfield.editorDisplayGrid << std::endl;
    outstream << "playfield.editorGridSize : " << playfield.editorGridSize << std::endl;
    outstream << "playfield.editorDisplayTexture : " << playfield.editorDisplayTexture << std::endl;

    /*
    outstream << "cabinet.texture : " << cabinet.texture << std::endl;
    outstream << "cabinet.woodColor : " << cabinet.woodColor << std::endl;
    outstream << "cabinet.buttonColor : " << cabinet.buttonColor << std::endl;

    outstream << "translite.width : " << translite.width << std::endl;
    outstream << "translite.height : " << translite.height << std::endl;
    outstream << "translite.texture : " << translite.texture << std::endl;
    outstream << "translite.color : " << translite.color << std::endl;
    outstream << "translite.editorDisplayGrid : " << translite.editorDisplayGrid << std::endl;
    outstream << "translite.editorGridSize : " << translite.editorGridSize << std::endl;
    outstream << "translite.editorDisplayTexture : " << translite.editorDisplayTexture << std::endl;
    */

    outstream << "nbLibraries : " << nbLibraries << std::endl;
    for (uint32_t i=0; i<nbLibraries; i++)
    {
        outstream << "\tname : " << libraries[i] << std::endl;
    }

    outstream << "nbImages : " << nbImages << std::endl;
    for (uint32_t i=0; i<nbImages; i++)
    {
        outstream << "\tname : " << images[i].name << std::endl;
        outstream << "\tpath : " << images[i].path << std::endl;
        outstream << "\ttype : " << images[i].type << std::endl;
        outstream << "\ttransparentColor : " << images[i].transparentColor << std::endl;
        outstream << "\ts3tcCompression : " << images[i].s3tcCompression << std::endl;
        outstream << "\tdisableFiltering : " << images[i].disableFiltering << std::endl;
    }

    outstream << "nbSounds : " << nbSounds << std::endl;
    for (uint32_t i=0; i<nbSounds; i++)
    {
        outstream << "\tname : " << sounds[i].name << std::endl;
        outstream << "\tpath : " << sounds[i].path << std::endl;
        outstream << "\ttype : " << sounds[i].type << std::endl;
    }

    outstream << "nbMusics : " << nbMusics << std::endl;
    for (uint32_t i=0; i<nbMusics; i++)
    {
        outstream << "\tname : " << musics[i].name << std::endl;
        outstream << "\tpath : " << musics[i].path << std::endl;
        outstream << "\ttype : " << musics[i].type << std::endl;
    }

    outstream << "nbModels : " << nbModels << std::endl;
    for (uint32_t i=0; i<nbModels; i++)
    {
        outstream << "\tname : " << models[i].name << std::endl;
        outstream << "\tpath : " << models[i].path << std::endl;
        outstream << "\ttype : " << models[i].type << std::endl;
    }

    outstream << "nbScripts : " << nbScripts << std::endl;
    for (uint32_t i=0; i<nbScripts; i++)
    {
        outstream << "\tname : " << scripts[i].name << std::endl;
        outstream << "\tpath : " << scripts[i].path << std::endl;
        outstream << "\ttype : " << scripts[i].type << std::endl;
    }

/*
    outstream << "nbImageLists : " << nbImageLists << std::endl;
    for (uint32_t i=0; i<nbImageLists; i++)
    {
        outstream << "\tname : " << imageLists[i].name << std::endl;
        for (uint32_t j=0; j<imageLists[i].nbValues; j++)
        {
            outstream << "\t\t" << imageLists[i].values[j] << std::endl;

        }
    }

    outstream << "nbLightLists : " << nbLightLists << std::endl;
    for (uint32_t i=0; i<nbLightLists; i++)
    {
        outstream << "\tname : " << lightLists[i].name << std::endl;
        for (uint32_t j=0; j<lightLists[i].nbValues; j++)
        {
            outstream << "\t\t" << lightLists[i].values[j] << std::endl;

        }
    }
*/

    outstream << "nbElements : " << nbElements << std::endl;
    for (uint32_t i=0; i<nbElements; i++)
    {
        outstream << "\tname : " << elements[i].name << std::endl;
        outstream << "\ttype : " << elements[i].type << std::endl;
        outstream << "\tlocked : " << elements[i].locked << std::endl;
        outstream << "\tlayer : " << elements[i].layer << std::endl;

        outstream << "\tposition.x : " << elements[i].position.x << std::endl;
        outstream << "\tposition.y : " << elements[i].position.y << std::endl;
        outstream << "\tposition.rotation : " << elements[i].position.rotation << std::endl;
        outstream << "\tposition.surface : " << elements[i].position.surface << std::endl;
        outstream << "\tposition.offset : " << elements[i].position.offset << std::endl;

        outstream << "\trenderOpt.model : " << elements[i].renderOpt.model << std::endl;
        outstream << "\trenderOpt.color : " << elements[i].renderOpt.color << std::endl;
        outstream << "\trenderOpt.texture : " << elements[i].renderOpt.texture << std::endl;
        outstream << "\trenderOpt.spheremap : " << elements[i].renderOpt.spheremap << std::endl;
        outstream << "\trenderOpt.ornamental : " << elements[i].renderOpt.ornamental << std::endl;
        outstream << "\trenderOpt.reflects_off_playfield : " << elements[i].renderOpt.reflects_off_playfield << std::endl;
        outstream << "\trenderOpt.playfield : " << elements[i].renderOpt.playfield << std::endl;
    }


}

bool Table::save(ops::files::AbsFile &outfile)
{
    if (!outfile.is_open())
    {
        ERROR << "can't open file for writing : " << outfile.toString();
        return false;
    }

    // header
    outfile.write(TABLE_HEADER_ID, 4);
    outfile.write(&TABLE_HEADER_VERSION, sizeof(TABLE_HEADER_VERSION));


    // general
	//TableInfo info;
    // TODO
    //TableLoadingScreen loadingScreen;
    // TODO

    //TablePlayfield playfield;
    outfile.writeFloat(playfield.width);
    outfile.writeFloat(playfield.length);
    outfile.writeFloat(playfield.frontGlassHeight);
    outfile.writeFloat(playfield.rearGlassHeight);
    outfile.writeFloat(playfield.slope);
    outfile.writeFloat(playfield.centerLine);
    outfile.writeFloat(playfield.flipperLine);
    outfile.writeString(playfield.texture);
    outfile.writeInt(playfield.color);
    outfile.writeInt(playfield.editorDisplayGrid);
    outfile.writeFloat(playfield.editorGridSize);
    outfile.writeInt(playfield.editorDisplayTexture);

    //TableCabinet cabinet;
    // TODO

    //TableTranslite translite;
    // TODO


    // resources
    outfile.writeInt(nbLibraries);
    for (uint32_t i=0; i<nbLibraries; i++)
    {
        outfile.writeString(libraries[i]);
    }

    outfile.writeInt(nbImages);
    for (uint32_t i=0; i<nbImages; i++)
    {
        outfile.writeString(images[i].name);
        outfile.writeString(images[i].path);
        outfile.writeInt(images[i].type);
        outfile.writeInt(images[i].transparentColor);
        outfile.writeInt(images[i].s3tcCompression);
        outfile.writeInt(images[i].disableFiltering);
    }

    outfile.writeInt(nbSounds);
    for (uint32_t i=0; i<nbSounds; i++)
    {
        outfile.writeString(sounds[i].name);
        outfile.writeString(sounds[i].path);
        outfile.writeInt(sounds[i].type);
    }

    outfile.writeInt(nbMusics);
    for (uint32_t i=0; i<nbMusics; i++)
    {
        outfile.writeString(musics[i].name);
        outfile.writeString(musics[i].path);
        outfile.writeInt(musics[i].type);
    }

    outfile.writeInt(nbModels);
    for (uint32_t i=0; i<nbModels; i++)
    {
        outfile.writeString(models[i].name);
        outfile.writeString(models[i].path);
        outfile.writeInt(models[i].type);
    }

    outfile.writeInt(nbScripts);
    for (uint32_t i=0; i<nbScripts; i++)
    {
        outfile.writeString(scripts[i].name);
        outfile.writeString(scripts[i].path);
        outfile.writeInt(scripts[i].type);
    }

/*
    // TODO
    outstream << "nbImageLists : " << nbImageLists << std::endl;
    for (uint32_t i=0; i<nbImageLists; i++)
    {
        outstream << "\tname : " << imageLists[i].name << std::endl;
        for (uint32_t j=0; j<imageLists[i].nbValues; j++)
        {
            outstream << "\t\t" << imageLists[i].values[j] << std::endl;

        }
    }

    outstream << "nbLightLists : " << nbLightLists << std::endl;
    for (uint32_t i=0; i<nbLightLists; i++)
    {
        outstream << "\tname : " << lightLists[i].name << std::endl;
        for (uint32_t j=0; j<lightLists[i].nbValues; j++)
        {
            outstream << "\t\t" << lightLists[i].values[j] << std::endl;

        }
    }
*/

    outfile.writeInt(nbElements);
    for (uint32_t i=0; i<nbElements; i++)
    {
        outfile.writeString(elements[i].name);
        outfile.writeInt(elements[i].type);
        outfile.writeInt(elements[i].locked);
        outfile.writeInt(elements[i].layer);

        outfile.writeFloat(elements[i].position.x);
        outfile.writeFloat(elements[i].position.y);
        outfile.writeFloat(elements[i].position.rotation);
        outfile.writeString(elements[i].position.surface);
        outfile.writeFloat(elements[i].position.offset);

        outfile.writeString(elements[i].renderOpt.model);
        outfile.writeInt(elements[i].renderOpt.color);
        outfile.writeString(elements[i].renderOpt.texture);
        outfile.writeString(elements[i].renderOpt.spheremap);
        outfile.writeInt(elements[i].renderOpt.ornamental);
        outfile.writeInt(elements[i].renderOpt.reflects_off_playfield);
        outfile.writeString(elements[i].renderOpt.playfield);
    }


    return true;
}

bool Table::load(ops::files::AbsFile &infile)
{

    // open file
    if (!infile.is_open())
    {
        ERROR << "can't open file : "  << infile.toString();
        return false;
    }

    // header
    char id[4];
    infile.read(id, 4);
	if (strncmp(id, ops::TABLE_HEADER_ID, 4) != 0)
	{
        ERROR << " file : " << infile.toString() << " is not a valid table";
        return false;
	}
    uint32_t version = 0;
    infile.readInt(&version);
    // TODO : test version



    // general

	//TableInfo info;
    // TODO
    //TableLoadingScreen loadingScreen;
    // TODO

    //TablePlayfield playfield;
    infile.readFloat(&playfield.width);
    infile.readFloat(&playfield.length);
    infile.readFloat(&playfield.frontGlassHeight);
    infile.readFloat(&playfield.rearGlassHeight);
    infile.readFloat(&playfield.slope);
    infile.readFloat(&playfield.centerLine);
    infile.readFloat(&playfield.flipperLine);

    infile.readString(&playfield.texture);
    infile.readInt(&playfield.color);

    infile.readInt(&playfield.editorDisplayGrid);
    infile.readFloat(&playfield.editorGridSize);
    infile.readInt(&playfield.editorDisplayTexture);

    //TableCabinet cabinet;
    // TODO

    //TableTranslite translite;
    // TODO

    // resources
    infile.readInt(&nbLibraries);
    libraries = new std::string[nbLibraries];
    for (uint32_t i=0; i<nbLibraries; i++)
    {
        infile.readString(&libraries[i]);
    }


    infile.readInt(&nbImages);
    images = new ImageResource[nbImages];
    for (uint32_t i=0; i<nbImages; i++)
    {
        infile.readString(&images[i].name);
        infile.readString(&images[i].path);
        infile.readInt(&images[i].type);
        infile.readInt(&images[i].transparentColor);
        infile.readInt(&images[i].s3tcCompression);
        infile.readInt(&images[i].disableFiltering);
    }

    infile.readInt(&nbSounds);
    sounds = new SoundResource[nbSounds];
    for (uint32_t i=0; i<nbSounds; i++)
    {
        infile.readString(&sounds[i].name);
        infile.readString(&sounds[i].path);
        infile.readInt(&sounds[i].type);
    }

    infile.readInt(&nbMusics);
    musics = new MusicResource[nbMusics];
    for (uint32_t i=0; i<nbMusics; i++)
    {
        infile.readString(&musics[i].name);
        infile.readString(&musics[i].path);
        infile.readInt(&musics[i].type);
    }

    infile.readInt(&nbModels);
    models = new ModelResource[nbModels];
    for (uint32_t i=0; i<nbModels; i++)
    {
        infile.readString(&models[i].name);
        infile.readString(&models[i].path);
        infile.readInt(&models[i].type);
    }

    infile.readInt(&nbScripts);
    scripts = new ScriptResource[nbScripts];
    for (uint32_t i=0; i<nbScripts; i++)
    {
        infile.readString(&scripts[i].name);
        infile.readString(&scripts[i].path);
        infile.readInt(&scripts[i].type);
    }


/*

    // lists
    uint32_t nbImageLists;
    ImageList * imageLists;

    uint32_t nbLightLists;
    LightList * lightLists;
    */


    infile.readInt(&nbElements);
    elements = new TableElement[nbElements];
    for (uint32_t i=0; i<nbElements; i++)
    {
        infile.readString(&elements[i].name);
        infile.readInt(&elements[i].type);
        infile.readInt(&elements[i].locked);
        infile.readInt(&elements[i].layer);

        infile.readFloat(&elements[i].position.x);
        infile.readFloat(&elements[i].position.y);
        infile.readFloat(&elements[i].position.rotation);
        infile.readString(&elements[i].position.surface);
        infile.readFloat(&elements[i].position.offset);

        infile.readString(&elements[i].renderOpt.model);
        infile.readInt(&elements[i].renderOpt.color);
        infile.readString(&elements[i].renderOpt.texture);
        infile.readString(&elements[i].renderOpt.spheremap);
        infile.readInt(&elements[i].renderOpt.ornamental);
        infile.readInt(&elements[i].renderOpt.reflects_off_playfield);
        infile.readString(&elements[i].renderOpt.playfield);
    }


    return false;
}



} // namespace ops
