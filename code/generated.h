/*@H
* File: generated.h
* Author: simple_meta.exe
* Created: November 27, 2017, 11:00
* Last modified: November 27, 2017, 11:00
*/

#pragma once

#define MAX_CIRCULAR_REFERENCE_DEPTH 2

#include "phys2d_debug.h"
#include "phys2d_rigid_body.h"
#include "phys2d_render_group.h"
#include "phys2d_assets.h"
#include "phys2d_random.h"
#include "phys2d_sort.h"
#include "phys2d_file_formats.h"
#include "phys2d_memory.h"
#include "phys2d_math.h"
#include "phys2d_intrinsics.h"
#include "phys2d_shared.h"
#include "phys2d_config.h"
#include "phys2d_debug_interface.h"
#include "phys2d_platform.h"
#include "phys2d.h"

inline void RecordDebugValue_(sphere *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(key_state *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(game_input *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(command_buffer *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(game_memory *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(platform_file *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(platform_memory_block *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(debug_event *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(debug_table *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(platform *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(bit_scan_result *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(temporary_memory *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(push_params *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(bootstrap_params *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(region_stats *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(string *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(format_builder *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(header_sla *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(texture_sla *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(font_sla *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(vertex_format *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(sort_entry *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(random_series *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(loaded_bitmap *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(texture *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(texture_set *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(font *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(asset *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(asset_metadata *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(assets *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(text_layout *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(render_setup *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(render_entry_header *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(render_entry_quads *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(render_entry_clear *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(camera *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(render_group *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(minkowski_difference_support *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(minkowski_simplex *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(expanding_polygon_vertex *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(expanding_polygon *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(circle *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(pill *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(convex_polygon *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(shape *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(rigid_body *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(index_pair *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(contact_data *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(contact_manifold *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(joint *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(bucket *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(aabb_query_result *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(aabb_tree_node *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(physics_state *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(move_spec *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(entity *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(game_state *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(debug_parsed_guid *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(debug_id *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(debug_stored_event *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(debug_ui_element *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(debug_ui_interaction *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(debug_time_block *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(debug_time_stat *ValuePtr, char *GUID, u32 Depth=0);
inline void RecordDebugValue_(debug_state *ValuePtr, char *GUID, u32 Depth=0);

inline void RecordDebugValue_(key *ValuePtr, char *GUID)
{
	u32 *SimpleValue = (u32 *)ValuePtr;
	switch(*ValuePtr)
	{
        case 0: {DEBUG_VALUE_NAME(SimpleValue, "Key_LeftClick");} break;
        case 1: {DEBUG_VALUE_NAME(SimpleValue, "Key_RightClick");} break;
        case 2: {DEBUG_VALUE_NAME(SimpleValue, "Key_Minus");} break;
        case 3: {DEBUG_VALUE_NAME(SimpleValue, "Key_Plus");} break;
        case 4: {DEBUG_VALUE_NAME(SimpleValue, "Key_OpenBracket");} break;
        case 5: {DEBUG_VALUE_NAME(SimpleValue, "Key_CloseBracket");} break;
        case 6: {DEBUG_VALUE_NAME(SimpleValue, "Key_Q");} break;
        case 7: {DEBUG_VALUE_NAME(SimpleValue, "Key_Up");} break;
        case 8: {DEBUG_VALUE_NAME(SimpleValue, "Key_Down");} break;
        case 9: {DEBUG_VALUE_NAME(SimpleValue, "Key_Left");} break;
        case 10: {DEBUG_VALUE_NAME(SimpleValue, "Key_Right");} break;
        case 11: {DEBUG_VALUE_NAME(SimpleValue, "Key_Space");} break;
        case 12: {DEBUG_VALUE_NAME(SimpleValue, "Key_Enter");} break;
        case 13: {DEBUG_VALUE_NAME(SimpleValue, "Key_F1");} break;
        case 14: {DEBUG_VALUE_NAME(SimpleValue, "Key_F2");} break;
        case 15: {DEBUG_VALUE_NAME(SimpleValue, "Key_F3");} break;
        case 16: {DEBUG_VALUE_NAME(SimpleValue, "Key_F4");} break;
        case 17: {DEBUG_VALUE_NAME(SimpleValue, "Key_F5");} break;
        case 18: {DEBUG_VALUE_NAME(SimpleValue, "Key_F6");} break;
        case 19: {DEBUG_VALUE_NAME(SimpleValue, "Key_F7");} break;
        case 20: {DEBUG_VALUE_NAME(SimpleValue, "Key_F8");} break;
        case 21: {DEBUG_VALUE_NAME(SimpleValue, "Key_F9");} break;
        case 22: {DEBUG_VALUE_NAME(SimpleValue, "Key_F10");} break;
        case 23: {DEBUG_VALUE_NAME(SimpleValue, "Key_F11");} break;
        case 24: {DEBUG_VALUE_NAME(SimpleValue, "Key_F12");} break;
        case 25: {DEBUG_VALUE_NAME(SimpleValue, "Key_Esc");} break;
        case 26: {DEBUG_VALUE_NAME(SimpleValue, "Key_Count");} break;
	}
}

inline void RecordDebugValue_(shader_type *ValuePtr, char *GUID)
{
	u32 *SimpleValue = (u32 *)ValuePtr;
	switch(*ValuePtr)
	{
        case 0: {DEBUG_VALUE_NAME(SimpleValue, "Shader_Null");} break;
        case 1: {DEBUG_VALUE_NAME(SimpleValue, "Shader_Header");} break;
        case 2: {DEBUG_VALUE_NAME(SimpleValue, "Shader_Quads");} break;
        case 3: {DEBUG_VALUE_NAME(SimpleValue, "Shader_Count");} break;
	}
}

inline void RecordDebugValue_(debug_event_type *ValuePtr, char *GUID)
{
	u32 *SimpleValue = (u32 *)ValuePtr;
	switch(*ValuePtr)
	{
        case 0: {DEBUG_VALUE_NAME(SimpleValue, "Event_None");} break;
        case 1: {DEBUG_VALUE_NAME(SimpleValue, "Event_FrameMark");} break;
        case 2: {DEBUG_VALUE_NAME(SimpleValue, "Event_BeginBlock");} break;
        case 3: {DEBUG_VALUE_NAME(SimpleValue, "Event_EndBlock");} break;
        case 4: {DEBUG_VALUE_NAME(SimpleValue, "Event_BeginDataBlock");} break;
        case 5: {DEBUG_VALUE_NAME(SimpleValue, "Event_EndDataBlock");} break;
        case 6: {DEBUG_VALUE_NAME(SimpleValue, "Event_TopClocksList");} break;
        case 7: {DEBUG_VALUE_NAME(SimpleValue, "Event_FrameSlider");} break;
        case 8: {DEBUG_VALUE_NAME(SimpleValue, "Event_FrameBarGraph");} break;
        case 9: {DEBUG_VALUE_NAME(SimpleValue, "Event_MemoryDisplay");} break;
        case 10: {DEBUG_VALUE_NAME(SimpleValue, "Event_AABBTree");} break;
        case 11: {DEBUG_VALUE_NAME(SimpleValue, "Event_char");} break;
        case 12: {DEBUG_VALUE_NAME(SimpleValue, "Event_b32");} break;
        case 13: {DEBUG_VALUE_NAME(SimpleValue, "Event_f32");} break;
        case 14: {DEBUG_VALUE_NAME(SimpleValue, "Event_u8");} break;
        case 15: {DEBUG_VALUE_NAME(SimpleValue, "Event_u16");} break;
        case 16: {DEBUG_VALUE_NAME(SimpleValue, "Event_u32");} break;
        case 17: {DEBUG_VALUE_NAME(SimpleValue, "Event_u64");} break;
        case 18: {DEBUG_VALUE_NAME(SimpleValue, "Event_v2");} break;
        case 19: {DEBUG_VALUE_NAME(SimpleValue, "Event_v2i");} break;
        case 20: {DEBUG_VALUE_NAME(SimpleValue, "Event_v3");} break;
        case 21: {DEBUG_VALUE_NAME(SimpleValue, "Event_v3i");} break;
        case 22: {DEBUG_VALUE_NAME(SimpleValue, "Event_v3u");} break;
        case 23: {DEBUG_VALUE_NAME(SimpleValue, "Event_v4");} break;
        case 24: {DEBUG_VALUE_NAME(SimpleValue, "Event_rectangle2");} break;
        case 25: {DEBUG_VALUE_NAME(SimpleValue, "Event_rectangle3");} break;
        case 26: {DEBUG_VALUE_NAME(SimpleValue, "Event_mat2");} break;
        case 27: {DEBUG_VALUE_NAME(SimpleValue, "Event_mat3");} break;
        case 28: {DEBUG_VALUE_NAME(SimpleValue, "Event_mat4");} break;
        case 29: {DEBUG_VALUE_NAME(SimpleValue, "Event_quaternion");} break;
        case 30: {DEBUG_VALUE_NAME(SimpleValue, "Event_void");} break;
        case 31: {DEBUG_VALUE_NAME(SimpleValue, "Event_string");} break;
        case 32: {DEBUG_VALUE_NAME(SimpleValue, "Event_memory_region");} break;
        case 33: {DEBUG_VALUE_NAME(SimpleValue, "Event_Count");} break;
	}
}

inline void RecordDebugValue_(region_flags *ValuePtr, char *GUID)
{
	u32 *SimpleValue = (u32 *)ValuePtr;
	DEBUG_VALUE_GUID(SimpleValue, GUID);
}

inline void RecordDebugValue_(push_param_flag *ValuePtr, char *GUID)
{
	u32 *SimpleValue = (u32 *)ValuePtr;
	DEBUG_VALUE_GUID(SimpleValue, GUID);
}

inline void RecordDebugValue_(asset_name *ValuePtr, char *GUID)
{
	u32 *SimpleValue = (u32 *)ValuePtr;
	switch(*ValuePtr)
	{
        case 0: {DEBUG_VALUE_NAME(SimpleValue, "Asset_None");} break;
        case 1: {DEBUG_VALUE_NAME(SimpleValue, "Asset_TexturesBegin");} break;
        case 2: {DEBUG_VALUE_NAME(SimpleValue, "Asset_DefaultTexture");} break;
        case 3: {DEBUG_VALUE_NAME(SimpleValue, "Asset_StructuredArt");} break;
        case 4: {DEBUG_VALUE_NAME(SimpleValue, "Asset_Circle");} break;
        case 5: {DEBUG_VALUE_NAME(SimpleValue, "Asset_TexturesEnd");} break;
        case 6: {DEBUG_VALUE_NAME(SimpleValue, "Asset_TextureSetsBegin");} break;
        case 7: {DEBUG_VALUE_NAME(SimpleValue, "Asset_DefaultTextureSet");} break;
        case 8: {DEBUG_VALUE_NAME(SimpleValue, "Asset_TextureSetsEnd");} break;
        case 9: {DEBUG_VALUE_NAME(SimpleValue, "Asset_FontsBegin");} break;
        case 10: {DEBUG_VALUE_NAME(SimpleValue, "Asset_CambriaFont");} break;
        case 11: {DEBUG_VALUE_NAME(SimpleValue, "Asset_LiberationMonoFont");} break;
        case 12: {DEBUG_VALUE_NAME(SimpleValue, "Asset_TimesFont");} break;
        case 13: {DEBUG_VALUE_NAME(SimpleValue, "Asset_FontsEnd");} break;
        case 14: {DEBUG_VALUE_NAME(SimpleValue, "Asset_Count");} break;
	}
}

inline void RecordDebugValue_(texture_type *ValuePtr, char *GUID)
{
	u32 *SimpleValue = (u32 *)ValuePtr;
	switch(*ValuePtr)
	{
        case 0: {DEBUG_VALUE_NAME(SimpleValue, "TextureType_None");} break;
        case 1: {DEBUG_VALUE_NAME(SimpleValue, "TextureType_Albedo");} break;
        case 2: {DEBUG_VALUE_NAME(SimpleValue, "TextureType_Normal");} break;
        case 3: {DEBUG_VALUE_NAME(SimpleValue, "TextureType_Roughness_Metallic_AO_Height");} break;
        case 4: {DEBUG_VALUE_NAME(SimpleValue, "TextureType_Count");} break;
	}
}

inline void RecordDebugValue_(asset_type *ValuePtr, char *GUID)
{
	u32 *SimpleValue = (u32 *)ValuePtr;
	switch(*ValuePtr)
	{
        case 0: {DEBUG_VALUE_NAME(SimpleValue, "AssetType_None");} break;
        case 1: {DEBUG_VALUE_NAME(SimpleValue, "AssetType_Texture");} break;
        case 2: {DEBUG_VALUE_NAME(SimpleValue, "AssetType_TextureSet");} break;
        case 3: {DEBUG_VALUE_NAME(SimpleValue, "AssetType_Font");} break;
        case 4: {DEBUG_VALUE_NAME(SimpleValue, "AssetType_Count");} break;
	}
}

inline void RecordDebugValue_(render_command_type *ValuePtr, char *GUID)
{
	u32 *SimpleValue = (u32 *)ValuePtr;
	switch(*ValuePtr)
	{
        case 0: {DEBUG_VALUE_NAME(SimpleValue, "RenderCommandType_render_entry_clear");} break;
        case 1: {DEBUG_VALUE_NAME(SimpleValue, "RenderCommandType_render_entry_quads");} break;
	}
}

inline void RecordDebugValue_(shape_type *ValuePtr, char *GUID)
{
	u32 *SimpleValue = (u32 *)ValuePtr;
	switch(*ValuePtr)
	{
        case 0: {DEBUG_VALUE_NAME(SimpleValue, "Shape_Circle");} break;
        case 1: {DEBUG_VALUE_NAME(SimpleValue, "Shape_Pill");} break;
        case 2: {DEBUG_VALUE_NAME(SimpleValue, "Shape_ConvexPolygon");} break;
        case 3: {DEBUG_VALUE_NAME(SimpleValue, "Shape_Count");} break;
	}
}

inline void RecordDebugValue_(joint_type *ValuePtr, char *GUID)
{
	u32 *SimpleValue = (u32 *)ValuePtr;
	switch(*ValuePtr)
	{
        case 0: {DEBUG_VALUE_NAME(SimpleValue, "Joint_Revolute");} break;
        case 1: {DEBUG_VALUE_NAME(SimpleValue, "Joint_Prismatic");} break;
        case 2: {DEBUG_VALUE_NAME(SimpleValue, "Joint_Count");} break;
	}
}

inline void RecordDebugValue_(entity_type *ValuePtr, char *GUID)
{
	u32 *SimpleValue = (u32 *)ValuePtr;
	switch(*ValuePtr)
	{
        case 0: {DEBUG_VALUE_NAME(SimpleValue, "Entity_None");} break;
        case 1: {DEBUG_VALUE_NAME(SimpleValue, "Entity_Circle");} break;
        case 2: {DEBUG_VALUE_NAME(SimpleValue, "Entity_Square");} break;
        case 3: {DEBUG_VALUE_NAME(SimpleValue, "Entity_Count");} break;
	}
}

inline void RecordDebugValue_(draw_mode *ValuePtr, char *GUID)
{
	u32 *SimpleValue = (u32 *)ValuePtr;
	switch(*ValuePtr)
	{
        case 0: {DEBUG_VALUE_NAME(SimpleValue, "Mode_AddImpulse");} break;
        case 1: {DEBUG_VALUE_NAME(SimpleValue, "Mode_PlaceBox");} break;
        case 2: {DEBUG_VALUE_NAME(SimpleValue, "Mode_PlaceCircle");} break;
        case 3: {DEBUG_VALUE_NAME(SimpleValue, "Mode_Count");} break;
	}
}

inline void RecordDebugValue_(interaction_type *ValuePtr, char *GUID)
{
	u32 *SimpleValue = (u32 *)ValuePtr;
	switch(*ValuePtr)
	{
        case 0: {DEBUG_VALUE_NAME(SimpleValue, "Interaction_None");} break;
        case 1: {DEBUG_VALUE_NAME(SimpleValue, "Interaction_Collapse");} break;
        case 2: {DEBUG_VALUE_NAME(SimpleValue, "Interaction_Resize");} break;
        case 3: {DEBUG_VALUE_NAME(SimpleValue, "Interaction_ChangeValue");} break;
        case 4: {DEBUG_VALUE_NAME(SimpleValue, "Interaction_DragValue");} break;
        case 5: {DEBUG_VALUE_NAME(SimpleValue, "Interaction_FrameSelect");} break;
        case 6: {DEBUG_VALUE_NAME(SimpleValue, "Interaction_Count");} break;
	}
}


inline void
RecordDebugValue_(sphere *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    DEBUG_VALUE_NAME(&ValuePtr->Center, "Center");
    {DEBUG_DATA_BLOCK("Radius");
        DEBUG_VALUE_NAME(&ValuePtr->Radius, "Radius");
    }
}

inline void
RecordDebugValue_(key_state *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    DEBUG_VALUE_NAME(&ValuePtr->Pressed, "Pressed");
    DEBUG_VALUE_NAME(&ValuePtr->WasPressed, "WasPressed");
    DEBUG_VALUE_NAME(&ValuePtr->WasReleased, "WasReleased");
    DEBUG_VALUE_NAME(&ValuePtr->TransitionCount, "TransitionCount");
    DEBUG_VALUE_NAME(&ValuePtr->DebugPressed, "DebugPressed");
    DEBUG_VALUE_NAME(&ValuePtr->DebugWasPressed, "DebugWasPressed");
    DEBUG_VALUE_NAME(&ValuePtr->DebugWasReleased, "DebugWasReleased");
    DEBUG_VALUE_NAME(&ValuePtr->DebugTransitionCount, "DebugTransitionCount");
}

inline void
RecordDebugValue_(game_input *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    {DEBUG_DATA_BLOCK("KeyStates");
        RecordDebugValue_(ValuePtr->KeyStates, DEBUG_NAME("KeyStates"), Depth+1);
    }
    DEBUG_VALUE_NAME(&ValuePtr->MouseP, "MouseP");
    {DEBUG_DATA_BLOCK("LastFramedt_");
        DEBUG_VALUE_NAME(&ValuePtr->LastFramedt_, "LastFramedt_");
    }
    {DEBUG_DATA_BLOCK("Simdt");
        DEBUG_VALUE_NAME(&ValuePtr->Simdt, "Simdt");
    }
}

inline void
RecordDebugValue_(command_buffer *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    DEBUG_VALUE_NAME(&ValuePtr->Size, "Size");
    DEBUG_VALUE_NAME(&ValuePtr->MaxSize, "MaxSize");
    DEBUG_VALUE_NAME(ValuePtr->Buffer, "Buffer");
    DEBUG_VALUE_NAME(&ValuePtr->QuadCount, "QuadCount");
    DEBUG_VALUE_NAME(&ValuePtr->MaxQuads, "MaxQuads");
    {DEBUG_DATA_BLOCK("QuadVertices");
        RecordDebugValue_(ValuePtr->QuadVertices, DEBUG_NAME("QuadVertices"), Depth+1);
    }
    DEBUG_VALUE_NAME(ValuePtr->TextureData, "TextureData");
    DEBUG_VALUE_NAME(&ValuePtr->WhiteTexture, "WhiteTexture");
    {DEBUG_DATA_BLOCK("CurrentRenderGroup");
        RecordDebugValue_(ValuePtr->CurrentRenderGroup, DEBUG_NAME("CurrentRenderGroup"), Depth+1);
    }
}

inline void
RecordDebugValue_(game_memory *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    {DEBUG_DATA_BLOCK("GameState");
        RecordDebugValue_(ValuePtr->GameState, DEBUG_NAME("GameState"), Depth+1);
    }
    DEBUG_VALUE_NAME(&ValuePtr->WindowResolution, "WindowResolution");
    {DEBUG_DATA_BLOCK("CommandBuffer");
        DEBUG_VALUE_NAME(&ValuePtr->CommandBuffer, "CommandBuffer");
    }
    {DEBUG_DATA_BLOCK("Platform");
        RecordDebugValue_(ValuePtr->Platform, DEBUG_NAME("Platform"), Depth+1);
    }
    {DEBUG_DATA_BLOCK("DebugTable");
        RecordDebugValue_(ValuePtr->DebugTable, DEBUG_NAME("DebugTable"), Depth+1);
    }
    {DEBUG_DATA_BLOCK("DebugState");
        RecordDebugValue_(ValuePtr->DebugState, DEBUG_NAME("DebugState"), Depth+1);
    }
}

inline void
RecordDebugValue_(platform_file *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    DEBUG_VALUE_NAME(ValuePtr->Handle_, "Handle_");
}

inline void
RecordDebugValue_(platform_memory_block *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    DEBUG_VALUE_NAME(ValuePtr->Base, "Base");
    {DEBUG_DATA_BLOCK("Size");
        DEBUG_VALUE_NAME(&ValuePtr->Size, "Size");
    }
    {DEBUG_DATA_BLOCK("Used");
        DEBUG_VALUE_NAME(&ValuePtr->Used, "Used");
    }
    DEBUG_VALUE_NAME(ValuePtr->Padding, "Padding");
}

inline void
RecordDebugValue_(debug_event *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    DEBUG_VALUE_NAME(ValuePtr->GUID, "GUID");
    {DEBUG_DATA_BLOCK("Type");
        DEBUG_VALUE_NAME(&ValuePtr->Type, "Type");
    }
    DEBUG_VALUE_NAME(&ValuePtr->Tick, "Tick");
    DEBUG_VALUE_NAME(&ValuePtr->Value_u64, "Value_u64");
    DEBUG_VALUE_NAME(ValuePtr->Value_ptr, "Value_ptr");
}

inline void
RecordDebugValue_(debug_table *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    DEBUG_VALUE_NAME(&ValuePtr->Recording, "Recording");
    DEBUG_VALUE_NAME(&ValuePtr->ArrayIndex, "ArrayIndex");
    DEBUG_VALUE_NAME(&ValuePtr->EventCount, "EventCount");
    {DEBUG_DATA_BLOCK("DebugEvents");
        RecordDebugValue_(ValuePtr->DebugEvents, DEBUG_NAME("DebugEvents"), Depth+1);
    }
}

inline void
RecordDebugValue_(platform *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    {DEBUG_DATA_BLOCK("LoadEntireFile");
        RecordDebugValue_(ValuePtr->LoadEntireFile, DEBUG_NAME("LoadEntireFile"), Depth+1);
    }
    {DEBUG_DATA_BLOCK("LoadEntireTextFileAndNullTerminate");
        RecordDebugValue_(ValuePtr->LoadEntireTextFileAndNullTerminate, DEBUG_NAME("LoadEntireTextFileAndNullTerminate"), Depth+1);
    }
    {DEBUG_DATA_BLOCK("LoadMesh");
        RecordDebugValue_(ValuePtr->LoadMesh, DEBUG_NAME("LoadMesh"), Depth+1);
    }
    {DEBUG_DATA_BLOCK("LoadTexture");
        RecordDebugValue_(ValuePtr->LoadTexture, DEBUG_NAME("LoadTexture"), Depth+1);
    }
    {DEBUG_DATA_BLOCK("LoadFontTexture");
        RecordDebugValue_(ValuePtr->LoadFontTexture, DEBUG_NAME("LoadFontTexture"), Depth+1);
    }
    {DEBUG_DATA_BLOCK("AllocateMemory");
        RecordDebugValue_(ValuePtr->AllocateMemory, DEBUG_NAME("AllocateMemory"), Depth+1);
    }
    {DEBUG_DATA_BLOCK("DeallocateMemory");
        RecordDebugValue_(ValuePtr->DeallocateMemory, DEBUG_NAME("DeallocateMemory"), Depth+1);
    }
    {DEBUG_DATA_BLOCK("OpenAssetPackFile");
        RecordDebugValue_(ValuePtr->OpenAssetPackFile, DEBUG_NAME("OpenAssetPackFile"), Depth+1);
    }
    {DEBUG_DATA_BLOCK("ReadFromFile");
        RecordDebugValue_(ValuePtr->ReadFromFile, DEBUG_NAME("ReadFromFile"), Depth+1);
    }
    {DEBUG_DATA_BLOCK("CloseFile");
        RecordDebugValue_(ValuePtr->CloseFile, DEBUG_NAME("CloseFile"), Depth+1);
    }
}

inline void
RecordDebugValue_(bit_scan_result *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    {DEBUG_DATA_BLOCK("Found");
        DEBUG_VALUE_NAME(&ValuePtr->Found, "Found");
    }
    {DEBUG_DATA_BLOCK("Index");
        DEBUG_VALUE_NAME(&ValuePtr->Index, "Index");
    }
}

inline void
RecordDebugValue_(temporary_memory *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    DEBUG_VALUE_NAME(ValuePtr->Region, "Region");
    {DEBUG_DATA_BLOCK("UsedMarked");
        DEBUG_VALUE_NAME(&ValuePtr->UsedMarked, "UsedMarked");
    }
    DEBUG_VALUE_NAME(&ValuePtr->TemporaryCountMarked, "TemporaryCountMarked");
    {DEBUG_DATA_BLOCK("Block");
        RecordDebugValue_(ValuePtr->Block, DEBUG_NAME("Block"), Depth+1);
    }
}

inline void
RecordDebugValue_(push_params *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    DEBUG_VALUE_NAME(&ValuePtr->Flags, "Flags");
    DEBUG_VALUE_NAME(&ValuePtr->Alignment, "Alignment");
}

inline void
RecordDebugValue_(bootstrap_params *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    DEBUG_VALUE_NAME(&ValuePtr->Flags, "Flags");
    {DEBUG_DATA_BLOCK("MinimumBlockSize");
        DEBUG_VALUE_NAME(&ValuePtr->MinimumBlockSize, "MinimumBlockSize");
    }
}

inline void
RecordDebugValue_(region_stats *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    {DEBUG_DATA_BLOCK("TotalSize");
        DEBUG_VALUE_NAME(&ValuePtr->TotalSize, "TotalSize");
    }
    {DEBUG_DATA_BLOCK("TotalUsed");
        DEBUG_VALUE_NAME(&ValuePtr->TotalUsed, "TotalUsed");
    }
    DEBUG_VALUE_NAME(&ValuePtr->BlockCount, "BlockCount");
}

inline void
RecordDebugValue_(string *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    DEBUG_VALUE_NAME(&ValuePtr->Length, "Length");
    DEBUG_VALUE_NAME(ValuePtr->Text, "Text");
}

inline void
RecordDebugValue_(format_builder *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    DEBUG_VALUE_NAME(ValuePtr->Dest, "Dest");
    {DEBUG_DATA_BLOCK("Size");
        DEBUG_VALUE_NAME(&ValuePtr->Size, "Size");
    }
}

inline void
RecordDebugValue_(header_sla *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    DEBUG_VALUE_NAME(&ValuePtr->MagicCode, "MagicCode");
    DEBUG_VALUE_NAME(&ValuePtr->Version, "Version");
    DEBUG_VALUE_NAME(&ValuePtr->MeshCount, "MeshCount");
    DEBUG_VALUE_NAME(&ValuePtr->TextureCount, "TextureCount");
    DEBUG_VALUE_NAME(&ValuePtr->FontCount, "FontCount");
    DEBUG_VALUE_NAME(&ValuePtr->DataOffset, "DataOffset");
}

inline void
RecordDebugValue_(texture_sla *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    {DEBUG_DATA_BLOCK("Name");
        DEBUG_VALUE_NAME(&ValuePtr->Name, "Name");
    }
    {DEBUG_DATA_BLOCK("Type");
        DEBUG_VALUE_NAME(&ValuePtr->Type, "Type");
    }
    DEBUG_VALUE_NAME(&ValuePtr->Width, "Width");
    DEBUG_VALUE_NAME(&ValuePtr->Height, "Height");
    DEBUG_VALUE_NAME(&ValuePtr->PixelsOffset, "PixelsOffset");
}

inline void
RecordDebugValue_(font_sla *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    {DEBUG_DATA_BLOCK("Name");
        DEBUG_VALUE_NAME(&ValuePtr->Name, "Name");
    }
    {DEBUG_DATA_BLOCK("Scale");
        DEBUG_VALUE_NAME(&ValuePtr->Scale, "Scale");
    }
    {DEBUG_DATA_BLOCK("Ascent");
        DEBUG_VALUE_NAME(&ValuePtr->Ascent, "Ascent");
    }
    {DEBUG_DATA_BLOCK("Descent");
        DEBUG_VALUE_NAME(&ValuePtr->Descent, "Descent");
    }
    {DEBUG_DATA_BLOCK("Linegap");
        DEBUG_VALUE_NAME(&ValuePtr->Linegap, "Linegap");
    }
    DEBUG_VALUE_NAME(ValuePtr->Offset, "Offset");
    {DEBUG_DATA_BLOCK("Advance");
        RecordDebugValue_(ValuePtr->Advance, DEBUG_NAME("Advance"), Depth+1);
    }
    {DEBUG_DATA_BLOCK("Kerning");
        RecordDebugValue_(ValuePtr->Kerning, DEBUG_NAME("Kerning"), Depth+1);
    }
    DEBUG_VALUE_NAME(ValuePtr->TexCoordMin, "TexCoordMin");
    DEBUG_VALUE_NAME(ValuePtr->TexCoordMax, "TexCoordMax");
    DEBUG_VALUE_NAME(&ValuePtr->Width, "Width");
    DEBUG_VALUE_NAME(&ValuePtr->Height, "Height");
    DEBUG_VALUE_NAME(&ValuePtr->PixelsOffset, "PixelsOffset");
}

inline void
RecordDebugValue_(vertex_format *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    DEBUG_VALUE_NAME(&ValuePtr->Position, "Position");
    DEBUG_VALUE_NAME(&ValuePtr->TexCoords, "TexCoords");
    DEBUG_VALUE_NAME(&ValuePtr->Color, "Color");
}

inline void
RecordDebugValue_(sort_entry *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    DEBUG_VALUE_NAME(&ValuePtr->Value, "Value");
    DEBUG_VALUE_NAME(&ValuePtr->Index, "Index");
}

inline void
RecordDebugValue_(random_series *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    DEBUG_VALUE_NAME(&ValuePtr->Seed, "Seed");
    DEBUG_VALUE_NAME(&ValuePtr->Index, "Index");
}

inline void
RecordDebugValue_(loaded_bitmap *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    DEBUG_VALUE_NAME(&ValuePtr->Width, "Width");
    DEBUG_VALUE_NAME(&ValuePtr->Height, "Height");
    DEBUG_VALUE_NAME(ValuePtr->Pixels, "Pixels");
}

inline void
RecordDebugValue_(texture *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    {DEBUG_DATA_BLOCK("MetaData");
        RecordDebugValue_(ValuePtr->MetaData, DEBUG_NAME("MetaData"), Depth+1);
    }
    DEBUG_VALUE_NAME(&ValuePtr->Data_, "Data_");
}

inline void
RecordDebugValue_(texture_set *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    {DEBUG_DATA_BLOCK("Textures");
        RecordDebugValue_(ValuePtr->Textures, DEBUG_NAME("Textures"), Depth+1);
    }
}

inline void
RecordDebugValue_(font *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    {DEBUG_DATA_BLOCK("MetaData");
        RecordDebugValue_(ValuePtr->MetaData, DEBUG_NAME("MetaData"), Depth+1);
    }
    DEBUG_VALUE_NAME(&ValuePtr->Data_, "Data_");
}

inline void
RecordDebugValue_(asset *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    {DEBUG_DATA_BLOCK("Type");
        DEBUG_VALUE_NAME(&ValuePtr->Type, "Type");
    }
    {DEBUG_DATA_BLOCK("Font");
        RecordDebugValue_(ValuePtr->Font, DEBUG_NAME("Font"), Depth+1);
    }
    {DEBUG_DATA_BLOCK("Texture");
        RecordDebugValue_(ValuePtr->Texture, DEBUG_NAME("Texture"), Depth+1);
    }
    {DEBUG_DATA_BLOCK("TextureSet");
        RecordDebugValue_(ValuePtr->TextureSet, DEBUG_NAME("TextureSet"), Depth+1);
    }
}

inline void
RecordDebugValue_(asset_metadata *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    {DEBUG_DATA_BLOCK("Header");
        RecordDebugValue_(ValuePtr->Header, DEBUG_NAME("Header"), Depth+1);
    }
    {DEBUG_DATA_BLOCK("TextureData");
        RecordDebugValue_(ValuePtr->TextureData, DEBUG_NAME("TextureData"), Depth+1);
    }
    {DEBUG_DATA_BLOCK("FontData");
        RecordDebugValue_(ValuePtr->FontData, DEBUG_NAME("FontData"), Depth+1);
    }
}

inline void
RecordDebugValue_(assets *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    DEBUG_VALUE_NAME(&ValuePtr->Region, "Region");
    {DEBUG_DATA_BLOCK("MetaData");
        DEBUG_VALUE_NAME(&ValuePtr->MetaData, "MetaData");
    }
    {DEBUG_DATA_BLOCK("AssetFile");
        DEBUG_VALUE_NAME(&ValuePtr->AssetFile, "AssetFile");
    }
    {DEBUG_DATA_BLOCK("Assets");
        RecordDebugValue_(ValuePtr->Assets, DEBUG_NAME("Assets"), Depth+1);
    }
}

inline void
RecordDebugValue_(text_layout *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    {DEBUG_DATA_BLOCK("FontName");
        DEBUG_VALUE_NAME(&ValuePtr->FontName, "FontName");
    }
    DEBUG_VALUE_NAME(&ValuePtr->Scale, "Scale");
    DEBUG_VALUE_NAME(&ValuePtr->Color, "Color");
    DEBUG_VALUE_NAME(&ValuePtr->AtY, "AtY");
}

inline void
RecordDebugValue_(render_setup *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    DEBUG_VALUE_NAME(&ValuePtr->ClipRectMin, "ClipRectMin");
    DEBUG_VALUE_NAME(&ValuePtr->ClipRectMax, "ClipRectMax");
    DEBUG_VALUE_NAME(&ValuePtr->Projection, "Projection");
    DEBUG_VALUE_NAME(&ValuePtr->View, "View");
}

inline void
RecordDebugValue_(render_entry_header *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    {DEBUG_DATA_BLOCK("Type");
        DEBUG_VALUE_NAME(&ValuePtr->Type, "Type");
    }
}

inline void
RecordDebugValue_(render_entry_quads *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    {DEBUG_DATA_BLOCK("Setup");
        DEBUG_VALUE_NAME(&ValuePtr->Setup, "Setup");
    }
    DEBUG_VALUE_NAME(&ValuePtr->FirstQuadIndex, "FirstQuadIndex");
    DEBUG_VALUE_NAME(&ValuePtr->QuadCount, "QuadCount");
}

inline void
RecordDebugValue_(render_entry_clear *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    DEBUG_VALUE_NAME(&ValuePtr->Color, "Color");
}

inline void
RecordDebugValue_(camera *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    {DEBUG_DATA_BLOCK("HalfWidth");
        DEBUG_VALUE_NAME(&ValuePtr->HalfWidth, "HalfWidth");
    }
    {DEBUG_DATA_BLOCK("HalfHeight");
        DEBUG_VALUE_NAME(&ValuePtr->HalfHeight, "HalfHeight");
    }
    {DEBUG_DATA_BLOCK("Near");
        DEBUG_VALUE_NAME(&ValuePtr->Near, "Near");
    }
    {DEBUG_DATA_BLOCK("Far");
        DEBUG_VALUE_NAME(&ValuePtr->Far, "Far");
    }
    DEBUG_VALUE_NAME(&ValuePtr->P, "P");
    {DEBUG_DATA_BLOCK("Rotation");
        DEBUG_VALUE_NAME(&ValuePtr->Rotation, "Rotation");
    }
    DEBUG_VALUE_NAME(&ValuePtr->Valid_, "Valid_");
    DEBUG_VALUE_NAME(&ValuePtr->Projection_, "Projection_");
    DEBUG_VALUE_NAME(&ValuePtr->InvProjection_, "InvProjection_");
    DEBUG_VALUE_NAME(&ValuePtr->View_, "View_");
    DEBUG_VALUE_NAME(&ValuePtr->InvView_, "InvView_");
}

inline void
RecordDebugValue_(render_group *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    {DEBUG_DATA_BLOCK("Assets");
        RecordDebugValue_(ValuePtr->Assets, DEBUG_NAME("Assets"), Depth+1);
    }
    DEBUG_VALUE_NAME(&ValuePtr->ScreenResolution, "ScreenResolution");
    {DEBUG_DATA_BLOCK("LastSetup");
        DEBUG_VALUE_NAME(&ValuePtr->LastSetup, "LastSetup");
    }
    {DEBUG_DATA_BLOCK("CurrentQuadEntry");
        RecordDebugValue_(ValuePtr->CurrentQuadEntry, DEBUG_NAME("CurrentQuadEntry"), Depth+1);
    }
    {DEBUG_DATA_BLOCK("CommandBuffer");
        RecordDebugValue_(ValuePtr->CommandBuffer, DEBUG_NAME("CommandBuffer"), Depth+1);
    }
}

inline void
RecordDebugValue_(minkowski_difference_support *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    DEBUG_VALUE_NAME(&ValuePtr->SupportA, "SupportA");
    DEBUG_VALUE_NAME(&ValuePtr->SupportB, "SupportB");
    DEBUG_VALUE_NAME(&ValuePtr->Support, "Support");
}

inline void
RecordDebugValue_(minkowski_simplex *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    DEBUG_VALUE_NAME(&ValuePtr->Size, "Size");
    {DEBUG_DATA_BLOCK("Points");
        RecordDebugValue_(ValuePtr->Points, DEBUG_NAME("Points"), Depth+1);
    }
    DEBUG_VALUE_NAME(&ValuePtr->ContainsOrigin, "ContainsOrigin");
}

inline void
RecordDebugValue_(expanding_polygon_vertex *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    {DEBUG_DATA_BLOCK("Vertex");
        DEBUG_VALUE_NAME(&ValuePtr->Vertex, "Vertex");
    }
}

inline void
RecordDebugValue_(expanding_polygon *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    DEBUG_VALUE_NAME(&ValuePtr->VertexCount, "VertexCount");
    {DEBUG_DATA_BLOCK("Sentinel");
        DEBUG_VALUE_NAME(&ValuePtr->Sentinel, "Sentinel");
    }
    {DEBUG_DATA_BLOCK("FirstFree");
        RecordDebugValue_(ValuePtr->FirstFree, DEBUG_NAME("FirstFree"), Depth+1);
    }
}

inline void
RecordDebugValue_(circle *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    DEBUG_VALUE_NAME(&ValuePtr->Center, "Center");
    DEBUG_VALUE_NAME(&ValuePtr->Radius, "Radius");
}

inline void
RecordDebugValue_(pill *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    DEBUG_VALUE_NAME(&ValuePtr->P0, "P0");
    DEBUG_VALUE_NAME(&ValuePtr->P1, "P1");
    DEBUG_VALUE_NAME(&ValuePtr->Radius, "Radius");
}

inline void
RecordDebugValue_(convex_polygon *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    DEBUG_VALUE_NAME(&ValuePtr->VertexCount, "VertexCount");
    DEBUG_VALUE_NAME(ValuePtr->Vertices, "Vertices");
}

inline void
RecordDebugValue_(shape *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    {DEBUG_DATA_BLOCK("Type");
        DEBUG_VALUE_NAME(&ValuePtr->Type, "Type");
    }
    {DEBUG_DATA_BLOCK("Circle");
        DEBUG_VALUE_NAME(&ValuePtr->Circle, "Circle");
    }
    {DEBUG_DATA_BLOCK("Pill");
        DEBUG_VALUE_NAME(&ValuePtr->Pill, "Pill");
    }
    {DEBUG_DATA_BLOCK("Polygon");
        DEBUG_VALUE_NAME(&ValuePtr->Polygon, "Polygon");
    }
}

inline void
RecordDebugValue_(rigid_body *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    DEBUG_VALUE_NAME(&ValuePtr->Index, "Index");
    {DEBUG_DATA_BLOCK("Entity");
        RecordDebugValue_(ValuePtr->Entity, DEBUG_NAME("Entity"), Depth+1);
    }
    DEBUG_VALUE_NAME(&ValuePtr->Collideable, "Collideable");
    DEBUG_VALUE_NAME(&ValuePtr->InvMass, "InvMass");
    DEBUG_VALUE_NAME(&ValuePtr->InvMomentOfInertia, "InvMomentOfInertia");
    DEBUG_VALUE_NAME(&ValuePtr->WorldCentroid, "WorldCentroid");
    DEBUG_VALUE_NAME(&ValuePtr->LocalCentroid, "LocalCentroid");
    DEBUG_VALUE_NAME(&ValuePtr->P, "P");
    DEBUG_VALUE_NAME(&ValuePtr->Rotation, "Rotation");
    DEBUG_VALUE_NAME(&ValuePtr->InvRotation, "InvRotation");
    DEBUG_VALUE_NAME(&ValuePtr->OldP, "OldP");
    DEBUG_VALUE_NAME(&ValuePtr->OldRotation, "OldRotation");
    DEBUG_VALUE_NAME(&ValuePtr->OldInvRotation, "OldInvRotation");
    DEBUG_VALUE_NAME(&ValuePtr->dP, "dP");
    DEBUG_VALUE_NAME(&ValuePtr->AngularVelocity, "AngularVelocity");
    {DEBUG_DATA_BLOCK("Shape");
        DEBUG_VALUE_NAME(&ValuePtr->Shape, "Shape");
    }
    {DEBUG_DATA_BLOCK("AABBNode");
        RecordDebugValue_(ValuePtr->AABBNode, DEBUG_NAME("AABBNode"), Depth+1);
    }
}

inline void
RecordDebugValue_(index_pair *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    DEBUG_VALUE_NAME(&ValuePtr->IndexA, "IndexA");
    DEBUG_VALUE_NAME(&ValuePtr->IndexB, "IndexB");
}

inline void
RecordDebugValue_(contact_data *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    DEBUG_VALUE_NAME(&ValuePtr->WorldContactPointA, "WorldContactPointA");
    DEBUG_VALUE_NAME(&ValuePtr->WorldContactPointB, "WorldContactPointB");
    DEBUG_VALUE_NAME(&ValuePtr->LocalContactPointA, "LocalContactPointA");
    DEBUG_VALUE_NAME(&ValuePtr->LocalContactPointB, "LocalContactPointB");
    DEBUG_VALUE_NAME(&ValuePtr->Normal, "Normal");
    DEBUG_VALUE_NAME(&ValuePtr->Penetration, "Penetration");
    DEBUG_VALUE_NAME(&ValuePtr->NormalImpulseSum, "NormalImpulseSum");
    DEBUG_VALUE_NAME(&ValuePtr->TangentImpulseSum, "TangentImpulseSum");
    DEBUG_VALUE_NAME(&ValuePtr->Persistent, "Persistent");
}

inline void
RecordDebugValue_(contact_manifold *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    {DEBUG_DATA_BLOCK("Pair");
        DEBUG_VALUE_NAME(&ValuePtr->Pair, "Pair");
    }
    DEBUG_VALUE_NAME(&ValuePtr->ContactCount, "ContactCount");
    {DEBUG_DATA_BLOCK("Contacts");
        RecordDebugValue_(ValuePtr->Contacts, DEBUG_NAME("Contacts"), Depth+1);
    }
}

inline void
RecordDebugValue_(joint *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    {DEBUG_DATA_BLOCK("Type");
        DEBUG_VALUE_NAME(&ValuePtr->Type, "Type");
    }
    {DEBUG_DATA_BLOCK("Pair");
        DEBUG_VALUE_NAME(&ValuePtr->Pair, "Pair");
    }
    DEBUG_VALUE_NAME(&ValuePtr->LocalAnchorA, "LocalAnchorA");
    DEBUG_VALUE_NAME(&ValuePtr->LocalAnchorB, "LocalAnchorB");
    DEBUG_VALUE_NAME(&ValuePtr->Collideable, "Collideable");
}

inline void
RecordDebugValue_(bucket *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    DEBUG_VALUE_NAME(&ValuePtr->PotentialCollisionCount, "PotentialCollisionCount");
    {DEBUG_DATA_BLOCK("PotentialCollisions");
        RecordDebugValue_(ValuePtr->PotentialCollisions, DEBUG_NAME("PotentialCollisions"), Depth+1);
    }
}

inline void
RecordDebugValue_(aabb_query_result *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    DEBUG_VALUE_NAME(&ValuePtr->Count, "Count");
    DEBUG_VALUE_NAME(ValuePtr->BodyIndices, "BodyIndices");
}

inline void
RecordDebugValue_(aabb_tree_node *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    DEBUG_VALUE_NAME(&ValuePtr->BodyIndex, "BodyIndex");
    DEBUG_VALUE_NAME(&ValuePtr->Height, "Height");
    DEBUG_VALUE_NAME(&ValuePtr->AABB, "AABB");
}

inline void
RecordDebugValue_(physics_state *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    DEBUG_VALUE_NAME(&ValuePtr->Region, "Region");
    DEBUG_VALUE_NAME(&ValuePtr->TempRegion, "TempRegion");
    DEBUG_VALUE_NAME(&ValuePtr->RigidBodyCount, "RigidBodyCount");
    {DEBUG_DATA_BLOCK("RigidBodies");
        RecordDebugValue_(ValuePtr->RigidBodies, DEBUG_NAME("RigidBodies"), Depth+1);
    }
    DEBUG_VALUE_NAME(&ValuePtr->BucketCount, "BucketCount");
    {DEBUG_DATA_BLOCK("PotentialCollisionBuckets");
        RecordDebugValue_(ValuePtr->PotentialCollisionBuckets, DEBUG_NAME("PotentialCollisionBuckets"), Depth+1);
    }
    {DEBUG_DATA_BLOCK("ContactManifoldHashTable");
        RecordDebugValue_(ValuePtr->ContactManifoldHashTable, DEBUG_NAME("ContactManifoldHashTable"), Depth+1);
    }
    {DEBUG_DATA_BLOCK("ManifoldFreeList");
        RecordDebugValue_(ValuePtr->ManifoldFreeList, DEBUG_NAME("ManifoldFreeList"), Depth+1);
    }
    {DEBUG_DATA_BLOCK("ManifoldSentinel");
        DEBUG_VALUE_NAME(&ValuePtr->ManifoldSentinel, "ManifoldSentinel");
    }
    DEBUG_VALUE_NAME(&ValuePtr->JointCount, "JointCount");
    {DEBUG_DATA_BLOCK("Joints");
        RecordDebugValue_(ValuePtr->Joints, DEBUG_NAME("Joints"), Depth+1);
    }
    DEBUG_VALUE_NAME(&ValuePtr->UncollideableCount, "UncollideableCount");
    {DEBUG_DATA_BLOCK("Uncollideables");
        RecordDebugValue_(ValuePtr->Uncollideables, DEBUG_NAME("Uncollideables"), Depth+1);
    }
    {DEBUG_DATA_BLOCK("AABBRoot");
        RecordDebugValue_(ValuePtr->AABBRoot, DEBUG_NAME("AABBRoot"), Depth+1);
    }
    {DEBUG_DATA_BLOCK("AABBNodeFreeList");
        RecordDebugValue_(ValuePtr->AABBNodeFreeList, DEBUG_NAME("AABBNodeFreeList"), Depth+1);
    }
}

inline void
RecordDebugValue_(move_spec *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    DEBUG_VALUE_NAME(&ValuePtr->LinearAirFriction, "LinearAirFriction");
    DEBUG_VALUE_NAME(&ValuePtr->RotationAirFriction, "RotationAirFriction");
}

inline void
RecordDebugValue_(entity *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    {DEBUG_DATA_BLOCK("Type");
        DEBUG_VALUE_NAME(&ValuePtr->Type, "Type");
    }
    DEBUG_VALUE_NAME(&ValuePtr->Dim, "Dim");
    DEBUG_VALUE_NAME(&ValuePtr->Color, "Color");
    {DEBUG_DATA_BLOCK("MoveSpec");
        DEBUG_VALUE_NAME(&ValuePtr->MoveSpec, "MoveSpec");
    }
    {DEBUG_DATA_BLOCK("RigidBody");
        RecordDebugValue_(ValuePtr->RigidBody, DEBUG_NAME("RigidBody"), Depth+1);
    }
}

inline void
RecordDebugValue_(game_state *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    DEBUG_VALUE_NAME(&ValuePtr->Region, "Region");
    DEBUG_VALUE_NAME(&ValuePtr->TemporaryRegion, "TemporaryRegion");
    {DEBUG_DATA_BLOCK("Assets");
        RecordDebugValue_(ValuePtr->Assets, DEBUG_NAME("Assets"), Depth+1);
    }
    {DEBUG_DATA_BLOCK("RenderGroup");
        RecordDebugValue_(ValuePtr->RenderGroup, DEBUG_NAME("RenderGroup"), Depth+1);
    }
    {DEBUG_DATA_BLOCK("Physics");
        RecordDebugValue_(ValuePtr->Physics, DEBUG_NAME("Physics"), Depth+1);
    }
    {DEBUG_DATA_BLOCK("WorldCamera");
        DEBUG_VALUE_NAME(&ValuePtr->WorldCamera, "WorldCamera");
    }
    {DEBUG_DATA_BLOCK("ScreenCamera");
        DEBUG_VALUE_NAME(&ValuePtr->ScreenCamera, "ScreenCamera");
    }
    {DEBUG_DATA_BLOCK("DefaultMouseCamera");
        DEBUG_VALUE_NAME(&ValuePtr->DefaultMouseCamera, "DefaultMouseCamera");
    }
    {DEBUG_DATA_BLOCK("OldWorldCamera");
        DEBUG_VALUE_NAME(&ValuePtr->OldWorldCamera, "OldWorldCamera");
    }
    {DEBUG_DATA_BLOCK("DrawMode");
        DEBUG_VALUE_NAME(&ValuePtr->DrawMode, "DrawMode");
    }
    DEBUG_VALUE_NAME(&ValuePtr->TimeScale, "TimeScale");
    {DEBUG_DATA_BLOCK("EntitySentinel");
        DEBUG_VALUE_NAME(&ValuePtr->EntitySentinel, "EntitySentinel");
    }
    {DEBUG_DATA_BLOCK("EntityFreeList");
        RecordDebugValue_(ValuePtr->EntityFreeList, DEBUG_NAME("EntityFreeList"), Depth+1);
    }
    DEBUG_VALUE_NAME(&ValuePtr->EntityCount, "EntityCount");
    {DEBUG_DATA_BLOCK("ClickedEntity");
        RecordDebugValue_(ValuePtr->ClickedEntity, DEBUG_NAME("ClickedEntity"), Depth+1);
    }
    DEBUG_VALUE_NAME(&ValuePtr->ClickedP, "ClickedP");
    {DEBUG_DATA_BLOCK("Highlight");
        RecordDebugValue_(ValuePtr->Highlight, DEBUG_NAME("Highlight"), Depth+1);
    }
    {DEBUG_DATA_BLOCK("ControlledEntity");
        RecordDebugValue_(ValuePtr->ControlledEntity, DEBUG_NAME("ControlledEntity"), Depth+1);
    }
}

inline void
RecordDebugValue_(debug_parsed_guid *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    DEBUG_VALUE_NAME(&ValuePtr->HashValue, "HashValue");
    DEBUG_VALUE_NAME(&ValuePtr->LineNumber, "LineNumber");
    DEBUG_VALUE_NAME(ValuePtr->Filename, "Filename");
    DEBUG_VALUE_NAME(ValuePtr->Name, "Name");
}

inline void
RecordDebugValue_(debug_id *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    DEBUG_VALUE_NAME(ValuePtr->Value, "Value");
    {DEBUG_DATA_BLOCK("Parsed");
        DEBUG_VALUE_NAME(&ValuePtr->Parsed, "Parsed");
    }
    DEBUG_VALUE_NAME(&ValuePtr->IsValid, "IsValid");
}

inline void
RecordDebugValue_(debug_stored_event *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    {DEBUG_DATA_BLOCK("ID");
        DEBUG_VALUE_NAME(&ValuePtr->ID, "ID");
    }
    {DEBUG_DATA_BLOCK("Type");
        DEBUG_VALUE_NAME(&ValuePtr->Type, "Type");
    }
    DEBUG_VALUE_NAME(&ValuePtr->Tick, "Tick");
    DEBUG_VALUE_NAME(&ValuePtr->Value_u64, "Value_u64");
    DEBUG_VALUE_NAME(ValuePtr->Value_ptr, "Value_ptr");
}

inline void
RecordDebugValue_(debug_ui_element *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    DEBUG_VALUE_NAME(&ValuePtr->FrameIndex, "FrameIndex");
    DEBUG_VALUE_NAME(&ValuePtr->Depth, "Depth");
    {DEBUG_DATA_BLOCK("StoredEvent");
        RecordDebugValue_(ValuePtr->StoredEvent, DEBUG_NAME("StoredEvent"), Depth+1);
    }
    DEBUG_VALUE_NAME(&ValuePtr->Open, "Open");
    DEBUG_VALUE_NAME(&ValuePtr->P, "P");
    DEBUG_VALUE_NAME(&ValuePtr->DisplayRect, "DisplayRect");
}

inline void
RecordDebugValue_(debug_ui_interaction *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    {DEBUG_DATA_BLOCK("Type");
        DEBUG_VALUE_NAME(&ValuePtr->Type, "Type");
    }
    {DEBUG_DATA_BLOCK("UIElement");
        RecordDebugValue_(ValuePtr->UIElement, DEBUG_NAME("UIElement"), Depth+1);
    }
    DEBUG_VALUE_NAME(&ValuePtr->FrameSelect, "FrameSelect");
}

inline void
RecordDebugValue_(debug_time_block *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    {DEBUG_DATA_BLOCK("ID");
        DEBUG_VALUE_NAME(&ValuePtr->ID, "ID");
    }
    DEBUG_VALUE_NAME(&ValuePtr->FrameIndex, "FrameIndex");
    DEBUG_VALUE_NAME(&ValuePtr->TickStart, "TickStart");
    DEBUG_VALUE_NAME(&ValuePtr->TickEnd, "TickEnd");
    DEBUG_VALUE_NAME(&ValuePtr->TicksUsed, "TicksUsed");
    DEBUG_VALUE_NAME(&ValuePtr->Depth, "Depth");
}

inline void
RecordDebugValue_(debug_time_stat *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    {DEBUG_DATA_BLOCK("ID");
        DEBUG_VALUE_NAME(&ValuePtr->ID, "ID");
    }
    DEBUG_VALUE_NAME(&ValuePtr->MaxTicks, "MaxTicks");
    DEBUG_VALUE_NAME(&ValuePtr->MinTicks, "MinTicks");
    DEBUG_VALUE_NAME(&ValuePtr->TotalTicks, "TotalTicks");
    DEBUG_VALUE_NAME(&ValuePtr->Count, "Count");
}

inline void
RecordDebugValue_(debug_state *ValuePtr, char *GUID, u32 Depth)
{
	if(Depth > MAX_CIRCULAR_REFERENCE_DEPTH) return;
    DEBUG_VALUE_NAME(&ValuePtr->Region, "Region");
    DEBUG_VALUE_NAME(&ValuePtr->TempRegion, "TempRegion");
    {DEBUG_DATA_BLOCK("GameState");
        RecordDebugValue_(ValuePtr->GameState, DEBUG_NAME("GameState"), Depth+1);
    }
    {DEBUG_DATA_BLOCK("RenderGroup");
        RecordDebugValue_(ValuePtr->RenderGroup, DEBUG_NAME("RenderGroup"), Depth+1);
    }
    {DEBUG_DATA_BLOCK("ScreenCamera");
        DEBUG_VALUE_NAME(&ValuePtr->ScreenCamera, "ScreenCamera");
    }
    {DEBUG_DATA_BLOCK("WorldCamera");
        DEBUG_VALUE_NAME(&ValuePtr->WorldCamera, "WorldCamera");
    }
    DEBUG_VALUE_NAME(&ValuePtr->PauseCollation, "PauseCollation");
    DEBUG_VALUE_NAME(&ValuePtr->ScreenResolution, "ScreenResolution");
    DEBUG_VALUE_NAME(&ValuePtr->CountsPerSecond, "CountsPerSecond");
    DEBUG_VALUE_NAME(&ValuePtr->CurrentFrame, "CurrentFrame");
    {DEBUG_DATA_BLOCK("FrameBlockTimings");
        RecordDebugValue_(ValuePtr->FrameBlockTimings, DEBUG_NAME("FrameBlockTimings"), Depth+1);
    }
    {DEBUG_DATA_BLOCK("TimeBlockFreeList");
        RecordDebugValue_(ValuePtr->TimeBlockFreeList, DEBUG_NAME("TimeBlockFreeList"), Depth+1);
    }
    {DEBUG_DATA_BLOCK("LastOpenBlock");
        RecordDebugValue_(ValuePtr->LastOpenBlock, DEBUG_NAME("LastOpenBlock"), Depth+1);
    }
    {DEBUG_DATA_BLOCK("StatHashTable");
        RecordDebugValue_(ValuePtr->StatHashTable, DEBUG_NAME("StatHashTable"), Depth+1);
    }
    {DEBUG_DATA_BLOCK("TimeStatFreeList");
        RecordDebugValue_(ValuePtr->TimeStatFreeList, DEBUG_NAME("TimeStatFreeList"), Depth+1);
    }
    {DEBUG_DATA_BLOCK("RootUIElement");
        DEBUG_VALUE_NAME(&ValuePtr->RootUIElement, "RootUIElement");
    }
    {DEBUG_DATA_BLOCK("LastOpenUIElement");
        RecordDebugValue_(ValuePtr->LastOpenUIElement, DEBUG_NAME("LastOpenUIElement"), Depth+1);
    }
    {DEBUG_DATA_BLOCK("FirstFreeUIElement");
        RecordDebugValue_(ValuePtr->FirstFreeUIElement, DEBUG_NAME("FirstFreeUIElement"), Depth+1);
    }
    {DEBUG_DATA_BLOCK("FirstFreeStoredEvent");
        RecordDebugValue_(ValuePtr->FirstFreeStoredEvent, DEBUG_NAME("FirstFreeStoredEvent"), Depth+1);
    }
    {DEBUG_DATA_BLOCK("HotInteraction");
        DEBUG_VALUE_NAME(&ValuePtr->HotInteraction, "HotInteraction");
    }
    {DEBUG_DATA_BLOCK("NextInteraction");
        DEBUG_VALUE_NAME(&ValuePtr->NextInteraction, "NextInteraction");
    }
    DEBUG_VALUE_NAME(&ValuePtr->ViewingFrame, "ViewingFrame");
    DEBUG_VALUE_NAME(&ValuePtr->MaxTooltipSize, "MaxTooltipSize");
    {DEBUG_DATA_BLOCK("Tooltip");
        DEBUG_VALUE_NAME(&ValuePtr->Tooltip, "Tooltip");
    }
    DEBUG_VALUE_NAME(&ValuePtr->TrackedMemoryRegionCount, "TrackedMemoryRegionCount");
    DEBUG_VALUE_NAME(ValuePtr->TrackedMemoryRegions, "TrackedMemoryRegions");
}