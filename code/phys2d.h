/*@H
* File: phys2d.h
* Author: Jesse Calvert
* Created: ember 6, 2017
* Last modified: November 27, 2017, 14:46
*/

#pragma once

#include "phys2d_platform.h"

#include "phys2d_config.h"
#include "phys2d_shared.h"
#include "phys2d_file_formats.h"

#include "phys2d_sort.h"
#include "phys2d_random.h"
#include "phys2d_assets.h"
#include "phys2d_render_group.h"
#include "phys2d_rigid_body.h"

struct move_spec
{
	f32 LinearAirFriction;
	f32 RotationAirFriction;
};

enum entity_type
{
	Entity_None,
	Entity_Circle,
	Entity_Square,

	Entity_Count,
};
struct entity
{
	entity *Next;
	entity *Prev;

	entity_type Type;
	v2 Dim;
	v4 Color;

	move_spec MoveSpec;

	rigid_body *RigidBody;
};

enum draw_mode
{
	Mode_AddImpulse,
	Mode_PlaceBox,
	Mode_PlaceCircle,

	Mode_Count,
};

struct game_state
{
	memory_region Region;
	memory_region TemporaryRegion;

	assets *Assets;
	render_group *RenderGroup;
	physics_state *Physics;

	camera WorldCamera;
	camera ScreenCamera;
	camera DefaultMouseCamera;

	camera OldWorldCamera;
	// camera OldScreenCamera;
	// camera OldDefaultMouseCamera;

	draw_mode DrawMode;

	f32 TimeScale;

	entity EntitySentinel;
	entity *EntityFreeList;
	u32 EntityCount;

	entity *ClickedEntity;
	v2 ClickedP;
	entity *Highlight;

	entity *ControlledEntity;
};

#if PHYS2D_DEBUG
#include "generated.h"
#endif
