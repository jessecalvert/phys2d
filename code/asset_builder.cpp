/*
* @Author: Jesse
* @Date:   2016-09-01 16:04:07
* @Last Modified by:   Jesse
* @Last Modified time: 2017-07-16 22:58:43
*/

#include "phys2d.h"
#include <cstdio>
#include <cstdlib>

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

struct intermediate_data_buffer
{
	u8 *MetaDataBuffer;
	u32 MetaDataOffset;
	u8 *DataBuffer;
	u32 DataOffset;
};

#pragma pack(push, 1)
struct bitmap_file_header
{
	u16 MagicValue;
	u32 Size;
	u16 Reserved0;
	u16 Reserved1;
	u32 ImageDataOffset;
};

struct bitmap_information_header
{
	u32 Size;
	u32 Width;
	u32 Height;
	u16 ColorPlanes;
	u16 BitsPerPixel;
	u32 Compression;
	u32 ImageSize;
	u32 HorizontalResolution;
	u32 VerticalResolution;
	u32 ColorsUsed;
	u32 ColorsImportant;

	u32 RedMask;
	u32 GreenMask;
	u32 BlueMask;
	u32 AlphaMask;
};
#pragma pack(pop)

internal u64
GetFileSize(char *Filename)
{
	u64 Result = 0;
	FILE *FileHandle;

	fopen_s(&FileHandle, Filename , "rb");
	if(FileHandle)
	{
		fseek(FileHandle, 0, SEEK_END);
		Result = ftell(FileHandle);
		fclose(FileHandle);
	}

	return Result;
}

internal u8 *
ReadEntireFileAndNullTerminate(char *Filename)
{
	u8 *Result = 0;
	FILE *FileHandle;
	u64 Size = GetFileSize(Filename);

	fopen_s(&FileHandle, Filename , "rb");
	if(FileHandle)
	{
		Result = (u8 *)calloc(1, Size + 1);
		fread(Result, Size, 1, FileHandle);
		Result[Size] = 0;

		fclose(FileHandle);
	}

	return Result;
}

internal loaded_bitmap *
LoadBitmap(char *Filename)
{
	loaded_bitmap *Result = 0;

	u8 *BitmapFileData = ReadEntireFileAndNullTerminate(Filename);
	if(BitmapFileData)
	{
		bitmap_file_header *Header = (bitmap_file_header *)BitmapFileData;
		bitmap_information_header *InfoHeader = (bitmap_information_header *) (Header + 1);

		Assert(Header->MagicValue == 0x4D42);
		Assert((InfoHeader->Compression == 3) || (InfoHeader->Compression == 0));

		if(InfoHeader->BitsPerPixel == 32)
		{
			Assert(InfoHeader->RedMask == 0xFF000000);
			Assert(InfoHeader->GreenMask == 0x00FF0000);
			Assert(InfoHeader->BlueMask == 0x0000FF00);
			Assert(InfoHeader->AlphaMask == 0x000000FF);

			Result = (loaded_bitmap *)calloc(1, sizeof(loaded_bitmap));
			u32 *Pixels = (u32 *)(BitmapFileData + Header->ImageDataOffset);
			Result->Width = InfoHeader->Width;
			Result->Height = InfoHeader->Height;
			Result->Pixels = Pixels;
		}
		else
		{
			Assert(InfoHeader->BitsPerPixel == 24);

			Result = (loaded_bitmap *)calloc(1, sizeof(loaded_bitmap));
			Result->Width = InfoHeader->Width;
			Result->Height = InfoHeader->Height;
			u32 PixelCount = Result->Width*Result->Height;
			Result->Pixels = (u32 *)calloc(PixelCount, sizeof(u32));

			u8 *Source = BitmapFileData + Header->ImageDataOffset;
			u32 *Dest = Result->Pixels;
			for(u32 Count = 0;
				Count < PixelCount;
				++Count)
			{
				u32 Blue = *Source++;
				u32 Green = *Source++;
				u32 Red = *Source++;
				u32 Alpha = 0xFF;

				*Dest++ = ((Red << 24) |
						   (Green << 16) |
						   (Blue << 8) |
						   (Alpha << 0));
			}
		}
	}

	return Result;
}

inline u32
Sample(loaded_bitmap *Bitmap, r32 x, r32 y)
{
	x *= Bitmap->Width;
	y *= Bitmap->Height;
	u32 X0 = FloorReal32ToInt32(x);
	r32 tx = x - X0;
	u32 Y0 = FloorReal32ToInt32(y);
	r32 ty = y - Y0;

	u32 SampleOffsets[4] =
	{
		Y0*Bitmap->Width + X0,
		Y0*Bitmap->Width + X0 + 1,
		(Y0 + 1)*Bitmap->Width + X0,
		(Y0 + 1)*Bitmap->Width + X0 + 1,
	};

	r32 R[4] =
	{
		((Bitmap->Pixels[SampleOffsets[0]] & 0xFF000000) >> 24) / 255.0f,
		((Bitmap->Pixels[SampleOffsets[1]] & 0xFF000000) >> 24) / 255.0f,
		((Bitmap->Pixels[SampleOffsets[2]] & 0xFF000000) >> 24) / 255.0f,
		((Bitmap->Pixels[SampleOffsets[3]] & 0xFF000000) >> 24) / 255.0f,
	};

	r32 G[4] =
	{
		((Bitmap->Pixels[SampleOffsets[0]] & 0x00FF0000) >> 16) / 255.0f,
		((Bitmap->Pixels[SampleOffsets[1]] & 0x00FF0000) >> 16) / 255.0f,
		((Bitmap->Pixels[SampleOffsets[2]] & 0x00FF0000) >> 16) / 255.0f,
		((Bitmap->Pixels[SampleOffsets[3]] & 0x00FF0000) >> 16) / 255.0f,
	};

	r32 B[4] =
	{
		((Bitmap->Pixels[SampleOffsets[0]] & 0x0000FF00) >> 8) / 255.0f,
		((Bitmap->Pixels[SampleOffsets[1]] & 0x0000FF00) >> 8) / 255.0f,
		((Bitmap->Pixels[SampleOffsets[2]] & 0x0000FF00) >> 8) / 255.0f,
		((Bitmap->Pixels[SampleOffsets[3]] & 0x0000FF00) >> 8) / 255.0f,
	};

	r32 SR = Lerp(Lerp(R[0], tx, R[1]), ty, Lerp(R[2], tx, R[3]));
	r32 SG = Lerp(Lerp(G[0], tx, G[1]), ty, Lerp(G[2], tx, G[3]));
	r32 SB = Lerp(Lerp(B[0], tx, B[1]), ty, Lerp(B[2], tx, B[3]));

	u32 Result = ((((u32) (SR*255.0)) << 24) |
				  (((u32) (SG*255.0)) << 16) |
				  (((u32) (SB*255.0)) << 8) |
				  (0x000000FF));
	return Result;
}

inline r32
GetHeightFromHeightMap(loaded_bitmap *HeightMap, s32 X, s32 Y)
{
	if(X < 0) {X = 0;}
	if(X >= (s32)HeightMap->Width) {X = HeightMap->Width - 1;}
	if(Y < 0) {Y = 0;}
	if(Y >= (s32)HeightMap->Height) {Y = HeightMap->Height - 1;}

	u32 TexelColor = HeightMap->Pixels[X + Y*HeightMap->Width];
	r32 Height = ((TexelColor & 0xFF000000) >> 24) / 255.0f;
	return Height;
}

internal loaded_bitmap *
CreateNormalFromHeightMap(loaded_bitmap *HeightMap)
{
	loaded_bitmap *Result = (loaded_bitmap *)calloc(1, sizeof(loaded_bitmap));
	Result->Width = HeightMap->Width;
	Result->Height = HeightMap->Height;
	Result->Pixels = (u32 *)calloc(Result->Width*Result->Height, sizeof(u32));

	v2 Offsets[] =
	{
		{-1, -1}, {0, -1}, {1, -1},
		{-1, 0}, {1, 0},
		{-1, 1}, {0, 1}, {1, 1},
	};

	u32 *DestRow = Result->Pixels;
	for(u32 Y = 0;
		Y < Result->Height;
		++Y)
	{
		u32 *Dest = DestRow;
		for(u32 X = 0;
			X < Result->Width;
			++X)
		{
			v3 Normal = V3(0, 0, 1.0f);
			r32 BaseHeight = GetHeightFromHeightMap(HeightMap, X, Y);
			for(u32 OffsetIndex = 0;
				OffsetIndex < ArrayCount(Offsets);
				++OffsetIndex)
			{
				v2 Offset = Offsets[OffsetIndex];
				r32 HeightSample = GetHeightFromHeightMap(HeightMap, (s32)(X + Offset.x), (s32)(Y + Offset.y));
				r32 HeightDifference = BaseHeight - HeightSample;
				r32 Fudge = 0.1f;
				Normal += Fudge * HeightDifference * Normalize(V3(Offset, 0.0f));
				Assert(Normal.z > 0.0f);
			}

			Normal = Normalize(Normal);

			*Dest++ = ((((u32)(255.0f*(0.5f*Normal.x + 0.5f))) << 24) |
					   (((u32)(255.0f*(0.5f*Normal.y + 0.5f))) << 16) |
					   (((u32)(255.0f*(0.5f*Normal.z + 0.5f))) << 8) |
					   (0xFF));
		}

		DestRow += Result->Width;
	}

	return Result;
}

#if 0
#define PNG_MAGIC_VALUE 0x0a1a0a0d474e5089

#pragma pack(push, 1)
struct png_header
{
	u64 MagicValue;
};

struct png_chunk_header
{
	u32 ChunkDataSize;
	u32 ChunkType;
};

struct png_chunk_footer
{
	u32 CRC;
};

struct png_ihdr_chunk_data
{
	u32 Width;
	u32 Height;
	u8 BitDepth;
	u8 ColorType;
	u8 CompressionMethod;
	u8 FilterMethod;
	u8 InterlaceMethod;
};

struct png_chrm_chunk_data
{
	u32 WhitePointX;
	u32 WhitePointY;
	u32 RedX;
	u32 RedY;
	u32 GreenX;
	u32 GreenY;
	u32 BlueX;
	u32 BlueY;
};

struct png_phys_chunk_data
{
	u32 PixelsPerUnitX;
	u32 PixelsPerUnitY;
	u8 UnitType;
};
#pragma pack(pop)

#define ChunkName(a, b, c, d) ((((u32)d) << 24) | (((u32)c) << 16) | (((u32)b) << 8) | ((u32)a))
#endif
#define FlipEndian(a) ((a << 24) | ((a << 8) & 0x00FF0000) | ((a >> 8) & 0x0000FF00) | (a >> 24))

internal void
WritePNG(header_sla *Header, intermediate_data_buffer *Buffer,
	asset_name Name, texture_type TextureType,
	char *Filename)
{
	u8 *PNGFileData = ReadEntireFileAndNullTerminate(Filename);
	if(PNGFileData)
	{
		// stbi_set_flip_vertically_on_load(true);
		s32 Width = 0;
		s32 Height = 0;
		s32 Channels = 0;
		u32 *LoadedPNG = (u32 *)stbi_load_from_memory(PNGFileData, (int)GetFileSize(Filename), &Width, &Height,
			&Channels, 4);

		u32 PixelCount = Width * Height;
		for(u32 PixelIndex = 0;
			PixelIndex < PixelCount;
			++PixelIndex)
		{
			u32 Color = LoadedPNG[PixelIndex];
			LoadedPNG[PixelIndex] = FlipEndian(Color);
		}

		texture_sla *MetaData = (texture_sla *)(Buffer->MetaDataBuffer + Buffer->MetaDataOffset);
		Buffer->MetaDataOffset += sizeof(texture_sla);
		Header->TextureCount += 1;

		loaded_bitmap BMP = {};
		BMP.Width = Width;
		BMP.Height = Height;
		BMP.Pixels = LoadedPNG;

		MetaData->Name = Name;
		MetaData->Type = TextureType;
		MetaData->Width = BMP.Width;
		MetaData->Height = BMP.Height;
		MetaData->PixelsOffset = Buffer->DataOffset;

		u32 DataSize = BMP.Width * BMP.Height*sizeof(u32);
		memcpy(Buffer->DataBuffer + Buffer->DataOffset, BMP.Pixels, DataSize);
		Buffer->DataOffset += DataSize;
	}
}

#define FLAT_WIDTH 4
#define FLAT_HEIGHT 4

internal void
WriteFlat(header_sla *Header, intermediate_data_buffer *Buffer,
	asset_name Name, texture_type TextureType,
	v4 Color)
{
	u32 Width = FLAT_WIDTH;
	u32 Height = FLAT_HEIGHT;
	u32 PixelCount = Width * Height;
	u32 Pixels[FLAT_HEIGHT*FLAT_WIDTH] = {};
	for(u32 PixelIndex = 0;
		PixelIndex < PixelCount;
		++PixelIndex)
	{
		u32 ColorU32 = ((((u32)(0xFF * Color.r)) << 24) |
			(((u32)(0xFF * Color.g)) << 16) |
			(((u32)(0xFF * Color.b)) << 8) |
			(((u32)(0xFF * Color.a)) << 0));
		Pixels[PixelIndex] = ColorU32;
	}

	texture_sla *MetaData = (texture_sla *)(Buffer->MetaDataBuffer + Buffer->MetaDataOffset);
	Buffer->MetaDataOffset += sizeof(texture_sla);
	Header->TextureCount += 1;

	loaded_bitmap BMP = {};
	BMP.Width = Width;
	BMP.Height = Height;
	BMP.Pixels = Pixels;

	MetaData->Name = Name;
	MetaData->Type = TextureType;
	MetaData->Width = BMP.Width;
	MetaData->Height = BMP.Height;
	MetaData->PixelsOffset = Buffer->DataOffset;

	u32 DataSize = BMP.Width * BMP.Height*sizeof(u32);
	memcpy(Buffer->DataBuffer + Buffer->DataOffset, BMP.Pixels, DataSize);
	Buffer->DataOffset += DataSize;
}

internal void
WriteAndCombinePNGS(header_sla *Header, intermediate_data_buffer *Buffer,
	asset_name Name, texture_type TextureType,
	char *Filename1, char *Filename2, char *Filename3, char *Filename4)
{
	u8 *PNGFileData1 = ReadEntireFileAndNullTerminate(Filename1);
	u8 *PNGFileData2 = ReadEntireFileAndNullTerminate(Filename2);
	u8 *PNGFileData3 = ReadEntireFileAndNullTerminate(Filename3);
	u8 *PNGFileData4 = ReadEntireFileAndNullTerminate(Filename4);
	if(PNGFileData1 && PNGFileData2 && PNGFileData3 && PNGFileData4)
	{
		// stbi_set_flip_vertically_on_load(true);
		s32 Width1 = 0;
		s32 Height1 = 0;
		s32 Channels1 = 0;
		u32 *LoadedPNG1 = (u32 *)stbi_load_from_memory(PNGFileData1, (int)GetFileSize(Filename1), &Width1, &Height1,
			&Channels1, 4);

		s32 Width2 = 0;
		s32 Height2 = 0;
		s32 Channels2 = 0;
		u32 *LoadedPNG2 = (u32 *)stbi_load_from_memory(PNGFileData2, (int)GetFileSize(Filename2), &Width2, &Height2,
			&Channels2, 4);

		s32 Width3 = 0;
		s32 Height3 = 0;
		s32 Channels3 = 0;
		u32 *LoadedPNG3 = (u32 *)stbi_load_from_memory(PNGFileData3, (int)GetFileSize(Filename3), &Width3, &Height3,
			&Channels3, 4);

		s32 Width4 = 0;
		s32 Height4 = 0;
		s32 Channels4 = 0;
		u32 *LoadedPNG4 = (u32 *)stbi_load_from_memory(PNGFileData4, (int)GetFileSize(Filename4), &Width4, &Height4,
			&Channels4, 4);

		Assert(Width1 == Width2);
		Assert(Width1 == Width3);
		Assert(Width1 == Width4);
		Assert(Height1 == Height2);
		Assert(Height1 == Height3);
		Assert(Height1 == Height4);

		u32 PixelCount = Width1 * Height1;
		loaded_bitmap BMP = {};
		BMP.Pixels = (u32 *)calloc(1, PixelCount*sizeof(u32));
		BMP.Width = Width1;
		BMP.Height = Height1;
		for(u32 PixelIndex = 0;
			PixelIndex < PixelCount;
			++PixelIndex)
		{
			BMP.Pixels[PixelIndex] = (((LoadedPNG1[PixelIndex] & 0xFF) << 24) |
									  ((LoadedPNG2[PixelIndex] & 0xFF) << 16) |
									  ((LoadedPNG3[PixelIndex] & 0xFF) << 8) |
									  ((LoadedPNG4[PixelIndex] & 0xFF) << 0));
		}

		texture_sla *MetaData = (texture_sla *)(Buffer->MetaDataBuffer + Buffer->MetaDataOffset);
		Buffer->MetaDataOffset += sizeof(texture_sla);
		Header->TextureCount += 1;

		MetaData->Name = Name;
		MetaData->Type = TextureType;
		MetaData->Width = BMP.Width;
		MetaData->Height = BMP.Height;
		MetaData->PixelsOffset = Buffer->DataOffset;

		u32 DataSize = BMP.Width * BMP.Height*sizeof(u32);
		memcpy(Buffer->DataBuffer + Buffer->DataOffset, BMP.Pixels, DataSize);
		Buffer->DataOffset += DataSize;
	}
}

internal void
WriteAndCombinePNGS(header_sla *Header, intermediate_data_buffer *Buffer,
	asset_name Name, texture_type TextureType,
	char *Filename1, char *Filename2, r32 FlatColor1, r32 FlatColor2)
{
	u8 *PNGFileData1 = ReadEntireFileAndNullTerminate(Filename1);
	u8 *PNGFileData2 = ReadEntireFileAndNullTerminate(Filename2);
	if(PNGFileData1 && PNGFileData2)
	{
		// stbi_set_flip_vertically_on_load(true);
		s32 Width1 = 0;
		s32 Height1 = 0;
		s32 Channels1 = 0;
		u32 *LoadedPNG1 = (u32 *)stbi_load_from_memory(PNGFileData1, (int)GetFileSize(Filename1), &Width1, &Height1,
			&Channels1, 4);

		s32 Width2 = 0;
		s32 Height2 = 0;
		s32 Channels2 = 0;
		u32 *LoadedPNG2 = (u32 *)stbi_load_from_memory(PNGFileData2, (int)GetFileSize(Filename2), &Width2, &Height2,
			&Channels2, 4);

		Assert(Width1 == Width2);
		Assert(Height1 == Height2);

		u32 PixelCount = Width1 * Height1;
		loaded_bitmap BMP = {};
		BMP.Pixels = (u32 *)calloc(1, PixelCount*sizeof(u32));
		BMP.Width = Width1;
		BMP.Height = Height1;
		u32 FlatColor1U32 = (u32)(0xFF * FlatColor1);
		u32 FlatColor2U32 = (u32)(0xFF * FlatColor2);
		for(u32 PixelIndex = 0;
			PixelIndex < PixelCount;
			++PixelIndex)
		{
			BMP.Pixels[PixelIndex] = (((LoadedPNG1[PixelIndex] & 0xFF) << 24) |
									  ((LoadedPNG2[PixelIndex] & 0xFF) << 16) |
									  ((FlatColor1U32) << 8) |
									  ((FlatColor2U32) << 0));
		}

		texture_sla *MetaData = (texture_sla *)(Buffer->MetaDataBuffer + Buffer->MetaDataOffset);
		Buffer->MetaDataOffset += sizeof(texture_sla);
		Header->TextureCount += 1;

		MetaData->Name = Name;
		MetaData->Type = TextureType;
		MetaData->Width = BMP.Width;
		MetaData->Height = BMP.Height;
		MetaData->PixelsOffset = Buffer->DataOffset;

		u32 DataSize = BMP.Width * BMP.Height*sizeof(u32);
		memcpy(Buffer->DataBuffer + Buffer->DataOffset, BMP.Pixels, DataSize);
		Buffer->DataOffset += DataSize;
	}
}

internal void
WriteLayeredBMP(header_sla *Header, intermediate_data_buffer *Buffer,
	asset_name Name, char *DiffuseFilename, char *HeightmapFilename,
	char *SpecularFilename, char *EmitFilename)
{
	texture_sla *MetaData = (texture_sla *)(Buffer->MetaDataBuffer + Buffer->MetaDataOffset);
	Buffer->MetaDataOffset += sizeof(texture_sla);
	Header->TextureCount += 1;

	loaded_bitmap *BMP = LoadBitmap(DiffuseFilename);
	loaded_bitmap *Specular = LoadBitmap(SpecularFilename);
	loaded_bitmap *HeightMap = LoadBitmap(HeightmapFilename);
	loaded_bitmap *Normal = CreateNormalFromHeightMap(HeightMap);
	loaded_bitmap *Emit = LoadBitmap(EmitFilename);
	Assert(BMP);
	Assert(Specular);
	Assert(HeightMap);
	Assert(Normal);
	Assert(Emit);

	Assert(BMP->Width == HeightMap->Width);
	Assert(BMP->Height == HeightMap->Height);
	Assert(BMP->Width == Specular->Width);
	Assert(BMP->Height == Specular->Height);
	Assert(BMP->Width == Emit->Width);
	Assert(BMP->Height == Emit->Height);

	MetaData->Name = Name;
	MetaData->Width = BMP->Width;
	MetaData->Height = BMP->Height + Normal->Height + HeightMap->Height + Specular->Height + Emit->Height;
	MetaData->PixelsOffset = Buffer->DataOffset;

	u32 DataSize = BMP->Width * BMP->Height*sizeof(u32);
	memcpy(Buffer->DataBuffer + Buffer->DataOffset, BMP->Pixels, DataSize);
	Buffer->DataOffset += DataSize;

	memcpy(Buffer->DataBuffer + Buffer->DataOffset, Specular->Pixels, DataSize);
	Buffer->DataOffset += DataSize;

	memcpy(Buffer->DataBuffer + Buffer->DataOffset, Normal->Pixels, DataSize);
	Buffer->DataOffset += DataSize;

	memcpy(Buffer->DataBuffer + Buffer->DataOffset, HeightMap->Pixels, DataSize);
	Buffer->DataOffset += DataSize;

	memcpy(Buffer->DataBuffer + Buffer->DataOffset, Emit->Pixels, DataSize);
	Buffer->DataOffset += DataSize;
}

internal void
WriteBMP(header_sla *Header, intermediate_data_buffer *Buffer,
	asset_name Name, char *Filename)
{
	texture_sla *MetaData = (texture_sla *)(Buffer->MetaDataBuffer + Buffer->MetaDataOffset);
	Buffer->MetaDataOffset += sizeof(texture_sla);
	Header->TextureCount += 1;

	loaded_bitmap *BMP = LoadBitmap(Filename);
	Assert(BMP);

	MetaData->Name = Name;
	MetaData->Width = BMP->Width;
	MetaData->Height = BMP->Height;
	MetaData->PixelsOffset = Buffer->DataOffset;

	u32 DataSize = BMP->Width * BMP->Height*sizeof(u32);
	memcpy(Buffer->DataBuffer + Buffer->DataOffset, BMP->Pixels, DataSize);
	Buffer->DataOffset += DataSize;
}

internal u32
AddIfNotInAlready(v2 Vertex, u32 Color, vertex_format *Vertices, u32 *Count)
{
	u32 Result = *Count;

	for(u32 Index = 0;
	    Index < *Count;
	    ++Index)
	{
		v2 VertexTest = Vertices[Index].Position;
		u32 ColorTest = Vertices[Index].Color;

		if((Vertex == VertexTest) &&
		   (Color == ColorTest))
		{
			Result = Index;
		}
	}

	if(Result == *Count)
	{
		++(*Count);
		Vertices[Result].Position = Vertex;
		Vertices[Result].Color = Color;
	}

	return Result;
}

internal void
WriteFont(header_sla *Header, intermediate_data_buffer *Buffer, asset_name Name, char *Filename)
{
	font_sla *MetaData = (font_sla *)(Buffer->MetaDataBuffer + Buffer->MetaDataOffset);
	Buffer->MetaDataOffset += sizeof(font_sla);
	Header->FontCount += 1;
	MetaData->Name = Name;

	u8 *FontFile = ReadEntireFileAndNullTerminate(Filename);
	Assert(FontFile);

	stbtt_fontinfo FontInfo = {};
	int Error = stbtt_InitFont(&FontInfo, FontFile, 0);
	Assert(Error);

	const r32 FontHeight = 50.0f;
	MetaData->Scale = stbtt_ScaleForPixelHeight(&FontInfo, FontHeight);
	s32 Ascent, Descent, Linegap;
	stbtt_GetFontVMetrics(&FontInfo, &Ascent, &Descent, &Linegap);
	MetaData->Ascent = MetaData->Scale*Ascent;
	MetaData->Descent = MetaData->Scale*Descent;
	MetaData->Linegap = MetaData->Scale*Linegap;

	for(u32 Codepoint = 0;
		Codepoint < 128;
		++Codepoint)
	{
		s32 LeftSideBearing, Advance;
		stbtt_GetCodepointHMetrics(&FontInfo, Codepoint, &Advance, &LeftSideBearing);
		MetaData->Advance[Codepoint] = MetaData->Scale*Advance;
		MetaData->Offset[Codepoint].x = MetaData->Scale*LeftSideBearing;

		for(u32 NextCodepoint = 0;
		NextCodepoint < 128;
		++NextCodepoint)
		{
			MetaData->Kerning[Codepoint][NextCodepoint] =
				MetaData->Scale*stbtt_GetCodepointKernAdvance(&FontInfo, Codepoint, NextCodepoint);
		}
	}

	u32 BufferWidth = (u32)(FontHeight + 1);
	u8 *TempTempBuffer = (u8 *)calloc(BufferWidth, BufferWidth);

	u8 *TempBuffers[128];
	v2i BufferDim[128] = {};
	for(u32 Codepoint = 0;
		Codepoint < 128;
		++Codepoint)
	{
		s32 x0, x1, y0, y1;
		stbtt_GetCodepointBox(&FontInfo, Codepoint, &x0, &y0, &x1, &y1);
		BufferDim[Codepoint] = V2i(CeilReal32ToInt32(MetaData->Scale*(x1 - x0)),
								   CeilReal32ToInt32(MetaData->Scale*(y1 - y0)));
		TempBuffers[Codepoint] = (u8 *)calloc(1, (x1 - x0)*(y1 - y0)*sizeof(u8));
		MetaData->Offset[Codepoint].y = MetaData->Scale*y0;

		stbtt_MakeCodepointBitmap(&FontInfo, TempTempBuffer,
			BufferWidth, BufferWidth, BufferWidth,
			MetaData->Scale, MetaData->Scale, Codepoint);

		if(Codepoint > ' ')
		{
			u8 *DestRow = TempBuffers[Codepoint];
			u8 *SourceRow = TempTempBuffer + (BufferDim[Codepoint].y - 1)*BufferWidth;
			for(s32 Y = 0;
				Y < BufferDim[Codepoint].y;
				++Y)
			{
				u8 *Dest = DestRow;
				u8 *Source = SourceRow;
				for(s32 X = 0;
					X < BufferDim[Codepoint].x;
					++X)
				{
					*Dest++ = *Source++;
				}

				DestRow += BufferDim[Codepoint].x;
				SourceRow -= BufferWidth;
			}
		}
	}

	u32 PixelsDim = 1024;
	u8 *Pixels = (u8 *)calloc(PixelsDim, PixelsDim);
	s32 CharSpacing = 5;
	s32 XPos = 0;
	s32 YPos = 0;
	s32 MaxHeight = 0;
	for(u32 Codepoint = 0;
		Codepoint < 128;
		++Codepoint)
	{
		s32 HorizontalSpaceLeft = PixelsDim - XPos;
		if(HorizontalSpaceLeft < BufferDim[Codepoint].x)
		{
			XPos = 0;
			YPos += MaxHeight + CharSpacing;
			MaxHeight = 0;
			Assert((YPos + BufferDim[Codepoint].y) < (s32)PixelsDim);
		}

		u8 *Source = TempBuffers[Codepoint];
		u8 *DestRow = Pixels + YPos*PixelsDim + XPos;
		for(s32 Y = 0;
			Y < BufferDim[Codepoint].y;
			++Y)
		{
			u8 *Dest = DestRow;
			for(s32 X = 0;
				X < BufferDim[Codepoint].x;
				++X)
			{
				*Dest++ = *Source++;
			}

			DestRow += PixelsDim;
		}

		MetaData->TexCoordMin[Codepoint] = V2((r32)XPos/(r32)PixelsDim, (r32)YPos/(r32)PixelsDim);
		MetaData->TexCoordMax[Codepoint] = V2((r32)(XPos + BufferDim[Codepoint].x)/(r32)PixelsDim,
											  (r32)(YPos + BufferDim[Codepoint].y)/(r32)PixelsDim);

		if(MaxHeight < BufferDim[Codepoint].y)
		{
			MaxHeight = BufferDim[Codepoint].y;
		}
		XPos += BufferDim[Codepoint].x + CharSpacing;
	}

	MetaData->PixelsOffset = Buffer->DataOffset;
	MetaData->Width = PixelsDim;
	MetaData->Height = PixelsDim;
	u32 DataSize = PixelsDim * PixelsDim * sizeof(u8);
	memcpy(Buffer->DataBuffer + Buffer->DataOffset, Pixels, DataSize);
	Buffer->DataOffset += DataSize;

	free(TempTempBuffer);
	for(u32 Codepoint = 0;
		Codepoint < 128;
		++Codepoint)
	{
		free(TempBuffers[Codepoint]);
	}
}

internal void
WriteTextureFiles(header_sla *Header, intermediate_data_buffer *Buffer)
{
	WriteBMP(Header, Buffer, Asset_Circle, "w:\\phys2d\\data\\Textures\\circle.bmp");
}

internal void
WriteFontFiles(header_sla *Header, intermediate_data_buffer *Buffer)
{
	WriteFont(Header, Buffer, Asset_LiberationMonoFont, "w:\\phys2d\\data\\Fonts\\LiberationMono-Regular.ttf");
	WriteFont(Header, Buffer, Asset_TimesFont, "w:\\phys2d\\data\\Fonts\\times.ttf");
}

s32 main()
{
	intermediate_data_buffer Buffer = {};
	Buffer.MetaDataBuffer = (u8 *)calloc(1, Gigabytes(1));
	Buffer.DataBuffer = (u8 *)calloc(1, Gigabytes(1));

	header_sla *Header = (header_sla *)Buffer.MetaDataBuffer;
	Buffer.MetaDataOffset += sizeof(header_sla);

	Header->MagicCode = MAGIC_CODE;
	Header->Version = 2;

	WriteTextureFiles(Header, &Buffer);
	WriteFontFiles(Header, &Buffer);

	Header->DataOffset = Buffer.MetaDataOffset;

	FILE *Out;
	fopen_s(&Out, "w:\\phys2d\\data\\asset_pack.sla", "wb");
	fwrite(Buffer.MetaDataBuffer, Buffer.MetaDataOffset, 1, Out);
	fwrite(Buffer.DataBuffer, Buffer.DataOffset, 1, Out);
	fclose(Out);

	u8 *WrittenFile = ReadEntireFileAndNullTerminate("w:\\phys2d\\data\\asset_pack.sla");
	int y = 3;
}
