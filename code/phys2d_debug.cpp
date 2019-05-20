/*@H
* File: phys2d_debug.cpp
* Author: Jesse Calvert
* Created: ember 6, 2017, 17:31
* Last modified: November 27, 2017, 15:28
*/


#include "phys2d_debug.h"

internal debug_id
CreateIDFromGUID(char *GUID)
{
	debug_id Result = {};
	Result.Value[0] = GUID;
	Result.Value[1] = 0;

	Result.Parsed.HashValue = 281;

	string Name = {};
	string Filename = {};
	Filename.Text = GUID;

	u32 PipeCount = 0;
	char *Scan = GUID;
	while(*Scan)
	{
		if(*Scan == '|')
		{
			switch(PipeCount)
			{
				case 0:
				{
					Filename.Length = (u32)(Scan - GUID);
					Result.Parsed.LineNumber = StringToS32(Scan + 1);
				} break;

				case 1:
				{
					Result.Parsed.HashValue += 2371*StringToS32(Scan + 1);
				} break;

				case 2:
				{
					Name.Text = (Scan + 1);
				} break;

				InvalidDefaultCase;
			}

			++PipeCount;
		}

		Result.Parsed.HashValue += 7661*(*Scan) + 23;
		++Scan;
	}
	Assert(Name.Text);
	Name.Length = (u32)(Scan - Name.Text);

	if((Name.Text[0] == 'P') && (Name.Text[1] == 'l'))
	{
		int y = 3;
	}

	CopySize(Name.Text, Result.Parsed.Name, Name.Length);
	CopySize(Filename.Text, Result.Parsed.Filename, Filename.Length);

	Result.IsValid = true;
	return Result;
}

internal debug_time_block *
CreateNewTimeBlock(debug_state *State, debug_event *BlockBegin)
{
	debug_time_block *Block = State->TimeBlockFreeList;
	if(Block)
	{
		State->TimeBlockFreeList = Block->Next;
		ZeroStruct(*Block);
	}
	else
	{
		Block = PushStruct(&State->Region, debug_time_block);
	}

	Block->ID = CreateIDFromGUID(BlockBegin->GUID);
	Block->TickStart = BlockBegin->Tick;
	DLIST_INIT(Block);

	return Block;
}

internal debug_time_block *
CreateNewFrameTimeBlock(debug_state *State)
{
	debug_event FrameDummy = {};
	FrameDummy.GUID = "a|0|0|FrameDummyGUID";
	debug_time_block *Result = CreateNewTimeBlock(State, &FrameDummy);
	Result->Depth = 0;
	return Result;
}

internal void
FreeTimeBlockRecursive_(debug_state *State, debug_time_block *Block)
{
	if(Block)
	{
		if(Block->FirstChild)
		{
			Block->FirstChild->Prev->Next = 0;
			FreeTimeBlockRecursive_(State, Block->FirstChild);
		}

		debug_time_block *NextToFree = Block->Next;
		Block->Next = State->TimeBlockFreeList;
		State->TimeBlockFreeList = Block;
		FreeTimeBlockRecursive_(State, NextToFree);
	}
}

internal void
FreeTimeBlockEntireTree(debug_state *State, debug_time_block *Block)
{
	if(Block)
	{
		Assert(Block->Parent == 0);
		Block->Next = 0;
		FreeTimeBlockRecursive_(State, Block);
	}
}

internal void
ClearStatHashTable(debug_state *State, u32 FrameIndex)
{
	for(u32 HashIndex = 0;
		HashIndex < ArrayCount(State->StatHashTable[FrameIndex]);
		++HashIndex)
	{
		debug_time_stat *Stat = State->StatHashTable[FrameIndex][HashIndex];
		while(Stat)
		{
			debug_time_stat *NextStat = Stat->NextInHash;
			Stat->NextInHash = State->TimeStatFreeList;
			State->TimeStatFreeList = Stat;
			Stat = NextStat;
		}
		State->StatHashTable[FrameIndex][HashIndex] = 0;
	}
}

internal debug_time_stat *
GetTimeStat(debug_state *State, debug_id ID, u32 FrameIndex)
{
	Assert(ID.IsValid);
	debug_parsed_guid Parsed = ID.Parsed;
	u32 HashIndex = Parsed.HashValue % ArrayCount(State->StatHashTable[0]);
	debug_time_stat *Stat = State->StatHashTable[FrameIndex][HashIndex];
	while(Stat)
	{
		if(Stat->ID == ID)
		{
			break;
		}
		Stat = Stat->NextInHash;
	}
	if(!Stat)
	{
		Stat = State->TimeStatFreeList;
		if(Stat)
		{
			State->TimeStatFreeList = Stat->NextInHash;
			ZeroStruct(*Stat);
		}
		else
		{
			Stat = PushStruct(&State->Region, debug_time_stat);
		}

		Stat->NextInHash = State->StatHashTable[FrameIndex][HashIndex];
		State->StatHashTable[State->CurrentFrame][HashIndex] = Stat;
		Stat->ID = ID;
		Stat->MinTicks = 0xFFFFFFFFFFFFFFFF;
	}

	return Stat;
}

internal void
BlockCalculateTicksUsed(debug_time_block *Block)
{
	Block->TicksUsed = Block->TickEnd - Block->TickStart;
	debug_time_block *Child = Block->FirstChild;
	if(Child)
	{
		do
		{
			Block->TicksUsed -= (Child->TickEnd - Child->TickStart);
			Child = Child->Next;
		} while(Child != Block->FirstChild);
	}
}

internal debug_stored_event *
StoreEvent(debug_state *State, debug_event *Event, debug_id ID)
{
	debug_stored_event *Result = State->FirstFreeStoredEvent;
	if(Result)
	{
		State->FirstFreeStoredEvent = Result->NextFree;
		ZeroStruct(*Result);
	}
	else
	{
		Result = PushStruct(&State->Region, debug_stored_event);
	}

	Result->ID = ID;
	Result->Type = Event->Type;
	Result->Tick = Event->Tick;
	Result->Value_u64 = Event->Value_u64;

	return Result;
}

internal debug_ui_element *
GetOrCreateUIElement(debug_state *State, debug_ui_element *LastOpenUIElement, u32 FrameIndex, debug_event *Event, debug_id ID)
{
	debug_ui_element *Result = LastOpenUIElement->FirstChild;
	while(Result)
	{
		if(Result->StoredEvent->ID == ID)
		{
			break;
		}

		Result = Result->Next;
		if(Result == LastOpenUIElement->FirstChild)
		{
			Result = 0;
			break;
		}
	}
	if(!Result)
	{
		Result = State->FirstFreeUIElement;
		if(Result)
		{
			State->FirstFreeUIElement = Result->NextFree;
			ZeroStruct(*Result);
		}
		else
		{
			Result = PushStruct(&State->Region, debug_ui_element);
		}

		Result->Depth = LastOpenUIElement->Depth + 1;
		Result->Open = false;
		Result->Parent = LastOpenUIElement;

		debug_stored_event *StoredEvent = StoreEvent(State, Event, ID);
		Result->StoredEvent = StoredEvent;

		if(LastOpenUIElement->FirstChild)
		{
			DLIST_INSERT(LastOpenUIElement->FirstChild->Prev, Result);
		}
		else
		{
			DLIST_INIT(Result);
			LastOpenUIElement->FirstChild = Result;
		}
	}

	Result->FrameIndex = FrameIndex;

	return Result;
}

internal void
CollateDebugRecords(debug_state *State, debug_event *EventArray, u32 EventCount)
{
	for(u32 EventIndex = 0;
		EventIndex < EventCount;
		++EventIndex)
	{
		debug_event *Event = EventArray + EventIndex;
		switch(Event->Type)
		{
			case Event_FrameMark:
			{
				Assert(!State->LastOpenBlock->Parent);

				debug_time_block *EndingFrameBlock = State->LastOpenBlock;
				EndingFrameBlock->ID = CreateIDFromGUID(Event->GUID);
				EndingFrameBlock->TickEnd = Event->Tick;
				EndingFrameBlock->TickStart = EndingFrameBlock->TickEnd - Event->Value_u64;
				BlockCalculateTicksUsed(EndingFrameBlock);
				State->LastOpenBlock = 0;

				State->CurrentFrame = (State->CurrentFrame + 1) % ArrayCount(State->FrameBlockTimings);
				ClearStatHashTable(State, State->CurrentFrame);
			} break;

			case Event_BeginBlock:
			{
				if(!State->LastOpenBlock)
				{
					State->LastOpenBlock = CreateNewFrameTimeBlock(State);
					State->LastOpenBlock->FrameIndex = State->CurrentFrame;

					debug_time_block *OldFrame = State->FrameBlockTimings[State->CurrentFrame];
					FreeTimeBlockEntireTree(State, OldFrame);
					State->FrameBlockTimings[State->CurrentFrame] = State->LastOpenBlock;
				}

				debug_time_block *TimeBlock = CreateNewTimeBlock(State, Event);
				TimeBlock->FrameIndex = State->CurrentFrame;

				if(State->LastOpenBlock->FirstChild)
				{
					DLIST_INSERT(State->LastOpenBlock->FirstChild->Prev, TimeBlock);
				}
				else
				{
					State->LastOpenBlock->FirstChild = TimeBlock;
				}
				TimeBlock->Parent = State->LastOpenBlock;
				TimeBlock->Depth = State->LastOpenBlock->Depth + 1;

				State->LastOpenBlock = TimeBlock;
			} break;

			case Event_EndBlock:
			{
				Assert(State->LastOpenBlock->Parent);
				debug_time_block *OpenTimeBlock = State->LastOpenBlock;
				Assert(OpenTimeBlock->TickEnd == 0);
				OpenTimeBlock->TickEnd = Event->Tick;
				BlockCalculateTicksUsed(OpenTimeBlock);

				debug_time_stat *Stat = GetTimeStat(State, OpenTimeBlock->ID, State->CurrentFrame);
				Stat->MaxTicks = Maximum(OpenTimeBlock->TicksUsed, Stat->MaxTicks);
				Stat->MinTicks = Minimum(OpenTimeBlock->TicksUsed, Stat->MinTicks);
				Stat->TotalTicks += OpenTimeBlock->TicksUsed;
				++Stat->Count;

				State->LastOpenBlock = State->LastOpenBlock->Parent;
			} break;

			case Event_BeginDataBlock:
			{
				debug_ui_element *UIElement = GetOrCreateUIElement(State, State->LastOpenUIElement, State->CurrentFrame, Event, CreateIDFromGUID(Event->GUID));
				State->LastOpenUIElement = UIElement;
			} break;

			case Event_EndDataBlock:
			{
				Assert(State->LastOpenUIElement->Parent);
				State->LastOpenUIElement = State->LastOpenUIElement->Parent;
			} break;

			case Event_memory_region:
			{
				State->TrackedMemoryRegions[State->TrackedMemoryRegionCount++] = (memory_region *)Event->Value_ptr;
			} break;

			default:
			{
				debug_ui_element *UIElement = GetOrCreateUIElement(State, State->LastOpenUIElement, State->CurrentFrame, Event, CreateIDFromGUID(Event->GUID));
			}
		}
	}
}

internal void
DebugDrawTimeBlockRecursive(debug_state *State, v2 AtMin, f32 BarWidth, f32 MaxHeight, u64 FrameStart, u64 LongestFrameTime, debug_time_block *Block, b32 Horizontal, f32 OutlineWidth, v2 MouseP, u32 MaxDepth)
{
	v3 Colors[] =
	{
		{1.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 1.0f},
		{1.0f, 0.0f, 1.0f},
		{0.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 0.0f},
		{1.0f, 1.0f, 1.0f},
	};

	if(Block && (Block->Depth < MaxDepth))
	{
		u64 CycleCount = (Block->TickEnd - Block->TickStart);
		u64 CycleStart = (Block->TickStart - FrameStart);
		f32 RectHeight = ((f32)CycleCount/(f32)LongestFrameTime)*MaxHeight;
		f32 RectStart = ((f32)CycleStart/(f32)LongestFrameTime)*MaxHeight;
		f32 RectWidth = BarWidth/(1.0f + Block->Depth);
		debug_parsed_guid *Parsed = &Block->ID.Parsed;

		if(RectWidth > 0.1f)
		{
			v2 Dim = V2(RectWidth, RectHeight);
			v3 Color = Colors[(Parsed->HashValue + Block->Depth) % ArrayCount(Colors)];

			rectangle2 DrawRect = {};
			if(Horizontal)
			{
				DrawRect.Min = AtMin + V2(RectStart, 0.0f);
				DrawRect.Max = DrawRect.Min + V2(Dim.y, Dim.x);
			}
			else
			{
				DrawRect.Min = AtMin + V2(0.0f, RectStart);
				DrawRect.Max = DrawRect.Min + Dim;
			}

			PushRectangleOutline(State->RenderGroup, DrawRect.Min, DrawRect.Max, V4(Color, 1.0f), OutlineWidth);
			PushQuad(State->RenderGroup, DrawRect.Min, DrawRect.Max, V4(Color, 0.50f));

			if(InsideRectangle(DrawRect, MouseP))
			{
				State->Tooltip = FormatString(State->Tooltip.Text, State->MaxTooltipSize, "%s : %.3f ms",
					String(Parsed->Name),
					(f32)CycleCount/(f32)State->CountsPerSecond);
			}

			if(Block->Parent && (Block->Next != Block->Parent->FirstChild))
			{
				DebugDrawTimeBlockRecursive(State, AtMin, BarWidth, MaxHeight, FrameStart,
				LongestFrameTime, Block->Next, Horizontal, OutlineWidth, MouseP, MaxDepth);
			}
			DebugDrawTimeBlockRecursive(State, AtMin, BarWidth, MaxHeight, FrameStart,
				LongestFrameTime, Block->FirstChild, Horizontal, OutlineWidth, MouseP, MaxDepth);
		}
	}
}

internal void
DebugDrawSingleFrameTimings(debug_state *State, u32 FrameIndex, v2 Min, v2 Max, v2 MouseP)
{
	v2 GraphMin = Min;
	v2 GraphMax = Max;
	v2 GraphDim = GraphMax - GraphMin;
	f32 BarWidth = GraphDim.y;

	v2 AtMin = GraphMin;
	debug_time_block *FrameBlock = State->FrameBlockTimings[FrameIndex];
	if(FrameBlock && FrameBlock->TickEnd)
	{
		u64 LongestFrameTime = (FrameBlock->TickEnd - FrameBlock->TickStart);
		DebugDrawTimeBlockRecursive(State, AtMin, BarWidth, GraphDim.x,
			FrameBlock->TickStart, LongestFrameTime, FrameBlock, true, 3.0f, MouseP, 4);
	}
}

internal void
DebugDrawFrameTimings(debug_state *State, v2 Min, v2 Max, game_input *Input)
{
	v2 MouseP = Input->MouseP;

	v2 GraphMin = Min;
	v2 GraphMax = Max;
	v2 GraphDim = GraphMax - GraphMin;
	f32 Spacing = 4.0f;
	f32 BarWidthAndSpacing = GraphDim.x / ArrayCount(State->FrameBlockTimings);
	f32 BarWidth = BarWidthAndSpacing - Spacing;
	Assert(BarWidth > 0.0f);

	f32 BorderWidth = 2.0f;
	PushRectangleOutline(State->RenderGroup, Min, Max, V4(0.75f, 0.75f, 0.75f, 0.75f), BorderWidth);

	TemporaryClipRectChange(State->RenderGroup, V2ToV2i(Min), V2ToV2i(Max));
	PushQuad(State->RenderGroup, Min, Max, V4(0.1f, 0.1f, 0.1f, 0.5f));

	u64 LongestFrameTime = 0;
	u32 FrameCount = 0;
	for(u32 FrameIndex = 0;
		FrameIndex < ArrayCount(State->FrameBlockTimings);
		++FrameIndex)
	{
		debug_time_block *FrameBlock = State->FrameBlockTimings[FrameIndex];
		if(FrameBlock)
		{
			++FrameCount;
			u64 FrameTime = (FrameBlock->TickEnd - FrameBlock->TickStart);
			if(FrameTime > LongestFrameTime)
			{
				LongestFrameTime = FrameTime;
			}
		}
	}

	v2 AtMin = GraphMin;
	for(u32 FrameIndex = 0;
		FrameIndex < ArrayCount(State->FrameBlockTimings);
		++FrameIndex)
	{
		debug_time_block *FrameBlock = State->FrameBlockTimings[FrameIndex];
		if(FrameBlock && FrameBlock->TickEnd)
		{
			DebugDrawTimeBlockRecursive(State, AtMin, BarWidth, GraphDim.y,
				FrameBlock->TickStart, LongestFrameTime, FrameBlock, false, 1.0f, MouseP, 2);

			rectangle2 DisplayRect = Rectangle2(AtMin, AtMin + V2(BarWidth, GraphDim.y));

			debug_ui_interaction FrameSelectInteraction = {};
			FrameSelectInteraction.Type = Interaction_FrameSelect;
			FrameSelectInteraction.FrameSelect = FrameIndex;
			b32 ShowOutline = false;
			v4 SelectionColor = V4(1.0f, 0.25f, 0.25f, 1.0f);

			if(InsideRectangle(DisplayRect, MouseP))
			{
				State->HotInteraction = FrameSelectInteraction;
				if(State->NextInteraction.Type == Interaction_None)
				{
					ShowOutline = true;
				}
				if(Input->KeyStates[Key_LeftClick].DebugWasPressed)
				{
					State->NextInteraction = FrameSelectInteraction;
				}
			}
			if(State->NextInteraction == FrameSelectInteraction)
			{
				ShowOutline = true;
			}

			if(ShowOutline)
			{
				PushRectangleOutline(State->RenderGroup, DisplayRect.Min, DisplayRect.Max, SelectionColor, 3.0f);
			}
		}

		AtMin.x += BarWidthAndSpacing;
	}
}

internal void
GatherStatTimingSortEntriesRecursive(debug_state *State, u32 FrameIndex,
	u32 *SortEntryCount, sort_entry *StatSortEntries)
{
	for(u32 HashIndex = 0;
		HashIndex < ArrayCount(State->StatHashTable[FrameIndex]);
		++HashIndex)
	{
		debug_time_stat *Stat = State->StatHashTable[FrameIndex][HashIndex];
		while(Stat)
		{
			Assert(*SortEntryCount < ArrayCount(State->StatHashTable[FrameIndex]));
			sort_entry *SortEntry = StatSortEntries + *SortEntryCount;
			*SortEntryCount += 1;
			SortEntry->Index = (u64)Stat;
			SortEntry->Value = (f32)(Stat->TotalTicks);

			Stat = Stat->NextInHash;
		}
	}
}

internal void
DebugDisplayTopClocks(debug_state *State, u32 FrameIndex, v2 Min, v2 Max, v2 MouseP)
{
	u32 SortEntryCount = 0;
	// TODO: Hopefully this is enough space.
	sort_entry StatSortEntries[ArrayCount(State->StatHashTable[FrameIndex])] = {};
	GatherStatTimingSortEntriesRecursive(State, FrameIndex, &SortEntryCount, StatSortEntries);
	Sort(&State->TempRegion, StatSortEntries, SortEntryCount);

	text_layout TextLayout = {};
	TextLayout.FontName = Asset_LiberationMonoFont;
	TextLayout.Scale = 0.35f;
	TextLayout.Color = V4(0.0f, 1.0f, 1.0f, 1.0f);
	f32 LineHeight = GetFontTotalLineHeight(State->RenderGroup, &TextLayout);
	TextLayout.AtY = V2(Min.x, Max.y - LineHeight);

	f32 BorderWidth = 2.0f;
	PushRectangleOutline(State->RenderGroup, Min, Max, V4(0.75f, 0.75f, 0.75f, 0.75f), BorderWidth);

	TemporaryClipRectChange(State->RenderGroup, V2ToV2i(Min), V2ToV2i(Max));
	PushQuad(State->RenderGroup, Min, Max, V4(0.1f, 0.1f, 0.1f, 0.5f));

	char Buffer[512];
	u32 NameColumnWidth = 30;
	u32 TotalMSColumnWidth = 8;
	u32 CountColumnWidth = 6;
	u32 AverageMSColumnWidth = 14;
	string ColumnNames = FormatString(Buffer, ArrayCount(Buffer), "%-*s:%*s:%*s:%*s",
		NameColumnWidth, String("Name"),
		TotalMSColumnWidth, String("Total"),
		CountColumnWidth, String("Count"),
		AverageMSColumnWidth, String("Average"));
	v4 OldColor = TextLayout.Color;
	TextLayout.Color = V4(1.0f, 1.0f, 0.0f, 1.0f);
	PushText(State->RenderGroup, &TextLayout, ColumnNames);
	TextLayout.Color = OldColor;

	for(s32 Index = SortEntryCount - 1;
		Index >= 0;
		--Index)
	{
		sort_entry *SortEntry = StatSortEntries + Index;
		debug_time_stat *Stat = PointerFromU32(debug_time_stat, SortEntry->Index);
		debug_parsed_guid *Parsed = &Stat->ID.Parsed;

		f32 TotalMS = (f32)Stat->TotalTicks/(f32)State->CountsPerSecond;
		f32 AverageMS = TotalMS/Stat->Count;
		string TopClockEntry = FormatString(Buffer, ArrayCount(Buffer), "%-*s:%*.3f:%*d:%*.5f",
			NameColumnWidth, String(Parsed->Name),
			TotalMSColumnWidth, TotalMS,
			CountColumnWidth, Stat->Count,
			AverageMSColumnWidth, AverageMS);

		rectangle2 TextRect = ClipTo(GetTextBounds(State->RenderGroup, &TextLayout, TopClockEntry), Rectangle2(Min, Max));
		PushText(State->RenderGroup, &TextLayout, TopClockEntry);

		if(InsideRectangle(TextRect, MouseP))
		{
			f32 MinMS = (f32)Stat->MinTicks/(f32)State->CountsPerSecond;
			f32 MaxMS = (f32)Stat->MaxTicks/(f32)State->CountsPerSecond;
			State->Tooltip = FormatString(State->Tooltip.Text, State->MaxTooltipSize, "%s L:%d, %f - %f",
				String(Parsed->Filename),
				Parsed->LineNumber,
				MinMS,
				MaxMS);
		}

		if(TextLayout.AtY.y < (Min.y - LineHeight))
		{
			break;
		}
	}
}

internal void
DebugDisplayMemoryRegions(debug_state *State, render_group *RenderGroup, v2 Min, v2 Max, v2 MouseP)
{
	text_layout TextLayout = {};
	TextLayout.FontName = Asset_LiberationMonoFont;
	TextLayout.Scale = 0.35f;
	TextLayout.Color = V4(0.0f, 1.0f, 1.0f, 1.0f);
	f32 LineHeight = GetFontTotalLineHeight(State->RenderGroup, &TextLayout);
	TextLayout.AtY = V2(Min.x, Max.y - LineHeight);

	f32 BorderWidth = 2.0f;
	PushRectangleOutline(State->RenderGroup, Min, Max, V4(0.75f, 0.75f, 0.75f, 0.75f), BorderWidth);

	TemporaryClipRectChange(State->RenderGroup, V2ToV2i(Min), V2ToV2i(Max));
	PushQuad(State->RenderGroup, Min, Max, V4(0.1f, 0.1f, 0.1f, 0.5f));

	char Buffer[128];
	for(u32 Index = 0;
		Index < State->TrackedMemoryRegionCount;
		++Index)
	{
		memory_region *Region = State->TrackedMemoryRegions[Index];
		Assert(Region->DebugName);

		region_stats RegionStats = GetRegionStats(Region);
		string RegionEntry = FormatString(Buffer, ArrayCount(Buffer), "Region %s : Total %d : Used %d : Blocks %d",
			String(Region->DebugName),
			SafeTruncateUInt64(RegionStats.TotalSize),
			SafeTruncateUInt64(RegionStats.TotalUsed),
			RegionStats.BlockCount);

		rectangle2 TextRect = ClipTo(GetTextBounds(State->RenderGroup, &TextLayout, RegionEntry), Rectangle2(Min, Max));
		PushText(State->RenderGroup, &TextLayout, RegionEntry);

		if(InsideRectangle(TextRect, MouseP))
		{
			State->Tooltip = FormatString(State->Tooltip.Text, State->MaxTooltipSize, "This is a tooltip!"
				);
		}

		if(TextLayout.AtY.y < (Min.y - LineHeight))
		{
			break;
		}
	}
}

internal debug_ui_interaction
GetUIElementInteraction(debug_ui_element *UIElement)
{
	debug_ui_interaction Result = {};
	Result.UIElement = UIElement;
	switch(UIElement->StoredEvent->Type)
	{
		case Event_b32:
		{
			Result.Type = Interaction_ChangeValue;
		} break;

		case Event_f32:
		{
			Result.Type = Interaction_DragValue;
		} break;

		default:
		{
			Result.Type = Interaction_Collapse;
		}
	}

	return Result;
}

internal void
DisplayResizeableUIElement(debug_state *State, debug_ui_element *UIElement, text_layout *Layout, f32 IndentWidth, game_input *Input, debug_event_type Type)
{
	v2 MouseP = Input->MouseP;

	f32 FontHeight = GetFontTotalLineHeight(State->RenderGroup, Layout);
	v2 TopLeft = Layout->AtY + V2(IndentWidth, FontHeight);

	v2 Dimension = Dim(UIElement->DisplayRect);
	if(Dimension == V2(0,0))
	{
		switch(Type)
		{
			case Event_TopClocksList: {UIElement->DisplayRect = Rectangle2(TopLeft - V2(0.0f, 400.0f), TopLeft + V2(1600.0f, 0.0f));} break;
			case Event_FrameSlider: {UIElement->DisplayRect = Rectangle2(TopLeft - V2(0.0f, 100.0f), TopLeft + V2(1600.0f, 0.0f));} break;
			case Event_FrameBarGraph: {UIElement->DisplayRect = Rectangle2(TopLeft - V2(0.0f, 80.0f), TopLeft + V2(1600.0f, 0.0f));} break;
			case Event_AABBTree: {UIElement->DisplayRect = Rectangle2(TopLeft - V2(0.0f, 400.0f), TopLeft + V2(1600.0f, 0.0f));} break;
			case Event_MemoryDisplay: {UIElement->DisplayRect = Rectangle2(TopLeft - V2(0.0f, 400.0f), TopLeft + V2(1600.0f, 0.0f));} break;
			InvalidDefaultCase;
		}
	}
	else
	{
		UIElement->DisplayRect.Min = TopLeft - V2(0.0f, Dimension.y);
		UIElement->DisplayRect.Max = TopLeft + V2(Dimension.x, 0.0f);
	}

	v2 ResizeButtonP = UIElement->DisplayRect.Min + V2(Dimension.x, 0.0f);
	v2 RectMin = UIElement->DisplayRect.Min;
	v2 RectMax = UIElement->DisplayRect.Max;

	switch(Type)
	{
		case Event_TopClocksList:
		{
			DebugDisplayTopClocks(State, State->ViewingFrame, RectMin, RectMax, MouseP);
		} break;

		case Event_FrameSlider:
		{
			DebugDrawFrameTimings(State, RectMin, RectMax, Input);
		} break;

		case Event_FrameBarGraph:
		{
			DebugDrawSingleFrameTimings(State, State->ViewingFrame, RectMin, RectMax, MouseP);
		} break;

		case Event_AABBTree:
		{
			DEBUGDisplayAABBTreeDiagram(State->GameState->Physics, State->RenderGroup, RectMin, RectMax);
		} break;

		case Event_MemoryDisplay:
		{
			DebugDisplayMemoryRegions(State, State->RenderGroup, RectMin, RectMax, MouseP);
		} break;

		InvalidDefaultCase;
	}
	Layout->AtY.y -= ((RectMax.y - RectMin.y));

	v2 ResizeButtonDim = V2(10.0f, 10.0f);
	rectangle2 ResizeRect = Rectangle2CenterDim(ResizeButtonP, ResizeButtonDim);
	v4 ResizeColor = V4(1,1,1,1);

	debug_ui_interaction ResizeInteraction = {};
	ResizeInteraction.Type = Interaction_Resize;
	ResizeInteraction.UIElement = UIElement;
	if(InsideRectangle(ResizeRect, MouseP))
	{
		State->HotInteraction = ResizeInteraction;
		if(State->NextInteraction.Type == Interaction_None)
		{
			ResizeColor = V4(1.0f, 0.25f, 0.25f, 1.0f);
		}
		if(Input->KeyStates[Key_LeftClick].DebugWasPressed)
		{
			State->NextInteraction = ResizeInteraction;
		}
	}
	if(State->NextInteraction == ResizeInteraction)
	{
		ResizeColor = V4(1.0f, 0.25f, 0.25f, 1.0f);
	}

	PushQuad(State->RenderGroup, ResizeRect.Min, ResizeRect.Max, ResizeColor);
}

internal void
FreeStoredEvent(debug_state *State, debug_stored_event *StoredEvent)
{
	if(StoredEvent)
	{
		StoredEvent->NextFree = State->FirstFreeStoredEvent;
		State->FirstFreeStoredEvent = StoredEvent;
	}
}

internal void
FreeUIElementRecursive(debug_state *State, debug_ui_element *UIElement)
{
	if(UIElement)
	{
		if(UIElement->FirstChild)
		{
			FreeUIElementRecursive(State, UIElement->FirstChild);
		}

		debug_ui_element *NextUIElement = (UIElement == UIElement->Next) ? 0 : UIElement->Next;

		DLIST_REMOVE(UIElement);
		UIElement->NextFree = State->FirstFreeUIElement;
		State->FirstFreeUIElement = UIElement;

		FreeStoredEvent(State, UIElement->StoredEvent);
		FreeUIElementRecursive(State, NextUIElement);
	}
}

internal void
FreeUIElement(debug_state *State, debug_ui_element *UIElement)
{
	if(UIElement)
	{
		if(UIElement->FirstChild)
		{
			FreeUIElementRecursive(State, UIElement->FirstChild);
			UIElement->FirstChild = 0;
		}

		if(UIElement->Parent)
		{
			if(UIElement->Parent->FirstChild == UIElement)
			{
				UIElement->Parent->FirstChild = UIElement->Next;
			}
		}

		DLIST_REMOVE(UIElement);
		UIElement->NextFree = State->FirstFreeUIElement;
		State->FirstFreeUIElement = UIElement;

		FreeStoredEvent(State, UIElement->StoredEvent);
	}
}

internal void
DebugDisplayUIElement(debug_state *State, debug_ui_element *UIElement,
	text_layout *Layout, u32 Indent, game_input *Input, u32 FrameIndex, b32 IsRoot = false)
{
	v2 MouseP = Input->MouseP;

	if(UIElement)
	{
		s32 FrameLifetime = 16;
		s32 FrameDelta = FrameIndex - UIElement->FrameIndex;
		if(UIElement->FrameIndex > FrameIndex)
		{
			FrameDelta += ArrayCount(State->FrameBlockTimings);
		}

		if(IsRoot || (FrameDelta < FrameLifetime))
		{
			f32 IndentWidth = 20.0f;
			debug_parsed_guid *Parsed = &UIElement->StoredEvent->ID.Parsed;
			f32 OldAtX = Layout->AtY.x;
			Layout->AtY.x += Indent*IndentWidth;
			string MenuItemText = {};
			char Buffer[256];
			switch(UIElement->StoredEvent->Type)
			{
				case Event_b32:
				{
					MenuItemText = FormatString(Buffer, ArrayCount(Buffer), "%s: %s", String(Parsed->Name), *(b32 *)UIElement->StoredEvent->Value_ptr ? String("true") : String("false"));
				} break;

				case Event_f32:
				{
					MenuItemText = FormatString(Buffer, ArrayCount(Buffer), "%s: %f", String(Parsed->Name), *(f32 *)UIElement->StoredEvent->Value_ptr);
				} break;

				case Event_char:
				{
					MenuItemText = FormatString(Buffer, ArrayCount(Buffer), "%s: %s", String(Parsed->Name), String((char *)UIElement->StoredEvent->Value_ptr));
				} break;

				case Event_u8:
				case Event_u16:
				case Event_u32:
				{
					MenuItemText = FormatString(Buffer, ArrayCount(Buffer), "%s: %d", String(Parsed->Name), *(u32 *)UIElement->StoredEvent->Value_ptr);
				} break;

				case Event_u64:
				{
					MenuItemText = FormatString(Buffer, ArrayCount(Buffer), "%s: %d", String(Parsed->Name), SafeTruncateUInt64(*(u64 *)UIElement->StoredEvent->Value_ptr));
				} break;

				case Event_v2:
				{
					v2 Vector2 = *(v2 *)UIElement->StoredEvent->Value_ptr;
					MenuItemText = FormatString(Buffer, ArrayCount(Buffer), "%s: (%f, %f)", String(Parsed->Name), Vector2.x, Vector2.y);
				} break;

				case Event_v2i:
				{
					v2i Vector2 = *(v2i *)UIElement->StoredEvent->Value_ptr;
					MenuItemText = FormatString(Buffer, ArrayCount(Buffer), "%s: (%d, %d)", String(Parsed->Name), Vector2.x, Vector2.y);
				} break;

				case Event_v3:
				{
					v3 Vector3 = *(v3 *)UIElement->StoredEvent->Value_ptr;
					MenuItemText = FormatString(Buffer, ArrayCount(Buffer), "%s: (%f, %f, %f)", String(Parsed->Name), Vector3.x, Vector3.y, Vector3.z);
				} break;

				case Event_v3i:
				{
					v3i Vector3 = *(v3i *)UIElement->StoredEvent->Value_ptr;
					MenuItemText = FormatString(Buffer, ArrayCount(Buffer), "%s: (%d, %d, %d)", String(Parsed->Name), Vector3.x, Vector3.y, Vector3.z);
				} break;

				case Event_v3u:
				{
					v3u Vector3 = *(v3u *)UIElement->StoredEvent->Value_ptr;
					MenuItemText = FormatString(Buffer, ArrayCount(Buffer), "%s: (%u, %u, %u)", String(Parsed->Name), Vector3.x, Vector3.y, Vector3.z);
				} break;

				case Event_v4:
				{
					v4 Vector4 = *(v4 *)UIElement->StoredEvent->Value_ptr;
					MenuItemText = FormatString(Buffer, ArrayCount(Buffer), "%s: (%f, %f, %f, %f)", String(Parsed->Name), Vector4.x, Vector4.y, Vector4.z, Vector4.w);
				} break;

				case Event_mat2:
				{
					mat2 Matrix2 = *(mat2 *)UIElement->StoredEvent->Value_ptr;
					MenuItemText = FormatString(Buffer, ArrayCount(Buffer), "%s: [%f %f], [%f %f]", String(Parsed->Name), Matrix2.M[0], Matrix2.M[1], Matrix2.M[2], Matrix2.M[3]);
				} break;

				case Event_mat3:
				{
					mat3 Matrix3 = *(mat3 *)UIElement->StoredEvent->Value_ptr;
					MenuItemText = FormatString(Buffer, ArrayCount(Buffer), "%s: [%f %f %f], [%f %f %f], [%f %f %f]", String(Parsed->Name), Matrix3.M[0], Matrix3.M[1], Matrix3.M[2], Matrix3.M[3], Matrix3.M[4], Matrix3.M[5], Matrix3.M[6], Matrix3.M[7], Matrix3.M[8]);
				} break;

				case Event_mat4:
				{
					mat4 Matrix4 = *(mat4 *)UIElement->StoredEvent->Value_ptr;
					MenuItemText = FormatString(Buffer, ArrayCount(Buffer), "%s: [%f %f %f %f], [%f %f %f %f], [%f %f %f %f], [%f %f %f %f]", String(Parsed->Name), Matrix4.M[0], Matrix4.M[1], Matrix4.M[2], Matrix4.M[3], Matrix4.M[4], Matrix4.M[5], Matrix4.M[6], Matrix4.M[7], Matrix4.M[8], Matrix4.M[9], Matrix4.M[10], Matrix4.M[11], Matrix4.M[12], Matrix4.M[13], Matrix4.M[14], Matrix4.M[15]);
				} break;

				case Event_rectangle2:
				{
					rectangle2 Rect = *(rectangle2 *)UIElement->StoredEvent->Value_ptr;
					MenuItemText = FormatString(Buffer, ArrayCount(Buffer), "%s: Min(%f, %f), Max:(%f, %f)", String(Parsed->Name), Rect.Min.x, Rect.Min.y, Rect.Max.x, Rect.Max.y);
				} break;

				case Event_rectangle3:
				{
					rectangle3 Rect = *(rectangle3 *)UIElement->StoredEvent->Value_ptr;
					MenuItemText = FormatString(Buffer, ArrayCount(Buffer), "%s: Min(%f, %f, %f), Max:(%f, %f, %f)", String(Parsed->Name), Rect.Min.x, Rect.Min.y, Rect.Min.z, Rect.Max.x, Rect.Max.y, Rect.Max.z);
				} break;

				case Event_quaternion:
				{
					quaternion Q = *(quaternion *)UIElement->StoredEvent->Value_ptr;
					MenuItemText = FormatString(Buffer, ArrayCount(Buffer), "%s: %fr + %fi + %fj + %fk)", String(Parsed->Name), Q.r, Q.i, Q.j, Q.k);
				} break;

				case Event_void:
				{
					u32 DisplayPtr = U32FromPointer(*(void **)UIElement->StoredEvent->Value_ptr);
					MenuItemText = FormatString(Buffer, ArrayCount(Buffer), "%s: %x", String(Parsed->Name), DisplayPtr);
				} break;

				case Event_TopClocksList:
				case Event_FrameBarGraph:
				case Event_FrameSlider:
				case Event_AABBTree:
				case Event_MemoryDisplay:
				{
					DisplayResizeableUIElement(State, UIElement, Layout, IndentWidth, Input, UIElement->StoredEvent->Type);
				} break;

				default:
				{
					MenuItemText = FormatString(Buffer, ArrayCount(Buffer), "%s", String(Parsed->Name));
				}
			}

			if(MenuItemText.Length)
			{
				rectangle2 TextRect = GetTextBounds(State->RenderGroup, Layout, MenuItemText);
				v4 OldColor = Layout->Color;

				debug_ui_interaction Interaction = GetUIElementInteraction(UIElement);
				if(InsideRectangle(TextRect, MouseP))
				{
					State->HotInteraction = Interaction;
					if(State->NextInteraction.Type == Interaction_None)
					{
						Layout->Color = V4(1.0f, 0.25f, 0.25f, 1.0f);
					}
					if(Input->KeyStates[Key_LeftClick].DebugWasPressed)
					{
						State->NextInteraction = Interaction;
						UIElement->P = MouseP;
					}
				}
				if(State->NextInteraction == Interaction)
				{
					Layout->Color = V4(1.0f, 0.25f, 0.25f, 1.0f);
				}

				PushText(State->RenderGroup, Layout, MenuItemText);
				Layout->Color = OldColor;
			}

			Layout->AtY.x = OldAtX;

			if(UIElement->Open && UIElement->FirstChild)
			{
				DebugDisplayUIElement(State, UIElement->FirstChild, Layout, Indent + 1, Input, FrameIndex);
			}

			if(UIElement->Parent && UIElement->Next != UIElement->Parent->FirstChild)
			{
				DebugDisplayUIElement(State, UIElement->Next, Layout, Indent, Input, FrameIndex);
			}
		}
		else
		{
			if(UIElement->Parent && UIElement->Next != UIElement->Parent->FirstChild)
			{
				DebugDisplayUIElement(State, UIElement->Next, Layout, Indent, Input, FrameIndex);
			}

			FreeUIElement(State, UIElement);
		}


	}
}

internal assets *
DebugGetAssets(game_memory *Memory)
{
	assets *Assets = 0;
	game_state *GameState = Memory->GameState;
	if(GameState)
	{
		Assets = GameState->Assets;
	}
	return Assets;
}

internal void
DebugStart(debug_state *State, game_input *Input)
{
	State->WorldCamera = State->GameState->WorldCamera;

	debug_ui_interaction NullInteraction = {};
	State->HotInteraction = NullInteraction;

	State->Tooltip.Length = 0;
}

internal void
DebugEnd(debug_state *State, game_input *Input)
{
	if(Input->KeyStates[Key_LeftClick].DebugWasReleased)
	{
		if(State->NextInteraction == State->HotInteraction)
		{
			debug_ui_interaction *Interaction = &State->NextInteraction;
			debug_ui_element *UIElement = Interaction->UIElement;

			switch(Interaction->Type)
			{
				case Interaction_Collapse:
				{
					UIElement->Open = !UIElement->Open;
				} break;

				case Interaction_ChangeValue:
				{
					switch(UIElement->StoredEvent->Type)
					{
						case Event_b32:
						{
							*(b32 *)UIElement->StoredEvent->Value_ptr = !*(b32 *)UIElement->StoredEvent->Value_ptr;
						} break;

						InvalidDefaultCase;
					}
				} break;

				case Interaction_FrameSelect:
				{
					State->ViewingFrame = Interaction->FrameSelect;
				} break;
			}
		}

		debug_ui_interaction NullInteraction = {};
		State->NextInteraction = NullInteraction;
	}

	if(Input->KeyStates[Key_LeftClick].DebugPressed)
	{
		debug_ui_element *UIElement = State->NextInteraction.UIElement;

		switch(State->NextInteraction.Type)
		{
			case Interaction_DragValue:
			{
				f32 DragScale = 0.001f;
				*(f32 *)UIElement->StoredEvent->Value_ptr += DragScale*(Input->MouseP.y - UIElement->P.y);
				UIElement->P = Input->MouseP;
			} break;

			case Interaction_Resize:
			{
				rectangle2 *DisplayRect = &UIElement->DisplayRect;
				v2 ResizeButtonP = Input->MouseP;
				if(ResizeButtonP.x < DisplayRect->Min.x) {ResizeButtonP.x = DisplayRect->Min.x;}
				if(ResizeButtonP.y > DisplayRect->Max.y) {ResizeButtonP.y = DisplayRect->Max.y;}
				DisplayRect->Min.y = ResizeButtonP.y;
				DisplayRect->Max.x = ResizeButtonP.x;
			} break;
		}
	}

	if(State->Tooltip.Length)
	{
		v2 TooltipOffset = V2(10.0f, 10.0f);
		v2 RectExpand = V2(5.0f, 5.0f);

		text_layout TooltipLayout = {};
		TooltipLayout.FontName = Asset_LiberationMonoFont;
		TooltipLayout.Scale = 0.35f;
		TooltipLayout.Color = V4(1.0f, 1.0f, 0.0f, 1.0f);
		TooltipLayout.AtY = Input->MouseP + TooltipOffset;

		rectangle2 TooltipRect = GetTextBounds(State->RenderGroup, &TooltipLayout, State->Tooltip);
		PushQuad(State->RenderGroup, TooltipRect.Min - RectExpand, TooltipRect.Max + RectExpand, V4(0.25f, 0.25f, 0.25f, 1.0f));
		PushText(State->RenderGroup, &TooltipLayout, State->Tooltip);
	}
}

internal debug_state *
DebugInit(game_memory *Memory, u64 CountsPerSecond)
{
	debug_state *DebugState = Memory->DebugState = BootstrapPushStruct(debug_state, Region);
	DebugState->ScreenResolution = Memory->WindowResolution;
	DebugState->CountsPerSecond = CountsPerSecond;

	DebugState->GameState = Memory->GameState;
	DebugState->RenderGroup = PushStruct(&DebugState->Region, render_group);
	assets *DebugAssets = DebugGetAssets(Memory);
	InitializeRenderGroup(DebugState->RenderGroup, DebugAssets, &Memory->CommandBuffer, DebugState->ScreenResolution);
	SetUpDefaultScreenCamera(&DebugState->ScreenCamera, DebugState->ScreenResolution);
	DebugState->WorldCamera = DebugState->GameState->WorldCamera;

	debug_stored_event *RootDummy = PushStruct(&DebugState->Region, debug_stored_event);
	RootDummy->ID = CreateIDFromGUID("a|0|0|RootDummy");
	DebugState->RootUIElement.StoredEvent = RootDummy;

	DebugState->RootUIElement.Open = false;
	DLIST_INIT(&DebugState->RootUIElement);
	DebugState->LastOpenUIElement = &DebugState->RootUIElement;

	DebugState->MaxTooltipSize = 512;
	DebugState->Tooltip.Text = PushArray(&DebugState->Region, DebugState->MaxTooltipSize, char);

	return DebugState;
}

extern "C" DEBUG_FRAME_END(DebugFrameEnd)
{
	game_state *GameState = Memory->GameState;
	if(GameState)
	{
		debug_state *DebugState = Memory->DebugState;
		if(!DebugState)
		{
			DebugState = DebugInit(Memory, CountsPerSecond);
		}

		if(Input->KeyStates[Key_F9].DebugWasPressed)
		{
			DebugState->PauseCollation = !DebugState->PauseCollation;
		}

		DebugStart(DebugState, Input);

		debug_table *DebugTable = Memory->DebugTable;
		u32 ArrayIndex = DebugTable->ArrayIndex;
		u32 EventCount = DebugTable->EventCount;
		debug_event *EventArray = DebugTable->DebugEvents[ArrayIndex];
		if(!DebugState->PauseCollation)
		{
			CollateDebugRecords(DebugState, EventArray, EventCount);
		}
		DebugTable->ArrayIndex = (DebugTable->ArrayIndex + 1) % ArrayCount(DebugTable->DebugEvents);
		DebugTable->EventCount = 0;

		BeginRender(DebugState->RenderGroup);
		PushSetup(DebugState->RenderGroup, &DebugState->ScreenCamera);

		debug_parsed_guid *RootParsed = &DebugState->RootUIElement.StoredEvent->ID.Parsed;
		f32 FrameFPS = 1.0f/Input->LastFramedt_;
		f32 FrameTimeMS = Input->LastFramedt_*1000;
		f32 SimFPS = 1.0f / Input->Simdt;
		FormatString(RootParsed->Name, ArrayCount(RootParsed->Name), "%fms : FPS %f : SimFPS %f", FrameTimeMS, FrameFPS, SimFPS);

		text_layout TextLayout = {};
		TextLayout.FontName = Asset_LiberationMonoFont;
		TextLayout.Scale = 0.4f;
		TextLayout.Color = V4(0.0f, 1.0f, 1.0f, 1.0f);
		TextLayout.AtY = V2(0.0f, DebugState->ScreenResolution.y - GetFontTotalLineHeight(DebugState->RenderGroup, &TextLayout));
		DebugDisplayUIElement(DebugState, &DebugState->RootUIElement, &TextLayout, 0, Input, DebugState->CurrentFrame, true);

		DebugEnd(DebugState, Input);

		PushSetup(DebugState->RenderGroup, &DebugState->WorldCamera);
		// NOTE: Physics debug display.
		DEBUGDisplayAABBTreeRegions(GameState->Physics, DebugState->RenderGroup);
		DEBUGDisplayContactInfo(GameState->Physics, DebugState->RenderGroup);

		EndRender(DebugState->RenderGroup);

		CheckMemory(&DebugState->Region);
		CheckMemory(&DebugState->TempRegion);
	}
}
