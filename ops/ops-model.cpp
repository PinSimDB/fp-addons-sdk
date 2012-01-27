// std
#include <stdint.h>
#include <string>
#include <fstream>
#include <string.h>

// ops
#include "ops-model.h"
#include "ops-files.h"
#include "ops-log.h"

namespace ops
{

char MODEL_HEADER_ID[] = "OPSM";
uint32_t MODEL_HEADER_VERSION = 1;

MeshModel::MeshModel() :  type(0), materialType(0), nbCollisionElements(0), collisionElements(NULL)
{
}

MeshModel::~MeshModel()
{
    if (collisionElements != NULL)
        delete [] collisionElements;
}

void MeshModel::dump(std::ostream &outstream)
{
    // general
    //outstream << "name : " << name << std::endl;
    outstream << "type : " << type << std::endl;
    outstream << "preview.len : " << preview.len << std::endl;
    // rendering
    outstream << "primaryMesh.len : " << primaryMesh.len << std::endl;
    outstream << "lowMesh.len : " << lowMesh.len << std::endl;
    outstream << "maskMesh.len : " << maskMesh.len << std::endl;
    outstream << "reflectionMesh.len : " << reflectionMesh.len << std::endl;
    // physic
    outstream << "materialType : " << materialType << std::endl;
    outstream << "nbCollisionElements : " << nbCollisionElements << std::endl;
    for (uint32_t i=0; i<nbCollisionElements; i++)
    {
        outstream << "\ttype : " << collisionElements[i].type << std::endl;
        outstream << "\tcollide : " << collisionElements[i].collide << std::endl;
        outstream << "\tdetect : " << collisionElements[i].detect << std::endl;
        outstream << "\teventID : " << collisionElements[i].eventID << std::endl;
        outstream << "\tx : " << collisionElements[i].x << std::endl;
        outstream << "\ty : " << collisionElements[i].y << std::endl;
        outstream << "\tz : " << collisionElements[i].z << std::endl;
        outstream << "\tvalue1 : " << collisionElements[i].value1 << std::endl;
        outstream << "\tvalue2 : " << collisionElements[i].value2 << std::endl;
        outstream << "\tvalue3 : " << collisionElements[i].value3 << std::endl;
        outstream << "\tvalue4 : " << collisionElements[i].value4 << std::endl;
    }
}


bool MeshModel::save(ops::files::AbsFile &outfile)
{
    if (!outfile.is_open())
    {
        ERROR << "can't open file for writing : " << outfile.toString();
        return false;
    }

    // header
    outfile.write(MODEL_HEADER_ID, 4);
    outfile.writeInt(MODEL_HEADER_VERSION);

    // general
    //uint32_t nameLen = name.size();
    //outfile.write(&nameLen, sizeof(nameLen));
    //outfile.write(name.c_str(), nameLen);

    outfile.writeInt(type);
    outfile.writeRawData(preview);

    // rendering
    outfile.writeRawData(primaryMesh);
    outfile.writeRawData(lowMesh);
    outfile.writeRawData(maskMesh);
    outfile.writeRawData(reflectionMesh);

    // physic
    outfile.writeInt(materialType);
    outfile.writeInt(nbCollisionElements);
    outfile.write(collisionElements, sizeof(ops::ModelCollisionData) * nbCollisionElements);

    return true;
}


bool MeshModel::load(ops::files::AbsFile &infile)
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
	if (strncmp(id, ops::MODEL_HEADER_ID, 4) != 0)
	{
        ERROR << " file : " << infile.toString() << " is not a valid model";
        return false;
	}
    uint32_t version = 0;

    infile.readInt(&version);

    // TODO : test version

    // general
    //uint32_t nameLen = 0;
    //infile.read(&nameLen, sizeof(nameLen));
    //char * buffer = new char[nameLen+1];
    //infile.read(buffer, nameLen);
    //buffer[nameLen] = 0;
    //std::string nameTmp((const char*)buffer);
    //delete [] buffer;
    //name = nameTmp;

    infile.readInt(&type);
    infile.readRawData(&preview);

    // rendering
    infile.readRawData(&primaryMesh);
    infile.readRawData(&lowMesh);
    infile.readRawData(&maskMesh);
    infile.readRawData(&reflectionMesh);

    // physic
    infile.readInt(&materialType);
    infile.readInt(&nbCollisionElements);
    collisionElements = new ops::ModelCollisionData[nbCollisionElements];
    infile.read(collisionElements, sizeof(ops::ModelCollisionData) * nbCollisionElements);

    return true;
}


} // namespace ops
