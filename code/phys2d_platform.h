/*@H
* File: phys2d_platform.h
* Author: Jesse Calvert
* Created: November 6, 2017, 17:07
* Last modified: November 27, 2017, 11:59
*/

#pragma once

//
// NOTE: Compilers
//

#if !defined(COMPILER_MSVC)
#define COMPILER_MSVC 0
#endif

#if !defined(COMPILER_LLVM)
#define COMPILER_LLVM 0
#endif

#if !COMPILER_MSVC && !COMPILER_LLVM
#if _MSC_VER
#undef COMPILER_MSVC
#define COMPILER_MSVC 1
#else
#undef COMPILER_LLVM
#define COMPILER_LLVM 1
#endif
#endif

#if COMPILER_MSVC
#include <intrin.h>
#elif COMPILER_LLVM
#include <x86intrin.h>
#else
#error SEE/NEON optimizations are not available for this compiler yet!!!!
#endif

#include <stdint.h>
#include <limits.h>
#include <float.h>

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef int32 bool32;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int8 s8;
typedef int16 s16;
typedef int32 s32;
typedef int64 s64;
typedef bool32 b32;

typedef uint8 u8;
typedef uint16 u16;
typedef uint32 u32;
typedef uint64 u64;

typedef uintptr_t uintptr;
typedef intptr_t intptr;

typedef size_t memory_index;

typedef float real32;
typedef double real64;

typedef real32 r32;
typedef real64 r64;

typedef r32 f32;
typedef r64 f64;

typedef uintptr_t umm;

#define Real32Maximum FLT_MAX
#define Real32Minimum -FLT_MAX

#ifndef internal
    #define internal static
#endif
#define local_persist static
#define global_variable static

#define Pi32 3.14159265359f

#if PHYS2D_DEBUG
#define Assert(Expression) if(!(Expression)) *(int *)0 = 0
#else
#define Assert(Expression)
#endif

#define InvalidCodePath Assert(!"InvalidCodePath")
#define InvalidDefaultCase default: {InvalidCodePath;} break

// TODO: should these all use 64-bit?
#define Kilobytes(Value) ((Value)*1024LL)
#define Megabytes(Value) (Kilobytes(Value)*1024LL)
#define Gigabytes(Value) (Megabytes(Value)*1024LL)
#define Terabytes(Value) (Gigabytes(Value)*1024LL)

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))
#define Minimum(A, B) ((A < B) ? (A) : (B))
#define Maximum(A, B) ((A > B) ? (A) : (B))

#define AlignPow2(Value, Alignment) ((Value + ((Alignment) - 1)) & ~((Alignment) - 1))
#define Align4(Value) ((Value + 3) & ~3)
#define Align8(Value) ((Value + 7) & ~7)
#define Align16(Value) ((Value + 15) & ~15)

#define U32FromPointer(Pointer) ((u32)(memory_index)(Pointer))
#define PointerFromU32(type, Value) (type *)((memory_index)Value)

#define OffsetOf(type, Member) (umm)&(((type *)0)->Member)

#define Swap(A, B, type) type TEMP_DONT_REDEFINE_THIS_##type = A; A = B; B = TEMP_DONT_REDEFINE_THIS_##type

union v2
{
	r32 E[2];
	struct
	{
		r32 x, y;
	};
	struct
	{
		r32 u, v;
	};
};

union v2i
{
	s32 E[2];
	struct
	{
		s32 x, y;
	};
	struct
	{
		s32 u, v;
	};
};

union v3
{
	r32 E[3];
	struct
	{
		r32 x, y, z;
	};
	struct
	{
		r32 r, g, b;
	};
	struct
	{
		r32 Ignored_0;
		v2 yz;
	};
	struct
	{
		v2 xy;
		r32 Ignored_1;
	};
};

union v3i
{
	s32 E[3];
	struct
	{
		s32 x, y, z;
	};
	struct
	{
		s32 r, g, b;
	};
	struct
	{
		s32 Ignored_0;
		v2i yz;
	};
	struct
	{
		v2i xy;
		s32 Ignored_1;
	};
};

union v3u
{
	u32 E[3];
	struct
	{
		u32 x, y, z;
	};
	struct
	{
		u32 r, g, b;
	};
};

union v4
{
	r32 E[4];
	struct
	{
		r32 x, y, z, w;
	};
	struct
	{
		r32 r, g, b, a;
	};
	struct
	{
		v2 xy;
		v2 zw;
	};
	struct
	{
		r32 Ignored_0;
		v3 yzw;
	};
	struct
	{
		v3 xyz;
		r32 Ignored_1;
	};
	struct
	{
		v3 rgb;
		r32 Ignored_2;
	};
};

struct rectangle2
{
	v2 Min;
	v2 Max;
};

struct rectangle3
{
	v3 Min;
	v3 Max;
};

union mat2
{
	r32 M[4];
	struct
	{
		v2 C[2];
	};
};

union mat3
{
	r32 M[9];
	struct
	{
		r32 a00, a10, a20;
		r32 a01, a11, a21;
		r32 a02, a12, a22;
	};
	struct
	{
		v3 C[3];
	};
};

union mat4
{
	r32 M[16];
	struct
	{
		v4 C[4];
	};
};

union quaternion
{
	r32 Q[4];
	struct
	{
		r32 r, i, j ,k;
	};
	struct
	{
		r32 Ignored_3;
		v3 xyz;
	};
};

struct sphere
{
	v3 Center;
	r32 Radius;
};

enum key
{
	Key_LeftClick,
	Key_RightClick,

	Key_Minus,
	Key_Plus,
	Key_OpenBracket,
	Key_CloseBracket,

	Key_Q,

	Key_Up,
	Key_Down,
	Key_Left,
	Key_Right,
	Key_Space,
	Key_Enter,

	Key_F1,
	Key_F2,
	Key_F3,
	Key_F4,
	Key_F5,
	Key_F6,
	Key_F7,
	Key_F8,
	Key_F9,
	Key_F10,
	Key_F11,
	Key_F12,

	Key_Esc,

	Key_Count,
};

struct key_state
{
	b32 Pressed;
	b32 WasPressed;
	b32 WasReleased;
	u32 TransitionCount;

#if PHYS2D_DEBUG
	b32 DebugPressed;
	b32 DebugWasPressed;
	b32 DebugWasReleased;
	u32 DebugTransitionCount;
#endif
};

struct game_input
{
	key_state KeyStates[Key_Count];
	v2 MouseP;

	r32 LastFramedt_;
	r32 Simdt;
};

enum shader_type
{
	Shader_Null,
	Shader_Header,

	Shader_Quads,

	Shader_Count,
};

struct command_buffer
{
	u32 Size;
	u32 MaxSize;
	u8 *Buffer;

	u32 QuadCount;
	u32 MaxQuads;
	struct vertex_format *QuadVertices; // NOTE: 4 verticies per quad.
	u64 *TextureData;

	u64 WhiteTexture;

	struct render_group *CurrentRenderGroup;
};

struct game_memory
{
	struct game_state *GameState;
	v2i WindowResolution;
	command_buffer CommandBuffer;
	struct platform *Platform;

#if PHYS2D_DEBUG
	struct debug_table *DebugTable;
	struct debug_state *DebugState;
#endif
};

#define GAME_UPDATE(Name) void Name(game_memory *Memory, game_input *Input)
typedef GAME_UPDATE(game_update);

#define GAME_RENDER(Name) void Name(game_memory *Memory, game_input *Input, f32 StateBlend)
typedef GAME_RENDER(game_render);

inline b32
IsWhitespace(char A)
{
	b32 Result = ((A == ' ') ||
	              (A == '\n') ||
	              (A == '\t') ||
	              (A == '\r'));
	return Result;
}

inline b32
StringsAreEqual(umm ALength, char *A, char *B)
{
	b32 Result = true;

	u32 Index = 0;
	while(*B)
	{
		if(A[Index] != *B)
		{
			Result = false;
			break;
		}

		++Index;
		++B;
	}

	Result = (Index == ALength);
	return Result;
}

struct platform_file
{
	void *Handle_;
};

struct platform_memory_block
{
	platform_memory_block *BlockPrev;
	u8 *Base;
	umm Size;
	umm Used;
	u64 Padding[4];
};

struct memory_region;
#define OPEN_ASSET_PACK_FILE(Name) platform_file Name()
typedef OPEN_ASSET_PACK_FILE(open_asset_pack_file);

#define READ_FROM_FILE(Name) u8 *Name(memory_region	*Region, platform_file *File, u32 Offset, u32 Length)
typedef READ_FROM_FILE(read_from_file);

#define CLOSE_FILE(Name) void Name(platform_file *File)
typedef CLOSE_FILE(close_file);

#define LOAD_ENTIRE_FILE(Name) u8 *Name(memory_region *Region, char *Filename)
typedef LOAD_ENTIRE_FILE(load_entire_file);

#define LOAD_ENTIRE_TEXT_FILE_AND_NULL_TERMINATE(Name) char *Name(memory_region *Region, char *Filename)
typedef LOAD_ENTIRE_TEXT_FILE_AND_NULL_TERMINATE(load_entire_text_file_and_null_terminate);

struct object_model;
struct vertex_format;
#define LOAD_MESH(Name) u64 Name(vertex_format *Vertices, u32 VertexCount, v3u *Faces, u32 FaceCount)
typedef LOAD_MESH(load_mesh);

#define LOAD_TEXTURE(Name) u64 Name(u32 *Pixels, u32 Width, u32 Height)
typedef LOAD_TEXTURE(load_texture);

#define LOAD_FONT_TEXTURE(Name) u64 Name(u8 *Pixels, u32 Width, u32 Height)
typedef LOAD_FONT_TEXTURE(load_font_texture);

#define ALLOCATE_MEMORY(Name) platform_memory_block *Name(umm Size, u32 Flags)
typedef ALLOCATE_MEMORY(allocate_memory);

#define DEALLOCATE_MEMORY(Name) void Name(platform_memory_block *Block)
typedef DEALLOCATE_MEMORY(deallocate_memory);

struct platform
{
	load_entire_file *LoadEntireFile;
	load_entire_text_file_and_null_terminate *LoadEntireTextFileAndNullTerminate;
	load_mesh *LoadMesh;
	load_texture *LoadTexture;
	load_font_texture *LoadFontTexture;
	allocate_memory *AllocateMemory;
	deallocate_memory *DeallocateMemory;

	open_asset_pack_file *OpenAssetPackFile;
	read_from_file *ReadFromFile;
	close_file *CloseFile;
};

global_variable platform *Platform;

#include "phys2d_debug_interface.h"
