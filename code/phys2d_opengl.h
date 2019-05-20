#ifndef PHYS2D_OPENGL_H

#define GL_SHADING_LANGUAGE_VERSION      	 	0x8B8C

#define GL_FRAMEBUFFER_SRGB               		0x8DB9
#define GL_SRGB8_ALPHA8                   		0x8C43

#define GL_ARRAY_BUFFER                   0x8892

#define GL_STREAM_DRAW                    0x88E0
#define GL_STATIC_DRAW                    0x88E4
#define GL_DYNAMIC_DRAW                   0x88E8

#define GL_COMPILE_STATUS                 0x8B81
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_LINK_STATUS                    0x8B82

#define GL_ELEMENT_ARRAY_BUFFER           0x8893

#define GL_BGRA                           0x80E1
#define GL_UNSIGNED_INT_8_8_8_8           0x8035
#define GL_CONSTANT_ALPHA                 0x8003
#define GL_FRAMEBUFFER                    0x8D40
#define GL_RENDERBUFFER                   0x8D41
#define GL_DEPTH24_STENCIL8               0x88F0
#define GL_DEPTH_STENCIL_ATTACHMENT       0x821A
#define GL_FRAMEBUFFER_COMPLETE           0x8CD5
#define GL_CLAMP_TO_EDGE                  0x812F
#define GL_CLAMP_TO_BORDER                0x812D
#define GL_DEPTH_ATTACHMENT               0x8D00
#define GL_DEPTH_COMPONENT16              0x81A5
#define GL_DEPTH_COMPONENT24              0x81A6
#define GL_DEPTH_COMPONENT32              0x81A7

#define GL_RGBA32F                        0x8814
#define GL_RGB32F                         0x8815
#define GL_RGBA16F                        0x881A
#define GL_RGB16F                         0x881B

#define GL_TEXTURE0                       0x84C0
#define GL_TEXTURE1                       0x84C1
#define GL_TEXTURE2                       0x84C2
#define GL_TEXTURE3                       0x84C3
#define GL_TEXTURE4                       0x84C4
#define GL_TEXTURE5                       0x84C5
#define GL_TEXTURE6                       0x84C6
#define GL_TEXTURE7                       0x84C7
#define GL_TEXTURE8                       0x84C8
#define GL_TEXTURE9                       0x84C9
#define GL_TEXTURE10                      0x84CA
#define GL_TEXTURE11                      0x84CB
#define GL_TEXTURE12                      0x84CC
#define GL_TEXTURE13                      0x84CD
#define GL_TEXTURE14                      0x84CE
#define GL_TEXTURE15                      0x84CF
#define GL_TEXTURE16                      0x84D0
#define GL_TEXTURE17                      0x84D1
#define GL_TEXTURE18                      0x84D2
#define GL_TEXTURE19                      0x84D3
#define GL_TEXTURE20                      0x84D4
#define GL_TEXTURE21                      0x84D5
#define GL_TEXTURE22                      0x84D6
#define GL_TEXTURE23                      0x84D7
#define GL_TEXTURE24                      0x84D8
#define GL_TEXTURE25                      0x84D9
#define GL_TEXTURE26                      0x84DA
#define GL_TEXTURE27                      0x84DB
#define GL_TEXTURE28                      0x84DC
#define GL_TEXTURE29                      0x84DD
#define GL_TEXTURE30                      0x84DE
#define GL_TEXTURE31                      0x84DF

#define GL_COLOR_ATTACHMENT0              0x8CE0
#define GL_COLOR_ATTACHMENT1              0x8CE1
#define GL_COLOR_ATTACHMENT2              0x8CE2
#define GL_COLOR_ATTACHMENT3              0x8CE3
#define GL_COLOR_ATTACHMENT4              0x8CE4
#define GL_COLOR_ATTACHMENT5              0x8CE5
#define GL_COLOR_ATTACHMENT6              0x8CE6
#define GL_COLOR_ATTACHMENT7              0x8CE7
#define GL_COLOR_ATTACHMENT8              0x8CE8
#define GL_COLOR_ATTACHMENT9              0x8CE9
#define GL_COLOR_ATTACHMENT10             0x8CEA
#define GL_COLOR_ATTACHMENT11             0x8CEB
#define GL_COLOR_ATTACHMENT12             0x8CEC
#define GL_COLOR_ATTACHMENT13             0x8CED
#define GL_COLOR_ATTACHMENT14             0x8CEE
#define GL_COLOR_ATTACHMENT15             0x8CEF
#define GL_COLOR_ATTACHMENT16             0x8CF0
#define GL_COLOR_ATTACHMENT17             0x8CF1
#define GL_COLOR_ATTACHMENT18             0x8CF2
#define GL_COLOR_ATTACHMENT19             0x8CF3
#define GL_COLOR_ATTACHMENT20             0x8CF4
#define GL_COLOR_ATTACHMENT21             0x8CF5
#define GL_COLOR_ATTACHMENT22             0x8CF6
#define GL_COLOR_ATTACHMENT23             0x8CF7
#define GL_COLOR_ATTACHMENT24             0x8CF8
#define GL_COLOR_ATTACHMENT25             0x8CF9
#define GL_COLOR_ATTACHMENT26             0x8CFA
#define GL_COLOR_ATTACHMENT27             0x8CFB
#define GL_COLOR_ATTACHMENT28             0x8CFC
#define GL_COLOR_ATTACHMENT29             0x8CFD
#define GL_COLOR_ATTACHMENT30             0x8CFE
#define GL_COLOR_ATTACHMENT31             0x8CFF

#define GL_READ_FRAMEBUFFER               0x8CA8
#define GL_DRAW_FRAMEBUFFER               0x8CA9

#define GL_RG                             0x8227
#define GL_R16F                           0x822D

#define GL_SAMPLES_PASSED                 0x8914
#define GL_QUERY_RESULT                   0x8866
#define GL_QUERY_RESULT_AVAILABLE         0x8867
#define GL_QUERY_RESULT_NO_WAIT           0x9194

#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS 0x8B4D

#if PHYS2D_DEBUG
#define OpenGLCheckErrors() \
GLenum Error = GL_NO_ERROR; \
while((Error = glGetError()) != GL_NO_ERROR) \
{ \
	char Buffer[64]; \
	string OpenGLErrorString = FormatString(Buffer, ArrayCount(Buffer), "OpenGL Error! %d\n", Error); \
	OutputDebugString(OpenGLErrorString.Text); \
	InvalidCodePath; \
}

#else
#define OpenGLCheckErrors()
#endif

struct opengl_info
{
	b32 ModernContext;

	char *Vendor;
	char *Renderer;
	char *Version;
	char *ShadingLanguageVersion;
	char *Extensions;

	b32 GL_EXT_texture_sRGB;
	b32 GL_EXT_framebuffer_sRGB;
};

enum opengl_render_flags
{
	RenderFlag_RenderWireframes = 0x1,
};

struct opengl_shader_common
{
	GLuint ShaderHandle;

	GLint ProjectionLoc;
	GLint ViewLoc;
	GLint ModelLoc;

	GLint TransformLoc;
	GLint TexTransformLoc;

	u32 SamplerCount;
	GLint Samplers[32];
};

enum framebuffer_type
{
	Framebuffer_Default,
	Framebuffer_Scene,
	Framebuffer_Count,
};

struct opengl_framebuffer
{
	GLuint FramebufferHandle;
	GLuint Texture;
	u32 Width;
	u32 Height;
};

struct opengl_state
{
	memory_region Region;
	v2i ScreenResolution;

	GLint MaxSamplersPerShader;

	opengl_shader_common *CurrentShader;

	opengl_shader_common Quads;
	opengl_shader_common Text;

	GLuint BasicQuad;
	u32 BasicQuadIndexCount;
	GLuint BasicCircle;
	u32 BasicCircleIndexCount;

	GLuint VertexArray;
	GLuint ArrayBuffer;

	opengl_framebuffer Framebuffers[Framebuffer_Count];
};

inline b32
OpenGLUniformLocationIsValid(GLint UniformLoc)
{
	b32 Result = (UniformLoc != -1);
	return Result;
}

#define PHYS2D_OPENGL_H
#endif
