/*@H
* File: win32_phys2d.h
* Author: Jesse Calvert
* Created: ember 6, 2017, 17: 9
* Last modified: October 9, 2017, 15:38
*/

#pragma once

#ifndef WIN32_PHYS2D_H

struct game_code
{
	game_update *GameUpdate;
	game_render *GameRender;
	debug_frame_end *DEBUGFrameEnd;

	HMODULE Handle;
	FILETIME LastWriteTime;
};

struct file_hotload_tag
{
	b32 ShaderNeedsHotloading;
	b32 ReloadShader[Shader_Count];
};

struct win32_hotloader
{
	file_hotload_tag FilesToHotload;

	FILETIME LastShaderWriteTimes[Shader_Count];
};

#define WIN32_PHYS2D_H
#endif
