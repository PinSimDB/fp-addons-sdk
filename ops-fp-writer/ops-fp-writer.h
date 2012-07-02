#ifndef __OPS_FP_WRITER_H_INCLUDED
#define __OPS_FP_WRITER_H_INCLUDED

// std
#include <stdint.h>
#include <string>
#include <vector>


// ops
#include "ops-base.h"
#include "ops-msole.h"
#include "ops-fp.h"

namespace ops
{
namespace fp
{

class FPMWriter : public FPMHandler
{
public:
  FPMWriter();
  ~FPMWriter();

  FPMFile * create( std::string filepath, std::string name );
  void save( std::string filepath, FPMFile * model );

protected:

  void saveStringChunk( ops::msole::FileWriter * writer, int32_t handler, uint32_t chunk, std::string msg );
  void saveUInt32Chunk( ops::msole::FileWriter * writer, int32_t handler, uint32_t chunk, uint32_t value );
  void saveTagChunk( ops::msole::FileWriter * writer, int32_t handler, uint32_t chunk );
  void saveFloatChunk( ops::msole::FileWriter * writer, int32_t handler, uint32_t chunk, float value );
  void saveRawDataChunk( ops::msole::FileWriter * writer, int32_t handler, uint32_t chunk, uint8_t* value, uint32_t len, bool compressed, uint32_t uncompressedLen );
};

class FPTWriter : public FPTHandler
{
public:
  FPTWriter();
  ~FPTWriter();

  void flexSave( std::string filepath, ChunkChunkList * globalChunks );

protected:
  void saveChunk( ops::msole::FileWriter * writer, int32_t handler, ChunkGeneric * chunk );

  void saveVector2DChunk( ops::msole::FileWriter * writer, int32_t handler, uint32_t chunk, Vector2D value );
  void saveStringChunk( ops::msole::FileWriter * writer, int32_t handler, uint32_t chunk, std::string msg );
  void saveWStringChunk( ops::msole::FileWriter * writer, int32_t handler, uint32_t chunk, std::string msg );
  void saveUInt32Chunk( ops::msole::FileWriter * writer, int32_t handler, uint32_t chunk, uint32_t value );
  void saveTagChunk( ops::msole::FileWriter * writer, int32_t handler, uint32_t chunk );
  void saveFloatChunk( ops::msole::FileWriter * writer, int32_t handler, uint32_t chunk, float value );
  void saveRawDataChunk( ops::msole::FileWriter * writer, int32_t handler, uint32_t chunk, uint8_t* value, uint32_t len, bool compressed, uint32_t uncompressedLen );
  void saveStringListChunk( ops::msole::FileWriter * writer, int32_t handler, uint32_t chunk, std::vector <std::string> value );
};

} // namespace fp
} // namespace ops



#endif // __OPS_FP_WRITER_H_INCLUDED

