#ifndef PHYS2D_ASSETS_H

struct loaded_bitmap
{
	u32 Width;
	u32 Height;
	u32 *Pixels;
};

struct texture
{
	texture_sla *MetaData;
	u64 Data_;
};

struct texture_set
{
	texture Textures[TextureType_Count];
};

struct font
{
	font_sla *MetaData;
	u64 Data_;
};

enum asset_type
{
	AssetType_None,

	AssetType_Texture,
	AssetType_TextureSet,
	AssetType_Font,

	AssetType_Count,
};

struct asset
{
	asset_type Type;
	union
	{
		font *Font;
		texture *Texture;
		texture_set *TextureSet;
	};
};

struct asset_metadata
{
	header_sla *Header;
	texture_sla *TextureData;
	font_sla *FontData;
};

struct assets
{
	memory_region Region;

	asset_metadata MetaData;
	platform_file AssetFile;

	asset Assets[Asset_Count];
};

inline asset_type
GetAssetType(asset_name Name)
{
	// TODO: we should really be storing metadata about the assets instead.
	asset_type Result = AssetType_None;
	if((Name > Asset_TexturesBegin) &&
	   (Name < Asset_TexturesEnd))
	{
		Result = AssetType_Texture;
	}
	else if((Name > Asset_TextureSetsBegin) &&
		    (Name < Asset_TextureSetsEnd))
	{
		Result = AssetType_TextureSet;
	}
	else if((Name > Asset_FontsBegin) &&
	   		(Name < Asset_FontsEnd))

	{
		Result = AssetType_Font;
	}
	else
	{
		InvalidCodePath;
	}

	return Result;
};

#define PHYS2D_ASSETS_H
#endif