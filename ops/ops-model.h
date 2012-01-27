#ifndef __OPS_MODEL_H_INCLUDED
#define __OPS_MODEL_H_INCLUDED

// std
// ops
#include "ops-base.h"
#include "ops-files.h"


namespace ops
{



struct ModelCollisionData
{
    uint32_t type;
    uint32_t collide;
	uint32_t detect;
	uint32_t eventID;
	float x, y, z;
	float value1, value2, value3, value4;
};

// TODO : remap type
/*
ModelCollisionData type VL:
 0 : Unknown -> Per Polygon Collision
 1 : Vertical Cylinder
 2 : Sphere
 3 : Flipper
 4 : Bumper // not used in FP
 5 : Box
 6 : Convex hull // not used in FP
 7 : Horizontal cylinder
*/


// TODO : ren Model to MeshModel or other
// TODO : optimise : use plain c_str ?
// TODO ? est-ce utile ?
// lowMesh
// fp lod distance
// fp custom value

class MeshModel
{
public :
    // general
	//std::string name;                       // model name
	uint32_t type;                          // model type
    ops::RawData preview;                   // raw picture 96x96 pixels

	// rendering
    ops::RawData primaryMesh;               // raw mesh in ms3d
    ops::RawData lowMesh;                   // raw mesh in ms3d
    ops::RawData maskMesh;                  // raw mesh in ms3d
    ops::RawData reflectionMesh;            // raw mesh in ms3d

    // physic
    uint32_t materialType;
    uint32_t nbCollisionElements;
    ModelCollisionData * collisionElements;

    MeshModel();
    ~MeshModel();

    void dump(std::ostream &outstream);
    bool save(ops::files::AbsFile &outfile);
    bool load(ops::files::AbsFile &infile);

    // TODO : optimize struct :
    // - convert ms3d to internal format
    // - clear memory
    // - prevent save()

};





} // namespace ops

#endif // __OPS_MODEL_H_INCLUDED

