#ifndef __OPS_FP_DEFS_H_INCLUDED
#define __OPS_FP_DEFS_H_INCLUDED

// std
#include <stdint.h>
#include <string>
#include <vector>

// ops-msole
#include "ops-msole.h"

// ops
#include "ops-base.h"
#include "ops-fp-defs-min.h"

namespace ops
{
namespace fp
{

#define ARRAY2VECTOR(T, NAME) std::vector<T> NAME(NAME##_ARRAY, NAME##_ARRAY + sizeof(NAME##_ARRAY)/sizeof(T))







	// FP definition

	static ChunkDescriptor CHUNK_PINMODEL = {0x00000000, T_CHUNK_CHUNKLIST, "pinmodel", -1};
    static ChunkDescriptor CHUNK_PINMODEL_RAW = {0x00000000, T_CHUNK_RAWDATA, "pinmodel_raw", -1};
	static ChunkDescriptor CHUNK_TABLE = {0x00000000, T_CHUNK_CHUNKLIST, "table", -1};

	static ChunkDescriptor CHUNK_LIBRARY = {0x00000000, T_CHUNK_CHUNKLIST, "library", -1};
    static ChunkDescriptor CHUNK_RESOURCE = {0x00000000, T_CHUNK_CHUNKLIST, "resource", -1};
    static ChunkDescriptor CHUNK_RESOURCE_NAME = {0xA4F1B9D1, T_CHUNK_STRING, "name", -1};
    static ChunkDescriptor CHUNK_RESOURCE_TYPE = {0xA4F1B9D1, T_CHUNK_INT, "type", -1};
    static ChunkDescriptor CHUNK_RESOURCE_PATH = {0xA4F1B9D1, T_CHUNK_STRING, "path", -1};
    static ChunkDescriptor CHUNK_RESOURCE_FLAD = {0xA4F1B9D1, T_CHUNK_RAWDATA, "flad", -1};
    static ChunkDescriptor CHUNK_RESOURCE_DATA = {0xA4F1B9D1, T_CHUNK_RAWDATA, "data", -1};

	static ChunkDescriptor CHUNK_TABLE_DATA = {0x00000000, T_CHUNK_CHUNKLIST, "table_data", -1};
	static ChunkDescriptor CHUNK_TABLE_ELEMENT = {0x00000000, T_CHUNK_CHUNKLIST, "table_element", -1};

	static ChunkDescriptor CHUNK_IMAGE = {0x00000000, T_CHUNK_CHUNKLIST, "image", -1};
	static ChunkDescriptor CHUNK_SOUND = {0x00000000, T_CHUNK_CHUNKLIST, "sound", -1};
	static ChunkDescriptor CHUNK_MUSIC = {0x00000000, T_CHUNK_CHUNKLIST, "music", -1};
	static ChunkDescriptor CHUNK_DMDFONT = {0x00000000, T_CHUNK_CHUNKLIST, "dmdfont", -1};
	static ChunkDescriptor CHUNK_IMAGELIST = {0x00000000, T_CHUNK_CHUNKLIST, "imagelist", -1};
	static ChunkDescriptor CHUNK_LIGHTLIST = {0x00000000, T_CHUNK_CHUNKLIST, "lightlist", -1};
	static ChunkDescriptor CHUNK_TABLE_MAC = {0x00000000, T_CHUNK_CHUNKLIST, "table_mac", -1};
	static ChunkDescriptor CHUNK_FILE_VERSION = {0x00000000, T_CHUNK_CHUNKLIST, "file_version", -1};

	static ChunkDescriptor CHUNK_TABLE_MAC_DATA = {0x00000000, T_CHUNK_RAWDATA, "table_mac_data", -1};
	static ChunkDescriptor CHUNK_FILE_VERSION_DATA = {0x00000000, T_CHUNK_RAWDATA, "file_version_data", -1};
	static ChunkDescriptor CHUNK_ELEMENT_TYPE = {0x00000000, T_CHUNK_INT, "element_type", -1};
	static ChunkDescriptor CHUNK_UNKNOWN = {0x00000000, T_CHUNK_RAWDATA, "unknown", -1};
	static ChunkDescriptor CHUNK_UNKNOWN_TABLE_ELEMENT = {0x00000000, T_CHUNK_RAWDATA, "unknown_table_element", -1};


	static VLDescriptor VL_MATERIAL_ARRAY[] = {
		{ 0, "metal"},
		{ 1, "wood"},
		{ 2, "plastic"},
		{ 3, "rubber"}
	};
	static ARRAY2VECTOR(VLDescriptor, VL_MATERIAL);

	static ChunkDescriptor CHUNKS_PINMODEL_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_STRING,	"name", offsetof(ops::fp::FPModel, name)},
		{0xA4F4C4DC, T_CHUNK_STRING,	"id", offsetof(ops::fp::FPModel, id)},
		{0x9EF3C6D9, T_CHUNK_INT,		"linked", offsetof(ops::fp::FPModel, linked)},

		{0xA4F1B9D1, T_CHUNK_INT,		"type", offsetof(ops::fp::FPModel, type)},
		{0x99E8BED8, T_CHUNK_VALUELIST,	"material_type", offsetof(ops::fp::FPModel, materialType), &VL_MATERIAL},

		{0x9D00C4DC, T_CHUNK_STRING,	"preview_path", offsetof(ops::fp::FPModel, previewPath)},
		{0x8FF8BFDC, T_CHUNK_INT,		"preview_data_len", -1},
		{0x9600CEDC, T_CHUNK_RAWDATA,	"preview_data", offsetof(ops::fp::FPModel, previewData)},

		{0x9AFEC2D5, T_CHUNK_INT,		"per_polygon_collision", offsetof(ops::fp::FPModel, collisionPerPolygon)},
		{0xA5F2C6E0, T_CHUNK_INT,		"secondary_model_enabled", -1},
		{0xA5FDC3D9, T_CHUNK_INT,		"secondary_model_z_distance", offsetof(ops::fp::FPModel, secondaryModelZDistance)},
		{0xA8EFCBD3, T_CHUNK_FLOAT,		"special_value", offsetof(ops::fp::FPModel, specialValue)},

		{0xA5F2C5D5, T_CHUNK_INT,		"primary_model_enabled", -1},
		{0x9D00C4D5, T_CHUNK_STRING,	"primary_model_path", offsetof(ops::fp::FPModel, primaryModelPath)},
		{0x8FF8BFD5, T_CHUNK_INT,		"primary_model_data_len", -1},
		{0x9600CED5, T_CHUNK_RAWDATA,	"primary_model_data", offsetof(ops::fp::FPModel, primaryModelData)},

		{0xA5F2C5D2, T_CHUNK_INT,		"secondary_model_enabled_at_z_distance", -1},
		{0x9D00C4D2, T_CHUNK_STRING,	"secondary_model_path", offsetof(ops::fp::FPModel, secondaryModelPath)},
		{0x8FF8BFD2, T_CHUNK_INT,		"secondary_model_data_len", -1},
		{0x9600CED2, T_CHUNK_RAWDATA,	"secondary_model_data", offsetof(ops::fp::FPModel, secondaryModelData)},

		{0xA5F2C5D1, T_CHUNK_INT,		"mask_model_enabled", -1},
		{0x9D00C4D1, T_CHUNK_STRING,	"mask_model_path", offsetof(ops::fp::FPModel, tertiaryModelPath)},
		{0x8FF8BFD1, T_CHUNK_INT,		"mask_model_data_len", -1},
		{0x9600CED1, T_CHUNK_RAWDATA,	"mask_model_data", offsetof(ops::fp::FPModel, tertiaryModelData)},

		{0x9CF1BDD3, T_CHUNK_INT,		"reflection_use_primary_model", -1},
		{0xA5F2C5D3, T_CHUNK_INT,		"reflection_model_enabled", -1},
		{0x9D00C4D3, T_CHUNK_STRING,	"reflection_model_path", offsetof(ops::fp::FPModel, reflectionModelPath)},
		{0x8FF8BFD3, T_CHUNK_INT,		"reflection_model_data_len", -1},
		{0x9600CED3, T_CHUNK_RAWDATA,	"reflection_model_data", offsetof(ops::fp::FPModel, reflectionModelData)},

		{0x8FEEC3E2, T_CHUNK_INT,		"nb_collision_shapes", -1},
		{0x93FBC3E2, T_CHUNK_INT,		"collision_shapes_enabled", -1},
		{0x9DFCC3E2, T_CHUNK_COLLISIONDATA, "collision_shapes_data", offsetof(ops::fp::FPModel, collisionElements)},

		{0xA1EDD1D5, T_CHUNK_STRING,	"linked_path", offsetof(ops::fp::FPModel, linkedPath)},
		{0xA7FDC4E0, T_CHUNK_GENERIC,	"end", -1}

	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_PINMODEL);

	static ChunkDescriptor CHUNKS_RESOURCE_ARRAY[] = {
		{0xA4F1B9D1, T_CHUNK_INT,		"type", -1},
		{0xA4F4D1D7, T_CHUNK_STRING,	"name", -1},
		{0xA4F4C4DC, T_CHUNK_STRING,	"id", -1},
		{0xA1EDD1D5, T_CHUNK_STRING,	"path", -1},
		{0x9EF3C6D9, T_CHUNK_INT,		"linked", -1},
		{0xA6E9BEE4, T_CHUNK_INT,		"s3tc_compression", -1},
		{0x95F5CCE1, T_CHUNK_INT,		"disable_filtering", -1},
		{0x96F3C0D1, T_CHUNK_COLOR,		"transparent_color", -1},
		{0xA4E7C9D2, T_CHUNK_INT,		"data_len", -1},
		{0xA8EDD1E1, T_CHUNK_RAWDATA,	"data", -1},
		{0x95EFBBD9, T_CHUNK_RAWDATA,	"unknown_1", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC,	"end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_RESOURCE);

	static ChunkDescriptor CHUNKS_LIST_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_STRING,	"name", -1},
		{0xA8EDD1E1, T_CHUNK_STRINGLIST,"items", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC,	"end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_LIST);

	static ChunkDescriptor CHUNKS_TABLE_ARRAY[] = {
		{0xA5F8BBD1, T_CHUNK_INT,		"width", -1},
		{0x9BFCC6D1, T_CHUNK_INT,		"length", -1},
		{0xA1FACCD1, T_CHUNK_INT,		"front_glass_height", -1},
		{0xA1FAC0D1, T_CHUNK_INT,		"rear_glass_height", -1},
		{0x9AF5BFD1, T_CHUNK_FLOAT,		"slope", -1},
		{0x9DF2CFD5, T_CHUNK_COLOR,		"playfield_color", -1},
		{0xA2F4C9D5, T_CHUNK_STRING,	"playfield_texture", -1},
		{0x9DF2CFE2, T_CHUNK_COLOR,		"cabinet_wood_color", -1},
		{0x9DF2CFE3, T_CHUNK_COLOR,		"button_color", -1},
		{0x9AFECBE3, T_CHUNK_COLOR,		"translite_color", -1},
		{0xA2F4C9E3, T_CHUNK_STRING,	"translite_image", -1},
		{0x96F2C6DE, T_CHUNK_INT,		"glossiness", -1},
		{0x95F5C9D1, T_CHUNK_INT,		"warnings_before_tilt", -1},
		{0xA5F8C0DE, T_CHUNK_INT,		"display_grid_in_editor", -1},
		{0x96FDC0DE, T_CHUNK_INT,		"grid_size", -1},
		{0xA0FBC2E1, T_CHUNK_INT,		"display_playfield_in_editor", -1},
		{0xA0FAD0E1, T_CHUNK_INT,		"display_tranlite_in_editor", -1},
		{0xA0EACBE3, T_CHUNK_INT,		"translite_width", -1},
		{0xA4F9CBE3, T_CHUNK_INT,		"translite_height", -1},
		{0x99E8BED8, T_CHUNK_INT,		"machine_type", -1},
		{0xA2F4C9E2, T_CHUNK_STRING,	"cabinet_texture", -1},
		{0x95EEC3D5, T_CHUNK_STRING,	"poster_image", -1},
		{0x9BF5CFD1, T_CHUNK_FLOAT,		"table_center_line", -1},
		{0x9BF5CCD1, T_CHUNK_FLOAT,		"table_flipper_line", -1},
		{0xA0FCBFE1, T_CHUNK_INT,		"unknown_3", -1},
		{0x9AFECDD2, T_CHUNK_INT,		"unknown_4_color", -1},
		{0xA1FACCD1, T_CHUNK_INT,		"unknown_5", -1},
		{0x9BEDC9D1, T_CHUNK_STRING,	"table_name", -1},
		{0xA4EBC9D1, T_CHUNK_STRING,	"version", -1},
		{0x9500C9D1, T_CHUNK_STRING,	"table_authors", -1},
		{0xA5EFC9D1, T_CHUNK_STRING,	"release_date", -1},
		{0x9CFCC9D1, T_CHUNK_STRING,	"mail", -1},
		{0x96EAC9D1, T_CHUNK_STRING,	"web_page", -1},
		{0xA4FDC9D1, T_CHUNK_STRING,	"description", -1},
		{0x96EFC9D1, T_CHUNK_INT,		"rules_len", -1},
		{0x94EFC9D1, T_CHUNK_RAWDATA,	"rules", -1},

		{0x99F5C9D1, T_CHUNK_STRING,	"loading_picture", -1},
		{0x95FEC9D1, T_CHUNK_COLOR,		"loading_color", -1},
		{0xA2F1D0DC, T_CHUNK_INT,		"ball_per_game", -1},
		{0xA700C8DC, T_CHUNK_INT,		"initial_jackpot", -1},
		{0x9C10CADC, T_CHUNK_STRING,	"high_scores_default_initial_1", -1},
		{0x9710CADC, T_CHUNK_INT,		"high_scores_default_score_1", -1},
		{0x9C0FCADC, T_CHUNK_STRING,	"high_scores_default_initial_2", -1},
		{0x970FCADC, T_CHUNK_INT,		"high_scores_default_score_2", -1},
		{0x9C0ECADC, T_CHUNK_STRING,	"high_scores_default_initial_3", -1},
		{0x970ECADC, T_CHUNK_INT,		"high_scores_default_score_3", -1},
		{0x9C0DCADC, T_CHUNK_STRING,	"high_scores_default_initial_4", -1},
		{0x970DCADC, T_CHUNK_INT,		"high_scores_default_score_4", -1},

		{0x9BFBBFDC, T_CHUNK_STRING,	"special_score_title", -1},
		{0x93FBBFDC, T_CHUNK_INT,		"special_score_value", -1},

		{0x96ECCFE2, T_CHUNK_RAWDATA,	"unknown_6", -1},

		{0xA4FBBFDC, T_CHUNK_STRING,	"special_score_text", -1},

		{0xA4F4D1D7, T_CHUNK_WSTRING,	"table_element_name", -1},
		{0x95FDCDD2, T_CHUNK_INT,		"unknown_7", -1},
		{0xA2F4C9D2, T_CHUNK_INT,		"unknown_8", -1},
		{0xA5F3BFD2, T_CHUNK_INT,		"unknown_9", -1},
		{0x96ECC5D2, T_CHUNK_INT,		"unknown_10", -1},
		{0xA5F2C5D2, T_CHUNK_INT,		"unknown_11", -1},
		{0x95F5C9D2, T_CHUNK_INT,		"unknown_12", -1},
		{0x95F5C6D2, T_CHUNK_INT,		"unknown_13", -1},
		{0x9BFBCED2, T_CHUNK_INT,		"unknown_14", -1},
		{0xA4FDC3E2, T_CHUNK_GENERIC,	"unknown_15", -1},

		{0x4F5A4C7A, T_CHUNK_RAWDATA,	"script", -1},

		{0x91FBCCD6, T_CHUNK_FLOAT,		"translate_x", -1},
		{0x90FBCCD6, T_CHUNK_FLOAT,		"translate_y", -1},
		{0x9200CFD2, T_CHUNK_FLOAT,		"scale_x", -1},
		{0x9100CFD2, T_CHUNK_FLOAT,		"scale_y", -1},
		{0x91EECCD6, T_CHUNK_FLOAT,		"bbox_translate_x", -1},
		{0x90EECCD6, T_CHUNK_FLOAT,		"bbox_translate_y", -1},
		{0x91EECFD2, T_CHUNK_FLOAT,		"bbox_scale_x", -1},
		{0x90EECFD2, T_CHUNK_FLOAT,		"bbox_scale_y", -1},

		{0xA7FDC4E0, T_CHUNK_GENERIC, "end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_TABLE);

	static ChunkDescriptor CHUNKS_ELEMENT_2_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING, "name", -1},
		{0x91EDBEE1, T_CHUNK_INT,		"display_image_in_editor", -1},
		{0x9DF2CFD1, T_CHUNK_COLOR,		"top_color", -1},
		{0xA2F4C9D1, T_CHUNK_STRING,	"top_texture", -1},
		{0x91EDCFE2, T_CHUNK_INT,		"cookie_cut", -1},
		{0x95F4C2D2, T_CHUNK_INT,		"sphere_map_the_top", -1},
		{0x9DF2CFD2, T_CHUNK_COLOR,		"side_color", -1},
		{0xA2F4C9D2, T_CHUNK_STRING,	"side_texture", -1},
		{0x9C00C0D1, T_CHUNK_INT,		"transparency", -1},
		{0x99F2BEDD, T_CHUNK_FLOAT,		"top_height", -1},
		{0x95F2D0DD, T_CHUNK_FLOAT,		"bottom_height", -1},
		{0x99E8BED8, T_CHUNK_VALUELIST,	"material_type", -1, &VL_MATERIAL},
		{0x96F4C2D2, T_CHUNK_INT,		"sphere_map_the_side", -1},
		{0x97F2C4DF, T_CHUNK_INT,		"flat_shading", -1},
		{0x9100C6D5, T_CHUNK_INT,		"surface_is_a_playfield", -1},
		{0x9DFBCDD3, T_CHUNK_INT,		"reflects_off_playfield", -1},
		{0x9A00C5E0, T_CHUNK_STRING,	"enamel_map", -1},
		{0x95E9BED3, T_CHUNK_INT,		"reflect_texture", -1},
		{0x97ECBFD3, T_CHUNK_STRING,	"playfield", -1},
		{0x99F2C0E1, T_CHUNK_INT,		"dropped", -1},
		{0x9DF5C3E2, T_CHUNK_INT,		"collidable", -1},
		{0x97FDC4D3, T_CHUNK_INT,		"render_object", -1},
		{0x95EBCDDD, T_CHUNK_INT,		"generate_hit_event", -1},
		{0x95F3C2D2, T_CHUNK_CHUNKLIST, "shape_point", -1},
		{0x9BFCCFCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0xA1EDC5D2, T_CHUNK_INT,		"smooth", -1},
		{0xA2F3C6D2, T_CHUNK_INT,		"unknown_1", -1},
		{0xA8FCC6D2, T_CHUNK_INT,		"unknown_2", -1},
		{0x99E9BEE4, T_CHUNK_INT,		"automatic_texture_coordinate", -1},
		{0x9AFEBAD1, T_CHUNK_FLOAT,		"texture_coordinate", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC,	"end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_2);


	static ChunkDescriptor CHUNKS_ELEMENT_3_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0x9BFCCFCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0x9BFCCFDD, T_CHUNK_VECTOR2D,	"glow_center", -1},
		{0xA3EFBDD2, T_CHUNK_STRING,	"surface", -1},
		{0x96F3CDD9, T_CHUNK_STRING,	"lens_texture", -1},
		{0x9D00C9E1, T_CHUNK_INT,		"diameter", -1},
		{0x9DF2CFD9, T_CHUNK_COLOR,		"lit_color", -1},
		{0xA6ECBFE4, T_CHUNK_INT,		"auto_set_unlit_color", -1},
		{0x9DF2CFD0, T_CHUNK_COLOR,		"unlit_color", -1},
		{0xA5F8BBE3, T_CHUNK_INT,		"border_width", -1},
		{0x9DF2CFE3, T_CHUNK_COLOR,		"border_color", -1},
		{0x91EDCFE2, T_CHUNK_INT,		"cookie_cut", -1},
		{0x96FDD1D3, T_CHUNK_INT,		"glow_radius", -1},
		{0x9600BED2, T_CHUNK_INT,		"state", -1},
		{0x95F3C9E3, T_CHUNK_INT,		"blink_interval", -1},
		{0x9600C2E3, T_CHUNK_STRING,	"blink_pattern", -1},
		{0xA6F2C6D3, T_CHUNK_INT,		"unknown_1", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC,	"end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_3);

	static ChunkDescriptor CHUNKS_ELEMENT_4_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0x9BFCCFCF, T_CHUNK_VECTOR2D,	"texture_position", -1},
		{0x9BFCCFDD, T_CHUNK_VECTOR2D,	"halo_position", -1},
		{0x96F3CDD9, T_CHUNK_STRING,	"lens_texture", -1},
		{0xB9ADAAAD, T_CHUNK_STRING,	"surface", -1},
		{0x9DF2CFD9, T_CHUNK_COLOR,		"lit_color", -1},
		{0xA6ECBFE4, T_CHUNK_INT,		"auto_set_unlit_color", -1},
		{0x9DF2CFD0, T_CHUNK_COLOR,		"unlit_color", -1},
		{0xA5F8BBE3, T_CHUNK_INT,		"border_width", -1},
		{0x9DF2CFE3, T_CHUNK_COLOR,		"border_color", -1},
		{0x91EDCFE2, T_CHUNK_INT,		"cookie_cut", -1},
		{0x96FDD1D3, T_CHUNK_INT,		"glow_radius", -1},
		{0x9600BED2, T_CHUNK_INT,		"state", -1},
		{0x95F3C9E3, T_CHUNK_INT,		"blink_interval", -1},
		{0x9600C2E3, T_CHUNK_STRING,	"blink_pattern", -1},
		{0xA6F2C6D3, T_CHUNK_INT,		"unknown_1", -1},
		{0x95F3C2D2, T_CHUNK_CHUNKLIST, "shape_point", -1},
		{0x9BFCCFCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0xA1EDC5D2, T_CHUNK_INT,		"smooth", -1},
		{0xA2F3C6D2, T_CHUNK_INT,		"unknown_2", -1},
		{0xA8FCC6D2, T_CHUNK_INT,		"unknown_3", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC,	"end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_4);

	static ChunkDescriptor CHUNKS_ELEMENT_6_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0x9BFCCFCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0x9DFDC3D8, T_CHUNK_STRING,	"model", -1},
		{0xA3EFBDD2, T_CHUNK_STRING,	"surface", -1},
		{0xA300C5DC, T_CHUNK_STRING,	"texture", -1},
		{0x99F4C2D2, T_CHUNK_INT,		"sphere_mapping", -1},
		{0x97F5C3E2, T_CHUNK_COLOR,		"color", -1},
		{0xA8F3C0D6, T_CHUNK_INT,		"mask_as_ornamental", -1},
		{0xA8EDC3D3, T_CHUNK_INT,		"rotation", -1},
		{0x96E8C0E2, T_CHUNK_INT,		"crystal", -1},
		{0x9DFBCDD3, T_CHUNK_INT,		"reflects_off_playfield", -1},
		{0xA5F3BFDD, T_CHUNK_STRING,	"sound_when_hit", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC, "end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_6);

	static ChunkDescriptor CHUNKS_ELEMENT_7_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0x9BFCCFCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0x9DFDC3D8, T_CHUNK_STRING,	"model", -1},
		{0xA3EFBDD2, T_CHUNK_STRING,	"surface", -1},
		{0xA300C5DC, T_CHUNK_STRING,	"texture", -1},
		{0x97F5C3E2, T_CHUNK_COLOR,		"color", -1},
		{0xA900BED2, T_CHUNK_INT,		"start_angle", -1},
		{0xA1FABED2, T_CHUNK_INT,		"strength", -1},
		{0xA2EABFE4, T_CHUNK_INT,		"swing", -1},
		{0x9700C6E0, T_CHUNK_INT,		"elasticity", -1},
		{0x9BEEBDDF, T_CHUNK_STRING,	"flipper_up_sound", -1},
		{0x9BEECEDF, T_CHUNK_STRING,	"flipper_down_sound", -1},
		{0x9DFBCDD3, T_CHUNK_INT,		"reflects_off_playfield", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC,	"end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_7);

	static ChunkDescriptor CHUNKS_ELEMENT_8_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0x9BFCCFCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0xA5F2C5E2, T_CHUNK_STRING,	"cap_model", -1},
		{0xA5F2C5E3, T_CHUNK_STRING,	"base_model", -1},
		{0xA3EFBDD2, T_CHUNK_STRING,	"surface", -1},
		{0x96F3CDD9, T_CHUNK_STRING,	"cap_texture", -1},
		{0xA0EED1D5, T_CHUNK_INT,		"passive", -1},
		{0x9EEED1DD, T_CHUNK_INT,		"trigger_skirt", -1},
		{0x9AFEC7D2, T_CHUNK_COLOR,		"skirt_color", -1},
		{0x9DF2CFD9, T_CHUNK_COLOR,		"lit_color", -1},
		{0xA6ECBFE4, T_CHUNK_INT,		"auto_set_unlit_color", -1},
		{0xA1FDC0D6, T_CHUNK_INT,		"ordered_halo_glow", -1},
		{0x9DF2CFD0, T_CHUNK_COLOR,		"unlit_color", -1},
		{0x9DF2CFE3, T_CHUNK_COLOR,		"base_color", -1},
		{0xA8EDC3D3, T_CHUNK_INT,		"unknown_1", -1},
		{0x96E8C0E2, T_CHUNK_INT,		"crystal", -1},
		{0x9DFBCDD3, T_CHUNK_INT,		"reflects_off_playfield", -1},
		{0xA1FABED2, T_CHUNK_INT,		"strength", -1},
		{0xA5F3BFE3, T_CHUNK_STRING,	"solenoid_sound", -1},
		{0x95F9BBDF, T_CHUNK_INT,		"flash_when_hit", -1},
		{0x9600BED2, T_CHUNK_INT,		"state", -1},
		{0x95F3C9E3, T_CHUNK_INT,		"blink_interval", -1},
		{0x9600C2E3, T_CHUNK_STRING,	"blink_pattern", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC,	"end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_8);

	static ChunkDescriptor CHUNKS_ELEMENT_10_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0x9BFCCFCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0x9DFDC3D8, T_CHUNK_STRING,	"model", -1},
		{0xA3EFBDD2, T_CHUNK_STRING,	"surface", -1},
		{0xA300C5DC, T_CHUNK_STRING,	"texture", -1},
		{0x97F5C3E2, T_CHUNK_COLOR,		"color", -1},
		{0xA8EDC3D3, T_CHUNK_INT,		"rotation", -1},
		{0x9DFBCDD3, T_CHUNK_INT,		"reflects_off_playfield", -1},
		{0xA5F3BFD1, T_CHUNK_STRING,	"sound_when_hit", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC,	"end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_10);

	static ChunkDescriptor CHUNKS_ELEMENT_11_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0x9BFCCFCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0x9DFDC3D8, T_CHUNK_STRING,	"model", -1},
		{0xA3EFBDD2, T_CHUNK_STRING,	"surface", -1},
		{0xA300C5DC, T_CHUNK_STRING,	"texture", -1},
		{0x97F5C3E2, T_CHUNK_COLOR,		"color", -1},
		{0xA8EDC3D3, T_CHUNK_INT,		"rotation", -1},
		{0x9DFBCDD3, T_CHUNK_INT,		"reflects_off_playfield", -1},
		{0x9035D306, T_CHUNK_STRING,	"sound_when_hit", -1},
		{0x9035D2F8, T_CHUNK_STRING,	"sound_when_reset", -1},
		{0x8035E308, T_CHUNK_INT,		"bank_count", -1},
		{0x9133D308, T_CHUNK_INT,		"bank_spacing", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC,	"end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_11);

	static ChunkDescriptor CHUNKS_ELEMENT_12_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0x9BFCCFCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0x9DFDC3D8, T_CHUNK_STRING,	"model", -1},
		{0xA300C5DC, T_CHUNK_STRING,	"texture", -1},
		{0x97F5C3E2, T_CHUNK_COLOR,		"plunger_color", -1},
		{0x9DF2CFE2, T_CHUNK_COLOR,		"face_plate_color", -1},
		{0xA1FABED2, T_CHUNK_INT,		"strength", -1},
		{0x9DFBCDD3, T_CHUNK_INT,		"reflects_off_playfield", -1},
		{0x95ECCFCF, T_CHUNK_INT,		"include_v_cut", -1},
		{0x90E9CFCF, T_CHUNK_VECTOR2D,	"v_cut_position", -1},
		{0x9BFCC6CF, T_CHUNK_INT,		"v_cut_lenght", -1},
		{0xA2F4C9CF, T_CHUNK_STRING,	"v_cut_texture", -1},
		{0x9DF2CFCF, T_CHUNK_COLOR,		"v_cut_color", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC,	"end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_12);

	static ChunkDescriptor CHUNKS_ELEMENT_13_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0x9BFCCFCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0xA4F1B9D1, T_CHUNK_INT,		"subtype", -1},
		{0xA3EFBDD2, T_CHUNK_STRING,	"surface", -1},
		{0x96FBCCD6, T_CHUNK_INT,		"offset", -1},
		{0x97F5C3E2, T_CHUNK_COLOR,		"color", -1},
		{0x9700C6E0, T_CHUNK_INT,		"elasticity", -1},
		{0x9DFBCDD3, T_CHUNK_INT,		"reflects_off_playfield", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC, "end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_13);

	static ChunkDescriptor CHUNKS_ELEMENT_14_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0xA3EFBDD2, T_CHUNK_STRING,	"surface", -1},
		{0x9DF2CFD9, T_CHUNK_COLOR,		"color", -1},
		{0x96FBCCD6, T_CHUNK_INT,		"offset", -1},
		{0x9DFBCDD3, T_CHUNK_INT,		"reflects_off_playfield", -1},
		{0xA1FABED2, T_CHUNK_INT,		"strength", -1},
		{0x9700C6E0, T_CHUNK_INT,		"elasticity", -1},
		{0xA5F3BFD2, T_CHUNK_STRING,	"sound_slingshot", -1},
		{0x95F3C2D2, T_CHUNK_CHUNKLIST,	"shape_point", -1},
		{0x9BFCCFCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0xA1EDC5D2, T_CHUNK_INT,		"smooth", -1},
		{0xA2F3C6D2, T_CHUNK_INT,		"slingshot", -1},
		{0xA8FCC6D2, T_CHUNK_INT,		"single_leaf", -1},
		{0x95F3BCE0, T_CHUNK_INT,		"event_id", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC,	"end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_14);

	static ChunkDescriptor CHUNKS_ELEMENT_15_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0x9BFCCFCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0x9DFDC3D8, T_CHUNK_STRING,	"model", -1},
		{0xA3EFBDD2, T_CHUNK_STRING,	"surface", -1},
		{0xA300C5DC, T_CHUNK_STRING,	"texture", -1},
		{0x99F4C2D2, T_CHUNK_INT,		"sphere_mapping", -1},
		{0x97F5C3E2, T_CHUNK_COLOR,		"color", -1},
		{0xA8F3C0D6, T_CHUNK_INT,		"mask_as_ornamental", -1},
		{0xA8EDC3D3, T_CHUNK_INT,		"rotation", -1},
		{0x96FBCCD6, T_CHUNK_INT,		"offset", -1},
		{0x9DFBCDD3, T_CHUNK_INT,		"reflects_off_playfield", -1},
		{0x97ECBFD3, T_CHUNK_STRING,	"playfield", -1},
		{0x9A00C5E0, T_CHUNK_STRING,	"unknown_1", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC,	"end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_15);

	static ChunkDescriptor CHUNKS_ELEMENT_16_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0xA3EFBDD2, T_CHUNK_STRING,	"surface", -1},
		{0x91EDBEE1, T_CHUNK_INT,		"display_image_in_editor", -1},
		{0x9DF2CFD1, T_CHUNK_COLOR,		"top_color", -1},
		{0xA2F4C9D1, T_CHUNK_STRING,	"top_texture", -1},
		{0x91EDCFE2, T_CHUNK_INT,		"cookie_cut", -1},
		{0x95F4C2D2, T_CHUNK_INT,		"sphere_map_the_top", -1},
		{0x9DF2CFD2, T_CHUNK_COLOR,		"side_color", -1},
		{0xA2F4C9D2, T_CHUNK_STRING,	"side_texture", -1},
		{0x96F4C2D2, T_CHUNK_INT,		"sphere_map_the_side", -1},
		{0x97F2C4DF, T_CHUNK_INT,		"flat_shading", -1},
		{0x9C00C0D1, T_CHUNK_INT,		"transparency", -1},
		{0xA2F8CDDD, T_CHUNK_FLOAT,		"top_height", -1},
		{0x95FDC9CE, T_CHUNK_FLOAT,		"bottom_height", -1},
		{0x99E8BED8, T_CHUNK_VALUELIST,	"material_type", -1, &VL_MATERIAL},
		{0x9DFBCDD3, T_CHUNK_INT,		"reflects_off_playfield", -1},
		{0x99F2C0E1, T_CHUNK_INT,		"dropped", -1},
		{0x9DF5C3E2, T_CHUNK_INT,		"collidable", -1},
		{0x97FDC4D3, T_CHUNK_INT,		"render_object", -1},
		{0x95EBCDDD, T_CHUNK_INT,		"generate_hit_event", -1},
		{0x95F3C2D2, T_CHUNK_CHUNKLIST, "shape_point", -1},
		{0x9BFCCFCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0xA1EDC5D2, T_CHUNK_INT,		"smooth", -1},
		{0xA2F3C6D2, T_CHUNK_INT,		"unknown_1", -1},
		{0xA8FCC6D2, T_CHUNK_INT,		"unknown_2", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC,	"end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_16);

	static ChunkDescriptor CHUNKS_ELEMENT_17_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0x9BFCCFCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0xA800C4E0, T_CHUNK_INT,		"enabled", -1},
		{0x97EDC4DC, T_CHUNK_INT,		"timer_interval", -1},
		{0xA6F2C6D3, T_CHUNK_INT,		"unknown_1", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC,	"end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_17);

	static ChunkDescriptor CHUNKS_ELEMENT_18_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0x9BFCCFCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0xA3EFBDD2, T_CHUNK_STRING,	"surface", -1},
		{0xA300C5DC, T_CHUNK_STRING,	"texture", -1},
		{0xA8EDC3D3, T_CHUNK_INT,		"rotation", -1},
		{0xA2F8CDDD, T_CHUNK_INT,		"width", -1},
		{0x95FDC9CE, T_CHUNK_INT,		"height", -1},
		{0x9C00C0D1, T_CHUNK_INT,		"transparency", -1},
		{0x97F5C3E2, T_CHUNK_COLOR,		"color", -1},
		{0xA6F2C6D3, T_CHUNK_INT,		"unknown_1", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC,	"end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_18);

	static ChunkDescriptor CHUNKS_ELEMENT_19_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0x9BFCCFCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0x9DFDC3D8, T_CHUNK_STRING,	"model", -1},
		{0xA3EFBDD2, T_CHUNK_STRING,	"surface", -1},
		{0xA300C5DC, T_CHUNK_STRING,	"texture", -1},
		{0x97F5C3E2, T_CHUNK_COLOR,		"color", -1},
		{0xA8EDC3D3, T_CHUNK_INT,		"rotation", -1},
		{0x99E8BEDA, T_CHUNK_INT,		"type", -1},
		{0xA1FABED2, T_CHUNK_INT,		"strength", -1},
		{0xA5F2C5D3, T_CHUNK_INT,		"render_model", -1},
		{0xA5F3BFDA, T_CHUNK_STRING,	"sound_when_hit", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC, "end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_19);

	static ChunkDescriptor CHUNKS_ELEMENT_20_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0x9BFCCFCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0x9DFDC3D8, T_CHUNK_STRING,	"model", -1},
		{0xA3EFBDD2, T_CHUNK_STRING,	"surface", -1},
		{0xA300C5DC, T_CHUNK_STRING,	"texture", -1},
		{0x97F5C3E2, T_CHUNK_COLOR,		"color", -1},
		{0xA8EDC3D3, T_CHUNK_INT,		"rotation", -1},
		{0x96FBCCD6, T_CHUNK_INT,		"offset", -1},
		{0x96E8C0E2, T_CHUNK_INT,		"crystal", -1},
		{0x9A00C5E0, T_CHUNK_STRING,	"unknown_1", -1},
		{0x9DFBCDD3, T_CHUNK_INT,		"reflects_off_playfield", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC, "end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_20);

	static ChunkDescriptor CHUNKS_ELEMENT_21_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0x9BFCCFCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0x9DFDC3D8, T_CHUNK_STRING,	"model", -1},
		{0xA3EFBDD2, T_CHUNK_STRING,	"surface", -1},
		{0xA300C5DC, T_CHUNK_STRING,	"texture", -1},
		{0x97F5C3E2, T_CHUNK_COLOR,		"color", -1},
		{0xA8EDC3D3, T_CHUNK_INT,		"rotation", -1},
		{0x96FBCCD6, T_CHUNK_INT,		"offset", -1},
		{0x9700C6E0, T_CHUNK_INT,		"elasticity", -1},
		{0x9DFBCDD3, T_CHUNK_INT,		"reflects_off_playfield", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC, "end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_21);

	static ChunkDescriptor CHUNKS_ELEMENT_22_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0x9BFCCFCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0x9DFDC3D8, T_CHUNK_STRING,	"model", -1},
		{0xA3EFBDD2, T_CHUNK_STRING,	"surface", -1},
		{0xA300C5DC, T_CHUNK_STRING,	"texture", -1},
		{0x99F4C2D2, T_CHUNK_INT,		"sphere_mapping", -1},
		{0x97F5C3E2, T_CHUNK_COLOR,		"color", -1},
		{0xA8EDC3D3, T_CHUNK_INT,		"rotation", -1},
		{0xA3F1C3D2, T_CHUNK_INT,		"sits_on_playfield", -1},
		{0xA5F2C5D3, T_CHUNK_INT,		"render_model", -1},
		{0x9DFBCDD3, T_CHUNK_INT,		"reflects_off_playfield", -1},
		{0xA5F3BFD1, T_CHUNK_STRING,	"sound_when_hit", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC,	"end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_22);

	static ChunkDescriptor CHUNKS_ELEMENT_23_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0x9BFCCFCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0x9DFDC3D8, T_CHUNK_STRING,	"model", -1},
		{0xA3EFBDD2, T_CHUNK_STRING,	"surface", -1},
		{0x9DF2CFD9, T_CHUNK_INT,		"lit_color", -1},
		{0xA6ECBFE4, T_CHUNK_INT,		"auto_set_unlit_color", -1},
		{0x9DF2CFD0, T_CHUNK_INT,		"unlit_color", -1},
		{0xA8EDC3D3, T_CHUNK_INT,		"rotation", -1},
		{0xA1FDC0D6, T_CHUNK_INT,		"ordered_halo_glow", -1},
		{0x9DFBCDD3, T_CHUNK_INT,		"reflects_off_playfield", -1},
		{0x97ECBFD3, T_CHUNK_STRING,	"playfield", -1},
		{0x9600BED2, T_CHUNK_INT,		"state", -1},
		{0x95F3C9E3, T_CHUNK_INT,		"blink_interval", -1},
		{0x9600C2E3, T_CHUNK_STRING,	"blink_pattern", -1},
		{0x9134D9F8, T_CHUNK_INT,		"unknown_1", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC,	"end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_23);

	static ChunkDescriptor CHUNKS_ELEMENT_24_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0xA3EFBDD2, T_CHUNK_STRING,	"surface", -1},
		{0x97F5C3E2, T_CHUNK_COLOR,		"color", -1},
		{0xA4FAC5DC, T_CHUNK_STRING,	"texture", -1},
		{0x99F4C2D2, T_CHUNK_INT,		"sphere_map", -1},
		{0xA2F8CDDD, T_CHUNK_INT,		"height", -1},
		{0x95FDC9CE, T_CHUNK_INT,		"width", -1},
		{0xA8F3C0D6, T_CHUNK_INT,		"mark_as_ornamental", -1},
		{0x9DFBCDD3, T_CHUNK_INT,		"reflects_off_playfield", -1},
		{0x95F3C2D2, T_CHUNK_CHUNKLIST, "shape_point", -1},
		{0x9BFCCFCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0xA1EDC5D2, T_CHUNK_INT,		"smooth", -1},
		{0xA2F3C6D2, T_CHUNK_INT,		"unknown_1", -1},
		{0xA8FCC6D2, T_CHUNK_INT,		"unknown_2", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC,	"end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_24);

	static ChunkDescriptor CHUNKS_ELEMENT_25_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0xB8EFCDCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0x90EDBFD3, T_CHUNK_INT,		"number_style", -1},
		{0xA700CCD3, T_CHUNK_INT,		"render_face_plate", -1},
		{0x9DF2CFDF, T_CHUNK_COLOR,		"face_color", -1},
		{0x95F3CFD3, T_CHUNK_INT,		"reels", -1},
		{0xA700C2D2, T_CHUNK_INT,		"spacing", -1},
		{0xA6F1BFDF, T_CHUNK_INT,		"face_spacing", -1},
		{0xA2F8CDDD, T_CHUNK_INT,		"height", -1},
		{0x95FDC9CE, T_CHUNK_INT,		"width", -1},
		{0xA5F1BFD2, T_CHUNK_INT,		"spin_speed", -1},
		{0x9BECC3D2, T_CHUNK_STRING,	"spin_sound", -1},
		{0x9DF2CFD9, T_CHUNK_COLOR,		"lit_color", -1},
		{0xA6ECBFE4, T_CHUNK_INT,		"auto_set_unlit_color", -1},
		{0x9DF2CFD0, T_CHUNK_COLOR,		"unlit_color", -1},
		{0x9600BED2, T_CHUNK_INT,		"state", -1},
		{0x95F3C9E3, T_CHUNK_INT,		"blink_interval", -1},
		{0x9600C2E3, T_CHUNK_STRING,	"blink_pattern", -1},
		{0xA6F2C6D3, T_CHUNK_INT,		"unknown_1", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC,	"end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_25);

	static ChunkDescriptor CHUNKS_ELEMENT_26_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0xB8EFCDCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0x90EDBFD3, T_CHUNK_INT,		"number_style", -1},
		{0xA700CCD3, T_CHUNK_INT,		"render_face_plate", -1},
		{0x9DF2CFDF, T_CHUNK_COLOR,		"face_color", -1},
		{0x95F3CFD3, T_CHUNK_INT,		"reels", -1},
		{0xA700C2D2, T_CHUNK_INT,		"spacing", -1},
		{0xA6F1BFDF, T_CHUNK_INT,		"face_spacing", -1},
		{0xA2F8CDDD, T_CHUNK_INT,		"height", -1},
		{0x95FDC9CE, T_CHUNK_INT,		"width", -1},
		{0xA5F1BFD2, T_CHUNK_INT,		"spin_speed", -1},
		{0x9BECC3D2, T_CHUNK_STRING,	"spin_sound", -1},
		{0x9DF2CFD9, T_CHUNK_COLOR,		"lit_color", -1},
		{0xA6ECBFE4, T_CHUNK_INT,		"auto_set_unlit_color", -1},
		{0x9DF2CFD0, T_CHUNK_COLOR,		"unlit_color", -1},
		{0x9600BED2, T_CHUNK_INT,		"state", -1},
		{0x95F3C9E3, T_CHUNK_INT,		"blink_interval", -1},
		{0x9600C2E3, T_CHUNK_STRING,	"blink_pattern", -1},
		{0xA6F2C6D3, T_CHUNK_INT,		"unknown_1", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC,	"end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_26);

	static ChunkDescriptor CHUNKS_ELEMENT_27_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0x9BEFBED3, T_CHUNK_INT,		"render_onto_translite", -1},
		{0xA3EFBDD2, T_CHUNK_STRING,	"surface", -1},
		{0x9BFCCFCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0x97F5C3E2, T_CHUNK_COLOR,		"color", -1},
		{0xA2F8CDDD, T_CHUNK_INT,		"0x95FDC9CE", -1},
		{0x95FDC9CE, T_CHUNK_INT,		"width", -1},
		{0xA8EDC3D3, T_CHUNK_INT,		"rotation", -1},
		{0x96F5C5DC, T_CHUNK_STRING,	"image_list", -1},
		{0x95F3C9D0, T_CHUNK_INT,		"update_interval", -1},
		{0xA6F2C6D3, T_CHUNK_INT,		"unknown_1", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC,	"end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_27);

	static ChunkDescriptor CHUNKS_ELEMENT_29_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0x9BFCCFCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0x9DFDC3D8, T_CHUNK_STRING,	"model", -1},
		{0xA3EFBDD2, T_CHUNK_STRING,	"surface", -1},
		{0xA5F2C5D3, T_CHUNK_INT,		"render_model", -1},
		{0x96F3CDD9, T_CHUNK_STRING,	"lens_texture", -1},
		{0x96FDD1D3, T_CHUNK_INT,		"glow_radius", -1},
		{0xA1FDC0D6, T_CHUNK_INT,		"ordered_halo_glow", -1},
		{0x9DF2CFD9, T_CHUNK_COLOR,		"lit_color", -1},
		{0xA6ECBFE4, T_CHUNK_INT,		"auto_set_unlit_color", -1},
		{0x9DF2CFD0, T_CHUNK_COLOR,		"unlit_color", -1},
		{0x96E8C0E2, T_CHUNK_INT,		"crystal", -1},
		{0xA8EDC3D3, T_CHUNK_INT,		"rotation", -1},
		{0x9DFBCDD3, T_CHUNK_INT,		"reflects_off_playfield", -1},
		{0x9600BED2, T_CHUNK_INT,		"state", -1},
		{0x95F3C9E3, T_CHUNK_INT,		"blink_interval", -1},
		{0x9600C2E3, T_CHUNK_STRING,	"blink_pattern", -1},
		{0xA6F2C6D3, T_CHUNK_INT,		"unknown_1", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC, "end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_29);

	static ChunkDescriptor CHUNKS_ELEMENT_30_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0x9BFCCFCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0x9DFDC3D8, T_CHUNK_STRING,	"model", -1},
		{0xA3EFBDD2, T_CHUNK_STRING,	"surface", -1},
		{0xA300C5DC, T_CHUNK_STRING,	"texture", -1},
		{0x99F4C2D2, T_CHUNK_INT,		"sphere_mapping", -1},
		{0x97F5C3E2, T_CHUNK_COLOR,		"color", -1},
		{0xA8EDC3D3, T_CHUNK_INT,		"rotation", -1},
		{0x9100BBD6, T_CHUNK_INT,		"one_way", -1},
		{0x9DFBCDD3, T_CHUNK_INT,		"reflects_off_playfield", -1},
		{0x97ECBFD3, T_CHUNK_STRING,	"playfield", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC,	"end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_30);

	static ChunkDescriptor CHUNKS_ELEMENT_31_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0x9BFCCFCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0x9DFDC3D8, T_CHUNK_STRING,	"model", -1},
		{0xA3EFBDD2, T_CHUNK_STRING,	"surface", -1},
		{0xA300C5DC, T_CHUNK_STRING,	"texture", -1},
		{0x97F5C3E2, T_CHUNK_COLOR,		"color", -1},
		{0xA8EDC3D3, T_CHUNK_INT,		"rotation", -1},
		{0x9DFBCDD3, T_CHUNK_INT,		"reflects_off_playfield", -1},
		{0x99F4D1E1, T_CHUNK_INT,		"damping", -1},
		{0x97ECBFD3, T_CHUNK_STRING,	"playfield", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC,	"end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_31);

	static ChunkDescriptor CHUNKS_ELEMENT_33_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0x9BFCCFCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0x9DFDC3D8, T_CHUNK_STRING,	"model", -1},
		{0xA3EFBDD2, T_CHUNK_STRING,	"surface", -1},
		{0xA300C5DC, T_CHUNK_STRING,	"texture", -1},
		{0x99F4C2D2, T_CHUNK_INT,		"sphere_mapping", -1},
		{0x97F5C3E2, T_CHUNK_COLOR,		"color", -1},
		{0x9C00C0D1, T_CHUNK_INT,		"transparency", -1},
		{0xA8EDC3D3, T_CHUNK_INT,		"rotation", -1},
		{0x96FBCCD6, T_CHUNK_INT,		"offset", -1},
		{0x9DFBCDD3, T_CHUNK_INT,		"reflects_off_playfield", -1},
		{0x95F3C9D0, T_CHUNK_INT,		"update_interval", -1},
		{0x97ECBFD3, T_CHUNK_STRING,	"playfield", -1},
		{0x9035D306, T_CHUNK_STRING,	"sound_when_hit", -1},
		{0x9035D2F8, T_CHUNK_STRING,	"sound_when_reset", -1},
		{0x8035E308, T_CHUNK_INT,		"bank_count", -1},
		{0x9133D308, T_CHUNK_INT,		"bank_spacing", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC,	"end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_33);

	static ChunkDescriptor CHUNKS_ELEMENT_34_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0xB8EFCDCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0x9BFCCFCF, T_CHUNK_VECTOR2D,	"virtual_center", -1},
		{0x95EEC6D9, T_CHUNK_STRING,	"light_list", -1},
		{0x95F3C9D0, T_CHUNK_INT,		"update_interval", -1},
		{0xA1EECCDF, T_CHUNK_INT,		"flasher_blink", -1},
		{0x9BF8BDDF, T_CHUNK_INT,		"blink_interval", -1},
		{0x96EFBEDC, T_CHUNK_INT,		"include_translite_light", -1},
		{0xA6F2C6D3, T_CHUNK_INT,		"unknown_3", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC, "end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_34);

	static ChunkDescriptor CHUNKS_ELEMENT_37_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0xB8EFCDCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0x99E8BED2, T_CHUNK_INT,		"segment_style", -1},
		{0x95F3CFD2, T_CHUNK_INT,		"digits", -1},
		{0x95FDC9CE, T_CHUNK_INT,		"size", -1},
		{0xA700C2D2, T_CHUNK_INT,		"spacing", -1},
		{0xA6F1BFDF, T_CHUNK_INT,		"face_spacing", -1},
		{0x9BF8C2D0, T_CHUNK_INT,		"update_interval", -1},
		{0xA2F8C6E4, T_CHUNK_INT,		"alignment", -1},
		{0x99EED0D2, T_CHUNK_INT,		"slow_blink_speed", -1},
		{0x99EED0DF, T_CHUNK_INT,		"fast_blink_speed", -1},
		{0x90F5CEE3, T_CHUNK_INT,		"boredom_delay", -1},
		{0x9DF2CFD9, T_CHUNK_COLOR,		"lit_color", -1},
		{0xA6F2C6D3, T_CHUNK_INT,		"unknown_1", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC, "end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_37);

	static ChunkDescriptor CHUNKS_ELEMENT_38_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0xB8EFCDCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0x99E8BED2, T_CHUNK_INT,		"segment_style", -1},
		{0x95F3CFD2, T_CHUNK_INT,		"digits", -1},
		{0x95FDC9CE, T_CHUNK_INT,		"size", -1},
		{0xA700C2D2, T_CHUNK_INT,		"spacing", -1},
		{0xA6F1BFDF, T_CHUNK_INT,		"face_spacing", -1},
		{0x9BF8C2D0, T_CHUNK_INT,		"update_interval", -1},
		{0xA2F8C6E4, T_CHUNK_INT,		"alignment", -1},
		{0x99EED0D2, T_CHUNK_INT,		"slow_blink_speed", -1},
		{0x99EED0DF, T_CHUNK_INT,		"fast_blink_speed", -1},
		{0x90F5CEE3, T_CHUNK_INT,		"boredom_delay", -1},
		{0x9DF2CFD9, T_CHUNK_COLOR,		"lit_color", -1},
		{0xA6F2C6D3, T_CHUNK_INT,		"unknown_1", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC, "end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_38);

	static ChunkDescriptor CHUNKS_ELEMENT_39_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0xB8EFCDCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0x97F5C3E2, T_CHUNK_COLOR,		"matrix_color", -1},
		{0x9E00CFD2, T_CHUNK_INT,		"scale", -1},
		{0x99E8BEE1, T_CHUNK_INT,		"type", -1},
		{0x9BF8C2D0, T_CHUNK_INT,		"update_interval", -1},
		{0xA2F8C6E4, T_CHUNK_INT,		"alignment", -1},
		{0x99EED0D2, T_CHUNK_INT,		"slow_blink_speed", -1},
		{0x99EED0DF, T_CHUNK_INT,		"fast_blink_speed", -1},
		{0xA6F2C6D3, T_CHUNK_INT,		"unknown_1", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC, "end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_39);

	static ChunkDescriptor CHUNKS_ELEMENT_40_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0xB8EFCDCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0x97F5C3E2, T_CHUNK_COLOR,		"matrix_color", -1},
		{0x9E00CFD2, T_CHUNK_INT,		"scale", -1},
		{0x99E8BEE1, T_CHUNK_INT,		"type", -1},
		{0x9BF8C2D0, T_CHUNK_INT,		"update_interval", -1},
		{0xA2F8C6E4, T_CHUNK_INT,		"alignment", -1},
		{0x99EED0D2, T_CHUNK_INT,		"slow_blink_speed", -1},
		{0x99EED0DF, T_CHUNK_INT,		"fast_blink_speed", -1},
		{0xA6F2C6D3, T_CHUNK_INT,		"unknown_1", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC, "end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_40);

	static ChunkDescriptor CHUNKS_ELEMENT_43_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0x9BFCCFCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0x9DFDC3D8, T_CHUNK_STRING,	"model", -1},
		{0xA3EFBDD2, T_CHUNK_STRING,	"surface", -1},
		{0xA300C5DC, T_CHUNK_STRING,	"texture", -1},
		{0x99F4C2D2, T_CHUNK_INT,		"sphere_mapping", -1},
		{0x97F5C3E2, T_CHUNK_COLOR,		"color", -1},
		{0xA900BED2, T_CHUNK_INT,		"start_angle", -1},
		{0xA8EDC3D3, T_CHUNK_INT,		"rotation", -1},
		{0xA2EABFE4, T_CHUNK_INT,		"swing", -1},
		{0x9DFBCDD3, T_CHUNK_INT,		"reflects_off_playfield", -1},
		{0x9035D306, T_CHUNK_STRING,	"solenoid", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC,	"end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_43);

	static ChunkDescriptor CHUNKS_ELEMENT_44_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0xB8EFCDCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0x97F5C3E2, T_CHUNK_COLOR,		"color", -1},
		{0xA2F8CDDD, T_CHUNK_INT,		"height", -1},
		{0x95FDC9CE, T_CHUNK_INT,		"width", -1},
		{0x96F5C5DC, T_CHUNK_STRING,	"image_list", -1},
		{0x95F3C9D0, T_CHUNK_INT,		"update_interval", -1},
		{0xA6F2C6D3, T_CHUNK_INT,		"unknown_1", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC,	"end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_44);

	static ChunkDescriptor CHUNKS_ELEMENT_46_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0x9BFCCFCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0xA300C5DC, T_CHUNK_STRING,	"texture", -1},
		{0x97F5C3E2, T_CHUNK_COLOR,		"color", -1},
		{0xA8EDC3D3, T_CHUNK_INT,		"rotation", -1},
		{0xA1FABED2, T_CHUNK_INT,		"strength", -1},
		{0x9DFBCDD3, T_CHUNK_INT,		"reflects_off_playfield", -1},
		{0x97ECBFD3, T_CHUNK_STRING,	"playfield", -1},
		{0x95ECCFCF, T_CHUNK_INT,		"include_v_cut", -1},
		{0x90E9CFCF, T_CHUNK_VECTOR2D,	"v_cut_position", -1},
		{0x9BFCC6CF, T_CHUNK_INT,		"v_cut_lenght", -1},
		{0xA2F4C9CF, T_CHUNK_STRING,	"v_cut_texture", -1},
		{0x9DF2CFCF, T_CHUNK_COLOR,		"v_cut_color", -1},
		{0xA5F3BFE4, T_CHUNK_STRING,	"solenoid", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC,	"end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_46);

	static ChunkDescriptor CHUNKS_ELEMENT_49_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0x9BFCCFCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0x9DFDC3D8, T_CHUNK_STRING,	"model", -1},
		{0x96F5C5DC, T_CHUNK_STRING,	"image_list", -1},
		{0x97F5C3E2, T_CHUNK_COLOR,		"color", -1},
		{0xA8EDC3D3, T_CHUNK_INT,		"rotation", -1},
		{0x96FBCCD6, T_CHUNK_INT,		"offset", -1},
		{0x9DFBCDD3, T_CHUNK_INT,		"reflects_off_playfield", -1},
		{0x9035D306, T_CHUNK_STRING,	"sound_motor_whir", -1},
		{0x9035D2F8, T_CHUNK_STRING,	"sound_when_hit", -1},
		{0x95F3CFD3, T_CHUNK_INT,		"target_count", -1},
		{0x90F5CED3, T_CHUNK_INT,		"step_delay", -1},
		{0x95FEBEDF, T_CHUNK_INT,		"firt_target_centered", -1},
		{0x9AEFD1E2, T_CHUNK_INT,		"carrousel_roto_target", -1},
		{0x96FDD1D3, T_CHUNK_INT,		"radius", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC,	"end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_49);

	static ChunkDescriptor CHUNKS_ELEMENT_50_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0x9BFCCFCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0x9DFDC3D8, T_CHUNK_STRING,	"model", -1},
		{0xA3EFBDD2, T_CHUNK_STRING,	"surface", -1},
		{0xA300C5DC, T_CHUNK_STRING,	"texture", -1},
		{0x96E8C0E2, T_CHUNK_INT,		"crystal", -1},
		{0x99F4C2D2, T_CHUNK_INT,		"sphere_mapping", -1},
		{0x97F5C3E2, T_CHUNK_COLOR,		"color", -1},
		{0xA8EDC3D3, T_CHUNK_INT,		"rotation", -1},
		{0x96FBCCD6, T_CHUNK_INT,		"offset", -1},
		{0x9DFBCDD3, T_CHUNK_INT,		"reflects_off_playfield", -1},
		{0x97ECBFD3, T_CHUNK_STRING,	"playfield", -1},
		{0xA5F3BFD2, T_CHUNK_STRING,	"solenoid", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC,	"end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_50);

	static ChunkDescriptor CHUNKS_ELEMENT_51_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0x9BFCCFCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0x9DFDC3D8, T_CHUNK_STRING,	"model", -1},
		{0xA3EFBDD2, T_CHUNK_STRING,	"surface", -1},
		{0xA300C5DC, T_CHUNK_STRING,	"texture", -1},
		{0x99F4C2D2, T_CHUNK_INT,		"sphere_mapping", -1},
		{0x97F5C3E2, T_CHUNK_COLOR,		"color", -1},
		{0xA8EDC3D3, T_CHUNK_FLOAT,		"rotation", -1},
		{0x96FBCCD6, T_CHUNK_INT,		"offset", -1},
		{0x9C00C0D1, T_CHUNK_INT,		"transparency", -1},
		{0x9DECCFE1, T_CHUNK_INT,		"disable_culling", -1},
		{0x9DFBCDD3, T_CHUNK_INT,		"reflects_off_playfield", -1},
		{0x97ECBFD3, T_CHUNK_STRING,	"playfield", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC,	"end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_51);

	static ChunkDescriptor CHUNKS_ELEMENT_53_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0xA3EFBDD2, T_CHUNK_STRING,	"surface", -1},
		{0x97F5C3E2, T_CHUNK_COLOR,		"color", -1},
		{0xA4FAC5DC, T_CHUNK_STRING,	"texture", -1},
		{0xA2F8CAD2, T_CHUNK_INT,		"start_height", -1},
		{0xA2F8CAE0, T_CHUNK_INT,		"end_height", -1},
		{0x96FCBBD3, T_CHUNK_STRING,	"start_model", -1},
		{0xA4FCBBD3, T_CHUNK_STRING,	"end_model", -1},
		{0x9DFBCDD3, T_CHUNK_INT,		"reflects_off_playfield", -1},
		{0x97ECBFD3, T_CHUNK_STRING,	"playfield", -1},
		{0x95F3C2D2, T_CHUNK_CHUNKLIST, "ramp_point", -1},
		{0x9BFCCFCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0xA1EDC5D2, T_CHUNK_INT,		"smooth", -1},
		{0xA4F5C9D8, T_CHUNK_INT,		"left_guide", -1},
		{0xA4F5C2D0, T_CHUNK_INT,		"left_upper_guide", -1},
		{0xA0EFC9D8, T_CHUNK_INT,		"right_guide", -1},
		{0xA0EFC2D0, T_CHUNK_INT,		"right_upper_guide", -1},
		{0x95ECC3D1, T_CHUNK_INT,		"top_wire", -1},
		{0x9AF1CAE0, T_CHUNK_INT,		"mark_as_ramp_end_point", -1},
		{0xA2F3C9D3, T_CHUNK_INT,		"ring_type", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC,	"end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_53);

	static ChunkDescriptor CHUNKS_ELEMENT_54_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0x9BFCCFCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0x9DFDC3D8, T_CHUNK_STRING,	"model", -1},
		{0xA300C5DC, T_CHUNK_STRING,	"texture", -1},
		{0x97F5C3E2, T_CHUNK_COLOR,		"color", -1},
		{0xA8EDC3D3, T_CHUNK_INT,		"rotation", -1},
		{0x9DFBCDD3, T_CHUNK_INT,		"reflects_off_playfield", -1},
		{0xA5F3BFD1, T_CHUNK_STRING,	"sound_when_hit", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC,	"end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_54);

	static ChunkDescriptor CHUNKS_ELEMENT_55_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0xA3EFBDD2, T_CHUNK_STRING,	"surface", -1},
		{0x97F5C3E2, T_CHUNK_COLOR,		"color", -1},
		{0xA3F2C0D5, T_CHUNK_INT,		"ramp_profile", -1},
		{0xA4FAC5DC, T_CHUNK_STRING,	"texture", -1},
		{0xA2F8CAD2, T_CHUNK_INT,		"start_height", -1},
		{0xA5F8BBD2, T_CHUNK_INT,		"start_width", -1},
		{0xA2F8CAE0, T_CHUNK_INT,		"end_height", -1},
		{0xA5F8BBE0, T_CHUNK_INT,		"end_width", -1},
		{0xA2F8CAD9, T_CHUNK_INT,		"left_side_height", -1},
		{0xA2F8CAD3, T_CHUNK_INT,		"right_side_height", -1},
		{0x9DFBCDD3, T_CHUNK_INT,		"reflects_off_playfield", -1},
		{0x97ECBFD3, T_CHUNK_STRING,	"playfield", -1},
		{0x9A00C5E0, T_CHUNK_STRING,	"unknown_1", -1},
		{0x99F4C2D2, T_CHUNK_INT,		"sphere_mapping", -1},
		{0x9C00C0D1, T_CHUNK_INT,		"transparency", -1},
		{0x95F3C2D2, T_CHUNK_CHUNKLIST, "ramp_point", -1},
		{0x9BFCCFCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0xA1EDC5D2, T_CHUNK_INT,		"smooth", -1},
		{0xA4F5C9D8, T_CHUNK_INT,		"unknown_2", -1},
		{0xA4F5C2D0, T_CHUNK_INT,		"unknown_3", -1},
		{0xA0EFC9D8, T_CHUNK_INT,		"unknown_4", -1},
		{0xA0EFC2D0, T_CHUNK_INT,		"unknown_5", -1},
		{0x95ECC3D1, T_CHUNK_INT,		"unknown_6", -1},
		{0x9AF1CAE0, T_CHUNK_INT,		"mark_as_ramp_end_point", -1},
		{0xA2F3C9D3, T_CHUNK_INT,		"unknown_7", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC,	"end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_55);

	static ChunkDescriptor CHUNKS_ELEMENT_56_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0x9BFCCFCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0x9DFDC3D8, T_CHUNK_STRING,	"model", -1},
		{0xA300C5DC, T_CHUNK_STRING,	"texture", -1},
		{0x97F5C3E2, T_CHUNK_COLOR,		"color", -1},
		{0xA1FABED2, T_CHUNK_INT,		"motor_power", -1},
		{0x99F4D1E1, T_CHUNK_INT,		"damping", -1},
		{0x9DFBCDD3, T_CHUNK_INT,		"reflects_off_playfield", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC,	"end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_56);

	static ChunkDescriptor CHUNKS_ELEMENT_57_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0x9BFCCFCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0x9BFCCFDD, T_CHUNK_VECTOR2D,	"glow_center", -1},
		{0xA3EFBDD2, T_CHUNK_STRING,	"surface", -1},
		{0x97F5C3E2, T_CHUNK_COLOR,		"color", -1},
		{0xA2F8CDDD, T_CHUNK_INT,		"height", -1},
		{0x95FDC9CE, T_CHUNK_INT,		"width", -1},
		{0x96F5C5DC, T_CHUNK_STRING,	"image_list", -1},
		{0xA8EDC3D3, T_CHUNK_INT,		"rotation", -1},
		{0x95F3C9D0, T_CHUNK_INT,		"animation_update_interval", -1},
		{0x9E00CAD3, T_CHUNK_INT,		"render_halo_glow", -1},
		{0x96FDD1D3, T_CHUNK_INT,		"glow_radius", -1},
		{0x9DF2CFDD, T_CHUNK_COLOR,		"halo_color", -1},
		{0x9600BED2, T_CHUNK_INT,		"state", -1},
		{0x95F3C9E3, T_CHUNK_INT,		"blink_interval", -1},
		{0x9600C2E3, T_CHUNK_STRING,	"blink_pattern", -1},
		{0xA6F2C6D3, T_CHUNK_INT,		"unknown_1", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC,	"end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_57);

	static ChunkDescriptor CHUNKS_ELEMENT_58_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0x9BFCCFCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0x9DFDC3D8, T_CHUNK_STRING,	"model", -1},
		{0xA3EFBDD2, T_CHUNK_STRING,	"surface", -1},
		{0xA300C5DC, T_CHUNK_STRING,	"texture", -1},
		{0x97F5C3E2, T_CHUNK_COLOR,		"color", -1},
		{0xA8EDC3D3, T_CHUNK_INT,		"rotation", -1},
		{0xA1FABED2, T_CHUNK_INT,		"strength", -1},
		{0xA5F3BFE4, T_CHUNK_STRING,	"solenoid", -1},
		{0x9DFBCDD3, T_CHUNK_INT,		"reflects_off_playfield", -1},
		{0x95ECCFCF, T_CHUNK_INT,		"include_v_cut", -1},
		{0x90E9CFCF, T_CHUNK_VECTOR2D,	"v_cut_position", -1},
		{0x9BFCC6CF, T_CHUNK_INT,		"v_cut_lenght", -1},
		{0xA2F4C9CF, T_CHUNK_STRING,	"v_cut_texture", -1},
		{0x9DF2CFCF, T_CHUNK_COLOR,		"v_cut_color", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC, "end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_58);

	static ChunkDescriptor CHUNKS_ELEMENT_60_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0x9BFCCFCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0x9BFCCFDD, T_CHUNK_VECTOR2D,	"glow_center", -1},
		{0x97F5C3E2, T_CHUNK_COLOR,		"color", -1},
		{0xA2F8CDDD, T_CHUNK_INT,		"height", -1},
		{0x95FDC9CE, T_CHUNK_INT,		"width", -1},
		{0x96F5C5DC, T_CHUNK_STRING,	"image_list", -1},
		{0xA8EDC3D3, T_CHUNK_INT,		"rotation", -1},
		{0x95F3C9D0, T_CHUNK_INT,		"animation_update_interval", -1},
		{0x9E00CAD3, T_CHUNK_INT,		"render_halo_glow", -1},
		{0x96FDD1D3, T_CHUNK_INT,		"glow_radius", -1},
		{0x9DF2CFDD, T_CHUNK_COLOR,		"halo_color", -1},
		{0x9600BED2, T_CHUNK_INT,		"state", -1},
		{0x95F3C9E3, T_CHUNK_INT,		"blink_interval", -1},
		{0x9600C2E3, T_CHUNK_STRING,	"blink_pattern", -1},
		{0xA6F2C6D3, T_CHUNK_INT,		"unknown_1", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC,	"end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_60);

	static ChunkDescriptor CHUNKS_ELEMENT_61_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0x9BFCCFCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0x9DFDC3D8, T_CHUNK_STRING,	"model", -1},
		{0xA3EFBDD2, T_CHUNK_STRING,	"surface", -1},
		{0xA6FAC5DC, T_CHUNK_STRING,	"texture_collector", -1},
		{0xA4FAC5DC, T_CHUNK_STRING,	"texture_emitter", -1},
		{0x9CEBC4DC, T_CHUNK_INT,		"invert", -1},
		{0x97F5C3E2, T_CHUNK_COLOR,		"color", -1},
		{0xA8EDC3D3, T_CHUNK_INT,		"rotation", -1},
		{0x95FDC9CE, T_CHUNK_INT,		"beam_width", -1},
		{0x9DFBCDD3, T_CHUNK_INT,		"reflects_off_playfield", -1},
		{0xA5F3BFD1, T_CHUNK_STRING,	"sound_when_hit", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC, "end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_61);

	static ChunkDescriptor CHUNKS_ELEMENT_62_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0x9BFCCFCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0x9DFDC3D8, T_CHUNK_STRING,	"model", -1},
		{0xA300C5DC, T_CHUNK_STRING,	"texture", -1},
		{0x97F5C3E2, T_CHUNK_COLOR,		"color", -1},
		{0x99F4C2D2, T_CHUNK_INT,		"sphere_mapping", -1},
		{0xA8EDC3D3, T_CHUNK_INT,		"rotation", -1},
		{0x95F3CFD2, T_CHUNK_INT,		"switch_count", -1},
		{0x9BFCBED2, T_CHUNK_INT,		"tension", -1},
		{0x9DFBCDD3, T_CHUNK_INT,		"reflects_off_playfield", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC,	"end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_62);

	static ChunkDescriptor CHUNKS_ELEMENT_64_ARRAY[] = {
		{0xA4F4D1D7, T_CHUNK_WSTRING,	"name", -1},
		{0x9BFCCFCF, T_CHUNK_VECTOR2D,	"position", -1},
		{0x9DFDC3D8, T_CHUNK_STRING,	"model", -1},
		{0xA3EFBDD2, T_CHUNK_STRING,	"surface", -1},
		{0x97F5C3E2, T_CHUNK_COLOR,		"color", -1},
		{0xA2F8CDDD, T_CHUNK_INT,		"height", -1},
		{0x95FDC9CE, T_CHUNK_INT,		"width", -1},
		{0xA8EDC3D3, T_CHUNK_INT,		"rotation", -1},
		{0x95F5C9D1, T_CHUNK_INT,		"tilt", -1},
		{0x96FBCCD6, T_CHUNK_INT,		"offset", -1},
		{0x96F5C5DC, T_CHUNK_INT,		"image_list", -1},
		{0x95F3C9D0, T_CHUNK_INT,		"animation_update_interval", -1},
		{0x97F5CFE1, T_CHUNK_COLOR,		"matrix_color", -1},
		{0x9CFDD1D3, T_CHUNK_INT,		"render_as_dmd_matrix", -1},
		{0x95F3CFCD, T_CHUNK_INT,		"x_dot_count", -1},
		{0x95F3CFCC, T_CHUNK_INT,		"y_dot_count", -1},
		{0xA0F1BDE1, T_CHUNK_INT,		"update_interval", -1},
		{0xA2F8C6E4, T_CHUNK_INT,		"alignment", -1},
		{0x99EED0D2, T_CHUNK_INT,		"slow_blink_speed", -1},
		{0x99EED0DF, T_CHUNK_INT,		"fast_blink_speed", -1},
		{0xA6F2C6D3, T_CHUNK_INT,		"unknown_1", -1},
		{0x9EFEC3D9, T_CHUNK_INT,		"locked", -1},
		{0x9100C6E4, T_CHUNK_INT,		"layer", -1},
		{0xA7FDC4E0, T_CHUNK_GENERIC,	"end", -1}
	};
	static ARRAY2VECTOR(ChunkDescriptor, CHUNKS_ELEMENT_64);



} // namespace fp
} // namespace ops



#endif // __OPS_FP_DEFS_H_INCLUDED

