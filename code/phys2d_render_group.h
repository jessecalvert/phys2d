#ifndef PHYS2D_RENDER_GROUP_H

struct text_layout
{
	asset_name FontName;
	f32 Scale;
	v4 Color;

	v2 AtY;
};

struct render_setup
{
	v2i ClipRectMin;
	v2i ClipRectMax;

	mat4 Projection;
	mat4 View;
};

enum render_command_type
{
	RenderCommandType_render_entry_clear,
	RenderCommandType_render_entry_quads,
};

struct render_entry_header
{
	render_command_type Type;
};

struct render_entry_quads
{
	render_setup Setup;
	u32 FirstQuadIndex;
	u32 QuadCount;
};

struct render_entry_clear
{
	v4 Color;
};

struct camera
{
	// NOTE: orthogonal
	r32 HalfWidth;
	r32 HalfHeight;
	r32 Near;
	r32 Far;

	v2 P;
	r32 Rotation;

	b32 Valid_;
	mat4 Projection_;
	mat4 InvProjection_;
	mat4 View_;
	mat4 InvView_;
};

#define MAX_CAMERAS 64

struct render_group
{
	assets *Assets;
	v2i ScreenResolution;

	render_setup LastSetup;
	render_entry_quads *CurrentQuadEntry;

	command_buffer *CommandBuffer;
};

#define PHYS2D_RENDER_GROUP_H
#endif
