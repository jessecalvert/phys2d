/*@H
* File: phys2d.cpp
* Author: Jesse Calvert
* Created: October 6, 2017, 16:31
* Last modified: November 27, 2017, 15:45
*/

#include "phys2d.h"

#include "phys2d_assets.cpp"
#include "phys2d_render_group.cpp"
#include "phys2d_rigid_body.cpp"

inline move_spec
DefaultMoveSpec()
{
	move_spec Result = {};
	Result.LinearAirFriction = 0.1f;
	Result.RotationAirFriction = 0.5f;
	return Result;
}

internal entity *
AddEntity(game_state *GameState)
{
	entity *Result = GameState->EntityFreeList;
	if(Result)
	{
		GameState->EntityFreeList = Result->Next;
		ZeroStruct(*Result);
	}
	else
	{
		Result = PushStruct(&GameState->Region, entity);
	}

	DLIST_INSERT(&GameState->EntitySentinel, Result);
	++GameState->EntityCount;

	return Result;
}

internal entity *
AddBox(game_state *GameState, v2 P, v2 Dim, f32 Rotation, b32 Collideable = true, v4 Color = V4(1,1,1,1))
{
	entity *Entity = AddEntity(GameState);

	Entity->Type = Entity_Square;
	Entity->Dim = Dim;
	Entity->Color = Color;
	Entity->MoveSpec = DefaultMoveSpec();

	f32 Density = 1.0f;
	f32 InvMass = 1.0f / (Density*Dim.x*Dim.y);
	f32 InvMomentOfInertia = (12.0f*InvMass)/(Square(Entity->Dim.x) + Square(Entity->Dim.y));
	shape Shape = {};
	Shape.Type = Shape_ConvexPolygon;
	Shape.Polygon.VertexCount = 4;
	// TODO: Should this be in the physics_state Region?
	v2 HalfDim = 0.5f*Entity->Dim;
	Shape.Polygon.Vertices = PushArray(&GameState->Region, Shape.Polygon.VertexCount, v2);
	Shape.Polygon.Vertices[0] = -HalfDim;
	Shape.Polygon.Vertices[1] = V2(HalfDim.x, -HalfDim.y);
	Shape.Polygon.Vertices[2] = HalfDim;
	Shape.Polygon.Vertices[3] = V2(-HalfDim.x, HalfDim.y);

	Entity->RigidBody = AddRigidBody(GameState->Physics, Entity, P, V2(0,0), Rotation, InvMass, InvMomentOfInertia, Shape, Collideable);

	return Entity;
}

internal entity *
AddCircle(game_state *GameState, v2 P, f32 Radius, b32 Collideable = true, v4 Color = V4(1,1,1,1))
{
	entity *Entity = AddEntity(GameState);

	Entity->Type = Entity_Circle;
	Entity->Dim = 2.0f*V2(Radius, Radius);
	Entity->Color = Color;
	Entity->MoveSpec = DefaultMoveSpec();

	f32 Density = 1.0f;
	f32 InvMass = 1.0f / (Density*Pi32*Radius*Radius);
	f32 InvMomentOfInertia = (2.0f*InvMass)/(Square(Radius));
	shape Shape = {};
	Shape.Type = Shape_Circle;
	Shape.Circle.Center = V2(0, 0);
	Shape.Circle.Radius = Radius;

	Entity->RigidBody = AddRigidBody(GameState->Physics, Entity, P, V2(0,0), 0.0f, InvMass, InvMomentOfInertia, Shape, Collideable);

	return Entity;
}

internal entity *
PickEntityAtPoint(game_state *GameState, v2 WorldMouseP)
{
	entity *Result = 0;
	rigid_body *Body = AABBQueryPoint(GameState->Physics, WorldMouseP);
	if(Body)
	{
		Result = Body->Entity;
	}

	return Result;
}

internal entity *
AddStaticBox(game_state *GameState, v2 P, v2 Dim, f32 Rotation, b32 Collideable = true, v4 Color = V4(1,1,1,1))
{
	entity *Entity = AddEntity(GameState);

	Entity->Type = Entity_Square;
	Entity->Dim = Dim;
	Entity->Color = Color;

	f32 InvMass = 0.0f;
	f32 InvMomentOfInertia = 0.0f;
	shape Shape = {};
	Shape.Type = Shape_ConvexPolygon;
	Shape.Polygon.VertexCount = 4;
	// TODO: Should this be in the physics_state Region?
	v2 HalfDim = 0.5f*Entity->Dim;
	Shape.Polygon.Vertices = PushArray(&GameState->Region, Shape.Polygon.VertexCount, v2);
	Shape.Polygon.Vertices[0] = -HalfDim;
	Shape.Polygon.Vertices[1] = V2(HalfDim.x, -HalfDim.y);
	Shape.Polygon.Vertices[2] = HalfDim;
	Shape.Polygon.Vertices[3] = V2(-HalfDim.x, HalfDim.y);

	Entity->RigidBody = AddRigidBody(GameState->Physics, Entity, P, V2(0,0), Rotation, InvMass, InvMomentOfInertia, Shape, Collideable);

	return Entity;
}

internal game_state *
InitializeGamestate(game_memory *Memory)
{
	game_state *GameState = Memory->GameState;
	Assert(!GameState);

	TIMED_BLOCK("GameState Initialization");

	GameState = Memory->GameState = BootstrapPushStruct(game_state, Region);
	GameState->Assets = BootstrapPushStruct(assets, Region);
	LoadAssets(GameState->Assets);
	GameState->RenderGroup = PushStruct(&GameState->Region, render_group);
	InitializeRenderGroup(GameState->RenderGroup, GameState->Assets, &Memory->CommandBuffer, Memory->WindowResolution);

	GameState->TimeScale = 1.0f;

	f32 WorldNear = 1.0f;
	f32 WorldFar = -1.0f;
	f32 WorldCamWidthOverHeight = 16.0f/9.0f;
	f32 WorldHalfHeight = 10.0f;
	f32 WorldHalfWidth = WorldHalfHeight*WorldCamWidthOverHeight;
	ChangeCameraSettings(&GameState->WorldCamera, WorldHalfWidth, WorldHalfHeight,
		WorldNear, WorldFar);

	SetUpDefaultScreenCamera(&GameState->ScreenCamera, Memory->WindowResolution);
	SetUpDefaultScreenCamera(&GameState->DefaultMouseCamera, Memory->WindowResolution);

	GameState->OldWorldCamera = GameState->WorldCamera;

	GameState->Physics = InitializePhysics(GameState->RenderGroup,
		&GameState->ScreenCamera, &GameState->WorldCamera);

	DLIST_INIT(&GameState->EntitySentinel);

	v2 GroundDim = V2(100.0f, 3.0f);
	v2 GroundP = V2(0.0f, -11.0f);
	entity *Ground = AddStaticBox(GameState, GroundP, GroundDim, 0.0f);

#if 1
	u32 BoxCountX = 1;
	u32 BoxCountY = 5;
	v2 Dim = V2(1.0f, 1.0f);
	f32 BaseX = -5.0f;
	f32 AtX = BaseX;
	f32 AtY = GroundP.y + 0.5f*(GroundDim.y + Dim.y);

	for(u32 Y = 0;
		Y < BoxCountY;
		++Y)
	{
		for(u32 X = 0;
			X < BoxCountX;
			++X)
		{
			v2 Offset = V2(0.05f, 0.0f);
			// if(Y % 2) {Offset = -Offset;}
			AddBox(GameState, V2(AtX, AtY) + Offset, Dim, 0.0f);
			AtX += Dim.x;
		}

		AtX = BaseX;
		AtY += Dim.y;
	}
#else
	v2 Dim = V2(1.0f, 0.25f);
	v2 At = V2(-2.0f, 2.0f);
	f32 Offset = 0.5f*Dim.x;
	v2 AnchorOffset = V2(0.5f*Dim.x - 0.2f*Offset, 0.0f);
	u32 ChainCount = 30;
	entity *LastBox = AddBox(GameState, At, Dim, 0.0f);
	AddRevoluteJoint(GameState->Physics, Ground->RigidBody, LastBox->RigidBody, V2(0.0f, 10.0f), -AnchorOffset, false);

	At.x += Dim.x + Offset;
	for(u32 ChainIndex = 1;
		ChainIndex < ChainCount;
		++ChainIndex)
	{
		entity *NewBox = AddBox(GameState, At, Dim, 0.0f);
		rigid_body *BodyA = LastBox->RigidBody;
		rigid_body *BodyB = NewBox->RigidBody;
		AddRevoluteJoint(GameState->Physics, BodyA, BodyB, AnchorOffset, -AnchorOffset, false);

		LastBox = NewBox;
		At.x += Dim.x + Offset;
	}

	// AddCircle(GameState, V2(2.0f, 1.0f), 0.75f);
#endif

#if 0
	GameState->ControlledEntity = AddCircle(GameState, V2(-1.0f, 1.0f), 0.5f);
	GameState->ControlledEntity->RigidBody->InvMass /= 4.0f;
	entity *Club = AddBox(GameState, V2(-2.0f, 1.0f), V2(1.0f, 1.0f), 0.0f, 0.005f, 0.01f);
	AddRevoluteJoint(GameState->Physics, GameState->ControlledEntity->RigidBody, Club->RigidBody, V2(0,0), V2(3.0f, 0.0f), false);
#endif

	u32 PendulumCount = 6;
	v2 At = V2(0.0, 14.0f);
	f32 PendulumLength = 2.0f;
	f32 PendulumRadius = 0.5f;
	entity *LastCircle = AddCircle(GameState, At, PendulumRadius, false);
	AddRevoluteJoint(GameState->Physics, LastCircle, Ground, V2(0.0f, PendulumLength), V2(0.0f, At.y + 6.0f), true);
	At.y -= PendulumLength;
	for(u32 Index = 1;
		Index < PendulumCount;
		++Index)
	{
		entity *Circle = AddCircle(GameState, At, PendulumRadius, false);
		AddRevoluteJoint(GameState->Physics, LastCircle, Circle, V2(0,0), V2(0.0f, PendulumLength), true);
		LastCircle = Circle;
		At.y -= PendulumLength;
	}

	return GameState;
}

#if PHYS2D_DEBUG
debug_table *GlobalDebugTable;
#endif

extern "C" GAME_UPDATE(GameUpdate)
{
	game_state *GameState = Memory->GameState;

	Platform = Memory->Platform;
#if PHYS2D_DEBUG
	GlobalDebugTable = Memory->DebugTable;
#endif

	TIMED_FUNCTION();

	if(!GameState)
	{
		GameState = InitializeGamestate(Memory);
	}

	camera *WorldCamera = &GameState->WorldCamera;
	CheckMemory(&GameState->Region);
	CheckMemory(&GameState->TemporaryRegion);
	f32 ScaledFramedt = GameState->TimeScale*Input->Simdt;

	v2 WorldMouseP = SwitchCameraSpace(Input->MouseP, &GameState->DefaultMouseCamera, &GameState->WorldCamera);

	if(Input->KeyStates[Key_LeftClick].WasPressed)
	{
		switch(GameState->DrawMode)
		{
			case Mode_AddImpulse:
			{
				GameState->ClickedEntity = PickEntityAtPoint(GameState, WorldMouseP);
				if(GameState->ClickedEntity)
				{
					rigid_body *Body = GameState->ClickedEntity->RigidBody;
					GameState->ClickedP = WorldPointToLocalPoint(Body, WorldMouseP);
				}
			} break;

			case Mode_PlaceBox:
			{
				v2 Dim = V2(1.0f, 1.0f);
				// f32 Rotation = RandomRangeR32(&GlobalSeries, 0, 2.0f*Pi32);
				f32 Rotation = 0.0f;
				entity *Box = AddBox(GameState, WorldMouseP, Dim, Rotation);
			} break;

			case Mode_PlaceCircle:
			{
				r32 Radius = 1.0f;
				entity *Circle = AddCircle(GameState, WorldMouseP, Radius);
			} break;

			InvalidDefaultCase;
		}
	}

	if(Input->KeyStates[Key_LeftClick].WasReleased)
	{
		if((GameState->DrawMode == Mode_AddImpulse) &&
		   (GameState->ClickedEntity))
		{
			rigid_body *Body = GameState->ClickedEntity->RigidBody;
			f32 ImpulseFactor = 5.0f;
			v2 ClickedP = LocalPointToWorldPoint(Body, GameState->ClickedP);
			ApplyImpulse(Body, ImpulseFactor*(WorldMouseP - ClickedP), ClickedP);
			GameState->ClickedEntity = 0;
		}
	}

	if(GameState->ClickedEntity)
	{
		GameState->Highlight = GameState->ClickedEntity;
	}
	else
	{
		GameState->Highlight = PickEntityAtPoint(GameState, WorldMouseP);
	}

	r32 CameraMoveScale = WorldCamera->HalfHeight*Input->Simdt;
	v2 WorldCameraMove = {};
	if(Input->KeyStates[Key_Up].Pressed)
	{
		WorldCameraMove += CameraMoveScale*V2(0.0f, 1.0f);
	}
	if(Input->KeyStates[Key_Down].Pressed)
	{
		WorldCameraMove += CameraMoveScale*V2(0.0f, -1.0f);
	}
	if(Input->KeyStates[Key_Left].Pressed)
	{
		WorldCameraMove += CameraMoveScale*V2(-1.0f, 0.0f);
	}
	if(Input->KeyStates[Key_Right].Pressed)
	{
		WorldCameraMove += CameraMoveScale*V2(1.0f, 0.0f);
	}
	GameState->OldWorldCamera = *WorldCamera;
	MoveCamera(WorldCamera, WorldCamera->P + WorldCameraMove);

	f32 ZoomScale = 1.05f;
	f32 WorldHalfWidth = WorldCamera->HalfWidth;
	f32 WorldHalfHeight = WorldCamera->HalfHeight;
	if(Input->KeyStates[Key_Minus].Pressed)
	{
		WorldHalfHeight *= ZoomScale;
		WorldHalfWidth *= ZoomScale;
	}
	if(Input->KeyStates[Key_Plus].Pressed)
	{
		WorldHalfHeight /= ZoomScale;
		WorldHalfWidth /= ZoomScale;
	}
	ChangeCameraSettings(WorldCamera, WorldHalfWidth, WorldHalfHeight,
		WorldCamera->Near, WorldCamera->Far);

	if(Input->KeyStates[Key_Q].WasPressed)
	{
		GameState->DrawMode = (draw_mode)((GameState->DrawMode + 1) % Mode_Count);
	}

	//
	// NOTE: Simulate
	//

	v2 GravityImpulse = ScaledFramedt*V2(0.0f, -15.0f);

	for(entity *Entity = GameState->EntitySentinel.Next;
	    Entity != &GameState->EntitySentinel;
	    Entity = Entity->Next)
	{
		rigid_body *Body = Entity->RigidBody;
		if(Body)
		{
			if(Body->InvMass)
			{
				f32 Mass = (1.0f/Body->InvMass);

				if(Global_GravityMode)
				{
					ApplyImpulse(Body, Mass*GravityImpulse, Body->WorldCentroid);
				}
				else
				{
					ApplyImpulse(Body, -Mass*Entity->MoveSpec.LinearAirFriction*Body->dP, Body->WorldCentroid);
					if(Body->InvMomentOfInertia && Body->AngularVelocity)
					{
						f32 MomentOfInertia = (1.0f/Body->InvMomentOfInertia);
						Body->AngularVelocity += -Entity->MoveSpec.RotationAirFriction*MomentOfInertia*Body->AngularVelocity;
					}
				}
			}
		}
	}

	if(GameState->ControlledEntity)
	{
		rigid_body *PlayerBody = GameState->ControlledEntity->RigidBody;
	 	f32 PlayerMass = (1.0f/PlayerBody->InvMass);
	 	f32 ImpulseFactor = 1.0f;
		ApplyImpulse(PlayerBody, ImpulseFactor*(WorldMouseP - PlayerBody->WorldCentroid), PlayerBody->WorldCentroid);
	}

	PhysicsIntegrate(GameState->Physics, ScaledFramedt);
	PhysicsFindAndResolveContacts(GameState->Physics, ScaledFramedt);
}

extern "C" GAME_RENDER(GameRender)
{
	game_state *GameState = Memory->GameState;

	Platform = Memory->Platform;
#if PHYS2D_DEBUG
	GlobalDebugTable = Memory->DebugTable;
#endif

	{DEBUG_DATA_BLOCK("Physics");
		if(GameState) {DEBUG_VALUE(&GameState->TimeScale);}
		{DEBUG_DATA_BLOCK("Contacts");
			DEBUG_VALUE(&Global_DisplayContacts);
			DEBUG_VALUE(&Global_DisplayContactNormals);
			DEBUG_VALUE(&Global_DisplayImpulses);
			DEBUG_VALUE(&Global_DisplayJoints);
		}
		DEBUG_VALUE(&Global_DisplayAABB);
		{DEBUG_DATA_BLOCK("AABB Tree Diagram");
			DEBUG_UI_ELEMENT(Event_AABBTree, "AABB Tree Diagram");
		}
		DEBUG_VALUE(&Global_GravityMode);
	}
	{DEBUG_DATA_BLOCK("Profiler");
		{DEBUG_DATA_BLOCK("Top Clocks");
			DEBUG_UI_ELEMENT(Event_TopClocksList, "Top Clocks");
		}
		{DEBUG_DATA_BLOCK("Frame Slider");
			DEBUG_UI_ELEMENT(Event_FrameSlider, "Frame Slider");
		}
		{DEBUG_DATA_BLOCK("Frame Graph");
			DEBUG_UI_ELEMENT(Event_FrameBarGraph, "Frame Graph");
		}
	}
	{DEBUG_DATA_BLOCK("Memory");
		DEBUG_UI_ELEMENT(Event_MemoryDisplay, "Memory Regions");
	}

	TIMED_FUNCTION();

	if(!GameState)
	{
		GameState = InitializeGamestate(Memory);
	}

	CheckMemory(&GameState->Region);
	CheckMemory(&GameState->TemporaryRegion);

	render_group *RenderGroup = GameState->RenderGroup;
	BeginRender(RenderGroup);
	v4 ClearColor = {0.2f, 0.2f, 0.2f, 1.0f};
	PushClear(RenderGroup, ClearColor);

	camera *WorldCamera = &GameState->WorldCamera;
	camera *OldWorldCamera = &GameState->OldWorldCamera;

	camera InterpolatedCamera = {};
	InterpolatedCamera.HalfWidth = Lerp(OldWorldCamera->HalfWidth, StateBlend, WorldCamera->HalfWidth);
	InterpolatedCamera.HalfHeight = Lerp(OldWorldCamera->HalfHeight, StateBlend, WorldCamera->HalfHeight);
	InterpolatedCamera.Near = Lerp(OldWorldCamera->Near, StateBlend, WorldCamera->Near);
	InterpolatedCamera.Far = Lerp(OldWorldCamera->Far, StateBlend, WorldCamera->Far);
	InterpolatedCamera.P = Lerp(OldWorldCamera->P, StateBlend, WorldCamera->P);
	InterpolatedCamera.Rotation = Lerp(OldWorldCamera->Rotation, StateBlend, WorldCamera->Rotation);

	PushSetup(RenderGroup, &InterpolatedCamera);
	for(entity *Entity = GameState->EntitySentinel.Next;
	    Entity != &GameState->EntitySentinel;
	    Entity = Entity->Next)
	{
		rigid_body *Body = Entity->RigidBody;
		if(Body)
		{
			v4 Color = Entity->Color;
			if(Entity == GameState->Highlight)
			{
				Color = V4(0.8f, 0.1f, 0.1f, 1.0f);
			}

			v2 DisplayP = Lerp(Body->OldP, StateBlend, Body->P);
#if 0
			f32 OldRotationAngle = RotationAngleFromMat2(Body->OldRotation);
			f32 NewRotationAngle = RotationAngleFromMat2(Body->Rotation);
			f32 DisplayRotationAngle = Lerp(OldRotationAngle, StateBlend, NewRotationAngle);
			mat2 DisplayRotation = RotationMat2(DisplayRotationAngle);
#else
			mat2 DisplayRotation = ReOrthonormalize(Lerp(Body->OldRotation, StateBlend, Body->Rotation));
#endif

			switch(Entity->Type)
			{
				case Entity_Square:
				{

					v2 Border = V2(0.05f, 0.05f);
					PushQuad(RenderGroup, DisplayP, Entity->Dim + Border, DisplayRotation, 0.5f*Color);
					PushQuad(RenderGroup, DisplayP, Entity->Dim, DisplayRotation, Color);
				} break;

				case Entity_Circle:
				{
					f32 Border = 0.025f;
					PushCircle(RenderGroup, DisplayP, Body->Shape.Circle.Radius + Border, DisplayRotation, 0.5f*Color);
					PushCircle(RenderGroup, DisplayP, Body->Shape.Circle.Radius, DisplayRotation, Color);
				} break;

				InvalidDefaultCase;
			}
		}
	}

	v2 WorldMouseP = SwitchCameraSpace(Input->MouseP, &GameState->DefaultMouseCamera, &InterpolatedCamera);
	if(Input->KeyStates[Key_LeftClick].Pressed)
	{
		if((GameState->DrawMode == Mode_AddImpulse) &&
		   (GameState->ClickedEntity))
		{
			rigid_body *Body = GameState->ClickedEntity->RigidBody;
			PushLine(RenderGroup, LocalPointToWorldPoint(Body ,GameState->ClickedP), WorldMouseP, 0.05f, V4(0.1f, 0.1f, 0.1f, 1.0f));
		}
	}

	PushSetup(RenderGroup, &GameState->ScreenCamera);
	text_layout TextLayout = {};
	TextLayout.FontName = Asset_LiberationMonoFont;
	TextLayout.Scale = 0.5f;
	TextLayout.Color = V4(1.0f, 1.0f, 0.0f, 1.0f);
	TextLayout.AtY = V2(0.0f, 0.0f + GetFontTotalLineHeight(RenderGroup, &TextLayout));

	string ModeStrings[] =
	{
		String("AddImpulse"),
		String("PlaceBox"),
		String("PlaceCircle"),
	};

	char Buffer[512] = {};
	string Display = FormatString(Buffer, ArrayCount(Buffer), "DrawMode: %s", ModeStrings[GameState->DrawMode]);
	PushText(RenderGroup, &TextLayout, Display);

	EndRender(RenderGroup);
}

#if PHYS2D_DEBUG
#include "phys2d_debug.cpp"
#else
extern "C" DEBUG_FRAME_END(DebugFrameEnd)
{
}
#endif
