/*
* @Author: Jesse
* @Date:   2016-09-01 17:31:23
* @Last Modified by:   Jesse
* @Last Modified time: 2017-07-16 23:00:13
*/

internal void
LoadAssets(assets *Assets)
{
	platform_file AssetFile_ = Platform->OpenAssetPackFile();
	platform_file *AssetFile = &AssetFile_;
	Assets->AssetFile = AssetFile_;

	asset_metadata *MetaData = &Assets->MetaData;
	MetaData->Header = (header_sla *)Platform->ReadFromFile(&Assets->Region, AssetFile, 0, sizeof(header_sla));
	Assert(MetaData->Header->MagicCode == MAGIC_CODE);
	Assert(MetaData->Header->Version == 2);

	MetaData->TextureData = (texture_sla *)Platform->ReadFromFile(&Assets->Region, AssetFile,
		sizeof(header_sla), MetaData->Header->TextureCount*sizeof(texture_sla));
	MetaData->FontData = (font_sla *)Platform->ReadFromFile(&Assets->Region, AssetFile,
		sizeof(header_sla) + MetaData->Header->TextureCount*sizeof(texture_sla),
		MetaData->Header->FontCount*sizeof(font_sla));

	for(u32 TextureIndex = 0;
		TextureIndex < MetaData->Header->TextureCount;
		++TextureIndex)
	{
		texture_sla *TextureData = MetaData->TextureData + TextureIndex;
		asset *Asset = Assets->Assets + TextureData->Name;

		if(TextureData->Type)
		{
			Asset->Type = AssetType_TextureSet;
			if(!Asset->TextureSet)
			{
				Asset->TextureSet = PushStruct(&Assets->Region, texture_set);
			}

			texture *Texture = Asset->TextureSet->Textures + TextureData->Type;
			Texture->MetaData = TextureData;

			// TODO: after loading the asset to the gpu, we don't need the data in memory anymore.
			u32 *Pixels = (u32 *)Platform->ReadFromFile(&Assets->Region, AssetFile,
				MetaData->Header->DataOffset + TextureData->PixelsOffset,
				TextureData->Width*TextureData->Height*sizeof(u32));
			Texture->Data_ = Platform->LoadTexture(Pixels, TextureData->Width, TextureData->Height);
		}
		else
		{
			Asset->Type = AssetType_Texture;

			texture *Texture = Asset->Texture = PushStruct(&Assets->Region, texture);
			Texture->MetaData = TextureData;

			// TODO: after loading the asset to the gpu, we don't need the data in memory anymore.
			switch(GetAssetType(TextureData->Name))
			{
				case AssetType_Texture:
				{
					u32 *Pixels = (u32 *)Platform->ReadFromFile(&Assets->Region, AssetFile,
						MetaData->Header->DataOffset + TextureData->PixelsOffset,
						TextureData->Width*TextureData->Height*sizeof(u32));
					Texture->Data_ = Platform->LoadTexture(Pixels, TextureData->Width, TextureData->Height);
				} break;

				InvalidDefaultCase;
			}
		}
	}

	for(u32 FontIndex = 0;
		FontIndex < MetaData->Header->FontCount;
		++FontIndex)
	{
		font_sla *FontData = MetaData->FontData + FontIndex;
		asset *Asset = Assets->Assets + FontData->Name;
		Asset->Type = AssetType_Font;

		font *Font = Asset->Font = PushStruct(&Assets->Region, font);
		Font->MetaData = FontData;

		// TODO: after loading the asset to the gpu, we don't need the data in memory anymore.
		u8 *Pixels = (u8 *)Platform->ReadFromFile(&Assets->Region, AssetFile,
			MetaData->Header->DataOffset + FontData->PixelsOffset,
			FontData->Width*FontData->Height*sizeof(u8));
		Font->Data_ = Platform->LoadFontTexture(Pixels, FontData->Width, FontData->Height);
	}
}

inline texture *
GetTexture(assets *Assets, asset_name Name)
{
	texture *Result = 0;
	if(Name)
	{
		// TODO: Load if not loaded
		asset Asset = Assets->Assets[Name];
		Assert(Asset.Type == AssetType_Texture);
		Result = Asset.Texture;
	}
	return Result;	
}

inline texture_set *
GetTextureSet(assets *Assets, asset_name Name)
{
	texture_set *Result = 0;
	if(Name)
	{
		// TODO: Load if not loaded
		asset Asset = Assets->Assets[Name];
		Assert(Asset.Type == AssetType_TextureSet);
		Result = Asset.TextureSet;
	}
	return Result;	
}

inline font *
GetFont(assets *Assets, asset_name Name)
{
	font *Result = 0;
	if(Name)
	{
		// TODO: Load if not loaded
		asset Asset = Assets->Assets[Name];
		Assert(Asset.Type == AssetType_Font);
		Result = Asset.Font;
	}
	return Result;	
}

