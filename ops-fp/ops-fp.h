#ifndef __OPS_FP_H_INCLUDED
#define __OPS_FP_H_INCLUDED

// std
#include <stdint.h>
#include <string>
#include <vector>

// win specific
#ifdef _WIN32
#include <windows.h>
#include <wincrypt.h>
#undef ERROR
#endif


// ops
#include "ops-base.h"
#include "ops-msole.h"
#include "ops-fp-defs-min.h"


namespace ops
{
namespace fp
{

//#define ARRAY2VECTOR(T, NAME) std::vector<T> NAME(NAME##_ARRAY, NAME##_ARRAY + sizeof(NAME##_ARRAY)/sizeof(T))


	// TODO

	// list when several element with same name

	// search optimisation with map

	// manager for resources

	// CHUNK WITH EXTRA_INFO
	//T_CHUNK_MODEL -> link to manager instead, restrict to type ?
	// code

	//T_CHUNK_INT_RANGE
	//  min, max

	//T_CHUNK_FLOAT_RANGE
	//  min, max



	class FPBaseHandler {
	public:
		void dumpChunks(std::ostream &outstream, ChunkGeneric * chunks, uint32_t tabs, bool showChunk, bool showType, int32_t rawDataLimit);
		ChunkGeneric * getChunk(ChunkGeneric * baseChunk, std::string path);
		ops::fp::ChunkGeneric * getChunkByLabel(ops::fp::ChunkChunkList * parentList, std::string label);
		ops::fp::ChunkChunkList * getChunksByLabel(ops::fp::ChunkChunkList * parentList, std::string label);
   static ops::RawData * getRawData(const uint8_t * data, uint32_t len, bool uncompress);
		ops::RawData * compress(ops::RawData * data);

	protected:
		const ChunkDescriptor * findChunkDescriptor(const std::vector<ChunkDescriptor> & chunkList, uint32_t chunk);
		const std::string * findValue(const std::vector<VLDescriptor> & valueList, int32_t code);

		ChunkGeneric * getChunkImpl(ChunkGeneric * chunk, std::vector<std::string> pathList);
		ChunkGeneric * analyseChunk (ChunkChunkList ** result, const std::vector<ChunkDescriptor> & chunkList, uint32_t chunk, uint32_t originalChunk, uint32_t sectionLen, const uint8_t * data, uint32_t retry);
		void analyseRawData(ChunkChunkList * result, const std::vector<ChunkDescriptor> & chunkList, const ops::RawData * rawData);
	};


	class FPMHandler : public FPBaseHandler {
	public:
		FPMHandler();
		~FPMHandler();

		FPMFile * load(std::string filepath);
		ChunkGeneric * flexLoad(std::string filepath);

	protected:
		void analyseChunk2 (FPModel * model, uint32_t chunk, uint32_t originalChunk, uint32_t sectionLen, const uint8_t * data, uint32_t retry);
		void analyseRawData2(FPModel * model, const ops::RawData * rawData);
	};

	class FPTHandler : public FPBaseHandler {
   int nCryptCount;
	public:
		FPTHandler();
		~FPTHandler();

		ChunkGeneric * flexLoad(std::string filepath, bool keepPinModelRaw = false);

#ifdef _WIN32
		bool calcMAC( ChunkChunkList *pChunkListP, unsigned char *pszHashP, unsigned long nBuffersizeP );
#endif

	protected:
#ifdef _WIN32
		void cryptChunkList( ChunkChunkList *pV, HCRYPTHASH hHash );
#endif
	};

	class FPLHandler : public FPBaseHandler {
	public:
		FPLHandler();
		~FPLHandler();

		ChunkGeneric * flexLoad(std::string filepath);
	protected:
	};

} // namespace fp
} // namespace ops



#endif // __OPS_FP_H_INCLUDED

