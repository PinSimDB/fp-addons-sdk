#ifndef __OPS_FP_DEFS_MIN_H_INCLUDED
#define __OPS_FP_DEFS_MIN_H_INCLUDED

// std
#include <stdint.h>
#include <string>
#include <vector>

// gets rid of annoying "deprecated conversion from string constant blah blah" warning
#pragma GCC diagnostic ignored "-Wwrite-strings"

// ops
namespace ops
{
namespace fp
{

class VLDescriptor {
public:
  int32_t code;
  std::string label;

  VLDescriptor( int32_t codeP, char *labelP )
  {
    code = codeP;
    label = labelP;
  }

  void operator = ( const VLDescriptor &c )
  {
    code = c.code;
    label = c.label;
  }




};

static std::vector <VLDescriptor> VL_EMPTY;

enum T_CHUNK_TYPE {
  // Container
  T_CHUNK_CHUNKLIST,
  // Generic types
  T_CHUNK_GENERIC,
  T_CHUNK_RAWDATA,
  T_CHUNK_INT,
  T_CHUNK_FLOAT,
  T_CHUNK_COLOR,
  T_CHUNK_VECTOR2D,
  T_CHUNK_STRING,
  T_CHUNK_WSTRING,
  T_CHUNK_STRINGLIST,
  T_CHUNK_VALUELIST,
  // Specific types
  T_CHUNK_COLLISIONDATA,
  T_CHUNK_RAWDATALZO, // compressed
  T_CHUNK_SCRIPT // SK1: Anomalie for table script
  };


class ChunkDescriptor {
public:
  uint32_t chunk;
  T_CHUNK_TYPE type;
  std::string label;
  int32_t offset;

  std::vector <VLDescriptor> * valueList;

  ChunkDescriptor( )
  {
    chunk = 0;
    type = (T_CHUNK_TYPE)0;
    label = "";
    offset = 0;
  }

  ChunkDescriptor( uint32_t chunkP, T_CHUNK_TYPE typeP, char * labelP, int32_t offsetP, std::vector <VLDescriptor> * valueListP = NULL )
  {
    chunk = chunkP;
    type = typeP;
    label = labelP;
    offset = offsetP;
    valueList = valueListP;
  }

  void operator = ( const ChunkDescriptor &c )
  {
    chunk = c.chunk;
    type = c.type;
    label = c.label;
    offset = c.offset;
    valueList = c.valueList;
  }

};


    static ChunkDescriptor CHUNK_GENERIC_EMPTY(0x00000000, T_CHUNK_GENERIC, "empty", -1);

    class ChunkGeneric {
    public:
      // Chunk description
      ChunkDescriptor descriptor;
      // Loaded infos
      uint32_t originalLen;
      uint32_t originalChunk;
      // Data len
      uint32_t len;
      ChunkGeneric( const ChunkDescriptor & initDescriptor = CHUNK_GENERIC_EMPTY );
      ChunkGeneric( uint32_t initOriginalLen, uint32_t initOriginalChunk, const ChunkDescriptor & initDescriptor );

      virtual ~ChunkGeneric() {};
      virtual void ShowXmlLike() {
        ShowXmlLikeTagBegin();
        ShowXmlLikeTagEndTiny();
      };

    protected:
      virtual void ShowXmlLikeTagName() { std::cout << "ChunkGeneric"; }
      void ShowXmlLikePadding() { std::cout.width(showXmlLikeLevel); }

      void ShowXmlLikeTagBegin()
      {
        ShowXmlLikePadding();
        std::cout << "<";
        ShowXmlLikeTagName();
        std::cout << " ";
        ShowXmlLikeData();
        ShowXmlLikeInc();
      }

      void ShowXmlLikeTagEndTiny()
      {
        std::cout << "/>" << std::endl;
        ShowXmlLikeDec();
      }

      void ShowXmlLikeTagEnd()
      {
        std::cout << "</";
        ShowXmlLikeTagName();
        std::cout << ">" << std::endl;
        ShowXmlLikeDec();
      }

      void ShowXmlLikeData() {
          std::cout << "Name=\"" << descriptor.label << "\" ";
#ifdef _DEBUG
          std::cout << "descriptorType=\"" << descriptor.type << "\" ";
          std::cout << "descriptorOffset=\"" << descriptor.offset << "\" ";
          std::cout << "originalLen=\"" << originalLen << "\" ";
          std::cout << "originalChunk=\"0x" << std::hex << std::uppercase << originalChunk << std::dec << std::nouppercase << "\" ";
          std::cout << "len=\"" << len << "\" ";
#endif
      };

    private:
      static int showXmlLikeLevel;
      void ShowXmlLikeInc() { ++showXmlLikeLevel; }
      void ShowXmlLikeDec() { --showXmlLikeLevel; }
};


    static ChunkDescriptor CHUNK_CHUNKLIST_EMPTY(0x00000000, T_CHUNK_CHUNKLIST, "empty", -1);

    class ChunkChunkList : public ChunkGeneric{
    public:
      std::vector <ChunkGeneric *> value;
      ChunkChunkList * parent;
      ChunkChunkList( const ChunkDescriptor & initDescriptor = CHUNK_CHUNKLIST_EMPTY );
      ChunkChunkList( uint32_t initOriginalLen, uint32_t initOriginalChunk, const ChunkDescriptor & initDescriptor, const std::vector <ChunkGeneric *> & initValue );
      ~ChunkChunkList();
      void add( ChunkGeneric * child );
      virtual void ShowXmlLikeTagName() { std::cout << "ChunkChunkList"; }
      virtual void ShowXmlLike() {
        ShowXmlLikeTagBegin();
        std::cout << ">" << std::endl;
        for (int i=0; i<(int)value.size(); ++i)
          value[i]->ShowXmlLike();
        ShowXmlLikePadding();
        ShowXmlLikeTagEnd();
      };
};


struct Vector2D {
  float x, y;
  Vector2D( float initX = 0.0f, float initY = 0.0f );
  void ShowXmlLike() {
    std::cout << "<Vector2D ";
    std::cout << "x=\"" << x << "\" ";
    std::cout << "y=\"" << y << "\" ";
    std::cout << "/>";
  }
};


	static ChunkDescriptor CHUNK_RAWDATA_EMPTY (0x00000000, T_CHUNK_RAWDATA, "empty", -1);
	static ChunkDescriptor CHUNK_INT_EMPTY (0x00000000, T_CHUNK_INT, "empty", -1);
	static ChunkDescriptor CHUNK_FLOAT_EMPTY (0x00000000, T_CHUNK_FLOAT, "empty", -1);
	static ChunkDescriptor CHUNK_COLOR_EMPTY (0x00000000, T_CHUNK_COLOR, "empty", -1);
	static ChunkDescriptor CHUNK_VECTOR2D_EMPTY (0x00000000, T_CHUNK_VECTOR2D, "empty", -1);
	static ChunkDescriptor CHUNK_STRING_EMPTY (0x00000000, T_CHUNK_STRING, "empty", -1);
	static ChunkDescriptor CHUNK_WSTRING_EMPTY (0x00000000, T_CHUNK_WSTRING, "empty", -1);
	static ChunkDescriptor CHUNK_STRINGLIST_EMPTY (0x00000000, T_CHUNK_STRINGLIST, "empty", -1);
	static ChunkDescriptor CHUNK_VALUELIST_EMPTY (0x00000000, T_CHUNK_VALUELIST, "empty", -1, &VL_EMPTY);
	static ChunkDescriptor CHUNK_COLLISIONDATA_EMPTY (0x00000000, T_CHUNK_COLLISIONDATA, "empty", -1);
	static ChunkDescriptor CHUNK_SCRIPT_EMPTY (0x00000000, T_CHUNK_SCRIPT, "empty", -1);


// Basic classes for chunk handling

class ChunkRawData : public ChunkGeneric {
public:
  ops::RawData value;
  ChunkRawData( const ChunkDescriptor & initDescriptor = CHUNK_RAWDATA_EMPTY );
  ChunkRawData( uint32_t initOriginalLen, uint32_t initOriginalChunk, const ChunkDescriptor & initDescriptor, const ops::RawData & initValue );
  virtual void ShowXmlLikeTagName() { std::cout << "ChunkRawData"; }
  virtual void ShowXmlLike() {
    ShowXmlLikeTagBegin();
    std::cout << ">";
    std::cout << "...";
    ShowXmlLikeTagEnd();
  };
};

class ChunkScript : public ChunkGeneric {
public:
  ops::RawData value;
  std::string script;
  ChunkScript( const ChunkDescriptor & initDescriptor = CHUNK_SCRIPT_EMPTY );
  ChunkScript( uint32_t initOriginalLen, uint32_t initOriginalChunk, const ChunkDescriptor & initDescriptor, const ops::RawData & initValue );
  virtual void ShowXmlLikeTagName() { std::cout << "ChunkScript"; }
  virtual void ShowXmlLike() {
    ShowXmlLikeTagBegin();
    std::cout << ">";
    std::cout << "...";
    ShowXmlLikeTagEnd();
  };
};


class ChunkInt : public ChunkGeneric {
public:
  int32_t value;
  ChunkInt( const ChunkDescriptor & initDescriptor = CHUNK_INT_EMPTY );
  ChunkInt( uint32_t initOriginalLen, uint32_t initOriginalChunk, const ChunkDescriptor & initDescriptor, const int32_t & initValue );
  virtual void ShowXmlLikeTagName() { std::cout << "ChunkInt"; }
  virtual void ShowXmlLike() {
    ShowXmlLikeTagBegin();
    std::cout << ">";
    std::cout << value;
    ShowXmlLikeTagEnd();
  };
};

class ChunkFloat : public ChunkGeneric {
public:
  float value;
  ChunkFloat( const ChunkDescriptor & initDescriptor = CHUNK_FLOAT_EMPTY );
  ChunkFloat( uint32_t initOriginalLen, uint32_t initOriginalChunk, const ChunkDescriptor & initDescriptor, const float & initValue );
  virtual void ShowXmlLikeTagName() { std::cout << "ChunkFloat"; }
  virtual void ShowXmlLike() {
    ShowXmlLikeTagBegin();
    std::cout << ">";
    std::cout << value;
    ShowXmlLikeTagEnd();
  };
};

class ChunkColor : public ChunkGeneric {
public:
  int32_t value;
  ChunkColor( const ChunkDescriptor & initDescriptor = CHUNK_COLOR_EMPTY );
  ChunkColor( uint32_t initOriginalLen, uint32_t initOriginalChunk, const ChunkDescriptor & initDescriptor, const int32_t & initValue );
  virtual void ShowXmlLikeTagName() { std::cout << "ChunkColor"; }
  virtual void ShowXmlLike() {
    ShowXmlLikeTagBegin();
    std::cout << ">0x";
    std::cout << std::hex << std::uppercase << value << std::dec << std::nouppercase;
    ShowXmlLikeTagEnd();
  };
};

class ChunkVector2D : public ChunkGeneric {
public:
  Vector2D value;
  ChunkVector2D( const ChunkDescriptor & initDescriptor = CHUNK_VECTOR2D_EMPTY );
  ChunkVector2D( uint32_t initOriginalLen, uint32_t initOriginalChunk, const ChunkDescriptor & initDescriptor, const Vector2D & initValue );
  virtual void ShowXmlLikeTagName() { std::cout << "ChunkVector2D"; }
  virtual void ShowXmlLike() {
    ShowXmlLikeTagBegin();
    std::cout << ">";
    value.ShowXmlLike();
    ShowXmlLikeTagEnd();
  };
};

class ChunkString : public ChunkGeneric {
public:
  std::string value;
  ChunkString( const ChunkDescriptor & initDescriptor = CHUNK_STRING_EMPTY );
  ChunkString( uint32_t initOriginalLen, uint32_t initOriginalChunk, const ChunkDescriptor & initDescriptor, const std::string & initValue );
  virtual void ShowXmlLikeTagName() { std::cout << "ChunkString"; }
  virtual void ShowXmlLike() {
    ShowXmlLikeTagBegin();
    std::cout << ">";
    std::cout << value;
    ShowXmlLikeTagEnd();
  };
};

class ChunkWString : public ChunkGeneric {
public:
  std::wstring value;
  ChunkWString( const ChunkDescriptor & initDescriptor = CHUNK_WSTRING_EMPTY );
  ChunkWString( uint32_t initOriginalLen, uint32_t initOriginalChunk, const ChunkDescriptor & initDescriptor, const std::wstring & initValue );
  virtual void ShowXmlLikeTagName() { std::cout << "ChunkWString"; }
  virtual void ShowXmlLike() {
    ShowXmlLikeTagBegin();
    std::cout << ">";
    std::wcout << value;
    ShowXmlLikeTagEnd();
  };
};

class ChunkStringList : public ChunkGeneric {
public:
  std::vector <std::string> value;
  ChunkStringList( const ChunkDescriptor & initDescriptor = CHUNK_STRINGLIST_EMPTY );
  ChunkStringList( uint32_t initOriginalLen, uint32_t initOriginalChunk, const ChunkDescriptor & initDescriptor, const std::vector <std::string> & initValue );
  virtual void ShowXmlLikeTagName() { std::cout << "ChunkStringList"; }
  virtual void ShowXmlLike() {
    ShowXmlLikeTagBegin();
    std::cout << ">";
    for (int i=0; i<(int)value.size(); ++i)
      std::cout << value[i] << std::endl;
    ShowXmlLikePadding();
    ShowXmlLikeTagEnd();
  };
};

class ChunkValueList : public ChunkGeneric {
public:
  int32_t value;
  ChunkValueList( const ChunkDescriptor & initDescriptor = CHUNK_INT_EMPTY );
  ChunkValueList( uint32_t initOriginalLen, uint32_t initOriginalChunk, const ChunkDescriptor & initDescriptor, const int32_t & initValue );
  virtual void ShowXmlLikeTagName() { std::cout << "ChunkValueList"; }
  virtual void ShowXmlLike() {
    ShowXmlLikeTagBegin();
    std::cout << ">";
    std::cout << value;
    ShowXmlLikeTagEnd();
  };
};


struct FPModelCollisionData {
  uint32_t type;
  uint32_t generateHit;
  uint32_t effectBall;
  uint32_t eventID;
  float x, y, z;
  float value1, value2, value3, value4;

  FPModelCollisionData();
};
class ChunkCollisionData : public ChunkGeneric {
public:
  std::vector <FPModelCollisionData> value;
  ChunkCollisionData( const ChunkDescriptor & initDescriptor = CHUNK_COLLISIONDATA_EMPTY );
  ChunkCollisionData( uint32_t initOriginalLen, uint32_t initOriginalChunk, const ChunkDescriptor & initDescriptor, const std::vector <FPModelCollisionData> & initValue );
  virtual void ShowXmlLikeTagName() { std::cout << "ChunkCollisionData"; }
  virtual void ShowXmlLike() {
    ShowXmlLikeTagBegin();
    std::cout << ">";
    for (int i=0; i<(int)value.size(); ++i)
      std::cout << ".";
    ShowXmlLikePadding();
    ShowXmlLikeTagEnd();
  };
};






struct FPModel {
  // general
  std::string name;
  std::string id;
  uint32_t linked;

  // if not linked
  uint32_t type;
  uint32_t materialType;
  float specialValue;

  std::string previewPath;
  ops::RawData previewData;

  std::string primaryModelPath;
  ops::RawData primaryModelData;

  std::string secondaryModelPath;
  uint32_t secondaryModelZDistance;
  ops::RawData secondaryModelData;

  std::string tertiaryModelPath;
  ops::RawData tertiaryModelData;

  std::string reflectionModelPath;
  ops::RawData reflectionModelData;

  uint32_t collisionPerPolygon;
  std::vector <FPModelCollisionData> collisionElements;

  // if linked
  std::string linkedPath;

  FPModel();
  ~FPModel();
};

struct FPMFile : ops::BaseFile {
  FPModel model;
};

} // namespace fp
} // namespace ops



#endif // __OPS_FP_DEFS_MIN_H_INCLUDED

