/*@H
* File: phys2d_file_formats.h
* Author: Jesse Calvert
* Created: November 27, 2017, 15:33
* Last modified: November 27, 2017, 15:33
*/

#pragma once

#ifndef PHYS2D_FILE_FORMATS_H

#define MAGIC_CODE 0xabcd1234

enum asset_name
{
	Asset_None,

	//

	Asset_TexturesBegin,

	Asset_DefaultTexture,
	Asset_StructuredArt,
	Asset_Circle,

	Asset_TexturesEnd,

	//

	Asset_TextureSetsBegin,

	Asset_DefaultTextureSet,

	Asset_TextureSetsEnd,

	//

	Asset_FontsBegin,

	Asset_CambriaFont,
	Asset_LiberationMonoFont,
	Asset_TimesFont,

	Asset_FontsEnd,

	//

	Asset_Count,
};

/*
	NOTE: File format:

		Header

		mesh_sla[]
		texture_sla[]
		font_sla[]

		data block
*/

struct header_sla
{
	u32 MagicCode;
	u32 Version;

	u32 MeshCount;
	u32 TextureCount;
	u32 FontCount;

	u32 DataOffset;
};

enum texture_type
{
	TextureType_None,

	TextureType_Albedo,
	TextureType_Normal,
	TextureType_Roughness_Metallic_AO_Height,

	TextureType_Count,
};

struct texture_sla
{
	asset_name Name;
	texture_type Type;

	u32 Width;
	u32 Height;
	u32 PixelsOffset;
};

struct font_sla
{
	asset_name Name;

	r32 Scale;
	r32 Ascent;
	r32 Descent;
	r32 Linegap;

	v2 Offset[128];
	r32 Advance[128];
	r32 Kerning[128][128];

	v2 TexCoordMin[128];
	v2 TexCoordMax[128];

	u32 Width;
	u32 Height;
	u32 PixelsOffset;
};

struct vertex_format
{
	v3 Position;
	v2 TexCoords;
	u32 Color;
};
inline b32
operator==(vertex_format A, vertex_format B)
{
	b32 Result = ((A.Position == B.Position) &&
	              (A.TexCoords == B.TexCoords) &&
	              (A.Color == B.Color));
	return Result;
}

#define PHYS2D_FILE_FORMATS_H
#endif
