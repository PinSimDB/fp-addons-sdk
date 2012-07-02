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

// ops
#include "ops-log.h"
#include "ops-tools.h"
#include "ops-lzo.h"

#include "ops-fp-defs.h"
#include "ops-fp-writer.h"

namespace ops
{
namespace fp
{
void FPMWriter::saveStringChunk( ops::msole::FileWriter * writer, int32_t handler, uint32_t chunk, std::string msg )
{

  // save chunk len
  uint32_t chunkLen =  msg.length() + 4 + 4;
  writer->fileWrite( handler, (uint8_t*)&chunkLen, (uint32_t)sizeof( uint32_t ));

  // save chunk
  writer->fileWrite( handler, (uint8_t*)&chunk, (uint32_t)sizeof( uint32_t ));

  // save nbchar
  uint32_t msgLen =  msg.length();
  writer->fileWrite( handler, (uint8_t*)&msgLen, (uint32_t)sizeof( uint32_t ));

  // save chars
  writer->fileWrite( handler, (uint8_t*)msg.c_str(), (uint32_t)sizeof( char ) * msgLen );
}

void FPMWriter::saveUInt32Chunk( ops::msole::FileWriter * writer, int32_t handler, uint32_t chunk, uint32_t value )
{
  // save chunk len
  uint32_t chunkLen =  4 + 4;
  writer->fileWrite( handler, (uint8_t*)&chunkLen, (uint32_t)sizeof( uint32_t ));

  // save chunk
  writer->fileWrite( handler, (uint8_t*)&chunk, (uint32_t)sizeof( uint32_t ));

  // save value
  writer->fileWrite( handler, (uint8_t*)&value, (uint32_t)sizeof( uint32_t ));
}

void FPMWriter::saveTagChunk( ops::msole::FileWriter * writer, int32_t handler, uint32_t chunk )
{
  // save chunk len
  uint32_t chunkLen =  4;
  writer->fileWrite( handler, (uint8_t*)&chunkLen, (uint32_t)sizeof( uint32_t ));

  // save chunk
  writer->fileWrite( handler, (uint8_t*)&chunk, (uint32_t)sizeof( uint32_t ));

}


// save chunk len
void FPMWriter::saveFloatChunk( ops::msole::FileWriter * writer, int32_t handler, uint32_t chunk, float value )
{
  uint32_t chunkLen =  4 + 4;
  writer->fileWrite( handler, (uint8_t*)&chunkLen, (uint32_t)sizeof( uint32_t ));

  // save chunk
  writer->fileWrite( handler, (uint8_t*)&chunk, (uint32_t)sizeof( uint32_t ));

  // save value
  writer->fileWrite( handler, (uint8_t*)&value, (uint32_t)sizeof( float ));
}





void FPMWriter::saveRawDataChunk( ops::msole::FileWriter * writer, int32_t handler, uint32_t chunk, uint8_t* value, uint32_t len, bool compressed, uint32_t uncompressedLen )
{

  if( compressed )
  {
    // save chunk len
    uint32_t chunkLen =  4 + 4 + 4 + len;
    writer->fileWrite( handler, (uint8_t*)&chunkLen, (uint32_t)sizeof( uint32_t ));

    // save chunk
    writer->fileWrite( handler, (uint8_t*)&chunk, (uint32_t)sizeof( uint32_t ));

    // save zLZO header
    char header [5] = "zLZO";
    writer->fileWrite( handler, (uint8_t*)header, (uint32_t)sizeof( uint32_t ));

    // save uncompressed len
    writer->fileWrite( handler, (uint8_t*)&uncompressedLen, (uint32_t)sizeof( uint32_t ));

    // save compressed bytes
    writer->fileWrite( handler, (uint8_t*)value, (uint32_t)sizeof( uint8_t ) * len );

  }
  else
  {

    // save chunk len
    uint32_t chunkLen =  4 + len;
    writer->fileWrite( handler, (uint8_t*)&chunkLen, (uint32_t)sizeof( uint32_t ));

    // save chunk
    writer->fileWrite( handler, (uint8_t*)&chunk, (uint32_t)sizeof( uint32_t ));

    // save uncompressed bytes
    writer->fileWrite( handler, (uint8_t*)value, (uint32_t)sizeof( uint8_t ) * len );
  }

}




FPMWriter::FPMWriter()
{
}

FPMWriter::~FPMWriter()
{
}

FPMFile * FPMWriter::create( std::string filepath, std::string name )
{

  DEBUG << "creating " + filepath;

  FPMFile *model = new FPMFile();
  model->path = filepath;
  model->model.name = name;

  DEBUG << filepath + " created";

  return model;
}


void FPMWriter::save( std::string filepath, FPMFile * model )
{

  DEBUG << "saving " + filepath;

  ops::msole::FileWriter * writer = new ops::msole::FileWriter();
  writer->open( filepath );

  int32_t handler = writer->fileOpen( "PinModel/ModelData" );

  saveStringChunk( writer, handler, 0xA4F4D1D7, model->model.name );
  std::string id( model->model.name );
  std::transform( id.begin(), id.end(), id.begin(), ( int ( * )( int )) std::tolower );
  saveStringChunk( writer, handler, 0xA4F4C4DC, id );
  saveUInt32Chunk( writer, handler, 0x9EF3C6D9, model->model.linked );

  if( model->model.linked == 0 )
  {
    // not linked

    saveUInt32Chunk( writer, handler, 0xA4F1B9D1, model->model.type );
    saveUInt32Chunk( writer, handler, 0x99E8BED8, model->model.materialType );


    if( model->model.previewPath != "" )
    {
      saveStringChunk( writer, handler, 0x9D00C4DC, model->model.previewPath );
      ops::RawData * compressedData = compress( &model->model.previewData );
      saveUInt32Chunk( writer, handler, 0x8FF8BFDC, compressedData->len + 8 );
      saveRawDataChunk( writer, handler, 0x9600CEDC, compressedData->data, compressedData->len, true, model->model.previewData.len );
      delete compressedData;
    }

    saveUInt32Chunk( writer, handler, 0x9AFEC2D5, model->model.collisionPerPolygon );

    if( model->model.secondaryModelPath != "" )
    {
      saveUInt32Chunk( writer, handler, 0xA5F2C6E0, 1 );
    }
    else
    {
      saveUInt32Chunk( writer, handler, 0xA5F2C6E0, 0 );
    }

    saveUInt32Chunk( writer, handler, 0xA5FDC3D9, model->model.secondaryModelZDistance );
    saveFloatChunk( writer, handler, 0xA8EFCBD3, model->model.specialValue );

    if( model->model.primaryModelPath != "" )
    {
      saveUInt32Chunk( writer, handler, 0xA5F2C5D5, 1 );
      saveStringChunk( writer, handler, 0x9D00C4D5, model->model.primaryModelPath );

      ops::RawData * compressedData = compress( &model->model.primaryModelData );
      saveUInt32Chunk( writer, handler, 0x8FF8BFD5, compressedData->len + 8 );
      saveRawDataChunk( writer, handler, 0x9600CED5, compressedData->data, compressedData->len, true, model->model.primaryModelData.len );
      delete compressedData;
    }

    if( model->model.secondaryModelPath != "" )
    {
      saveUInt32Chunk( writer, handler, 0xA5F2C5D2, 1 );
      saveStringChunk( writer, handler, 0x9D00C4D2, model->model.secondaryModelPath );

      ops::RawData * compressedData = compress( &model->model.secondaryModelData );
      saveUInt32Chunk( writer, handler, 0x8FF8BFD2, compressedData->len + 8 );
      saveRawDataChunk( writer, handler, 0x9600CED2, compressedData->data, compressedData->len, true, model->model.secondaryModelData.len );
      delete compressedData;
    }

    if( model->model.tertiaryModelPath != "" )
    {
      saveUInt32Chunk( writer, handler, 0xA5F2C5D1, 1 );
      saveStringChunk( writer, handler, 0x9D00C4D1, model->model.tertiaryModelPath );

      ops::RawData * compressedData = compress( &model->model.tertiaryModelData );
      saveUInt32Chunk( writer, handler, 0x8FF8BFD1, compressedData->len + 8 );
      saveRawDataChunk( writer, handler, 0x9600CED1, compressedData->data, compressedData->len, true, model->model.tertiaryModelData.len );
      delete compressedData;
    }

    if( model->model.reflectionModelPath == "" )
    {
      saveUInt32Chunk( writer, handler, 0x9CF1BDD3, 1 );
    }
    else
    {
      saveUInt32Chunk( writer, handler, 0x9CF1BDD3, 0 );
    }
    if( model->model.reflectionModelPath != "" )
    {
      saveUInt32Chunk( writer, handler, 0xA5F2C5D3, 1 );
      saveStringChunk( writer, handler, 0x9D00C4D3, model->model.reflectionModelPath );

      ops::RawData * compressedData = compress( &model->model.reflectionModelData );
      saveUInt32Chunk( writer, handler, 0x8FF8BFD3, compressedData->len + 8 );
      saveRawDataChunk( writer, handler, 0x9600CED3, compressedData->data, compressedData->len, true, model->model.reflectionModelData.len );
      delete compressedData;
    }

    if( model->model.collisionElements.size() > 0 )
    {
      saveUInt32Chunk( writer, handler, 0x8FEEC3E2, model->model.collisionElements.size());
      saveUInt32Chunk( writer, handler, 0x93FBC3E2, 1 );

      // save all elements data
      // save chunk len
      uint32_t chunkLen =  4 + 44 * model->model.collisionElements.size();
      writer->fileWrite( handler, (uint8_t*)&chunkLen, (uint32_t)sizeof( uint32_t ));

      // save chunk
      uint32_t chunk = 0x9DFCC3E2;
      writer->fileWrite( handler, (uint8_t*)&chunk, (uint32_t)sizeof( uint32_t ));

      // save value
      for( uint32_t i = 0; i < model->model.collisionElements.size(); i++ )
      {
        FPModelCollisionData * element = &model->model.collisionElements[i];
        writer->fileWrite( handler, (uint8_t*)element, (uint32_t)sizeof( uint8_t ) * 44 );
      }
    }


  }
  else
  {
    // linked
    saveStringChunk( writer, handler, 0xA1EDD1D5, model->model.linkedPath );
  }

  //Tag end
  saveTagChunk( writer, handler, 0xA7FDC4E0 );

  writer->fileClose( handler );
  writer->close();

  DEBUG << filepath + " saved";
}


FPTWriter::FPTWriter()
{


}

FPTWriter::~FPTWriter()
{


}


void FPTWriter::saveStringChunk( ops::msole::FileWriter * writer, int32_t handler, uint32_t chunk, std::string msg )
{

  // save chunk len
  uint32_t chunkLen =  msg.length() + 4 + 4;
  writer->fileWrite( handler, (uint8_t*)&chunkLen, (uint32_t)sizeof( uint32_t ));

  // save chunk
  writer->fileWrite( handler, (uint8_t*)&chunk, (uint32_t)sizeof( uint32_t ));

  // save nbchar
  uint32_t msgLen =  msg.length();
  writer->fileWrite( handler, (uint8_t*)&msgLen, (uint32_t)sizeof( uint32_t ));

  // save chars
  writer->fileWrite( handler, (uint8_t*)msg.c_str(), (uint32_t)sizeof( char ) * msgLen );
}

void FPTWriter::saveWStringChunk( ops::msole::FileWriter * writer, int32_t handler, uint32_t chunk, std::string msg )
{

  // save chunk len
  uint32_t chunkLen =  msg.length() * 2 + 4 + 4;
  writer->fileWrite( handler, (uint8_t*)&chunkLen, (uint32_t)sizeof( uint32_t ));

  // save chunk
  writer->fileWrite( handler, (uint8_t*)&chunk, (uint32_t)sizeof( uint32_t ));

  // save nbchar
  uint32_t msgLen =  msg.length() * 2;
  writer->fileWrite( handler, (uint8_t*)&msgLen, (uint32_t)sizeof( uint32_t ));

  // save chars
  //writer->fileWrite(handler, (uint8_t*)msg.c_str(), (uint32_t)sizeof(char) * msgLen);
  for( uint32_t i = 0; i < msg.length(); i++ )
  {
    uint8_t tmp = 0;
    writer->fileWrite( handler, (uint8_t*)msg.c_str() + i, (uint32_t)sizeof( char ));
    writer->fileWrite( handler, &tmp, (uint32_t)sizeof( char ));
  }

}


void FPTWriter::saveStringListChunk( ops::msole::FileWriter * writer, int32_t handler, uint32_t chunk, std::vector <std::string> value )
{

  // save chunk len : TODO
  uint32_t chunkLen = 4 + 4;
  writer->fileWrite( handler, (uint8_t*)&chunkLen, (uint32_t)sizeof( uint32_t ));

  // save chunk
  writer->fileWrite( handler, (uint8_t*)&chunk, (uint32_t)sizeof( uint32_t ));

  // save nb items
  uint32_t nbItems = value.size();
  writer->fileWrite( handler, (uint8_t*)&nbItems, (uint32_t)sizeof( uint32_t ));


  for( uint32_t i = 0; i < nbItems; i++ )
  {
    uint32_t msgLen =  value[i].length();
    writer->fileWrite( handler, (uint8_t*)&msgLen, (uint32_t)sizeof( uint32_t ));

    // save chars
    writer->fileWrite( handler, (uint8_t*)value[i].c_str(), (uint32_t)sizeof( char ) * msgLen );
  }

}


void FPTWriter::saveUInt32Chunk( ops::msole::FileWriter * writer, int32_t handler, uint32_t chunk, uint32_t value )
{
  // save chunk len
  uint32_t chunkLen =  4 + 4;
  writer->fileWrite( handler, (uint8_t*)&chunkLen, (uint32_t)sizeof( uint32_t ));

  // save chunk
  writer->fileWrite( handler, (uint8_t*)&chunk, (uint32_t)sizeof( uint32_t ));

  // save value
  writer->fileWrite( handler, (uint8_t*)&value, (uint32_t)sizeof( uint32_t ));
}

void FPTWriter::saveVector2DChunk( ops::msole::FileWriter * writer, int32_t handler, uint32_t chunk, Vector2D value )
{
  // save chunk len
  uint32_t chunkLen =  4 + 8;
  writer->fileWrite( handler, (uint8_t*)&chunkLen, (uint32_t)sizeof( uint32_t ));

  // save chunk
  writer->fileWrite( handler, (uint8_t*)&chunk, (uint32_t)sizeof( uint32_t ));

  // save value
  writer->fileWrite( handler, (uint8_t*)&value, (uint32_t)sizeof( float ) * 2 );
}

void FPTWriter::saveTagChunk( ops::msole::FileWriter * writer, int32_t handler, uint32_t chunk )
{
  // save chunk len
  uint32_t chunkLen =  4;
  writer->fileWrite( handler, (uint8_t*)&chunkLen, (uint32_t)sizeof( uint32_t ));

  // save chunk
  writer->fileWrite( handler, (uint8_t*)&chunk, (uint32_t)sizeof( uint32_t ));

}


void FPTWriter::saveFloatChunk( ops::msole::FileWriter * writer, int32_t handler, uint32_t chunk, float value )
{
  uint32_t chunkLen =  4 + 4;
  writer->fileWrite( handler, (uint8_t*)&chunkLen, (uint32_t)sizeof( uint32_t ));

  // save chunk
  writer->fileWrite( handler, (uint8_t*)&chunk, (uint32_t)sizeof( uint32_t ));

  // save value
  writer->fileWrite( handler, (uint8_t*)&value, (uint32_t)sizeof( float ));
}



void FPTWriter::saveRawDataChunk( ops::msole::FileWriter * writer, int32_t handler, uint32_t chunk, uint8_t* value, uint32_t len, bool compressed, uint32_t uncompressedLen )
{

  if( compressed )
  {
    // save chunk len
    uint32_t chunkLen =  4 + 4 + 4 + len;
    writer->fileWrite( handler, (uint8_t*)&chunkLen, (uint32_t)sizeof( uint32_t ));

    // save chunk
    writer->fileWrite( handler, (uint8_t*)&chunk, (uint32_t)sizeof( uint32_t ));

    // save zLZO header
    char header [5] = "zLZO";
    writer->fileWrite( handler, (uint8_t*)header, (uint32_t)sizeof( uint32_t ));

    // save uncompressed len
    writer->fileWrite( handler, (uint8_t*)&uncompressedLen, (uint32_t)sizeof( uint32_t ));

    // save compressed bytes
    writer->fileWrite( handler, (uint8_t*)value, (uint32_t)sizeof( uint8_t ) * len );

  }
  else
  {

    // save chunk len
    uint32_t chunkLen =  4 + len;
    writer->fileWrite( handler, (uint8_t*)&chunkLen, (uint32_t)sizeof( uint32_t ));

    // save chunk
    writer->fileWrite( handler, (uint8_t*)&chunk, (uint32_t)sizeof( uint32_t ));

    // save uncompressed bytes
    writer->fileWrite( handler, (uint8_t*)value, (uint32_t)sizeof( uint8_t ) * len );
  }

}


void FPTWriter::saveChunk( ops::msole::FileWriter * writer, int32_t handler, ChunkGeneric * chunk )
{
  switch( chunk->descriptor.type )
  {
    case T_CHUNK_CHUNKLIST:

      if( chunk->descriptor.label == "shape_point" || chunk->descriptor.label == "ramp_point" )
      {
        saveTagChunk( writer, handler, chunk->originalChunk );
      }

      for( uint32_t i = 0; i < ((ChunkChunkList*)chunk )->value.size(); i++ )
      {
        saveChunk( writer, handler, ((ChunkChunkList*)chunk )->value[i] );
      }
      return;
      break;

    case T_CHUNK_GENERIC:
      saveTagChunk( writer, handler, chunk->originalChunk );
      break;

    case T_CHUNK_COLLISIONDATA:
    case T_CHUNK_RAWDATA:
      saveRawDataChunk( writer, handler, chunk->originalChunk, ((ChunkRawData*)chunk )->value.data, ((ChunkRawData*)chunk )->value.len, false, ((ChunkRawData*)chunk )->value.len );
      break;

    case T_CHUNK_INT:
      if( chunk->descriptor.chunk == CHUNK_ELEMENT_TYPE.chunk
          && chunk->descriptor.label == CHUNK_ELEMENT_TYPE.label
          && chunk->descriptor.type == CHUNK_ELEMENT_TYPE.type
          )
      {
        writer->fileWrite( handler, (uint8_t*)&(((ChunkInt*)chunk )->value ), (uint32_t)sizeof( uint32_t ));
      }
      else
      {
        saveUInt32Chunk( writer, handler, chunk->originalChunk,  ((ChunkInt*)chunk )->value );
      }
      break;

    case T_CHUNK_COLOR:
      saveUInt32Chunk( writer, handler, chunk->originalChunk,  ((ChunkColor*)chunk )->value );
      break;

    case T_CHUNK_VALUELIST:
      saveUInt32Chunk( writer, handler, chunk->originalChunk,  ((ChunkValueList*)chunk )->value );
      break;

    case T_CHUNK_FLOAT:
      saveFloatChunk( writer, handler, chunk->originalChunk,  ((ChunkFloat*)chunk )->value );
      break;

    case T_CHUNK_VECTOR2D:
      saveVector2DChunk( writer, handler, chunk->originalChunk,  ((ChunkVector2D*)chunk )->value );
      break;

    case T_CHUNK_STRING:
      saveStringChunk( writer, handler, chunk->originalChunk,  ((ChunkString*)chunk )->value );
      break;

    case T_CHUNK_WSTRING:
      saveWStringChunk( writer, handler, chunk->originalChunk,  ((ChunkWString*)chunk )->value );
      break;

    case T_CHUNK_STRINGLIST:
    {
      saveStringListChunk( writer, handler, chunk->originalChunk, ((ChunkStringList*)chunk )->value );
    }
    break;

    default:
      std::cout << " TODO " << std::endl;
      break;
  }
}

void FPTWriter::flexSave( std::string filepath, ChunkChunkList * globalChunks )
{
  DEBUG << "saving " + filepath;

  ops::msole::FileWriter * writer = new ops::msole::FileWriter();
  writer->open( filepath );

  {
    int32_t handler = writer->fileOpen( "Future Pinball/File Version" );

    ChunkChunkList * chunks = (ChunkChunkList*)getChunkByLabel( globalChunks, CHUNK_FILE_VERSION.label );
    ChunkRawData * chunkRawData = (ChunkRawData*)getChunkByLabel( chunks, CHUNK_FILE_VERSION_DATA.label );
    writer->fileWrite( handler, &chunkRawData->value );

    writer->fileClose( handler );
  }
  {
    int32_t handler = writer->fileOpen( "Future Pinball/Table MAC" );

    ChunkChunkList * chunks = (ChunkChunkList*)getChunkByLabel( globalChunks, CHUNK_TABLE_MAC.label );
    ChunkRawData * chunkRawData = (ChunkRawData*)getChunkByLabel( chunks, CHUNK_TABLE_MAC_DATA.label );
    writer->fileWrite( handler, &chunkRawData->value );

    writer->fileClose( handler );
  }
  {
    int32_t handler = writer->fileOpen( "Future Pinball/Table Data" );

    ChunkChunkList * chunks = (ChunkChunkList*)getChunkByLabel( globalChunks, CHUNK_TABLE_DATA.label );
    saveChunk( writer, handler, chunks );

    writer->fileClose( handler );
  }
  {
    ChunkChunkList * chunksList = (ChunkChunkList*)getChunksByLabel( globalChunks, CHUNK_IMAGE.label );
    for( uint32_t i = 0; i < chunksList->value.size(); i++ )
    {
      ChunkChunkList * currentChunk = (ChunkChunkList *)chunksList->value[i];
      std::string name = "Future Pinball/Image ";
      std::stringstream ss;
      std::string number;
      ss << ( i + 1 );
      ss >> number;
      int32_t handler = writer->fileOpen( name + number );
      saveChunk( writer, handler, currentChunk );
      writer->fileClose( handler );
    }
  }
  {
    ChunkChunkList * chunksList = (ChunkChunkList*)getChunksByLabel( globalChunks, CHUNK_SOUND.label );
    for( uint32_t i = 0; i < chunksList->value.size(); i++ )
    {
      ChunkChunkList * currentChunk = (ChunkChunkList *)chunksList->value[i];
      std::string name = "Future Pinball/Sound ";
      std::stringstream ss;
      std::string number;
      ss << ( i + 1 );
      ss >> number;
      int32_t handler = writer->fileOpen( name + number );
      saveChunk( writer, handler, currentChunk );
      writer->fileClose( handler );
    }
  }
  {
    ChunkChunkList * chunksList = (ChunkChunkList*)getChunksByLabel( globalChunks, CHUNK_MUSIC.label );
    for( uint32_t i = 0; i < chunksList->value.size(); i++ )
    {
      ChunkChunkList * currentChunk = (ChunkChunkList *)chunksList->value[i];
      std::string name = "Future Pinball/Music ";
      std::stringstream ss;
      std::string number;
      ss << ( i + 1 );
      ss >> number;
      int32_t handler = writer->fileOpen( name + number );
      saveChunk( writer, handler, currentChunk );
      writer->fileClose( handler );
    }
  }
  {
    ChunkChunkList * chunksList = (ChunkChunkList*)getChunksByLabel( globalChunks, CHUNK_DMDFONT.label );
    for( uint32_t i = 0; i < chunksList->value.size(); i++ )
    {
      ChunkChunkList * currentChunk = (ChunkChunkList *)chunksList->value[i];
      std::string name = "Future Pinball/DmdFont ";
      std::stringstream ss;
      std::string number;
      ss << ( i + 1 );
      ss >> number;
      int32_t handler = writer->fileOpen( name + number );
      saveChunk( writer, handler, currentChunk );
      writer->fileClose( handler );
    }
  }
  {
    ChunkChunkList * chunksList = (ChunkChunkList*)getChunksByLabel( globalChunks, CHUNK_PINMODEL.label );
    for( uint32_t i = 0; i < chunksList->value.size(); i++ )
    {
      ChunkChunkList * currentChunk = (ChunkChunkList *)chunksList->value[i];

      std::string name = "Future Pinball/PinModel ";
      std::stringstream ss;
      std::string number;
      ss << ( i + 1 );
      ss >> number;
      int32_t handler = writer->fileOpen( name + number );
      saveChunk( writer, handler, currentChunk );
      writer->fileClose( handler );

    }
  }
  {
    ChunkChunkList * chunksList = (ChunkChunkList*)getChunksByLabel( globalChunks, CHUNK_IMAGELIST.label );
    for( uint32_t i = 0; i < chunksList->value.size(); i++ )
    {
      ChunkChunkList * currentChunk = (ChunkChunkList *)chunksList->value[i];
      std::string name = "Future Pinball/ImageList ";
      std::stringstream ss;
      std::string number;
      ss << ( i + 1 );
      ss >> number;
      int32_t handler = writer->fileOpen( name + number );
      saveChunk( writer, handler, currentChunk );
      writer->fileClose( handler );
    }
  }
  {
    ChunkChunkList * chunksList = (ChunkChunkList*)getChunksByLabel( globalChunks, CHUNK_LIGHTLIST.label );
    for( uint32_t i = 0; i < chunksList->value.size(); i++ )
    {
      ChunkChunkList * currentChunk = (ChunkChunkList *)chunksList->value[i];
      std::string name = "Future Pinball/LightList ";
      std::stringstream ss;
      std::string number;
      ss << ( i + 1 );
      ss >> number;
      int32_t handler = writer->fileOpen( name + number );
      saveChunk( writer, handler, currentChunk );
      writer->fileClose( handler );
    }
  }

  {
    ChunkChunkList * chunksList = (ChunkChunkList*)getChunksByLabel( globalChunks, CHUNK_TABLE_ELEMENT.label );
    for( uint32_t i = 0; i < chunksList->value.size(); i++ )
    {
      ChunkChunkList * currentChunk = (ChunkChunkList *)chunksList->value[i];
      std::string name = "Future Pinball/Table Element ";
      std::stringstream ss;
      std::string number;
      ss << ( i + 1 );
      ss >> number;
      int32_t handler = writer->fileOpen( name + number );
      saveChunk( writer, handler, currentChunk );
      writer->fileClose( handler );
    }
  }

  writer->close();
  DEBUG << filepath + " saved";
}



} // namespace fp
} // namespace ops
