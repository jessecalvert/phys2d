/*@H
* File: phys2d_render_group.cpp
* Author: Jesse Calvert
* Created: October 10, 2017, 13:06
* Last modified: November 27, 2017, 15:45
*/


/*
* @Author: Jesse
* @Date:   2016-08-26 22:26:10
* @Last Modified by:   Jesse
* @Last Modified time: 2017-07-20 09:38:21
*/

internal void
CameraSetMatrices(camera *Camera)
{
	r32 HalfWidth = Camera->HalfWidth;
	r32 HalfHeight = Camera->HalfHeight;

	Camera->View_ = TransformationMat4(V2(0,0), RotationMat2(-Camera->Rotation), V2(1,1), -Camera->P);
	Camera->InvView_ = TransformationMat4(Camera->P, RotationMat2(Camera->Rotation), V2(1,1), V2(0,0));

	// NOTE: Orthongonal projection.
	Assert(HalfWidth && HalfHeight);
	Camera->Projection_ =
	{
		1.0f/HalfWidth, 0, 0, 0,
		0, 1.0f/HalfHeight, 0, 0,
		0, 0, -2.0f/(Camera->Far - Camera->Near), 0,
		0, 0, -1.0f*(Camera->Far + Camera->Near)/(Camera->Far - Camera->Near), 1,
	};

	Camera->InvProjection_ =
	{
		HalfWidth, 0, 0, 0,
		0, HalfHeight, 0, 0,
		0, 0, 0.5f*(Camera->Near - Camera->Far), 0,
		0, 0, -0.5f*(Camera->Near + Camera->Far), 1,
	};

	Camera->Valid_ = true;
}

internal v2
SwitchCameraSpace(v2 P, camera *CameraFrom, camera *CameraTo)
{
	if(!CameraFrom->Valid_)
	{
		CameraSetMatrices(CameraFrom);
	}
	if(!CameraTo->Valid_)
	{
		CameraSetMatrices(CameraTo);
	}

	v4 P4 = V4(P.x, P.y, 0.0f, 1.0f);
	v4 Result4 = (CameraTo->InvView_*(CameraTo->InvProjection_*
				(CameraFrom->Projection_*(CameraFrom->View_*P4))));
	v2 Result = (1.0f/Result4.w)*Result4.xy;
	return Result;
}

inline void
MoveCamera(camera *Camera, v2 NewP)
{
	Camera->P = NewP;
	Camera->Valid_ = false;
}

inline void
RotateCamera(camera *Camera, f32 NewRotation)
{
	Camera->Rotation = NewRotation;
	Camera->Valid_ = false;
}

inline void
ChangeCameraSettings(camera *Camera, f32 HalfWidth, f32 HalfHeight,
	f32 Near, f32 Far)
{
	Camera->HalfWidth = HalfWidth;
	Camera->HalfHeight = HalfHeight;
	Camera->Near = Near;
	Camera->Far = Far;
	Camera->Valid_ = false;
}

internal void
SetUpDefaultScreenCamera(camera *Camera, v2i ScreenResolution)
{
	v2 ScreenDim = V2((f32)ScreenResolution.x, (f32)ScreenResolution.y);
	v2 ScreenCamP = 0.5f*ScreenDim;
	f32 ScreenCamNear = 10.0f;
	f32 ScreenCamFar = -10.0f;
	f32 ScreenCamWidthOverHeight = ScreenDim.x/ScreenDim.y;
	f32 ScreenCamHalfHeight = 0.5f*ScreenDim.y;
	f32 ScreenCamHalfWidth = ScreenCamHalfHeight*ScreenCamWidthOverHeight;
	ChangeCameraSettings(Camera, ScreenCamHalfWidth, ScreenCamHalfHeight,
		ScreenCamNear, ScreenCamFar);
	MoveCamera(Camera, ScreenCamP);
}

internal void
InitializeRenderGroup(render_group *Group, assets *Assets,
	command_buffer *CommandBuffer, v2i ScreenResolution)
{
	Group->Assets = Assets;
	Group->ScreenResolution = ScreenResolution;
	Group->CommandBuffer = CommandBuffer;
}

internal void
BeginRender(render_group *Group)
{
	Assert(!Group->CommandBuffer->CurrentRenderGroup);
	Group->LastSetup = {};
	Group->CurrentQuadEntry = 0;

	Group->CommandBuffer->CurrentRenderGroup = Group;
}

internal void
EndRender(render_group *Group)
{
	Group->LastSetup = {};
	Group->CurrentQuadEntry = 0;
	Group->CommandBuffer->CurrentRenderGroup = 0;
}

#define PushRenderEntry(Group, type) (type *)PushRenderEntry_((Group), sizeof(type), RenderCommandType_##type)
internal u8 *
PushRenderEntry_(render_group *Group,
	u32 EntrySize, render_command_type Type)
{
	command_buffer *CommandBuffer = Group->CommandBuffer;
	Assert(CommandBuffer->CurrentRenderGroup == Group);
	u32 TotalSize = EntrySize + sizeof(render_entry_header);
	Assert(TotalSize < (CommandBuffer->MaxSize - CommandBuffer->Size));

	render_entry_header *Header = (render_entry_header *)(CommandBuffer->Buffer + CommandBuffer->Size);
	CommandBuffer->Size += TotalSize;

	Header->Type = Type;

	u8 *Result = (u8 *)(Header + 1);
	return Result;
}

inline render_setup
DefaultRenderSetup(render_group *Group)
{
	render_setup Result = {};
	Result.ClipRectMin = {};
	Result.ClipRectMax = Group->ScreenResolution;
	return Result;
}

internal void
PushSetup(render_group *Group, render_setup Setup)
{
	Group->LastSetup = Setup;
	Group->CurrentQuadEntry = 0;
}

internal void
PushSetup(render_group *Group, camera *Camera)
{
	render_setup Setup = DefaultRenderSetup(Group);
	if(!Camera->Valid_)
	{
		CameraSetMatrices(Camera);
	}
	Setup.Projection = Camera->Projection_;
	Setup.View = Camera->View_;

	PushSetup(Group, Setup);
}

internal void
PushSetup(render_group *Group, camera *Camera, v2i ClipRectMin, v2i ClipRectMax)
{
	render_setup Setup = {};
	Setup.ClipRectMin = ClipRectMin;
	Setup.ClipRectMax = ClipRectMax;
	if(!Camera->Valid_)
	{
		CameraSetMatrices(Camera);
	}
	Setup.Projection = Camera->Projection_;
	Setup.View = Camera->View_;

	PushSetup(Group, Setup);
}

internal void
ChangeClipRect(render_group *Group, v2i ClipRectMin, v2i ClipRectMax)
{
	render_setup Setup = Group->LastSetup;
	Setup.ClipRectMin = ClipRectMin;
	Setup.ClipRectMax = ClipRectMax;
	PushSetup(Group, Setup);
}

#define TemporaryClipRectChange__(Group, ClipMin, ClipMax, Number) temp_clip_rect_setup TEMPORARY_CLIP_RECT##Number(Group, ClipMin, ClipMax)
#define TemporaryClipRectChange_(Group, ClipMin, ClipMax, Number) TemporaryClipRectChange__(Group, ClipMin, ClipMax, Number)
#define TemporaryClipRectChange(Group, ClipMin, ClipMax) TemporaryClipRectChange_(Group, ClipMin, ClipMax, __COUNTER__);

class temp_clip_rect_setup
{
public:
	render_group *Group;
	v2i OldClipMin;
	v2i OldClipMax;
	render_entry_quads *LastQuadEntry_;

	temp_clip_rect_setup(render_group *GroupInit, v2i ClipRectMin, v2i ClipRectMax)
	{
		Group = GroupInit;
		OldClipMin = Group->LastSetup.ClipRectMin;
		OldClipMax = Group->LastSetup.ClipRectMax;
		LastQuadEntry_ = Group->CurrentQuadEntry;
		ChangeClipRect(Group, ClipRectMin, ClipRectMax);
	}

	~temp_clip_rect_setup()
	{
		ChangeClipRect(Group, OldClipMin, OldClipMax);
	}
};

internal render_entry_quads *
GetQuadEntry(render_group *Group)
{
	render_entry_quads *Result = Group->CurrentQuadEntry;
	if(!Result)
	{
		Result = Group->CurrentQuadEntry = PushRenderEntry(Group, render_entry_quads);
		Result->Setup = Group->LastSetup;
		Result->FirstQuadIndex = Group->CommandBuffer->QuadCount;
		Result->QuadCount = 0;
	}

	return Result;
}

inline u32
PackV4ToU32(v4 V)
{
	u32 R = (u32)(255.0f*V.r);
	u32 G = (u32)(255.0f*V.g);
	u32 B = (u32)(255.0f*V.b);
	u32 A = (u32)(255.0f*V.a);

	u32 Result = ((A << 24) |
		          (B << 16) |
		          (G << 8) |
		          (R << 0));
	return Result;
}

internal void
PushQuad_(render_group *Group, v2 Min, v2 Max,
	v2 TexMin, v2 TexMax, mat2 RotationMatrix, u64 Data, v4 Color = V4(1,1,1,1))
{
	render_entry_quads *Entry = GetQuadEntry(Group);
	u32 QuadIndex = Entry->FirstQuadIndex + Entry->QuadCount;
	vertex_format *Verts = Group->CommandBuffer->QuadVertices + 4*QuadIndex;
	u64 *TextureData = Group->CommandBuffer->TextureData + QuadIndex;
	*TextureData = Data;
	++Entry->QuadCount;
	++Group->CommandBuffer->QuadCount;
	Assert(Group->CommandBuffer->QuadCount < Group->CommandBuffer->MaxQuads);

	v2 Dim = (Max - Min);
	v2 P = Min + 0.5f*Dim;
	Verts[0].Position = V3(RotationMatrix*(Min - P) + P, 0.0f);
	Verts[1].Position = V3(RotationMatrix*(Min + V2(Dim.x, 0.0f) - P) + P, 0.0f);
	Verts[2].Position = V3(RotationMatrix*(Max - P) + P, 0.0f);
	Verts[3].Position = V3(RotationMatrix*(Min + V2(0.0f, Dim.y) - P) + P, 0.0f);

	v2 TexScale = TexMax - TexMin;
	Verts[0].TexCoords = TexMin;
	Verts[1].TexCoords = TexMin + V2(TexScale.x, 0.0f);
	Verts[2].TexCoords = TexMax;
	Verts[3].TexCoords = TexMin + V2(0.0f, TexScale.y);

	u32 ColorPacked = PackV4ToU32(Color);
	Verts[0].Color = ColorPacked;
	Verts[1].Color = ColorPacked;
	Verts[2].Color = ColorPacked;
	Verts[3].Color = ColorPacked;
}

internal void
PushQuad_(render_group *Group, v2 Min, v2 Max,
	v2 TexMin, v2 TexMax, u64 Data, v4 Color = V4(1,1,1,1))
{
	render_entry_quads *Entry = GetQuadEntry(Group);
	u32 QuadIndex = Entry->FirstQuadIndex + Entry->QuadCount;
	vertex_format *Verts = Group->CommandBuffer->QuadVertices + 4*QuadIndex;
	u64 *TextureData = Group->CommandBuffer->TextureData + QuadIndex;
	*TextureData = Data;
	++Entry->QuadCount;
	++Group->CommandBuffer->QuadCount;
	Assert(Group->CommandBuffer->QuadCount < Group->CommandBuffer->MaxQuads);

	v2 Dim = (Max - Min);
	v2 P = Min + 0.5f*Dim;
	Verts[0].Position = V3(Min, 0.0f);
	Verts[1].Position = V3(Min + V2(Dim.x, 0.0f), 0.0f);
	Verts[2].Position = V3(Max, 0.0f);
	Verts[3].Position = V3(Min + V2(0.0f, Dim.y), 0.0f);

	v2 TexScale = TexMax - TexMin;
	Verts[0].TexCoords = TexMin;
	Verts[1].TexCoords = TexMin + V2(TexScale.x, 0.0f);
	Verts[2].TexCoords = TexMax;
	Verts[3].TexCoords = TexMin + V2(0.0f, TexScale.y);

	u32 ColorPacked = PackV4ToU32(Color);
	Verts[0].Color = ColorPacked;
	Verts[1].Color = ColorPacked;
	Verts[2].Color = ColorPacked;
	Verts[3].Color = ColorPacked;
}

internal void
PushQuad_(render_group *Group, v2 Min, v2 Max,
	v2 TexMin, v2 TexMax, f32 Rotation, u64 Data, v4 Color = V4(1,1,1,1))
{
	if(Rotation)
	{
		PushQuad_(Group, Min, Max, TexMin, TexMax, RotationMat2(Rotation), Data, Color);
	}
	else
	{
		PushQuad_(Group, Min, Max, TexMin, TexMax, Data, Color);
	}
}

internal void
PushTexture(render_group *Group, v2 Min, v2 Max, v2 TexMin, v2 TexMax, asset_name TextureName)
{
	texture *Texture = GetTexture(Group->Assets, TextureName);
	PushQuad_(Group, Min, Max, TexMin, TexMax, Texture->Data_);
}

internal void
PushTexture(render_group *Group, v2 Min, v2 Max, asset_name TextureName)
{
	PushTexture(Group, Min, Max, V2(0.0f, 0.0f), V2(1.0f, 1.0f), TextureName);
}

internal void
PushText_(render_group *Group, v2 Position, string String, font *Font, f32 Scale,
	v4 Color = V4(1.0f, 1.0f, 1.0f, 1.0f))
{
	char LastChar = 0;
	for(u32 CharIndex = 0;
		CharIndex < String.Length;
		++CharIndex)
	{
		char Character = String.Text[CharIndex];
		Assert(Character);
		Position.x += Font->MetaData->Kerning[LastChar][Character];

		v2 CharacterTexelDim = Font->MetaData->TexCoordMax[Character] - Font->MetaData->TexCoordMin[Character];
		v2 CharacterPixelDim = Scale*Hadamard(V2((r32)Font->MetaData->Width, (r32)Font->MetaData->Height),
			CharacterTexelDim);
		v2 CharacterPosition = Position + Scale*Font->MetaData->Offset[Character];
		PushQuad_(Group, CharacterPosition, CharacterPosition + CharacterPixelDim,
			Font->MetaData->TexCoordMin[Character], Font->MetaData->TexCoordMax[Character], Font->Data_, Color);

		Position.x += Scale*Font->MetaData->Advance[Character];
	}
}

internal rectangle2
GetTextBounds(render_group *Group, text_layout *Layout, string String)
{
	rectangle2 Result = {};
	font *Font = GetFont(Group->Assets, Layout->FontName);
	if(Font)
	{
		v2 TotalMin = {Real32Maximum, Real32Maximum};
		v2 TotalMax = {Real32Minimum, Real32Minimum};

		v2 Position = Layout->AtY;
		f32 Scale = Layout->Scale;

		char LastChar = 0;
		for(u32 CharIndex = 0;
			CharIndex < String.Length;
			++CharIndex)
		{
			char Character = String.Text[CharIndex];
			Position.x += Font->MetaData->Kerning[LastChar][Character];

			v2 CharacterTexelDim = Font->MetaData->TexCoordMax[Character] - Font->MetaData->TexCoordMin[Character];
			v2 CharacterPixelDim = Scale*Hadamard(V2((r32)Font->MetaData->Width, (r32)Font->MetaData->Height),
				CharacterTexelDim);
			v2 CharacterPosition = Position + Scale*Font->MetaData->Offset[Character];
			v2 MinPixels = CharacterPosition;
			v2 MaxPixels = CharacterPosition + CharacterPixelDim;

			TotalMin.x = Minimum(TotalMin.x, MinPixels.x);
			TotalMin.y = Minimum(TotalMin.y, MinPixels.y);
			TotalMax.x = Maximum(TotalMax.x, MaxPixels.x);
			TotalMax.y = Maximum(TotalMax.y, MaxPixels.y);

			Position.x += Scale*Font->MetaData->Advance[Character];
		}

		Result = Rectangle2(TotalMin, TotalMax);
	}

	return Result;
}

internal f32
GetFontTotalLineHeight(render_group *Group, text_layout *Layout)
{
	f32 Result = 0.0f;
	font *Font = GetFont(Group->Assets, Layout->FontName);
	if(Font)
	{
		font_sla *FontInfo = Font->MetaData;
		r32 DescentPixels = (FontInfo->Ascent - FontInfo->Descent + FontInfo->Linegap);
		r32 Descent01 = (f32)DescentPixels;
		Result = Layout->Scale*Descent01;
	}
	return Result;
}

internal void
PushText(render_group *Group, text_layout *Layout, string String, u32 Length = 0)
{
	TIMED_FUNCTION();

	font *Font = GetFont(Group->Assets, Layout->FontName);
	if(Font)
	{
		f32 LineHeight = GetFontTotalLineHeight(Group, Layout);
		v2 DropShadowOffset = -0.04f*V2(LineHeight, LineHeight);
		PushText_(Group, Layout->AtY + DropShadowOffset, String, Font, Layout->Scale, 0.5f*Layout->Color);
		PushText_(Group, Layout->AtY, String, Font, Layout->Scale, Layout->Color);
		Layout->AtY.y -= LineHeight;
	}
}

internal void
PushClear(render_group *Group, v4 Color)
{
	render_entry_clear *Entry = PushRenderEntry(Group, render_entry_clear);

	Entry->Color = Color;
}

internal void
PushCircle(render_group *Group, v2 Center, f32 Radius, mat2 Rotation, v4 Color)
{
	v2 Dim = V2(Radius, Radius);
	texture *Texture = GetTexture(Group->Assets, Asset_Circle);
	u64 TextureData = Texture->Data_;
	PushQuad_(Group, Center - Dim, Center + Dim, V2(0, 0), V2(1, 1), Rotation, TextureData, Color);
}

internal void
PushQuad(render_group *Group, v2 P, v2 Dim, mat2 Rotation, v4 Color)
{
	v2 Min = P - 0.5f*Dim;
	v2 Max = P + 0.5f*Dim;
	PushQuad_(Group, Min, Max, V2(0,0), V2(1,1), Rotation, Group->CommandBuffer->WhiteTexture, Color);
}

internal void
PushQuad(render_group *Group, v2 Min, v2 Max, v4 Color = V4(1,1,1,1))
{
	PushQuad_(Group, Min, Max, V2(0,0), V2(1,1), Group->CommandBuffer->WhiteTexture, Color);
}

internal void
PushRectangleOutline(render_group *Group, v2 Min, v2 Max, v4 Color, r32 Width = 0.1f)
{
	PushQuad(Group, V2(Min.x - Width, Min.y - Width), V2(Max.x + Width, Min.y), Color);
	PushQuad(Group, V2(Min.x - Width, Min.y), V2(Min.x, Max.y + Width), Color);
	PushQuad(Group, V2(Min.x, Max.y), V2(Max.x + Width, Max.y + Width), Color);
	PushQuad(Group, V2(Max.x, Min.y), V2(Max.x + Width, Max.y), Color);
}

internal void
PushPoint(render_group *Group, v2 P, v4 Color = V4(1.0f, 1.0f, 0.0f, 1.0f))
{
	v2 HalfPointDim = 0.5f*V2(0.1f, 0.1f);
	PushQuad(Group, P - HalfPointDim, P + HalfPointDim, Color);
}

internal void
PushLine(render_group *Group, v2 A, v2 B, f32 Width, v4 Color = V4(1.0f, 1.0f, 0.0f, 1.0f))
{
	v2 Line = B - A;
	r32 Distance = Length(Line);
	v2 Dim = V2(Distance, Width);
	v2 P = A + 0.5f*(Line);
	f32 Rotation = ATan2(Line.y, Line.x);
	PushQuad(Group, P, Dim, RotationMat2(Rotation), Color);
}
