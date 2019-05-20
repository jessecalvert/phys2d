/*@H
* File: phys2d_debug.h
* Author: Jesse Calvert
* Created: ember 6, 2017, 16:31
* Last modified: November 27, 2017, 12:41
*/

#pragma once

struct debug_parsed_guid
{
	u32 HashValue;
	u32 LineNumber;

	char Filename[64];
	char Name[64];
};

struct debug_id
{
	void *Value[2];
	debug_parsed_guid Parsed;
	b32 IsValid;
};
inline b32
operator==(debug_id A, debug_id B)
{
	b32 Result = (A.Value[0] == B.Value[0]) && (A.Value[1] == B.Value[1]);
	return Result;
}
inline b32
operator!=(debug_id A, debug_id B)
{
	b32 Result = !(A == B);
	return Result;
}

struct debug_stored_event
{
	debug_id ID;
	debug_event_type Type;
	u64 Tick;

	union
	{
		u64 Value_u64;
		void *Value_ptr;
		debug_stored_event *NextFree;
	};
};

struct debug_ui_element
{
	union
	{
		debug_ui_element *Next;
		debug_ui_element *NextFree;
	};
	debug_ui_element *Prev;
	debug_ui_element *Parent;
	debug_ui_element *FirstChild;

	u32 FrameIndex;
	u32 Depth;

	debug_stored_event *StoredEvent;
	b32 Open;

	v2 P;
	rectangle2 DisplayRect;
};

enum interaction_type
{
	Interaction_None,
	Interaction_Collapse,
	Interaction_Resize,
	Interaction_ChangeValue,
	Interaction_DragValue,
	Interaction_FrameSelect,
	Interaction_Count,
};

struct debug_ui_interaction
{
	interaction_type Type;
	debug_ui_element *UIElement;

	u32 FrameSelect;
};

inline b32
operator==(debug_ui_interaction A, debug_ui_interaction B)
{
	b32 Result = ((A.Type == B.Type) &&
		(A.UIElement == B.UIElement) &&
		(A.FrameSelect == B.FrameSelect));
	return Result;
}

struct debug_time_block
{
	debug_time_block *Next;
	debug_time_block *Prev;
	debug_time_block *Parent;
	debug_time_block *FirstChild;

	debug_id ID;
	u32 FrameIndex;
	u64 TickStart;
	u64 TickEnd;
	u64 TicksUsed;

	u32 Depth;
};

struct debug_time_stat
{
	debug_id ID;

	u64 MaxTicks;
	u64 MinTicks;
	u64 TotalTicks;
	u32 Count;

	debug_time_stat *NextInHash;
};

#define MAX_COLLATED_FRAMES 128
#define MAX_BLOCK_DEPTH 32
struct debug_state
{
	memory_region Region;
	memory_region TempRegion;

	game_state *GameState;

	render_group *RenderGroup;
	camera ScreenCamera;
	camera WorldCamera;

	b32 PauseCollation;
	v2i ScreenResolution;
	u64 CountsPerSecond;

	u32 CurrentFrame;
	debug_time_block *FrameBlockTimings[MAX_COLLATED_FRAMES];
	debug_time_block *TimeBlockFreeList;
	debug_time_block *LastOpenBlock;

	debug_time_stat *StatHashTable[MAX_COLLATED_FRAMES][256];
	debug_time_stat *TimeStatFreeList;

	debug_ui_element RootUIElement;
	debug_ui_element *LastOpenUIElement;
	debug_ui_element *FirstFreeUIElement;
	debug_stored_event *FirstFreeStoredEvent;

	debug_ui_interaction HotInteraction;
	debug_ui_interaction NextInteraction;

	u32 ViewingFrame;
	u32 MaxTooltipSize;
	string Tooltip;

	u32 TrackedMemoryRegionCount;
	memory_region *TrackedMemoryRegions[32];
};

