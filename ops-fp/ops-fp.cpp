// std
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <ios>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <string.h>

// ops-fp
#include "ops-fp.h"
#include "ops-fp-defs.h"

// ops
#include "ops-log.h"
#include "ops-tools.h"
#include "ops-lzo.h"


namespace ops
{
namespace fp
{

std::string bin_to_hex( const void *inp, unsigned int len )
{
    const char *input = (const char *) inp;
    static const char* const lut = "0123456789ABCDEF";
    std::string output;
    output.reserve(3 * len);
    for (size_t i = 0; i < len; ++i)
    {
        const unsigned char c = input[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
        output.push_back(' ');
    }
    return output;
}


unsigned int reverse_bytes( const unsigned int v )
{
  unsigned char *p = (unsigned char *)&v;
  unsigned int ret = (p[0]<<0) | (p[1]<<8) | (p[2]<<16) | (p[3]<<24);
  return ret;
}

	// TODO : use a map for faster access
	//http://objectmix.com/c/350233-initializing-map.html


	ChunkGeneric::ChunkGeneric(const ChunkDescriptor & initDescriptor) {
		descriptor = initDescriptor;
		originalLen = 0;
		originalChunk = 0;
		len = 0;
	}
	ChunkGeneric::ChunkGeneric(uint32_t initOriginalLen, uint32_t initOriginalChunk, const ChunkDescriptor & initDescriptor) {
		descriptor = initDescriptor;
		originalLen = initOriginalLen;
		originalChunk = initOriginalChunk;
		len=originalLen;
	}


	ChunkChunkList::ChunkChunkList(const ChunkDescriptor & initDescriptor) {
		descriptor = initDescriptor;
		originalLen = 0;
		originalChunk = 0;
		len = 0;
		parent = NULL;
	}
	ChunkChunkList::ChunkChunkList(uint32_t initOriginalLen, uint32_t initOriginalChunk, const ChunkDescriptor & initDescriptor, const std::vector<ChunkGeneric *> & initValue) {
		descriptor = initDescriptor;
		originalLen = initOriginalLen;
		originalChunk = initOriginalChunk;
		len=originalLen;
		value = initValue;
		parent = NULL;
	}
	ChunkChunkList::~ChunkChunkList() {
    // 08/05/12 SK1: typesafe delete of chunk objects
    uint32_t size = value.size();
    for (uint32_t i=0; i<size; i++)
    {
      ChunkGeneric *chunk = value[i];
      switch( chunk->descriptor.type )
      {
        case T_CHUNK_CHUNKLIST :
                            delete (ChunkChunkList*) chunk;
                            break;
        case T_CHUNK_GENERIC :
                            delete (ChunkGeneric*) chunk;
                            break;
        case T_CHUNK_COLLISIONDATA :
        case T_CHUNK_RAWDATA :
                            delete (ChunkRawData*) chunk;
                            break;
        case T_CHUNK_SCRIPT :
                            delete (ChunkScript*) chunk;
                            break;
        case T_CHUNK_INT :
                            delete (ChunkInt*) chunk;
                            break;
        case T_CHUNK_COLOR :
                            delete (ChunkColor*) chunk;
                            break;
        case T_CHUNK_VALUELIST :
                            delete (ChunkValueList*) chunk;

                            break;
        case T_CHUNK_FLOAT :
                            delete (ChunkFloat*) chunk;
                            break;
        case T_CHUNK_VECTOR2D :
                            delete (ChunkVector2D*) chunk;
                            break;
        case T_CHUNK_STRING :
                            delete (ChunkString*) chunk;
                            break;
        case T_CHUNK_WSTRING :
                            delete (ChunkWString*) chunk;
                            break;
        case T_CHUNK_STRINGLIST :
                            {
                              ChunkStringList* list = (ChunkStringList*)chunk;
                              list->value.clear();
                            }
                            delete (ChunkStringList*) chunk;
                            break;
        default:
                            delete chunk;
                            break;
      }
    }

    value.clear();
	}

	void ChunkChunkList::add(ChunkGeneric * child) {
		value.push_back(child);
	}


	ChunkRawData::ChunkRawData(const ChunkDescriptor & initDescriptor) {
		descriptor = initDescriptor;
		originalLen = 0;
		originalChunk = 0;
		len = 0;
	}
	ChunkRawData::ChunkRawData(uint32_t initOriginalLen, uint32_t initOriginalChunk, const ChunkDescriptor & initDescriptor, const ops::RawData & initValue) {
		descriptor = initDescriptor;

		originalLen = initOriginalLen;
		originalChunk = initOriginalChunk;
		len=originalLen;

		value = initValue;
	}

	ChunkScript::ChunkScript(const ChunkDescriptor & initDescriptor) {
		descriptor = initDescriptor;
		originalLen = 0;
		originalChunk = 0;
		len = 0;
	}
	ChunkScript::ChunkScript(uint32_t initOriginalLen, uint32_t initOriginalChunk, const ChunkDescriptor & initDescriptor, const ops::RawData & initValue) {
		descriptor = initDescriptor;

		originalLen = initOriginalLen;
		originalChunk = initOriginalChunk;
		len=originalLen;

		value = initValue;
   // Decompress
   ops::RawData *rd = FPBaseHandler::getRawData(initValue.data+4, len-4, true);
   script = std::string( (char *)rd->data, rd->len );
   delete rd;
	}


	ChunkString::ChunkString(const ChunkDescriptor & initDescriptor) {
		descriptor = initDescriptor;
		originalLen = 0;
		originalChunk = 0;
		len = 0;
	}
	ChunkString::ChunkString(uint32_t initOriginalLen, uint32_t initOriginalChunk, const ChunkDescriptor & initDescriptor, const std::string & initValue) {
		descriptor = initDescriptor;

		originalLen = initOriginalLen;
		originalChunk = initOriginalChunk;
		len=originalLen;

		value = initValue;
	}



	ChunkWString::ChunkWString(const ChunkDescriptor & initDescriptor) {
		descriptor = initDescriptor;
		originalLen = 0;
		originalChunk = 0;
		len = 0;
	}
	ChunkWString::ChunkWString(uint32_t initOriginalLen, uint32_t initOriginalChunk, const ChunkDescriptor & initDescriptor, const std::wstring & initValue) {
		descriptor = initDescriptor;
		originalLen = initOriginalLen;
		originalChunk = initOriginalChunk;
		len=originalLen;
		value = initValue;
	}


	ChunkInt::ChunkInt(const ChunkDescriptor & initDescriptor) {
		descriptor = initDescriptor;
		originalLen = 0;
		originalChunk = 0;
		len = 0;
		value = 0;
	}
	ChunkInt::ChunkInt(uint32_t initOriginalLen, uint32_t initOriginalChunk, const ChunkDescriptor & initDescriptor, const int32_t & initValue) {
		descriptor = initDescriptor;

		originalLen = initOriginalLen;
		originalChunk = initOriginalChunk;
		len=originalLen;

		value = initValue;
	}


	ChunkFloat::ChunkFloat(const ChunkDescriptor & initDescriptor) {
		descriptor = initDescriptor;
		originalLen = 0;
		originalChunk = 0;
		len = 0;
		value = 0.0f;
	}
	ChunkFloat::ChunkFloat(uint32_t initOriginalLen, uint32_t initOriginalChunk, const ChunkDescriptor & initDescriptor, const float & initValue) {
		descriptor = initDescriptor;

		originalLen = initOriginalLen;
		originalChunk = initOriginalChunk;
		len=originalLen;

		value = initValue;
	}


	ChunkColor::ChunkColor(const ChunkDescriptor & initDescriptor) {
		descriptor = initDescriptor;
		originalLen = 0;
		originalChunk = 0;
		len = 0;
		value = 0;
	}
	ChunkColor::ChunkColor(uint32_t initOriginalLen, uint32_t initOriginalChunk, const ChunkDescriptor & initDescriptor, const int32_t & initValue) {
		descriptor = initDescriptor;
		originalLen = initOriginalLen;
		originalChunk = initOriginalChunk;
		len=originalLen;
		value = initValue;
	}


	ChunkVector2D::ChunkVector2D(const ChunkDescriptor & initDescriptor) {
		descriptor = initDescriptor;
		originalLen = 0;
		originalChunk = 0;
		len = 0;
	}
	ChunkVector2D::ChunkVector2D(uint32_t initOriginalLen, uint32_t initOriginalChunk, const ChunkDescriptor & initDescriptor, const Vector2D & initValue) {
		descriptor = initDescriptor;
		originalLen = initOriginalLen;
		originalChunk = initOriginalChunk;
		len=originalLen;
		value = initValue;
	}


	ChunkStringList::ChunkStringList(const ChunkDescriptor & initDescriptor) {
		descriptor = initDescriptor;
		originalLen = 0;
		originalChunk = 0;
		len = 0;
	}
	ChunkStringList::ChunkStringList(uint32_t initOriginalLen, uint32_t initOriginalChunk, const ChunkDescriptor & initDescriptor, const std::vector<std::string> & initValue) {
		descriptor = initDescriptor;
		originalLen = initOriginalLen;
		originalChunk = initOriginalChunk;
		len=originalLen;
		value = initValue;
	}


	ChunkValueList::ChunkValueList(const ChunkDescriptor & initDescriptor) {
		descriptor = initDescriptor;
		originalLen = 0;
		originalChunk = 0;
		len = 0;
		value = 0;
	}
	ChunkValueList::ChunkValueList(uint32_t initOriginalLen, uint32_t initOriginalChunk, const ChunkDescriptor & initDescriptor, const int32_t & initValue) {
		descriptor = initDescriptor;
		originalLen = initOriginalLen;
		originalChunk = initOriginalChunk;
		len=originalLen;
		value = initValue;
	}


	ChunkCollisionData::ChunkCollisionData(const ChunkDescriptor & initDescriptor) {
		descriptor = initDescriptor;
		originalLen = 0;
		originalChunk = 0;
		len = 0;
	}
	ChunkCollisionData::ChunkCollisionData(uint32_t initOriginalLen, uint32_t initOriginalChunk, const ChunkDescriptor & initDescriptor, const std::vector<FPModelCollisionData> & initValue) {
		descriptor = initDescriptor;
		originalLen = initOriginalLen;
		originalChunk = initOriginalChunk;
		len=originalLen;
		value = initValue;
	}


	FPModelCollisionData::FPModelCollisionData() :
		type(0), generateHit(0), effectBall(0), eventID(0),
		x(0.0f), y(0.0f), z(0.0f),
		value1(0.0f), value2(0.0f), value3(0.0f), value4(0.0f)
	{
	}

	Vector2D::Vector2D(float initX, float initY) {
		x = initX;
		y = initY;
	}


	FPModel::FPModel() :
		linked(0), type(1),	materialType(0), specialValue(0.0f), secondaryModelZDistance(750), collisionPerPolygon(0)
	{
	}
	FPModel::~FPModel() {
	}




	ops::RawData * FPBaseHandler::getRawData(const uint8_t * data, uint32_t len, bool uncompress) {

		if ( uncompress && data[0] == 'z' && data[1] == 'L' && data[2] == 'Z' && data[3] == 'O') {
			// Decompress if 'zLZO'
			ops::RawData * value = new ops::RawData();
            uint32_t rawLen;
			value->len =  *((uint32_t *) &data[4]);
			value->data = new uint8_t[value->len];
            // TODO cast, int
			int r = lzo1x_decompress( (const unsigned char *) &data[8], len - 8, (unsigned char *)value->data, (lzo_uint*)&rawLen, NULL);
			if (r != LZO_E_OK || rawLen != value->len) {
                // this should NEVER happen
				ERROR << " internal error - decompression failed : " << r;
				delete [] value->data;
				value->len = 0;
				value->data = NULL;
			}
			return value;

        } else {
            // Plain copy
			return new ops::RawData(len, data);
        }

    }


	ops::RawData * FPBaseHandler::compress(ops::RawData * data) {
		ops::RawData * result = new ops::RawData();
		// TODO : free not used memory ?
		result->data = new uint8_t[data->len * 2];
		result->len = 0;
		int r = lzo1x_1_compress(data->data, data->len, result->data, (lzo_uint*)&result->len, wrkmem);
		if (r == LZO_E_OK)
		{
			return result;
		} else {
			return NULL;
		}
	}




const ChunkDescriptor * FPBaseHandler::findChunkDescriptor(const std::vector<ChunkDescriptor> & chunkList, uint32_t chunk) {
	for (uint32_t i=0; i<chunkList.size(); i++) {
		if (chunk == chunkList[i].chunk) {
			return &(chunkList[i]);
		}
	}
	return NULL;
}


const std::string * FPBaseHandler::findValue(const std::vector<VLDescriptor> & valueList, int32_t code) {
	for (uint32_t i=0; i<valueList.size(); i++) {
		if (code == valueList[i].code) {
			return &valueList[i].label;
		}
	}
	return new std::string("unknown");
}


void FPBaseHandler::dumpChunks(std::ostream &outstream, ChunkGeneric * chunk, uint32_t tabs, bool showChunk, bool showType, int32_t rawDataLimit) {
	std::ostringstream out;
	for (uint32_t j=0; j<tabs; j++) {
		out << "\t";
	}
	if (showChunk) {
		out << "[0x" << std::setw(8) << std::right << std::hex << std::uppercase << chunk->descriptor.chunk << "] ";
		out << "[0x" << std::setw(8) << std::right << std::hex << std::uppercase << chunk->originalChunk << "] ";
	}
	switch(chunk->descriptor.type) {
	case T_CHUNK_CHUNKLIST :
		if (showType) out << "chunklist ";
		out << chunk->descriptor.label;
		outstream << out.str() << std::endl;
		for (uint32_t i=0; i<((ChunkChunkList*)chunk)->value.size(); i++) {
			dumpChunks(outstream, ((ChunkChunkList*)chunk)->value[i], tabs + 1, showChunk, showType, rawDataLimit);
		}
		return;
	case T_CHUNK_GENERIC :
		if (showType) out << "tag ";
		out << chunk->descriptor.label;
		break;
	case T_CHUNK_COLLISIONDATA :
	case T_CHUNK_RAWDATA :
		{
		if (showType) out << "rawdata ";
		out << chunk->descriptor.label << ".len=" << std::dec << ((ChunkRawData*)chunk)->value.len;
		out << " (0x" << std::setw(8) << std::setfill('0') << std::right << std::hex << std::uppercase << ((ChunkRawData*)chunk)->value.len << ")";
		if (rawDataLimit != 0)
            out << std::endl;
		int32_t limit = ((ChunkRawData*)chunk)->value.len;
		if (rawDataLimit != -1) {
			limit = rawDataLimit;
		}
		if (rawDataLimit != 0)
            ops::tools::dumpHex(out, tabs + 1, ((ChunkRawData*)chunk)->value, limit);
		}
		break;
	case T_CHUNK_SCRIPT :
		if (showType) out << "script ";
		out << chunk->descriptor.label << "=" << ((ChunkScript*)chunk)->script;
		break;
	case T_CHUNK_INT :
		if (showType) out << "int ";
		out << chunk->descriptor.label << "=" << std::dec << ((ChunkInt*)chunk)->value;
		out << " (0x" << std::setw(8) << std::setfill('0') << std::right << std::hex << std::uppercase << ((ChunkInt*)chunk)->value << ")";
		break;
	case T_CHUNK_COLOR :
		if (showType) out << "color ";
		out << chunk->descriptor.label << "=0x" << std::setw(8) << std::setfill('0') << std::right << std::hex << std::uppercase << ((ChunkInt*)chunk)->value;
		break;
	case T_CHUNK_VALUELIST :
		if (showType) out << "valuelist ";
		out << chunk->descriptor.label << "=" << std::dec << ((ChunkInt*)chunk)->value;
		out << " (" << *findValue(*chunk->descriptor.valueList, ((ChunkInt*)chunk)->value)  << ")";
		break;
	case T_CHUNK_FLOAT :
		if (showType) out << "float ";
		out << chunk->descriptor.label << "=" << ((ChunkFloat*)chunk)->value;
		break;
	case T_CHUNK_VECTOR2D :
		out << "vector2d ";
		out << chunk->descriptor.label << "= [" << ((ChunkVector2D*)chunk)->value.x << ", " << ((ChunkVector2D*)chunk)->value.y << "]";
		break;
	case T_CHUNK_STRING :
		if (showType) out << "string ";
		out << chunk->descriptor.label << "=" << ((ChunkString*)chunk)->value;
		break;
	case T_CHUNK_WSTRING :
		{
			if (showType) out << "wstring ";
			std::wstring wide = ((ChunkWString*)chunk)->value;
			out << chunk->descriptor.label << "=" << std::string( wide.begin(), wide.end() );
		}
		break;
	case T_CHUNK_STRINGLIST :
		{
			ChunkStringList* list = (ChunkStringList*)chunk;
			if (showType) out << "stringlist ";
			out << chunk->descriptor.label << std::endl;
			for (uint32_t j=0; j<list->value.size(); j++) {
				for (uint32_t i=0; i<tabs + 1; i++) {
					out << "\t";
				}
				out << list->value[j] << std::endl;
			}
		}
		break;
	default:
		if (showType) out << "unknown ";
		out << chunk->descriptor.label;
		break;
	}
	outstream << out.str() << std::endl;
}


ChunkGeneric * FPBaseHandler::getChunkImpl(ChunkGeneric * chunk, std::vector<std::string> pathList) {
	if (pathList.size() > 1 && chunk->descriptor.type == T_CHUNK_CHUNKLIST && pathList[0] == chunk->descriptor.label) {
		ChunkChunkList * chunkList = (ChunkChunkList *)chunk;
		for (uint32_t i=0; i<chunkList->value.size(); i++) {
			if (chunkList->value[i]->descriptor.label == pathList[1]) {
				pathList.erase(pathList.begin());
				return getChunkImpl(chunkList->value[i], pathList);
			}
		}
		return NULL;
	} else if (pathList.size() == 1 && pathList[0] == chunk->descriptor.label) {
		return chunk;
	} else {
		return NULL;
	}
}


ops::fp::ChunkGeneric * FPBaseHandler::getChunkByLabel(ops::fp::ChunkChunkList * parentList, std::string label)
{
    for (uint32_t i=0; i<parentList->value.size(); i++) {
        ops::fp::ChunkGeneric * current = parentList->value[i];
        if (current->descriptor.label == label)
        {
            return current;
        }
	}
	return NULL;
}
ops::fp::ChunkChunkList * FPBaseHandler::getChunksByLabel(ops::fp::ChunkChunkList * parentList, std::string label)
{
    ops::fp::ChunkChunkList * chunks = new ops::fp::ChunkChunkList();
    for (uint32_t i=0; i<parentList->value.size(); i++) {
        ops::fp::ChunkGeneric * current = parentList->value[i];
        if (current->descriptor.label == label)
        {
            chunks->add(current);
        }
	}
	return chunks;
}


ChunkGeneric * FPBaseHandler::getChunk(ChunkGeneric * baseChunk, std::string path) {
	return getChunkImpl(baseChunk, ops::tools::split(path, '.'));
}



ChunkGeneric * FPBaseHandler::analyseChunk (ChunkChunkList ** result, const std::vector<ChunkDescriptor> & chunkList, uint32_t chunk, uint32_t originalChunk, uint32_t sectionLen, const uint8_t * data, uint32_t retry) {
	ChunkGeneric * chunkBlock = NULL;

	DEBUG << "\tanalyse chunk [0x" << std::setw(8) << std::right << std::hex << std::uppercase << chunk << "] " << "[0x" << std::setw(8) << std::right << std::hex << std::uppercase << originalChunk << "] ";


	// Search for known chunk
	const ChunkDescriptor * descriptor = findChunkDescriptor(chunkList, chunk);
	if (descriptor != NULL) {
		switch (descriptor->type) {
		case T_CHUNK_CHUNKLIST :
			{
				std::vector<ChunkGeneric *> initValue;
				ChunkChunkList * newChunkList = new ChunkChunkList(sectionLen, originalChunk, *descriptor, initValue);
				chunkBlock = (ChunkGeneric *)newChunkList;
				newChunkList->parent = *result;
				(*result)->add(chunkBlock);
				*result = newChunkList;
				return chunkBlock;
			}
		case T_CHUNK_GENERIC :
			{
				chunkBlock = (ChunkGeneric *)new ChunkGeneric(sectionLen, originalChunk, *descriptor);
				if (descriptor->label == "end") {
					(*result)->add(chunkBlock);
					if ((*result)->parent != NULL) {
						(*result) = (*result)->parent;
					}
					return chunkBlock;
				}
			}
			break;
		case T_CHUNK_INT :
			chunkBlock = (ChunkGeneric *)new ChunkInt(sectionLen, originalChunk, *descriptor, *((uint32_t *)data));
			break;
		case T_CHUNK_COLOR :
			chunkBlock = (ChunkGeneric *)new ChunkColor(sectionLen, originalChunk, *descriptor, *((uint32_t *)data));
			break;
		case T_CHUNK_VALUELIST :
			chunkBlock = (ChunkGeneric *)new ChunkValueList(sectionLen, originalChunk, *descriptor, *((uint32_t *)data));
			break;
		case T_CHUNK_FLOAT :
			chunkBlock = (ChunkGeneric *)new ChunkFloat(sectionLen, originalChunk, *descriptor, *((float *)data));
			break;
		case T_CHUNK_VECTOR2D :
			chunkBlock = (ChunkGeneric *)new ChunkVector2D(sectionLen, originalChunk, *descriptor, Vector2D(*((float *)data), *((float *)(data+4))) );
			break;
		case T_CHUNK_STRING :
			chunkBlock = (ChunkGeneric *)new ChunkString(sectionLen, originalChunk, *descriptor, std::string((const char*)(data+4), *((uint32_t*)data)) );
			break;
		case T_CHUNK_WSTRING :
			{
	 		uint32_t dataLen = *((uint32_t *)data);
			chunkBlock = (ChunkGeneric *)new ChunkWString(sectionLen, originalChunk, *descriptor, std::wstring((const wchar_t*)(data+4), dataLen/sizeof(wchar_t)) );
           break;
           }

		case T_CHUNK_STRINGLIST :
			{
			uint32_t nbItems = *((uint32_t *)data);
			std::vector<std::string> items;
			uint32_t offset = 4;
			for (uint32_t i=0; i< nbItems; i++) {
				uint32_t strLen = *((uint32_t*)(data+offset));
				items.push_back(std::string((const char*)(data+offset+4), strLen) ) ;
				offset+= 4 + strLen;
			}
			chunkBlock = (ChunkGeneric *)new ChunkStringList(sectionLen, originalChunk, *descriptor, items );
			chunkBlock->len = offset;
			}
			break;
		case T_CHUNK_RAWDATA :
		case T_CHUNK_COLLISIONDATA :
     {
     ops::RawData *rd = getRawData(data, sectionLen, false);
			chunkBlock = (ChunkGeneric *)new ChunkRawData(sectionLen, originalChunk, *descriptor, *rd);
     delete rd;
			break;
     }
		case T_CHUNK_SCRIPT :
     {
     int len = *(int *) data + 4;
     ops::RawData *rd = getRawData(data, len, false);
			chunkBlock = (ChunkGeneric *)new ChunkScript(len, originalChunk, *descriptor, *rd);
     delete rd;
			break;
     }
		default:
     {
     ops::RawData *rd = getRawData(data, sectionLen, false);
			chunkBlock = (ChunkGeneric *)new ChunkRawData(sectionLen, originalChunk, *descriptor, *rd);
     delete rd;
			break;
     }
		}
	}
	if (chunkBlock == NULL) {
		// Unkown chunk, try to check for older FP chunk:
		if ( retry == 0) {
			// diff [17,18,19] <-> 12
			return analyseChunk(result, chunkList, chunk - 0x15BDECDB, chunk, sectionLen, data, retry + 1);
		} else {
			// Unknown section
			WARNING << "unkown chunk : " << "0x" << std::setw(8) << std::setfill('0') << std::right << std::hex << std::uppercase << chunk << " ["  << "0x" << originalChunk << "]";
                                                            ops::RawData *rd = getRawData(data, sectionLen, false);
			chunkBlock = (ChunkGeneric *)new ChunkRawData(sectionLen, originalChunk, CHUNK_UNKNOWN, *rd);
                                                            delete rd;
		}
	}

	(*result)->add(chunkBlock);
	return chunkBlock;
}





void FPBaseHandler::analyseRawData(ChunkChunkList * result, const std::vector<ChunkDescriptor> & chunkList, const ops::RawData * rawData) {

	ChunkChunkList * currentChunks = result;
	uint32_t offset = 0;
	while (offset<rawData->len) {
		uint32_t sectionLen = *((uint32_t *)&rawData->data[offset]);
		offset+=4;
		uint32_t sectionChunk = *((uint32_t *)&rawData->data[offset]);
		offset+=4;
		sectionLen-=4;

		ChunkGeneric * chunkBlock = analyseChunk(&currentChunks, chunkList, sectionChunk, sectionChunk, sectionLen, &rawData->data[offset], 0);

		// Get calculated len because of light/image list anomaly ...
		offset += chunkBlock->len;
	}
}





void FPMHandler::analyseChunk2 (ops::fp::FPModel * model, uint32_t chunk, uint32_t originalChunk, uint32_t sectionLen, const uint8_t * data, uint32_t retry) {
	// Search for known chunk
	const ChunkDescriptor * descriptor = findChunkDescriptor(CHUNKS_PINMODEL, chunk);
	if (descriptor != NULL) {
		if (descriptor->offset != -1) {
			switch (descriptor->type) {
			case T_CHUNK_VALUELIST :
			case T_CHUNK_INT :
				*((uint32_t*)(((uint8_t*)model) + descriptor->offset)) = *((uint32_t *)data);
				break;
			case T_CHUNK_FLOAT :
				*((float*)(((uint8_t*)model) + descriptor->offset)) = *((float *)data);
				break;
			case T_CHUNK_STRING :
				*((std::string*)(((uint8_t*)model) + descriptor->offset)) = std::string((const char*)(data+4), *((uint32_t*)data));
				break;
			case T_CHUNK_RAWDATA :
				*((ops::RawData*)(((uint8_t*)model) + descriptor->offset)) = *getRawData(data, sectionLen, true);
				break;
			case T_CHUNK_COLLISIONDATA :
				{
					uint32_t nbShapes = sectionLen / sizeof(FPModelCollisionData);
					for (uint32_t j=0; j<nbShapes; j++) {
                                                (*((std::vector<FPModelCollisionData>*)(((uint8_t*)model) + descriptor->offset)))
								.push_back(*((FPModelCollisionData*)(data+j*sizeof(FPModelCollisionData))));
					}
				}
				break;
			default:
				// TODO
				break;
			}
		}
	} else {
		// Unkown chunk, try to check for older FP chunk:
		if ( retry == 0) {
			// diff [17,18,19] <-> 12
			analyseChunk2(model, chunk - 0x15BDECDB, chunk, sectionLen, data, retry + 1);
		} else {
			// Unknown section
			WARNING << "unkown chunk : " << "0x" << std::setw(8) << std::setfill('0') << std::right << std::hex << std::uppercase << chunk << " [" << originalChunk << "]";
		}
	}

}

void FPMHandler::analyseRawData2(FPModel * model, const ops::RawData * rawData) {
	uint32_t offset = 0;
	while (offset<rawData->len) {
		uint32_t sectionLen = *((uint32_t *)&rawData->data[offset]);
		offset+=4;
		uint32_t sectionChunk = *((uint32_t *)&rawData->data[offset]);
		offset+=4;
		sectionLen-=4;
		analyseChunk2(model, sectionChunk, sectionChunk, sectionLen, &rawData->data[offset], 0);
		offset += sectionLen;
	}
}






FPMHandler::FPMHandler() {
}

FPMHandler::~FPMHandler() {
}



FPMFile * FPMHandler::load(std::string filepath) {

	DEBUG << "loading " + filepath;

    ops::msole::FileReader * reader = new ops::msole::FileReader();
    reader->open(filepath);
	ops::RawData * rawData = reader->readAll("PinModel/ModelData");

	FPMFile *model = new FPMFile();
	model->path = filepath;

	analyseRawData2(&model->model, rawData);

	reader->close();
	delete reader;
	delete rawData;

	DEBUG << filepath + " loaded";

    return model;
}


ChunkGeneric * FPMHandler::flexLoad(std::string filepath) {
	DEBUG << "analysing " + filepath;

	ops::msole::FileReader * reader = new ops::msole::FileReader();
	reader->open(filepath);
	ops::RawData * rawData = reader->readAll("PinModel/ModelData");

	ChunkChunkList * chunks = new ChunkChunkList(CHUNK_PINMODEL);

	analyseRawData(chunks, CHUNKS_PINMODEL, rawData);




	reader->close();
	delete reader;
	delete rawData;

	DEBUG << filepath + " analysed";

	return (ChunkGeneric *)chunks;
}



FPTHandler::FPTHandler() {
}

FPTHandler::~FPTHandler() {
}


std::vector<std::string> reorder(std::vector<std::string> & srcList) {
	std::vector<std::string> destList;

	destList.resize(srcList.size());
	for (uint32_t i=0; i<srcList.size(); i++) {
		std::vector<std::string> cut = ops::tools::split(srcList[i], ' ');
		int number;
		std::istringstream ss( cut[cut.size()-1] );
		ss >> number;
		destList[number-1] = srcList[i];
	}
	return destList;
}


ChunkGeneric * FPTHandler::flexLoad(std::string filepath, bool keepPinModelRaw) {
	DEBUG << "analysing " + filepath;

	ops::msole::FileReader * reader = new ops::msole::FileReader();
	reader->open(filepath);
	std::vector<std::string> * files = reader->list();

	ChunkChunkList * globalChunks = new ChunkChunkList(CHUNK_TABLE);

	std::vector<std::string> images;
	std::vector<std::string> sounds;
	std::vector<std::string> musics;
	std::vector<std::string> dmdfonts;
	std::vector<std::string> pinmodels;
	std::vector<std::string> imagelists;
	std::vector<std::string> lightlists;
	std::vector<std::string> tableelements;

    std::vector<std::string>::iterator it;
    for( it = files->begin(); it != files->end(); ++it )
    {
        std::string filename = *it;
		if (filename.find("Future Pinball/Image ") != std::string::npos) {
			images.push_back(filename);
		} else if (filename.find("Future Pinball/Sound ") != std::string::npos)	{
			sounds.push_back(filename);
		} else if (filename.find("Future Pinball/Music ") != std::string::npos)	{
			musics.push_back(filename);
		} else if (filename.find("Future Pinball/PinModel ") != std::string::npos) {
			pinmodels.push_back(filename);
		} else if (filename.find("Future Pinball/DmdFont ") != std::string::npos) {
			dmdfonts.push_back(filename);
		} else if (filename.find("Future Pinball/ImageList ") != std::string::npos) {
			imagelists.push_back(filename);
		} else if (filename.find("Future Pinball/LightList ") != std::string::npos) {
			lightlists.push_back(filename);
		} else if (filename.find("Future Pinball/Table Element ") != std::string::npos)	{
			tableelements.push_back(filename);
		}
	}

	std::vector<std::string> imagesSorted = reorder(images);
	std::vector<std::string> soundsSorted = reorder(sounds);
	std::vector<std::string> musicsSorted = reorder(musics);
	std::vector<std::string> dmdfontsSorted = reorder(dmdfonts);
	std::vector<std::string> pinmodelsSorted = reorder(pinmodels);
	std::vector<std::string> imagelistsSorted = reorder(imagelists);
	std::vector<std::string> lightlistsSorted = reorder(lightlists);
	std::vector<std::string> tableelementsSorted = reorder(tableelements);


	{
		std::string filename = "Future Pinball/File Version";

		ops::RawData * rawData = reader->readAll(filename);
		ChunkChunkList * chunks = new ChunkChunkList(CHUNK_FILE_VERSION);
		chunks->add((ChunkGeneric*)new ChunkRawData(rawData->len, 0, CHUNK_FILE_VERSION_DATA, *rawData));
		delete rawData;
		chunks->parent = globalChunks;
		globalChunks->add(chunks);
	}
	{
		std::string filename = "Future Pinball/Table MAC";

		ops::RawData * rawData = reader->readAll(filename);
		ChunkChunkList * chunks = new ChunkChunkList(CHUNK_TABLE_MAC);
		chunks->add((ChunkGeneric*)new ChunkRawData(rawData->len, 0, CHUNK_TABLE_MAC_DATA, *rawData));
		delete rawData;
		chunks->parent = globalChunks;
		globalChunks->add(chunks);
	}
	{
		std::string filename = "Future Pinball/Table Data";

		ops::RawData * rawData = reader->readAll(filename);
		ChunkChunkList * chunks = new ChunkChunkList(CHUNK_TABLE_DATA);
		analyseRawData(chunks, CHUNKS_TABLE, rawData);
		delete rawData;
		chunks->parent = globalChunks;
		globalChunks->add(chunks);
	}
	for (uint32_t i=0; i<imagesSorted.size(); i++) {
		std::string & filename = imagesSorted[i];

		ops::RawData * rawData = reader->readAll(filename);
		ChunkChunkList * chunks = new ChunkChunkList(CHUNK_IMAGE);
		analyseRawData(chunks, CHUNKS_RESOURCE, rawData);
		delete rawData;
		chunks->parent = globalChunks;
		globalChunks->add(chunks);
	}
	for (uint32_t i=0; i<soundsSorted.size(); i++) {
		std::string & filename = soundsSorted[i];

		ops::RawData * rawData = reader->readAll(filename);
		ChunkChunkList * chunks = new ChunkChunkList(CHUNK_SOUND);
		analyseRawData(chunks, CHUNKS_RESOURCE, rawData);
		delete rawData;
		chunks->parent = globalChunks;
		globalChunks->add(chunks);
	}
	for (uint32_t i=0; i<musicsSorted.size(); i++) {
		std::string & filename = musicsSorted[i];

		ops::RawData * rawData = reader->readAll(filename);
		ChunkChunkList * chunks = new ChunkChunkList(CHUNK_MUSIC);
		analyseRawData(chunks, CHUNKS_RESOURCE, rawData);
		delete rawData;
		chunks->parent = globalChunks;
		globalChunks->add(chunks);
	}
	for (uint32_t i=0; i<dmdfontsSorted.size(); i++) {
		std::string & filename = dmdfontsSorted[i];

		ops::RawData * rawData = reader->readAll(filename);
		ChunkChunkList * chunks = new ChunkChunkList(CHUNK_DMDFONT);
		analyseRawData(chunks, CHUNKS_RESOURCE, rawData);
		delete rawData;
		chunks->parent = globalChunks;
		globalChunks->add(chunks);
	}
	for (uint32_t i=0; i<pinmodelsSorted.size(); i++) {
		std::string & filename = pinmodelsSorted[i];
		ops::RawData * rawData = reader->readAll(filename);
		ChunkChunkList * chunks = new ChunkChunkList(CHUNK_PINMODEL);
		analyseRawData(chunks, CHUNKS_PINMODEL, rawData);
        chunks->parent = globalChunks;
        if (keepPinModelRaw) {
            ChunkRawData * chunkRaw = new ChunkRawData(0, 0, CHUNK_PINMODEL_RAW, *rawData);
            chunks->add(chunkRaw);
        }
		delete rawData;

		globalChunks->add(chunks);

	}
	for (uint32_t i=0; i<imagelistsSorted.size(); i++) {
		std::string & filename = imagelistsSorted[i];

		ops::RawData * rawData = reader->readAll(filename);
		ChunkChunkList * chunks = new ChunkChunkList(CHUNK_IMAGELIST);
		analyseRawData(chunks, CHUNKS_LIST, rawData);
		delete rawData;
		chunks->parent = globalChunks;
		globalChunks->add(chunks);

	}
	for (uint32_t i=0; i<lightlistsSorted.size(); i++) {
		std::string & filename = lightlistsSorted[i];

		ops::RawData * rawData = reader->readAll(filename);
		ChunkChunkList * chunks = new ChunkChunkList(CHUNK_LIGHTLIST);
		analyseRawData(chunks, CHUNKS_LIST, rawData);
		delete rawData;
		chunks->parent = globalChunks;
		globalChunks->add(chunks);

	}
	for (uint32_t i=0; i<tableelementsSorted.size(); i++) {
		std::string & filename = tableelementsSorted[i];

		ops::RawData * rawData = reader->readAll(filename);
		ChunkChunkList * chunks = new ChunkChunkList(CHUNK_TABLE_ELEMENT);
		// Table Element anomaly ...
		chunks->add((ChunkGeneric*)new ChunkInt(4, 0, CHUNK_ELEMENT_TYPE, *((uint32_t*)rawData->data)));
		// remove 1st int
		ops::RawData * validRawData = new ops::RawData(rawData->len-4, rawData->data+4);
		switch (*((uint32_t*)rawData->data)) {
		case 2 :
			analyseRawData(chunks, CHUNKS_ELEMENT_2, validRawData);
			break;
		case 3 :
			analyseRawData(chunks, CHUNKS_ELEMENT_3, validRawData);
			break;
		case 4 :
			analyseRawData(chunks, CHUNKS_ELEMENT_4, validRawData);
			break;
		case 6 :
			analyseRawData(chunks, CHUNKS_ELEMENT_6, validRawData);
			break;
		case 7 :
			analyseRawData(chunks, CHUNKS_ELEMENT_7, validRawData);
			break;
		case 8 :
			analyseRawData(chunks, CHUNKS_ELEMENT_8, validRawData);
			break;
		case 10 :
			analyseRawData(chunks, CHUNKS_ELEMENT_10, validRawData);
			break;
		case 11 :
			analyseRawData(chunks, CHUNKS_ELEMENT_11, validRawData);
			break;
		case 12 :
			analyseRawData(chunks, CHUNKS_ELEMENT_12, validRawData);
			break;
		case 13 :
			analyseRawData(chunks, CHUNKS_ELEMENT_13, validRawData);
			break;
		case 14 :
			analyseRawData(chunks, CHUNKS_ELEMENT_14, validRawData);
			break;
		case 15 :
			analyseRawData(chunks, CHUNKS_ELEMENT_15, validRawData);
			break;
		case 16 :
			analyseRawData(chunks, CHUNKS_ELEMENT_16, validRawData);
			break;
		case 17 :
			analyseRawData(chunks, CHUNKS_ELEMENT_17, validRawData);
			break;
		case 18 :
			analyseRawData(chunks, CHUNKS_ELEMENT_18, validRawData);
			break;
		case 19 :
			analyseRawData(chunks, CHUNKS_ELEMENT_19, validRawData);
			break;
		case 20 :
			analyseRawData(chunks, CHUNKS_ELEMENT_20, validRawData);
			break;
		case 21 :
			analyseRawData(chunks, CHUNKS_ELEMENT_21, validRawData);
			break;
		case 22 :
			analyseRawData(chunks, CHUNKS_ELEMENT_22, validRawData);
			break;
		case 23 :
			analyseRawData(chunks, CHUNKS_ELEMENT_23, validRawData);
			break;
		case 24 :
			analyseRawData(chunks, CHUNKS_ELEMENT_24, validRawData);
			break;
		case 25 :
			analyseRawData(chunks, CHUNKS_ELEMENT_25, validRawData);
			break;
		case 26 :
			analyseRawData(chunks, CHUNKS_ELEMENT_26, validRawData);
			break;
		case 27 :
			analyseRawData(chunks, CHUNKS_ELEMENT_27, validRawData);
			break;
		case 29 :
			analyseRawData(chunks, CHUNKS_ELEMENT_29, validRawData);
			break;
		case 30 :
			analyseRawData(chunks, CHUNKS_ELEMENT_30, validRawData);
			break;
		case 33 :
			analyseRawData(chunks, CHUNKS_ELEMENT_33, validRawData);
			break;
		case 31 :
			analyseRawData(chunks, CHUNKS_ELEMENT_31, validRawData);
			break;
		case 34 :
			analyseRawData(chunks, CHUNKS_ELEMENT_34, validRawData);
			break;
		case 37 :
			analyseRawData(chunks, CHUNKS_ELEMENT_37, validRawData);
			break;
		case 38 :
			analyseRawData(chunks, CHUNKS_ELEMENT_38, validRawData);
			break;
		case 39 :
			analyseRawData(chunks, CHUNKS_ELEMENT_39, validRawData);
			break;
		case 40 :
			analyseRawData(chunks, CHUNKS_ELEMENT_40, validRawData);
			break;
		case 43 :
			analyseRawData(chunks, CHUNKS_ELEMENT_43, validRawData);
			break;
		case 44 :
			analyseRawData(chunks, CHUNKS_ELEMENT_44, validRawData);
			break;
		case 46 :
			analyseRawData(chunks, CHUNKS_ELEMENT_46, validRawData);
			break;
		case 49 :
			analyseRawData(chunks, CHUNKS_ELEMENT_49, validRawData);
			break;
		case 50 :
			analyseRawData(chunks, CHUNKS_ELEMENT_50, validRawData);
			break;
		case 51 :
			analyseRawData(chunks, CHUNKS_ELEMENT_51, validRawData);
			break;
		case 53 :
			analyseRawData(chunks, CHUNKS_ELEMENT_53, validRawData);
			break;
		case 54 :
			analyseRawData(chunks, CHUNKS_ELEMENT_54, validRawData);
			break;
		case 55 :
			analyseRawData(chunks, CHUNKS_ELEMENT_55, validRawData);
			break;
		case 56 :
			analyseRawData(chunks, CHUNKS_ELEMENT_56, validRawData);
			break;
		case 57 :
			analyseRawData(chunks, CHUNKS_ELEMENT_57, validRawData);
			break;
		case 58 :
			analyseRawData(chunks, CHUNKS_ELEMENT_58, validRawData);
			break;
		case 60 :
			analyseRawData(chunks, CHUNKS_ELEMENT_60, validRawData);
			break;
		case 61 :
			analyseRawData(chunks, CHUNKS_ELEMENT_61, validRawData);
			break;
		case 62 :
			analyseRawData(chunks, CHUNKS_ELEMENT_62, validRawData);
			break;
		case 64 :
			analyseRawData(chunks, CHUNKS_ELEMENT_64, validRawData);
			break;
		default:
			chunks->add((ChunkGeneric*)new ChunkRawData(validRawData->len, 0, CHUNK_UNKNOWN_TABLE_ELEMENT, *validRawData));
			break;
		}
		delete rawData;
   delete validRawData; // 08/05/12 Added by SK1
		chunks->parent = globalChunks;
		globalChunks->add(chunks);
	}


	delete files;

	reader->close();
	delete reader;

	DEBUG << filepath + " analysed";

	return (ChunkGeneric *)globalChunks;
}



#ifdef _WIN32
bool FPTHandler::calcMAC( ChunkChunkList *pChunkListP, unsigned char *pszHashBufferP, unsigned long nBufferSizeP )
{
  // Reverse engineering 2013 by SK1
  HCRYPTPROV hProv;
  HCRYPTHASH hHash;
  char *pszFP = "Future Pinball (ADVANCED)";


  if( !CryptAcquireContext( &hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_NEWKEYSET ) )
  {
    return false;
  }


  if( !CryptCreateHash( hProv, CALG_MD2, NULL, NULL, &hHash ) )
  {
    return false;
  }



  DWORD dwHashSize = 0;
  DWORD dwSize = sizeof( dwHashSize );
  CryptGetHashParam(hHash, HP_HASHSIZE, (BYTE *)&dwHashSize, &dwSize, 0 );

  if( nBufferSizeP < dwHashSize )
    return false;


  // Lets go
  CryptHashData( hHash, (unsigned char *) pszFP, lstrlen( pszFP ), 0  );

  // File Version
  ChunkChunkList *pV = (ChunkChunkList *) getChunkByLabel( (ChunkChunkList *)pChunkListP, "file_version" );
  ChunkRawData *pR = (ChunkRawData*) pV->value[0];
  CryptHashData( hHash, pR->value.data, 4, 0 );

  ULONG lFileVersion = *(ULONG*) pR->value.data;

  ChunkDescriptor HashChunksTable[] = { CHUNK_TABLE_DATA, CHUNK_TABLE_ELEMENT };

  if( lFileVersion < 0x1902 )
  {
    for( int c=0; c<sizeof( HashChunksTable ) / sizeof(ChunkDescriptor); c++ )
    {
      ChunkChunkList *pList = getChunksByLabel( pChunkListP, HashChunksTable[c].label );
      if( pList )
      {
        int nSize = pList->value.size();
        for( int i=0; i<nSize; i++ )
        {
          cryptChunkList( (ChunkChunkList *)pList->value[i], hHash );
        }
      }
    }
  }
  else
    return false;

  // Done!
  CryptGetHashParam(hHash, HP_HASHVAL, pszHashBufferP, &dwHashSize, 0 );

  if(hHash)
    CryptDestroyHash(hHash);

  if(hProv)
   CryptReleaseContext(hProv,0);

  return true;
}
#endif

#ifdef _WIN32
void FPTHandler::cryptChunkList( ChunkChunkList *pV, HCRYPTHASH hHash )
{
  // Reverse engineering by SK1
  unsigned int nSize = pV->value.size();
  char szBuffer[200];
  for( unsigned int i=0; i<nSize; i++ )
  {
    ops::fp::ChunkGeneric *pR1 = (ops::fp::ChunkGeneric*) pV->value[i];



    if( pR1->originalChunk != 0x4F5A4C7A &&
        pR1->originalChunk != 0x00 )
    {
      CryptHashData( hHash, (unsigned char *) &pR1->originalChunk, 4, 0 ); // Element type
    }


    switch( pR1->originalChunk )
    {
      case 0:
        break;


      default:
      {
        switch( pR1->descriptor.type )
        {
          case T_CHUNK_CHUNKLIST:
          {
            cryptChunkList( (ChunkChunkList *)pR1, hHash );
            break;
          }

          case T_CHUNK_INT:
          {

            int v = ((ChunkInt*)pR1)->value;                    // 4 Bytes
            CryptHashData( hHash, (unsigned char *) &v, 4, 0 );

            break;
          }

          case T_CHUNK_FLOAT:
          {
            int v = ((ChunkInt*)pR1)->value;                    // 4 Bytes
            CryptHashData( hHash, (unsigned char *) &v, 4, 0 );

            break;
          }

          case T_CHUNK_COLOR:
          {
            int v = ((ChunkInt*)pR1)->value;                    // 4 Bytes
            CryptHashData( hHash, (unsigned char *) &v, 4, 0 );

            break;
          }

          case T_CHUNK_VALUELIST:
          {
            int v = ((ChunkInt*)pR1)->value;                    // 4 Bytes
            CryptHashData( hHash, (unsigned char *) &v, 4, 0 );

            break;
          }


          case T_CHUNK_STRING:
          {
            std::string s = ((ChunkString*)pR1)->value;
            int nLen = s.length();
            CryptHashData( hHash, (unsigned char *) &nLen, 4, 0 );

            //char *pS = s.begin();
            //char *pS = (char *)s.data();
            //CryptHashData( hHash, (unsigned char *) pS, nLen, 0 );

            CryptHashData( hHash, (unsigned char *) s.data(), nLen, 0 );

            break;
          }

          case T_CHUNK_WSTRING:
          {
            std::wstring s = ((ChunkWString*)pR1)->value;
            int nLen = s.length()*sizeof(wchar_t);
            CryptHashData( hHash, (unsigned char *) &nLen, sizeof(nLen), 0 ); // String length

            //char *pS = (char*)s.begin();
            //char *pS = (char *)s.c_str();
            //CryptHashData( hHash, (unsigned char *) pS, nLen, 0 );
            CryptHashData( hHash, (unsigned char *) s.data(), nLen, 0 );

            break;
          }

          case T_CHUNK_SCRIPT:
          {
            ChunkRawData*pR2 = (ChunkRawData*)pR1;
            unsigned int nLen = pR2->value.len-4;
            unsigned char *p = pR2->value.data;
            p += 4;

            if( nLen > 0 )
            {
              CryptHashData( hHash, p, nLen, 0 );
            }


            break;
          }

          case T_CHUNK_RAWDATA:
          {
            switch( pR1->originalChunk )
            {
              case 0x4F5A4C7A: // LZ0 compressed data
              case 0xAAADB6AC:
              {

                ChunkRawData*pR2 = (ChunkRawData*)pR1;
                int nLen = pR2->value.len;
                unsigned char *p = pR2->value.data;

                CryptHashData( hHash, p, nLen, 0 );

                break;
              }

              case 0xBBB1ACBC:
              {
                ChunkRawData*pR2 = (ChunkRawData*)pR1;
                int nLen = pR2->value.len;
                unsigned char *p = pR2->value.data;

                if( nLen > 0 )
                {
                  CryptHashData( hHash, p, 4, 0 );

                  if( nLen > 4 )
                  {
                    CryptHashData( hHash, p+4, nLen-4, 0 );
                  }
                }
                break;
              }

              default:
              {
                ChunkRawData*pR2 = (ChunkRawData*)pR1;
                int nLen = pR2->value.len;
                unsigned char *p = pR2->value.data;

                if( nLen > 0 )
                {
                  CryptHashData( hHash, p, nLen, 0 );
                }

                break;
              }
            }
            break;
          }

          case T_CHUNK_VECTOR2D:
          {
            Vector2D v = ((ChunkVector2D*)pR1)->value;
            CryptHashData( hHash, (unsigned char *)&v, 8, 0 );

            break;
          }

          case T_CHUNK_GENERIC:
          {
            switch( pR1->originalChunk )
            {
              case 0xBABBB0BD:
              case 0xA4FDC3E2:
              {

                break;
              }
            }

            break;
          }
        }
      }
    }
  }
}
#endif


FPLHandler::FPLHandler() {
}

FPLHandler::~FPLHandler() {
}

ChunkGeneric * FPLHandler::flexLoad(std::string filepath) {
	DEBUG << "analysing " + filepath;

	ops::msole::FileReader * reader = new ops::msole::FileReader();
	reader->open(filepath);
	std::vector<std::string> * files = reader->list();

    ChunkChunkList * globalChunks = new ChunkChunkList(CHUNK_LIBRARY);

    std::vector<std::string>::iterator it;
    for( it = files->begin(); it != files->end(); ++it )
    {
        std::string name = *it;

        std::vector<std::string> nameSplitted = ops::tools::split(name, '/');
        if (nameSplitted.size() == 1)
        {
            ChunkChunkList * chunks = new ChunkChunkList(CHUNK_RESOURCE);

            chunks->add((ChunkGeneric*)new ChunkString(0, 0, CHUNK_RESOURCE_NAME, name));

            std::string fileFTYP = name + "/FTYP";
            ops::RawData * rawDataFTYP = reader->readAll(fileFTYP);
            chunks->add((ChunkGeneric*)new ChunkInt(rawDataFTYP->len, 0, CHUNK_RESOURCE_TYPE, *((uint32_t *)rawDataFTYP->data)));
            delete rawDataFTYP;

            std::string fileFPAT = name + "/FPAT";
            ops::RawData * rawDataFPAT = reader->readAll(fileFPAT);
            chunks->add((ChunkGeneric*)new ChunkString(rawDataFPAT->len, 0, CHUNK_RESOURCE_PATH, std::string((const char*)(rawDataFPAT->data), rawDataFPAT->len)));
            delete rawDataFPAT;

            std::string fileFDAT = name + "/FDAT";
            ops::RawData * rawDataFDAT = reader->readAll(fileFDAT);
            chunks->add((ChunkGeneric*)new ChunkRawData(rawDataFDAT->len, 0, CHUNK_RESOURCE_DATA, *rawDataFDAT));
            delete rawDataFDAT;

            std::string fileFLAD = name + "/FLAD";
            ops::RawData * rawDataFLAD = reader->readAll(fileFLAD);
            chunks->add((ChunkGeneric*)new ChunkRawData(rawDataFLAD->len, 0, CHUNK_RESOURCE_FLAD, *rawDataFLAD));
            delete rawDataFLAD;

            globalChunks->add(chunks);
        }
    }

    delete files;
	reader->close();
	delete reader;

	DEBUG << filepath + " analysed";

	return (ChunkGeneric *)globalChunks;
}

} // namespace fp
} // namespace ops


















