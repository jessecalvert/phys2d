/*@H
* File: phys2d_opengl.cpp
* Author: Jesse Calvert
* Created: November 27, 2017, 15:38
* Last modified: November 27, 2017, 15:38
*/

/*
* @Author: Jesse
* @Date:   2016-08-26 22:55:10
* @Last Modified by:   Jesse
* @Last Modified time: 2017-07-20 09:42:56
*/

#include "phys2d_opengl.h"

internal opengl_info
OpenGLGetInfo(b32 ModernContext)
{
	opengl_info Result = {};

	Result.ModernContext = ModernContext;
	Result.Vendor = (char *)glGetString(GL_VENDOR);
	Result.Renderer = (char *)glGetString(GL_RENDERER);
	Result.Version = (char *)glGetString(GL_VERSION);
	if(Result.ModernContext)
	{
		Result.ShadingLanguageVersion = (char *)glGetString(GL_SHADING_LANGUAGE_VERSION);
	}
	else
	{
		Result.ShadingLanguageVersion = "(none)";
	}

	Result.Extensions = (char *)glGetString(GL_EXTENSIONS);

	char *At = Result.Extensions;
	while(*At)
	{
		while(IsWhitespace(*At)) {++At;}
		char *End = At;
		while(*End && !IsWhitespace(*End)) {++End;}

		umm Count = End - At;

		if(0) {}
        else if(StringsAreEqual(Count, At, "GL_EXT_texture_sRGB")) {Result.GL_EXT_texture_sRGB=true;}
        else if(StringsAreEqual(Count, At, "GL_EXT_framebuffer_sRGB")) {Result.GL_EXT_framebuffer_sRGB=true;}
        else if(StringsAreEqual(Count, At, "GL_ARB_framebuffer_sRGB")) {Result.GL_EXT_framebuffer_sRGB=true;}
        // TODO: Is there some kind of ARB string to look for that indicates GL_EXT_texture_sRGB?

		At = End;
	}

	return Result;
}

internal GLuint
OpenGLCompileShader(char **ShaderSource, u32 ShaderSourceCount, GLenum ShaderType, char *ShaderFilename)
{
	GLuint Result = 0;

	GLuint ShaderHandle = glCreateShader(ShaderType);
	glShaderSource(ShaderHandle, ShaderSourceCount, (const GLchar **)ShaderSource, 0);
	glCompileShader(ShaderHandle);

	GLint Compiled;
	GLchar InfoLog[512];
	glGetShaderiv(ShaderHandle, GL_COMPILE_STATUS, &Compiled);
	if(!Compiled)
	{
		glGetShaderInfoLog(ShaderHandle, 512, 0, InfoLog);
		char Buffer[256];
		char *ShaderTypeName = (ShaderType == GL_VERTEX_SHADER) ? "Vertex" : "Fragment";
		string ShaderError = FormatString(Buffer, ArrayCount(Buffer), "Failed to load shader '%s', shader type: %s\n", String(ShaderFilename), String(ShaderTypeName));

		// TODO: Display log in a console
		OutputDebugString(ShaderError.Text);
		OutputDebugString(InfoLog);
	}
	else
	{
		Result = ShaderHandle;
	}

	return Result;
}

#define SHADER_DEFINE_(String, DefinedValueString) "#define " ## String ## " " ## #DefinedValueString ## "\n"
#define SHADER_DEFINE(String, DefinedValue) SHADER_DEFINE_(String, DefinedValue)
global_variable char *ShaderDefines[] = {
	SHADER_DEFINE("PARALLAX_HEIGHT_SCALE", 0.04f),
	SHADER_DEFINE("PARALLAX_MIN_LAYERS", 8.0f),
	SHADER_DEFINE("PARALLAX_MAX_LAYERS", 32.0f),
};

internal GLuint
OpenGLLoadCompileAndLinkShaderProgram(opengl_state *State, char *HeaderFileName, char *ShaderFileName)
{
	GLuint Result = 0;

	temporary_memory TempMem = BeginTemporaryMemory(&State->Region);

	char *ShaderSource = Platform->LoadEntireTextFileAndNullTerminate(&State->Region, ShaderFileName);
	char *HeaderSource = Platform->LoadEntireTextFileAndNullTerminate(&State->Region, HeaderFileName);
	if(ShaderSource && HeaderSource)
	{
		char *VertexShader[16] = {"#version 330 core\n", "#define VERTEX_SHADER\n",};
		char *FragmentShader[16] = {"#version 330 core\n", "#define FRAGMENT_SHADER\n",};

		u32 Offset = 2;
		for(u32 DefineIndex = 0;
			DefineIndex < ArrayCount(ShaderDefines);
			++DefineIndex)
		{
			VertexShader[Offset + DefineIndex] = ShaderDefines[DefineIndex];
			FragmentShader[Offset + DefineIndex] = ShaderDefines[DefineIndex];
		}

		VertexShader[Offset + ArrayCount(ShaderDefines)] = HeaderSource;
		VertexShader[Offset + ArrayCount(ShaderDefines) + 1] = ShaderSource;
		FragmentShader[Offset + ArrayCount(ShaderDefines)] = HeaderSource;
		FragmentShader[Offset + ArrayCount(ShaderDefines) + 1] = ShaderSource;

		u32 VertexSize = Offset + ArrayCount(ShaderDefines) + 2;
		u32 FragmentSize = Offset + ArrayCount(ShaderDefines) + 2;

		GLuint VertexShaderHandle = OpenGLCompileShader(VertexShader, VertexSize, GL_VERTEX_SHADER, ShaderFileName);
		GLuint FragmentShaderHandle = OpenGLCompileShader(FragmentShader, FragmentSize, GL_FRAGMENT_SHADER, ShaderFileName);

		if((VertexShaderHandle > 0) && (FragmentShaderHandle > 0))
		{
			GLuint ProgramIndex = glCreateProgram();
			glAttachShader(ProgramIndex, VertexShaderHandle);
			glAttachShader(ProgramIndex, FragmentShaderHandle);
			glLinkProgram(ProgramIndex);

			GLint Linked;
			GLchar InfoLog[512];
			glGetProgramiv(ProgramIndex, GL_LINK_STATUS, &Linked);
			if(!Linked)
			{
				glGetProgramInfoLog(ProgramIndex, 512, 0, InfoLog);
				// TODO: logging
				//Assert(!InfoLog);
				char Buffer[256];
				string ShaderLinkerError = FormatString(Buffer, ArrayCount(Buffer), "Failed to link shader '%s'\n", String(ShaderFileName));

				// TODO: Logging
				OutputDebugString(ShaderLinkerError.Text);
				OutputDebugString(InfoLog);
			}
			else
			{
				Result = ProgramIndex;

				glDeleteShader(VertexShaderHandle);
				glDeleteShader(FragmentShaderHandle);
			}
		}
	}

	EndTemporaryMemory(&TempMem);
	return Result;
}

internal void
OpenGLLinkSamplers(opengl_shader_common *Common,
	char *Sampler0 = 0,
	char *Sampler1 = 0,
	char *Sampler2 = 0,
	char *Sampler3 = 0,
	char *Sampler4 = 0,
	char *Sampler5 = 0,
	char *Sampler6 = 0,
	char *Sampler7 = 0,
	char *Sampler8 = 0,
	char *Sampler9 = 0,
	char *Sampler10 = 0,
	char *Sampler11 = 0,
	char *Sampler12 = 0,
	char *Sampler13 = 0,
	char *Sampler14 = 0,
	char *Sampler15 = 0)
{
    char *SamplerNames[] =
    {
        Sampler0,
        Sampler1,
        Sampler2,
        Sampler3,
        Sampler4,
        Sampler5,
        Sampler6,
        Sampler7,
        Sampler8,
        Sampler9,
        Sampler10,
        Sampler11,
        Sampler12,
        Sampler13,
        Sampler14,
        Sampler15,
    };

    for(u32 SamplerIndex = 0;
        SamplerIndex < ArrayCount(SamplerNames);
        ++SamplerIndex)
    {
        char *Name = SamplerNames[SamplerIndex];
        if(Name)
        {
            GLuint SamplerID = glGetUniformLocation(Common->ShaderHandle, Name);
            Assert(Common->SamplerCount < ArrayCount(Common->Samplers));
            Common->Samplers[Common->SamplerCount++] = SamplerID;
            Assert(SamplerID != -1);
        }
    }
}

internal b32
OpenGLLoadShader(opengl_state *State, shader_type Type)
{
	b32 Result = false;
	GLuint ShaderHandle = OpenGLLoadCompileAndLinkShaderProgram(State,
		ShaderFilenames[Shader_Header],
		ShaderFilenames[Type]);

	if(ShaderHandle)
	{
		opengl_shader_common Common = {};
		Common.ShaderHandle = ShaderHandle;

		Common.ProjectionLoc = glGetUniformLocation(Common.ShaderHandle, "Projection");
		Common.ViewLoc = glGetUniformLocation(Common.ShaderHandle, "View");
		Common.ModelLoc = glGetUniformLocation(Common.ShaderHandle, "Model");

		Common.TransformLoc = glGetUniformLocation(Common.ShaderHandle, "Transform");
		Common.TexTransformLoc = glGetUniformLocation(Common.ShaderHandle, "TexTransform");

		switch(Type)
		{
			case Shader_Quads:
			{
				opengl_shader_common *Quads = &State->Quads;
				*Quads = Common;
				OpenGLLinkSamplers(Quads, "Texture");
			} break;

			InvalidDefaultCase;
		}

		OpenGLCheckErrors();

		Result = true;
	}

	return Result;
}

internal u64
OpenGLLoadTexture(u32 *Pixels, u32 Width, u32 Height)
{
	u64 Result = 0;

	GLuint TextureIndex;

	glGenTextures(1, &TextureIndex);
	glBindTexture(GL_TEXTURE_2D, TextureIndex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, Pixels);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	OpenGLCheckErrors();

	Result = TextureIndex;
	return Result;
}

internal u64
OpenGLLoadFontTexture(u8 *Pixels, u32 Width, u32 Height)
{
	u64 Result = 0;

	GLuint TextureIndex;

	glGenTextures(1, &TextureIndex);
	glBindTexture(GL_TEXTURE_2D, TextureIndex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, Width, Height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, Pixels);

	glBindTexture(GL_TEXTURE_2D, 0);

	OpenGLCheckErrors();

	Result = TextureIndex;
	return Result;
}

internal void
OpenGLSetUpVertexAttribs()
{
	// NOTE: A vertex array should be bound first!

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_format), (GLvoid *)OffsetOf(vertex_format, Position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_format), (GLvoid *)OffsetOf(vertex_format, TexCoords));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(vertex_format), (GLvoid *)OffsetOf(vertex_format, Color));
	glEnableVertexAttribArray(2);

	OpenGLCheckErrors();
}

internal u64
OpenGLLoadMesh(vertex_format *Vertices, u32 VerticesCount,
               v3u *Faces, u32 FaceCount)
{
	u64 Result = 0;

	GLuint VAOIndex;
	GLuint VBOIndex;
	GLuint EBOIndex;

	glGenVertexArrays(1, &VAOIndex);
	glGenBuffers(1, &VBOIndex);
	glGenBuffers(1, &EBOIndex);

	glBindVertexArray(VAOIndex);

	glBindBuffer(GL_ARRAY_BUFFER, VBOIndex);
	glBufferData(GL_ARRAY_BUFFER, VerticesCount*sizeof(vertex_format), Vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, FaceCount*sizeof(v3u), Faces, GL_STATIC_DRAW);

	OpenGLSetUpVertexAttribs();

	glBindVertexArray(0);

	OpenGLCheckErrors();

	Result = VAOIndex;
	return Result;
}

internal void
OpenGLInitFramebuffers(opengl_state *State)
{
	{
		opengl_framebuffer *SceneFramebuffer = State->Framebuffers + Framebuffer_Scene;
		glGenFramebuffers(1, &SceneFramebuffer->FramebufferHandle);
		glBindFramebuffer(GL_FRAMEBUFFER, SceneFramebuffer->FramebufferHandle);

		SceneFramebuffer->Width = State->ScreenResolution.x;
		SceneFramebuffer->Height = State->ScreenResolution.y;

		// NOTE: Main color buffer.
		glGenTextures(1, &SceneFramebuffer->Texture);
		glBindTexture(GL_TEXTURE_2D, SceneFramebuffer->Texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SceneFramebuffer->Width, SceneFramebuffer->Height, 0, GL_RGBA, GL_UNSIGNED_INT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_2D, 0);

		// NOTE: Depth/stencil buffer.
		GLuint Renderbuffer;
		glGenRenderbuffers(1, &Renderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, Renderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SceneFramebuffer->Width, SceneFramebuffer->Height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, SceneFramebuffer->Texture, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, Renderbuffer);

		Assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		OpenGLCheckErrors();
	}
}

internal void
InitOpenGL(opengl_state *State, b32 ModernContext)
{
	opengl_info Info = OpenGLGetInfo(ModernContext);

	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	// glDepthFunc(GL_LEQUAL);

    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &State->MaxSamplersPerShader);

	for(u32 ShaderHandle = Shader_Header + 1;
		ShaderHandle < Shader_Count;
		++ShaderHandle)
	{
		OpenGLLoadShader(State, (shader_type) ShaderHandle);
	}

	OpenGLInitFramebuffers(State);

	glGenVertexArrays(1, &State->VertexArray);
	glGenBuffers(1, &State->ArrayBuffer);
	glBindVertexArray(State->VertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, State->ArrayBuffer);
	OpenGLSetUpVertexAttribs();
	glBindVertexArray(0);

	vertex_format BasicQuadVertices[4] =
	{
		{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {0xFFFFFFFF}},
		{{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {0xFFFFFFFF}},
		{{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f}, {0xFFFFFFFF}},
		{{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, {0xFFFFFFFF}},
	};
	v3u BasicQuadFaces[2] =
	{
		{0, 3, 2},
		{0, 2, 1},
	};
	State->BasicQuad = (GLuint) OpenGLLoadMesh(BasicQuadVertices, ArrayCount(BasicQuadVertices),
		BasicQuadFaces, ArrayCount(BasicQuadFaces));
	State->BasicQuadIndexCount = 3*ArrayCount(BasicQuadFaces);

	u32 CircleVertexCount = 24;
	Assert(CircleVertexCount > 2);
	vertex_format *BasicCircleVertices = PushArray(&State->Region, CircleVertexCount, vertex_format);
	u32 CircleFaceCount = CircleVertexCount - 2;
	v3u *BasicCircleFaces = PushArray(&State->Region, CircleFaceCount, v3u);

	f32 AngleStep = (2.0f*Pi32) / CircleVertexCount;
	for(u32 VertexIndex = 0;
		VertexIndex < CircleVertexCount;
		++VertexIndex)
	{
		vertex_format *Vertex = BasicCircleVertices + VertexIndex;
		f32 Angle = AngleStep*VertexIndex;
		Vertex->Position = {Cos(Angle), Sin(Angle)};
		Vertex->Color = 0xFFFFFFFF;
	}

	for(u32 FaceIndex = 0;
		FaceIndex < CircleFaceCount;
		++FaceIndex)
	{
		v3u *Face = BasicCircleFaces + FaceIndex;
		*Face = {0, FaceIndex + 1, FaceIndex + 2};
	}

	State->BasicCircle = (GLuint) OpenGLLoadMesh(BasicCircleVertices, CircleVertexCount,
		BasicCircleFaces, CircleFaceCount);
	State->BasicCircleIndexCount = 3*CircleFaceCount;

	OpenGLCheckErrors();
}

inline void
DrawCircle(opengl_state *State)
{
	glBindVertexArray(State->BasicCircle);
	glDrawElements(GL_TRIANGLES, State->BasicCircleIndexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

inline void
DrawQuad(opengl_state *State)
{
	glBindVertexArray(State->BasicQuad);
	glDrawElements(GL_TRIANGLES, State->BasicQuadIndexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

inline void
SetUniform(GLint UniformLocation, b32 Boolean)
{
	glUniform1i(UniformLocation, Boolean);
	OpenGLCheckErrors();
}

inline void
SetUniform(GLint UniformLocation, u32 UInt)
{
	glUniform1i(UniformLocation, UInt);
	OpenGLCheckErrors();
}

inline void
SetUniform(GLint UniformLocation, r32 Value)
{
	glUniform1f(UniformLocation, Value);
	OpenGLCheckErrors();
}

inline void
SetUniform(GLint UniformLocation, mat3 Matrix3)
{
	glUniformMatrix3fv(UniformLocation, 1, GL_FALSE, Matrix3.M);
	OpenGLCheckErrors();
}

inline void
SetUniform(GLint UniformLocation, mat4 Matrix4)
{
	glUniformMatrix4fv(UniformLocation, 1, GL_FALSE, Matrix4.M);
	OpenGLCheckErrors();
}

inline void
SetUniform(GLint UniformLocation, v2 Vector2)
{
	glUniform2f(UniformLocation, Vector2.x, Vector2.y);
	OpenGLCheckErrors();
}

inline void
SetUniform(GLint UniformLocation, v2i Vector2i)
{
	SetUniform(UniformLocation, V2((r32)Vector2i.x, (r32)Vector2i.y));
	OpenGLCheckErrors();
}

inline void
SetUniform(GLint UniformLocation, v3 Vector3)
{
	glUniform3fv(UniformLocation, 1, Vector3.E);
	OpenGLCheckErrors();
}

inline void
SetUniform(GLint UniformLocation, v4 Vector4)
{
	glUniform4fv(UniformLocation, 1, Vector4.E);
	OpenGLCheckErrors();
}

internal void
SetMatrices(opengl_state *State, opengl_shader_common *Shader,
	mat4 Projection, mat4 View)
{
	Assert(OpenGLUniformLocationIsValid(Shader->ProjectionLoc));
	Assert(OpenGLUniformLocationIsValid(Shader->ViewLoc));
	SetUniform(Shader->ProjectionLoc, Projection);
	SetUniform(Shader->ViewLoc, View);
}

internal void
BeginShader(opengl_state *State, opengl_shader_common *Shader)
{
	TIMED_FUNCTION();

	glUseProgram(Shader->ShaderHandle);
	State->CurrentShader = Shader;

	for(u32 SamplerIndex = 0;
		SamplerIndex < Shader->SamplerCount;
		++SamplerIndex)
	{
		SetUniform(Shader->Samplers[SamplerIndex], SamplerIndex);
	}
}

internal void
EndShader(opengl_state *State)
{
	glUseProgram(0);
	OpenGLCheckErrors();
	State->CurrentShader = 0;
}

inline void
BindTexture(GLenum Slot, GLint TextureHandle)
{
	glActiveTexture(Slot);
	glBindTexture(GL_TEXTURE_2D, TextureHandle);
}

internal void
OpenGLExecuteRenderCommands(opengl_state *State, command_buffer *CommandBuffer,	u32 Flags)
{
	TIMED_FUNCTION();

	opengl_shader_common *Shader = State->CurrentShader;
	Assert(Shader);

	u32 BytesRead = 0;
	while(BytesRead < CommandBuffer->Size)
	{
		render_entry_header *Header = (render_entry_header *)(CommandBuffer->Buffer + BytesRead);
		BytesRead += sizeof(render_entry_header);
		switch(Header->Type)
		{
			case RenderCommandType_render_entry_clear:
			{
				render_entry_clear *Entry = (render_entry_clear *)(Header + 1);

				glClearColor(Entry->Color.r, Entry->Color.g, Entry->Color.b, Entry->Color.a);
				glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

				BytesRead += sizeof(render_entry_clear);
				OpenGLCheckErrors();
			} break;

			case RenderCommandType_render_entry_quads:
			{
				TIMED_BLOCK("OpenGL Render Quads");

				render_entry_quads *Entry = (render_entry_quads *)(Header + 1);

				render_setup *Setup = &Entry->Setup;
				SetMatrices(State, Shader, Setup->Projection, Setup->View);
				u32 Width = Setup->ClipRectMax.x - Setup->ClipRectMin.x;
				u32 Height = Setup->ClipRectMax.y - Setup->ClipRectMin.y;
				glScissor(Setup->ClipRectMin.x, Setup->ClipRectMin.y,
					Width, Height);

				glBindVertexArray(State->VertexArray);
				glBindBuffer(GL_ARRAY_BUFFER, State->ArrayBuffer);
				glBufferData(GL_ARRAY_BUFFER, Entry->QuadCount*4*sizeof(vertex_format),
					CommandBuffer->QuadVertices + 4*Entry->FirstQuadIndex, GL_STREAM_DRAW);
				SetUniform(Shader->ModelLoc, IdentityMat4());
				SetUniform(Shader->TexTransformLoc, IdentityMat3());

				for(u32 QuadIndex = 0;
					QuadIndex < Entry->QuadCount;
					++QuadIndex)
				{
					u64 *TextureData = CommandBuffer->TextureData + Entry->FirstQuadIndex + QuadIndex;

					BindTexture(GL_TEXTURE0, (GLuint)*TextureData);
					glDrawArrays(GL_TRIANGLE_FAN, 4*QuadIndex, 4);
				}
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(0);

				BytesRead += sizeof(render_entry_quads);
				OpenGLCheckErrors();
			} break;

			InvalidDefaultCase;
		}
	}

	OpenGLCheckErrors();
}

internal void
OpenGLBindFramebuffer(opengl_framebuffer *Framebuffer, u32 Width = 0, u32 Height = 0)
{
	TIMED_FUNCTION();

	if(Framebuffer)
	{
		if(!Width)
		{
			Width = Framebuffer->Width;
		}
		if(!Height)
		{
			Height = Framebuffer->Height;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, Framebuffer->FramebufferHandle);
	}
	else
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	glViewport(0, 0, Width, Height);
	glScissor(0, 0, Width, Height);
}

internal void
OpenGLRender(opengl_state *State, command_buffer *CommandBuffer)
{
	TIMED_FUNCTION();

	CheckMemory(&State->Region);

	opengl_framebuffer *SceneFramebuffer = State->Framebuffers + Framebuffer_Scene;
	OpenGLBindFramebuffer(0, SceneFramebuffer->Width, SceneFramebuffer->Height);
	BeginShader(State, &State->Quads);
	OpenGLExecuteRenderCommands(State, CommandBuffer, 0);
	EndShader(State);

	OpenGLCheckErrors();
}
