/*@H
* File: win32_phys2d.cpp
* Author: Jesse Calvert
* Created: October 6, 2017, 17:42
* Last modified: November 27, 2017, 11:59
*/

#include "windows.h"
#include "Windowsx.h"
#include "GL/gl.h"

#include "phys2d.h"
#include "win32_phys2d.h"

global_variable b32 GlobalRunning;
global_variable b32 GlobalFullScreen;
global_variable WINDOWPLACEMENT GlobalWindowPosition = { sizeof(GlobalWindowPosition) };
global_variable v2i GlobalWindowSize;
global_variable HGLRC GlobalRenderingContext;
global_variable v2 GlobalMouseResetPoint;

global_variable char *ShaderFilenames[Shader_Count] = {};

typedef const char * WINAPI wgl_get_extensions_string_ext();
global_variable wgl_get_extensions_string_ext *wglGetExtensionsStringEXT;

typedef BOOL WINAPI wgl_choose_pixel_format_arb(HDC hdc,
                                                const int *piAttribIList,
                                                const FLOAT *pfAttribFList,
                                                UINT nMaxFormats,
                                                int *piFormats,
                                                UINT *nNumFormats);
global_variable wgl_choose_pixel_format_arb *wglChoosePixelFormatARB;

typedef BOOL WINAPI wgl_swap_interval_ext(int Interval);
global_variable wgl_swap_interval_ext *wglSwapIntervalEXT;

typedef HGLRC WINAPI wgl_create_context_attribs_arb(HDC DeviceContext, HGLRC ShareContext,
                                                    const int *Attribs);
global_variable wgl_create_context_attribs_arb *wglCreateContextAttribsARB;

typedef void gl_gen_buffers(GLsizei n, GLuint *buffers);
global_variable gl_gen_buffers *glGenBuffers;

typedef void gl_bind_buffer(GLenum target, GLuint buffer);
global_variable gl_bind_buffer *glBindBuffer;

typedef ptrdiff_t GLsizeiptr;
typedef void gl_buffer_data(GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage);
global_variable gl_buffer_data *glBufferData;

typedef GLuint gl_create_shader(GLenum shaderType);
global_variable gl_create_shader *glCreateShader;

typedef char GLchar;
typedef void gl_shader_source(GLuint shader, GLsizei count, const GLchar **string, const GLint *length);
global_variable gl_shader_source *glShaderSource;

typedef void gl_compile_shader(GLuint shader);
global_variable gl_compile_shader *glCompileShader;

typedef void gl_get_shader_iv(GLuint shader, GLenum pname, GLint *params);
global_variable gl_get_shader_iv *glGetShaderiv;

typedef void gl_get_shader_info_log(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
global_variable gl_get_shader_info_log *glGetShaderInfoLog;

typedef GLuint gl_create_program(void);
global_variable gl_create_program *glCreateProgram;

typedef void gl_attach_shader(GLuint program, GLuint shader);
global_variable gl_attach_shader *glAttachShader;

typedef void gl_link_program(GLuint program);
global_variable gl_link_program *glLinkProgram;

typedef void gl_get_program_iv(GLuint program, GLenum pname, GLint *params);
global_variable gl_get_program_iv *glGetProgramiv;

typedef void gl_get_program_info_log(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
global_variable gl_get_program_info_log *glGetProgramInfoLog;

typedef void gl_use_program(GLuint program);
global_variable gl_use_program *glUseProgram;

typedef void gl_delete_shader(GLuint shader);
global_variable gl_delete_shader *glDeleteShader;

typedef void gl_gen_vertex_arrays(GLsizei n, GLuint *arrays);
global_variable gl_gen_vertex_arrays *glGenVertexArrays;

typedef void gl_bind_vertex_array(GLuint array);
global_variable gl_bind_vertex_array *glBindVertexArray;

typedef void gl_vertex_attrib_pointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer);
global_variable gl_vertex_attrib_pointer *glVertexAttribPointer;

typedef void gl_enable_vertex_attrib_array(GLuint index);
global_variable gl_enable_vertex_attrib_array *glEnableVertexAttribArray;

typedef GLint gl_get_uniform_location(GLuint program, const GLchar *name);
global_variable gl_get_uniform_location *glGetUniformLocation;

typedef void gl_uniform_4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
global_variable gl_uniform_4f *glUniform4f;

typedef void gl_uniform_matrix_4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
global_variable gl_uniform_matrix_4fv *glUniformMatrix4fv;

typedef void gl_uniform_3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
global_variable gl_uniform_3f *glUniform3f;

typedef void gl_uniform_3fv(GLint location, GLsizei count, const GLfloat *value);
global_variable gl_uniform_3fv *glUniform3fv;

typedef void gl_uniform_1f(GLint location, GLfloat v0);
global_variable gl_uniform_1f *glUniform1f;

typedef void gl_uniform_1i(GLint location, GLint v0);
global_variable gl_uniform_1i *glUniform1i;

typedef void gl_generate_mipmap(GLuint Target);
global_variable gl_generate_mipmap *glGenerateMipmap;

typedef void gl_uniform_matrix_3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
global_variable gl_uniform_matrix_3fv *glUniformMatrix3fv;

typedef void gl_uniform_4fv(GLint location, GLsizei count, const GLfloat *value);
global_variable gl_uniform_4fv *glUniform4fv;

typedef void gl_gen_framebuffers(GLsizei n, GLuint *ids);
global_variable gl_gen_framebuffers *glGenFramebuffers;

typedef void gl_bind_framebuffer(GLenum target, GLuint framebuffer);
global_variable gl_bind_framebuffer *glBindFramebuffer;

typedef void gl_gen_renderbuffers(GLsizei n, GLuint * renderbuffers);
global_variable gl_gen_renderbuffers *glGenRenderbuffers;

typedef void gl_bind_renderbuffer(GLenum target, GLuint renderbuffer);
global_variable gl_bind_renderbuffer *glBindRenderbuffer;

typedef void gl_renderbuffer_storage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
global_variable gl_renderbuffer_storage *glRenderbufferStorage;

typedef void gl_framebuffer_texture2d(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
global_variable gl_framebuffer_texture2d *glFramebufferTexture2D;

typedef void gl_framebuffer_renderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
global_variable gl_framebuffer_renderbuffer *glFramebufferRenderbuffer;

typedef GLenum gl_check_framebuffer_status(GLenum target);
global_variable gl_check_framebuffer_status *glCheckFramebufferStatus;

typedef void gl_framebuffer_texture(GLenum target, GLenum attachment, GLuint texture, GLint level);
global_variable gl_framebuffer_texture *glFramebufferTexture;

typedef void gl_active_texture(GLenum texture);
global_variable gl_active_texture *glActiveTexture;

typedef void gl_uniform_1fv(GLint location, GLsizei count, const GLfloat *value);
global_variable gl_uniform_1fv *glUniform1fv;

typedef void gl_draw_buffers(GLsizei n, const GLenum *bufs);
global_variable gl_draw_buffers *glDrawBuffers;

typedef void gl_uniform_2f(GLint location, GLfloat v0, GLfloat v1);
global_variable gl_uniform_2f *glUniform2f;

typedef void gl_blit_framebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
global_variable gl_blit_framebuffer *glBlitFramebuffer;

typedef void gl_framebuffer_texture1d(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
global_variable gl_framebuffer_texture1d *glFramebufferTexture1D;

typedef void gl_gen_queries(GLsizei n, GLuint * ids);
global_variable gl_gen_queries *glGenQueries;

typedef void gl_begin_query(GLenum target, GLuint id);
global_variable gl_begin_query *glBeginQuery;

typedef void gl_end_query(GLenum target);
global_variable gl_end_query *glEndQuery;

typedef void gl_get_query_object_uiv(GLuint id, GLenum pname, GLuint * params);
global_variable gl_get_query_object_uiv *glGetQueryObjectuiv;

#define WGL_DRAW_TO_WINDOW_ARB                  0x2001
#define WGL_ACCELERATION_ARB                    0x2003
#define WGL_SUPPORT_OPENGL_ARB                  0x2010
#define WGL_DOUBLE_BUFFER_ARB                   0x2011
#define WGL_PIXEL_TYPE_ARB                      0x2013

#define WGL_TYPE_RGBA_ARB                       0x202B
#define WGL_FULL_ACCELERATION_ARB               0x2027

#define WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB        0x20A9

#define WGL_COLOR_BITS_ARB                      0x2014
#define WGL_DEPTH_BITS_ARB                      0x2022
#define WGL_STENCIL_BITS_ARB                    0x2023

#define WGL_CONTEXT_MAJOR_VERSION_ARB           0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB           0x2092
#define WGL_CONTEXT_FLAGS_ARB                   0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB            0x9126
#define WGL_CONTEXT_DEBUG_BIT_ARB               0x0001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002

#include "phys2d_opengl.cpp"
global_variable opengl_state *GlobalOpenGLState;

internal void
Win32ProcessKey(key_state *KeyState, b32 Pressed)
{
	if(KeyState->Pressed != Pressed)
	{
		KeyState->Pressed = Pressed;
		++KeyState->TransitionCount;
		if(Pressed)
		{
			KeyState->WasPressed = true;
		}
		else
		{
			KeyState->WasReleased = true;
		}

#if PHYS2D_DEBUG
		KeyState->DebugPressed = Pressed;
		++KeyState->DebugTransitionCount;
		if(Pressed)
		{
			KeyState->DebugWasPressed = true;
		}
		else
		{
			KeyState->DebugWasReleased = true;
		}
#endif
	}
}

internal void
Win32ToggleFullscreen(HWND Window)
{
    // NOTE: this follows Raymond Chen's prescription for fullscreen
    // toggling, see:
    // http://blogs.msdn.com/b/oldnewthing/archive/2010/04/12/9994016.aspx
    DWORD Style = GetWindowLong(Window, GWL_STYLE);
    if (Style & WS_OVERLAPPEDWINDOW)
    {
        MONITORINFO MonitorInfo = { sizeof(MonitorInfo) };
        if (GetWindowPlacement(Window, &GlobalWindowPosition) &&
            GetMonitorInfo(MonitorFromWindow(Window, MONITOR_DEFAULTTOPRIMARY), &MonitorInfo))
        {
            SetWindowLong(Window, GWL_STYLE, Style & ~WS_OVERLAPPEDWINDOW);
            SetWindowPos(Window, HWND_TOP,
                         MonitorInfo.rcMonitor.left, MonitorInfo.rcMonitor.top,
                         MonitorInfo.rcMonitor.right - MonitorInfo.rcMonitor.left,
                         MonitorInfo.rcMonitor.bottom - MonitorInfo.rcMonitor.top,
                         SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        }
    }
    else
    {
        SetWindowLong(Window, GWL_STYLE, Style | WS_OVERLAPPEDWINDOW);
        SetWindowPlacement(Window, &GlobalWindowPosition);
        SetWindowPos(Window, 0, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
                     SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
    }
}

internal void
Win32SetPixelFormat(HDC DeviceContext)
{
	s32 SuggestedPixelFormatIndex = 0;
	GLuint ExtendedPick = 0;

	if(wglChoosePixelFormatARB)
	{
		s32 AttributeList[] =
		{
		    WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
            WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
		    WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		    WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		    WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		    WGL_COLOR_BITS_ARB, 32,
		    WGL_DEPTH_BITS_ARB, 24,
		    WGL_STENCIL_BITS_ARB, 8,
            WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, GL_TRUE,
		    0,
		};

		wglChoosePixelFormatARB(DeviceContext, AttributeList, 0, 1, &SuggestedPixelFormatIndex, &ExtendedPick);
	}

	if(!ExtendedPick)
	{
		PIXELFORMATDESCRIPTOR DesiredPixelFormat = {};
		DesiredPixelFormat.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		DesiredPixelFormat.nVersion = 1;
		DesiredPixelFormat.dwFlags = PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER;
		DesiredPixelFormat.iPixelType = PFD_TYPE_RGBA;
		DesiredPixelFormat.cColorBits = 32;
		DesiredPixelFormat.cDepthBits = 24;
		DesiredPixelFormat.cStencilBits = 8;

		SuggestedPixelFormatIndex = ChoosePixelFormat(DeviceContext, &DesiredPixelFormat);
	}

	PIXELFORMATDESCRIPTOR ChosenPixelFormatDescription;
	DescribePixelFormat(DeviceContext, SuggestedPixelFormatIndex, sizeof(PIXELFORMATDESCRIPTOR), &ChosenPixelFormatDescription);

	SetPixelFormat(DeviceContext, SuggestedPixelFormatIndex, &ChosenPixelFormatDescription);
}

internal void
Win32LoadOpenGLExtensions()
{
	WNDCLASS WindowClass = {};
	WindowClass.style = CS_OWNDC;
	WindowClass.lpfnWndProc = DefWindowProcA;
	WindowClass.hInstance = GetModuleHandle(0);
	WindowClass.lpszClassName = "WGLExtensionsLoader";

	RegisterClass(&WindowClass);
	HWND DummyWindow =  CreateWindow("WGLExtensionsLoader",
			                         "WGLDummy",
			                         0,
			                         CW_USEDEFAULT,
			                         CW_USEDEFAULT,
			                         CW_USEDEFAULT,
			                         CW_USEDEFAULT,
			                         0,
			                         0,
			                         WindowClass.hInstance,
			                         0);

	if(DummyWindow)
	{
		HDC DummyDeviceContext = GetDC(DummyWindow);
		Win32SetPixelFormat(DummyDeviceContext);

		HGLRC DummyRenderingContext = wglCreateContext(DummyDeviceContext);
		b32 DummyContextCreated = wglMakeCurrent(DummyDeviceContext, DummyRenderingContext);
		if(DummyContextCreated)
		{
			// TODO: Load extensions.

			wglChoosePixelFormatARB = (wgl_choose_pixel_format_arb *)
				wglGetProcAddress("wglChoosePixelFormatARB)");
			wglCreateContextAttribsARB = (wgl_create_context_attribs_arb *)
				wglGetProcAddress("wglCreateContextAttribsARB");
			wglSwapIntervalEXT = (wgl_swap_interval_ext *)
				wglGetProcAddress("wglSwapIntervalEXT");
        	wglGetExtensionsStringEXT = (wgl_get_extensions_string_ext *)
        		wglGetProcAddress("wglGetExtensionsStringEXT");
        	if(wglGetExtensionsStringEXT)
        	{
        		char *Extensions = (char *)wglGetExtensionsStringEXT();

        		// TODO: Check and load more extensions.
        		glGenBuffers = (gl_gen_buffers *)wglGetProcAddress("glGenBuffers");
        		glBindBuffer = (gl_bind_buffer *)wglGetProcAddress("glBindBuffer");
        		glBufferData = (gl_buffer_data *)wglGetProcAddress("glBufferData");
        		glCreateShader = (gl_create_shader *)wglGetProcAddress("glCreateShader");
        		glShaderSource = (gl_shader_source *)wglGetProcAddress("glShaderSource");
        		glCompileShader = (gl_compile_shader *)wglGetProcAddress("glCompileShader");
        		glGetShaderiv = (gl_get_shader_iv *)wglGetProcAddress("glGetShaderiv");
        		glGetShaderInfoLog = (gl_get_shader_info_log *)wglGetProcAddress("glGetShaderInfoLog");
        		glCreateProgram = (gl_create_program *)wglGetProcAddress("glCreateProgram");
        		glAttachShader = (gl_attach_shader *)wglGetProcAddress("glAttachShader");
        		glLinkProgram = (gl_link_program *)wglGetProcAddress("glLinkProgram");
        		glGetProgramiv = (gl_get_program_iv *)wglGetProcAddress("glGetProgramiv");
        		glGetProgramInfoLog = (gl_get_program_info_log *)wglGetProcAddress("glGetProgramInfoLog");
        		glUseProgram = (gl_use_program *)wglGetProcAddress("glUseProgram");
        		glDeleteShader = (gl_delete_shader *)wglGetProcAddress("glDeleteShader");
        		glGenVertexArrays = (gl_gen_vertex_arrays *)wglGetProcAddress("glGenVertexArrays");
        		glBindVertexArray = (gl_bind_vertex_array *)wglGetProcAddress("glBindVertexArray");
        		glVertexAttribPointer = (gl_vertex_attrib_pointer *)wglGetProcAddress("glVertexAttribPointer");
        		glEnableVertexAttribArray = (gl_enable_vertex_attrib_array *)wglGetProcAddress("glEnableVertexAttribArray");
        		glGetUniformLocation = (gl_get_uniform_location *)wglGetProcAddress("glGetUniformLocation");
        		glUniform4f = (gl_uniform_4f *)wglGetProcAddress("glUniform4f");
        		glUniformMatrix4fv = (gl_uniform_matrix_4fv *)wglGetProcAddress("glUniformMatrix4fv");
        		glUniform3f = (gl_uniform_3f *)wglGetProcAddress("glUniform3f");
        		glUniform3fv = (gl_uniform_3fv *)wglGetProcAddress("glUniform3fv");
        		glUniform4fv = (gl_uniform_4fv *)wglGetProcAddress("glUniform4fv");
        		glUniform1f = (gl_uniform_1f *)wglGetProcAddress("glUniform1f");
        		glUniform1i = (gl_uniform_1i *)wglGetProcAddress("glUniform1i");
        		glGenerateMipmap = (gl_generate_mipmap *)wglGetProcAddress("glGenerateMipmap");
        		glUniformMatrix3fv = (gl_uniform_matrix_3fv *)wglGetProcAddress("glUniformMatrix3fv");
        		glGenFramebuffers = (gl_gen_framebuffers *)wglGetProcAddress("glGenFramebuffers");
        		glBindFramebuffer = (gl_bind_framebuffer *)wglGetProcAddress("glBindFramebuffer");
        		glGenRenderbuffers = (gl_gen_renderbuffers *)wglGetProcAddress("glGenRenderbuffers");
        		glBindRenderbuffer = (gl_bind_renderbuffer *)wglGetProcAddress("glBindRenderbuffer");
        		glRenderbufferStorage = (gl_renderbuffer_storage *)wglGetProcAddress("glRenderbufferStorage");
        		glFramebufferTexture2D = (gl_framebuffer_texture2d *)wglGetProcAddress("glFramebufferTexture2D");
        		glFramebufferRenderbuffer = (gl_framebuffer_renderbuffer *)wglGetProcAddress("glFramebufferRenderbuffer");
        		glCheckFramebufferStatus = (gl_check_framebuffer_status *)wglGetProcAddress("glCheckFramebufferStatus");
        		glFramebufferTexture = (gl_framebuffer_texture *)wglGetProcAddress("glFramebufferTexture");
        		glActiveTexture = (gl_active_texture *)wglGetProcAddress("glActiveTexture");
        		glUniform1fv = (gl_uniform_1fv *)wglGetProcAddress("glUniform1fv");
                glDrawBuffers = (gl_draw_buffers *)wglGetProcAddress("glDrawBuffers");
                glUniform2f = (gl_uniform_2f *)wglGetProcAddress("glUniform2f");
                glBlitFramebuffer = (gl_blit_framebuffer *)wglGetProcAddress("glBlitFramebuffer");
                glFramebufferTexture1D = (gl_framebuffer_texture1d *)wglGetProcAddress("glFramebufferTexture1D");
                glGenQueries = (gl_gen_queries *)wglGetProcAddress("glGenQueries");
                glBeginQuery = (gl_begin_query *)wglGetProcAddress("glBeginQuery");
                glEndQuery = (gl_end_query *)wglGetProcAddress("glEndQuery");
                glGetQueryObjectuiv = (gl_get_query_object_uiv *)wglGetProcAddress("glGetQueryObjectuiv");
        	}

			wglMakeCurrent(0, 0);
		}

		wglDeleteContext(DummyRenderingContext);
	    ReleaseDC(DummyWindow, DummyDeviceContext);
		DestroyWindow(DummyWindow);
	}
}

internal b32
Win32InitOpenGLWithExtensions(HDC DeviceContext)
{
	b32 Result = false;
	Win32SetPixelFormat(DeviceContext);

	// NOTE: Init modern OpenGL context.
	s32 ContextAttributeList[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
    	WGL_CONTEXT_MINOR_VERSION_ARB, 2,
    	WGL_CONTEXT_FLAGS_ARB, 0 // NOTE: Enable for testing WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB
#if PHYS2D_DEBUG
    	|WGL_CONTEXT_DEBUG_BIT_ARB
#endif
    	,
    	WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
		0,
	};

	if(wglCreateContextAttribsARB)
	{
		GlobalRenderingContext = wglCreateContextAttribsARB(DeviceContext, 0, ContextAttributeList);
		Result = wglMakeCurrent(DeviceContext, GlobalRenderingContext);
		if(wglSwapIntervalEXT)
		{
			wglSwapIntervalEXT(1);
		}

		InitOpenGL(GlobalOpenGLState, true);
	}

	return Result;
}

internal b32
Win32InitOpenGL(HDC DeviceContext)
{
	Win32LoadOpenGLExtensions();
	b32 Result = Win32InitOpenGLWithExtensions(DeviceContext);
	return Result;
}

internal v2i
Win32GetScreenResolution()
{
	v2i Result = {};

	WNDCLASS WindowClass = {};
	WindowClass.style = CS_OWNDC;
	WindowClass.lpfnWndProc = DefWindowProcA;
	WindowClass.hInstance = GetModuleHandle(0);
	WindowClass.lpszClassName = "ResolutionFinder";

	RegisterClass(&WindowClass);
	HWND DummyWindow =  CreateWindow("ResolutionFinder",
			                         "Phys2d",
			                         0,
			                         CW_USEDEFAULT,
			                         CW_USEDEFAULT,
			                         CW_USEDEFAULT,
			                         CW_USEDEFAULT,
			                         0,
			                         0,
			                         WindowClass.hInstance,
			                         0);

	if(DummyWindow)
	{
		HDC DeviceContext = GetDC(DummyWindow);

		Result.x = GetDeviceCaps(DeviceContext, HORZRES);
		Result.y = GetDeviceCaps(DeviceContext, VERTRES);
	}

	return Result;
}

internal void
Win32ResetOpenGLContext(HDC DeviceContext)
{
	// TODO: Is there a way to just resize the context?
	wglMakeCurrent(0, 0);
	wglDeleteContext(GlobalRenderingContext);

	Win32InitOpenGLWithExtensions(DeviceContext);
}

#define WIN32_PROCESS_KEY(VkCode, GameKey) \
case (VkCode): { \
key_state *KeyState = &Input->KeyStates[(GameKey)]; \
Win32ProcessKey(KeyState, IsDown);} break;

internal void
Win32ProcessKeyboardInput(MSG *Message, game_input *Input)
{
	b32 AltKeyWasDown = (Message->lParam & (1<<29));
	b32 WasDown = ((Message->lParam & (1<<30)) != 0);
    b32 IsDown = ((Message->lParam & (1<<31)) == 0);

    if(WasDown != IsDown)
	{
		switch(Message->wParam)
		{
			WIN32_PROCESS_KEY('Q', Key_Q);

			WIN32_PROCESS_KEY(VK_OEM_MINUS, Key_Minus);
			WIN32_PROCESS_KEY(VK_OEM_PLUS, Key_Plus);
			WIN32_PROCESS_KEY(VK_OEM_4, Key_OpenBracket);
			WIN32_PROCESS_KEY(VK_OEM_6, Key_CloseBracket);

			WIN32_PROCESS_KEY(VK_UP, Key_Up);
			WIN32_PROCESS_KEY(VK_DOWN, Key_Down);
			WIN32_PROCESS_KEY(VK_LEFT, Key_Left);
			WIN32_PROCESS_KEY(VK_RIGHT, Key_Right);
			WIN32_PROCESS_KEY(VK_SPACE, Key_Space);
			case VK_RETURN:
			{
				if(AltKeyWasDown && IsDown)
				{
					HWND Window = Message->hwnd;
					Win32ToggleFullscreen(Window);
					Win32ResetOpenGLContext(GetDC(Window));
				}
				else
				{
					key_state *KeyState = &Input->KeyStates[Key_Enter];
					Win32ProcessKey(KeyState, IsDown);
				}
			}

			WIN32_PROCESS_KEY(VK_F1, Key_F1);
			WIN32_PROCESS_KEY(VK_F2, Key_F2);
			WIN32_PROCESS_KEY(VK_F3, Key_F3);

			case VK_F4:
			{
				if(AltKeyWasDown)
				{
					GlobalRunning = false;
				}
				else
				{
					key_state *KeyState = &Input->KeyStates[Key_F4];
					Win32ProcessKey(KeyState, IsDown);
				}
			} break;

			WIN32_PROCESS_KEY(VK_F5, Key_F5);
			WIN32_PROCESS_KEY(VK_F6, Key_F6);
			WIN32_PROCESS_KEY(VK_F7, Key_F7);
			WIN32_PROCESS_KEY(VK_F8, Key_F8);
			WIN32_PROCESS_KEY(VK_F9, Key_F9);
			WIN32_PROCESS_KEY(VK_F10, Key_F10);
			WIN32_PROCESS_KEY(VK_F11, Key_F11);
			WIN32_PROCESS_KEY(VK_F12, Key_F12);
			WIN32_PROCESS_KEY(VK_ESCAPE, Key_Esc);
		}
	}
}

internal b32
Win32ProcessMouseInputs(MSG *Message, game_input *Input, v2i WindowDim)
{
	b32 SomethingWasProcessed = true;

	SetCursor(LoadCursor(0, IDC_ARROW));

	// TODO: Keep track of the MouseP where the buttons were clicked for
	// better accuracy.
	switch(Message->message)
	{
		case WM_LBUTTONDOWN:
		{
			Win32ProcessKey(&Input->KeyStates[Key_LeftClick], true);
		} break;

		case WM_RBUTTONDOWN:
		{
			Win32ProcessKey(&Input->KeyStates[Key_RightClick], true);
		} break;

		case WM_LBUTTONUP:
		{
			Win32ProcessKey(&Input->KeyStates[Key_LeftClick], false);
		} break;

		case WM_RBUTTONUP:
		{
			Win32ProcessKey(&Input->KeyStates[Key_RightClick], false);
		} break;

		case WM_MOUSEMOVE:
		{
			// NOTE: The mouse origin is in the bottom-left of the screen,
			// with y going up and x going right.
			LPARAM lParam = Message->lParam;
			v2i MousePInt = {GET_X_LPARAM(lParam), WindowDim.y - GET_Y_LPARAM(lParam)};
			v2 MouseP = V2((f32)MousePInt.x, (f32)MousePInt.y);
			Input->MouseP = MouseP;
		} break;

		default:
		{
			SomethingWasProcessed = false;
		}
	}

	return SomethingWasProcessed;
}

internal LRESULT CALLBACK
Win32WindowProc(HWND Window, UINT Message,
                WPARAM wParam, LPARAM lParam)
{
	LRESULT Result = 0;
    switch (Message)
    {
        case WM_CREATE:
        {
            // Initialize the window.
        } break;

        case WM_PAINT:
		{
        	// Paint the window's client area.
			PAINTSTRUCT Paint;
            HDC DeviceContext = BeginPaint(Window, &Paint);
            EndPaint(Window, &Paint);
        } break;

        case WM_SIZE:
        {
            // Set the size and position of the window.
        	s32 NewWidth = (s32)(lParam & 0xFFFF);
        	s32 NewHeight = (s32)(lParam >> 16);
        	GlobalWindowSize = {NewWidth, NewHeight};
			Win32ResetOpenGLContext(GetDC(Window));
        } break;

        case WM_DESTROY:
        {
            // Clean up window-specific data objects.
        	GlobalRunning = false;
        } break;

        case WM_CLOSE:
        {
        	GlobalRunning = false;
        } break;

        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        {
        	Assert(!"All the keyboard messages should have been handled already!");
        } break;

        case WM_SETCURSOR:
        {
			SetCursor(LoadCursor(0, IDC_ARROW));
        } break;

        default:
        {
            Result = DefWindowProc(Window, Message, wParam, lParam);
        }
    }

	return Result;
}

internal FILETIME
Win32GetLastWriteTime(char *Filename)
{
	WIN32_FILE_ATTRIBUTE_DATA FileData = {};
	GetFileAttributesEx(Filename, GetFileExInfoStandard, &FileData);
	return FileData.ftLastWriteTime;
}

internal b32
Win32FileHasBeenWritten(FILETIME LastWriteTime, char *Filename)
{
	FILETIME NewWriteFileTime = Win32GetLastWriteTime(Filename);
	ULARGE_INTEGER NewWriteTime = {};
	NewWriteTime.LowPart = NewWriteFileTime.dwLowDateTime;
	NewWriteTime.HighPart = NewWriteFileTime.dwHighDateTime;

	ULARGE_INTEGER OldWriteTime = {};
	OldWriteTime.LowPart = LastWriteTime.dwLowDateTime;
	OldWriteTime.HighPart = LastWriteTime.dwHighDateTime;

	b32 Result = (OldWriteTime.QuadPart != NewWriteTime.QuadPart);
	return Result;

}

internal b32
Win32NewGameDLLAvailable(game_code *Game, char *DLLName)
{
	b32 Result = Win32FileHasBeenWritten(Game->LastWriteTime, DLLName);
	return Result;
}

inline u32
Win32GetFileSize(char *Filename)
{
	WIN32_FILE_ATTRIBUTE_DATA FileData = {};
	GetFileAttributesEx(Filename, GetFileExInfoStandard, &FileData);
	Assert(FileData.nFileSizeHigh == 0);
	u32 FileSize = FileData.nFileSizeLow;
	return FileSize;
}

internal b32
Win32DLLLockIsActive(char *LockName)
{
	b32 Result = (Win32GetFileSize(LockName) == 0);
	return Result;
}

internal void
Win32UnloadGameDLL(game_code *Game)
{
	Game->GameUpdate = 0;
	Game->GameRender = 0;
	FreeLibrary(Game->Handle);
	Game->LastWriteTime = {};
}

internal game_code
Win32LoadGameCode(char *DLLName, char *DLLNameTemp, char *LockName)
{
	game_code Game = {};

	if(Win32DLLLockIsActive(LockName))
	{
		b32 FileCopied = CopyFile(DLLName, DLLNameTemp, 0);
		if(FileCopied)
		{
			Game.Handle = LoadLibraryEx(DLLNameTemp, 0, 0);
			if(Game.Handle)
			{
				Game.GameUpdate = (game_update *)GetProcAddress(Game.Handle, "GameUpdate");
				Game.GameRender = (game_render *)GetProcAddress(Game.Handle, "GameRender");
#if PHYS2D_DEBUG
				Game.DEBUGFrameEnd = (debug_frame_end *)GetProcAddress(Game.Handle, "DebugFrameEnd");
#endif
				Game.LastWriteTime = Win32GetLastWriteTime(DLLName);
			}
			else
			{
				OutputDebugString("Failed to load temporary DLL!\n");
			}
		}
		else
		{
			OutputDebugString("Failed to copy DLL!\n");
		}
	}

	return Game;
}

internal u8 *
Win32OpenFileOfSize(memory_region *Region, u32 Size, char *Filename)
{
	u8 *Result = 0;

	HANDLE FileHandle = CreateFile(Filename, GENERIC_READ, FILE_SHARE_READ, 0,
	                               OPEN_EXISTING, 0, 0);
	if(FileHandle != INVALID_HANDLE_VALUE)
	{
		DWORD BytesRead = 0;
		Result = PushSize(Region, Size);
		b32 FileRead = ReadFile(FileHandle, Result, Size, &BytesRead, 0);
		Assert(FileRead);
		CloseHandle(FileHandle);
	}
	else
	{
		OutputDebugString("Could not open file!\n");
	}

	return Result;
}

internal
LOAD_ENTIRE_TEXT_FILE_AND_NULL_TERMINATE(Win32LoadEntireTextFileAndNullTerminate)
{
	u32 FileSize = Win32GetFileSize(Filename) + 1;
	char *Result = (char *)Win32OpenFileOfSize(Region, FileSize, Filename);
	if(Result)
	{
		Result[FileSize - 1] = 0;
	}

	return Result;
}

internal
LOAD_ENTIRE_FILE(Win32LoadEntireFile)
{
	u32 FileSize = Win32GetFileSize(Filename);
	u8 *Result = Win32OpenFileOfSize(Region, FileSize, Filename);
	return Result;
}

internal
LOAD_MESH(Win32LoadMesh)
{
	u64 Result = 0;
	if(GlobalOpenGLState)
	{
		Result = OpenGLLoadMesh(Vertices, VertexCount, Faces, FaceCount);
	}

	return Result;
}

internal
LOAD_TEXTURE(Win32LoadTexture)
{
	u64 Result = 0;
	if(GlobalOpenGLState)
	{
		Result = OpenGLLoadTexture(Pixels, Width, Height);
	}

	return Result;
}

internal
LOAD_FONT_TEXTURE(Win32LoadFontTexture)
{
	u64 Result = 0;
	if(GlobalOpenGLState)
	{
		Result = OpenGLLoadFontTexture(Pixels, Width, Height);
	}

	return Result;
}

internal void
Win32InitalizeShaderFilenames(win32_hotloader *Hotloader)
{
	ShaderFilenames[Shader_Header] = "Shaders/phys2d_header.glsl";
	ShaderFilenames[Shader_Quads] = "Shaders/phys2d_quads.glsl";

	for(u32 ShaderIndex = 0;
		ShaderIndex < Shader_Count;
		++ShaderIndex)
	{
		if(ShaderFilenames[ShaderIndex])
		{
			Hotloader->LastShaderWriteTimes[ShaderIndex] =
				Win32GetLastWriteTime(ShaderFilenames[ShaderIndex]);
		}
	}
}

internal void
Win32CheckFilesForHotload(win32_hotloader *Hotloader)
{
	file_hotload_tag *FilesToHotload = &Hotloader->FilesToHotload;

	b32 AllShadersNeedReloading = false;
	if(Win32FileHasBeenWritten(Hotloader->LastShaderWriteTimes[Shader_Header], ShaderFilenames[Shader_Header]))
	{
		AllShadersNeedReloading = true;
	}
	for(u32 ShaderIndex = Shader_Header + 1;
		ShaderIndex < Shader_Count;
		++ShaderIndex)
	{
		if(ShaderFilenames[ShaderIndex])
		{
			if(AllShadersNeedReloading || Win32FileHasBeenWritten(Hotloader->LastShaderWriteTimes[ShaderIndex], ShaderFilenames[ShaderIndex]))
			{
				Hotloader->LastShaderWriteTimes[ShaderIndex] = Win32GetLastWriteTime(ShaderFilenames[ShaderIndex]);
				FilesToHotload->ReloadShader[ShaderIndex] = true;
				FilesToHotload->ShaderNeedsHotloading = true;
			}
		}
	}
}

internal void
Win32ReloadShaders(win32_hotloader *Hotloader)
{
	file_hotload_tag *FilesToHotload = &Hotloader->FilesToHotload;

	b32 NoFailures = true;
	for(u32 ShaderIndex = Shader_Header + 1;
		ShaderIndex < Shader_Count;
		++ShaderIndex)
	{
		if(FilesToHotload->ReloadShader[ShaderIndex])
		{
			b32 Success = OpenGLLoadShader(GlobalOpenGLState, (shader_type) ShaderIndex);
			if(Success)
			{
				FilesToHotload->ReloadShader[ShaderIndex] = false;
			}
			else
			{
				NoFailures = false;
			}
		}
	}

	if(NoFailures)
	{
		FilesToHotload->ShaderNeedsHotloading = false;
	}
}

internal r32
Win32GetTimeSince(LARGE_INTEGER CountsPerSecond, LARGE_INTEGER OldTime)
{
	LARGE_INTEGER CurrentTime;
	QueryPerformanceCounter(&CurrentTime);
	u64 CountsSinceLast = CurrentTime.QuadPart - OldTime.QuadPart;

	r32 Result = (r32)CountsSinceLast / (r32)CountsPerSecond.QuadPart;
	return Result;
}

internal
OPEN_ASSET_PACK_FILE(Win32OpenAssetPackFile)
{
    platform_file Result = {};

    HANDLE FileHandle = CreateFile("asset_pack.sla", GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    if(FileHandle == INVALID_HANDLE_VALUE)
    {
        // TODO: logging
        InvalidCodePath;
    }
    else
    {
        Result.Handle_ = FileHandle;
    }

    return Result;
}

internal
READ_FROM_FILE(Win32ReadFromFile)
{
    u8 *Result = 0;
    if(Length)
    {
        Result = PushSize(Region, Length);
        HANDLE FileHandle = File->Handle_;
        OVERLAPPED Overlapped = {};
        Overlapped.Offset = Offset;
        Overlapped.OffsetHigh = 0;

        DWORD BytesRead = 0;
        b32 Error = ReadFile(FileHandle, Result, Length, &BytesRead, &Overlapped);
        Assert(BytesRead == Length);
    }

    return Result;
}

internal
ALLOCATE_MEMORY(Win32AllocateMemory)
{
	// NOTE: we don't want the platform_memory_block to change cache line alignment
	Assert(sizeof(platform_memory_block) == 64);

	umm PageSize = 4096; // TODO: Query system for this.
	umm TotalSize = Size + sizeof(platform_memory_block);
	umm BaseOffset = sizeof(platform_memory_block);
	umm ProtectOffset = 0;

	if(Flags & RegionFlag_UnderflowChecking)
	{
		BaseOffset = 2*PageSize;
		TotalSize = Size + 2*PageSize;
		ProtectOffset = PageSize;
	}
	else if(Flags & RegionFlag_OverflowChecking)
	{
		umm InflatedSize = AlignPow2(Size, PageSize);
		TotalSize = InflatedSize + 2*PageSize;
		BaseOffset = PageSize + InflatedSize - Size;
		ProtectOffset = PageSize + InflatedSize;
	}

	platform_memory_block *NewBlock = (platform_memory_block *)VirtualAlloc(0, TotalSize,
		MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
	Assert(NewBlock);
	NewBlock->Base = (u8 *)NewBlock + BaseOffset;
	NewBlock->Size = Size;
	Assert(NewBlock->Used == 0);
	Assert(NewBlock->BlockPrev == 0);

	if(ProtectOffset)
	{
		DWORD OldProtect = 0;
		VirtualProtect((u8 *)NewBlock + ProtectOffset, PageSize, PAGE_NOACCESS, &OldProtect);
	}

	return NewBlock;
}

internal
DEALLOCATE_MEMORY(Win32DeallocateMemory)
{
	if(Block)
	{
		BOOL Success = VirtualFree(Block, 0, MEM_RELEASE);
		Assert(Success);
	}
}

#if PHYS2D_DEBUG
global_variable debug_table GlobalDebugTable_;
debug_table *GlobalDebugTable = &GlobalDebugTable_;
#endif

int CALLBACK WinMain(HINSTANCE Instance,
                     HINSTANCE PreviousInstance,
                     LPSTR CommandLine,
                     s32 CommandShow)
{
	LARGE_INTEGER CountsPerSecond;
	QueryPerformanceFrequency(&CountsPerSecond);

	win32_hotloader Hotloader = {};
	Win32InitalizeShaderFilenames(&Hotloader);

	WNDCLASS WindowClass = {};
	WindowClass.style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
	WindowClass.lpfnWndProc = Win32WindowProc;
	WindowClass.hInstance = Instance;
    WindowClass.hCursor = LoadCursor(0, IDC_ARROW);;
    WindowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WindowClass.lpszClassName = "Phys2dClass";

	v2i TotalScreenResolution = Win32GetScreenResolution();
	GlobalMouseResetPoint = V2(0.5f, 0.5f);

	RegisterClass(&WindowClass);
	HWND Window =  CreateWindowExA(0,
							 	WindowClass.lpszClassName,
		                        "Phys2d",
		                        WS_OVERLAPPEDWINDOW,
		                        CW_USEDEFAULT,
		                        CW_USEDEFAULT,
		                        TotalScreenResolution.x,
		                        TotalScreenResolution.y,
		                        0,
		                        0,
		                        Instance,
		                        0);

	if(Window)
	{
		Win32ToggleFullscreen(Window);
		ShowWindow(Window, SW_SHOW);
		GlobalWindowSize = TotalScreenResolution;

		LPVOID BaseAddress = 0;
#if PHYS2D_DEBUG
		BaseAddress = (LPVOID)Terabytes(2);
#endif

		game_memory GameMemory = {};
		command_buffer *CommandBuffer = &GameMemory.CommandBuffer;
        CommandBuffer->MaxSize = Megabytes(32);
        CommandBuffer->MaxQuads = 65536;
        umm VertexSize = CommandBuffer->MaxQuads*4*sizeof(vertex_format);
        umm TextureDataSize = CommandBuffer->MaxQuads*sizeof(u64);

		umm TotalStorageSize = CommandBuffer->MaxSize +
			VertexSize + TextureDataSize;

        u8 *Memory = (u8*)VirtualAlloc(BaseAddress, TotalStorageSize,
            MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);

        CommandBuffer->Buffer = Memory;
        Memory += CommandBuffer->MaxSize;
        CommandBuffer->QuadVertices = (vertex_format *)Memory;
        Memory += VertexSize;
        CommandBuffer->TextureData = (u64 *)Memory;
        Memory += TextureDataSize;

		// TODO: Generate these in a real way.
		char *DLLName = "w:\\phys2d\\build\\phys2d.dll";
		char *DLLNameTemp = "w:\\phys2d\\build\\phys2d_temp.dll";
		char *LockName = "w:\\phys2d\\build\\Lock.tmp";
		game_code Game = Win32LoadGameCode(DLLName, DLLNameTemp, LockName);

		platform Win32Platform = {};
		Platform = &Win32Platform;

		Platform->LoadEntireFile = Win32LoadEntireFile;
		Platform->LoadEntireTextFileAndNullTerminate = Win32LoadEntireTextFileAndNullTerminate;
		Platform->LoadMesh = Win32LoadMesh;
		Platform->LoadTexture = Win32LoadTexture;
		Platform->LoadFontTexture = Win32LoadFontTexture;
        Platform->OpenAssetPackFile = Win32OpenAssetPackFile;
        Platform->ReadFromFile = Win32ReadFromFile;
        Platform->AllocateMemory = Win32AllocateMemory;
        Platform->DeallocateMemory = Win32DeallocateMemory;

#if PHYS2D_DEBUG
        GameMemory.DebugTable = GlobalDebugTable;
		DEBUGSetRecording(true);
#endif

		GameMemory.WindowResolution = GlobalWindowSize;
		GameMemory.Platform = Platform;

        GlobalOpenGLState = BootstrapPushStruct(opengl_state, Region);
		GlobalOpenGLState->ScreenResolution = GlobalWindowSize;

		HDC DeviceContext = GetDC(Window);
		b32 OpenGLInitialized = Win32InitOpenGL(DeviceContext);

		if(OpenGLInitialized)
		{
			u8 EmptyPixels = 0xFF;
			CommandBuffer->WhiteTexture = Win32LoadFontTexture(&EmptyPixels, 1, 1);

			s32 RefreshRate = GetDeviceCaps(DeviceContext, VREFRESH);
			ReleaseDC(Window, DeviceContext);
			r32 dtForFrameTarget = 1.0f / (r32)RefreshRate;

			TIMECAPS TimeCaps = {};
			timeGetDevCaps(&TimeCaps, sizeof(TimeCaps));
			timeBeginPeriod(TimeCaps.wPeriodMin);

			game_input Input = {};
			Input.Simdt = 1.0f / 60.0f;
			u32 MaxFrameSkip = 5;
			r32 Accumulator = 0.0f;

			SetCapture(Window);
			b32 CursorIsVisible = true;
			ShowCursor(CursorIsVisible);

			LARGE_INTEGER LastFrameEndTime;
			QueryPerformanceCounter(&LastFrameEndTime);
			r32 LastFrameFPS = 1.0f / dtForFrameTarget;
			Input.LastFramedt_ = 1.0f / LastFrameFPS;

			GlobalRunning = true;
			u64 FrameCycleStart = __rdtsc();
			while(GlobalRunning)
			{
				{DEBUG_DATA_BLOCK("Platform");
					DEBUG_VALUE(&GlobalRunning);
				}

#if PHYS2D_DEBUG
				BEGIN_BLOCK("Check Hotloads");

				Win32CheckFilesForHotload(&Hotloader);
				if(Hotloader.FilesToHotload.ShaderNeedsHotloading)
				{
					Win32ReloadShaders(&Hotloader);
				}

				END_BLOCK();
#endif
				//
				//
				//

				u32 LoopCount = 0;
				while((Accumulator > Input.Simdt) && (LoopCount < MaxFrameSkip))
				{
					BEGIN_BLOCK("Read Input");

					for(u32 KeyIndex = 0;
					    KeyIndex < Key_Count;
					    ++KeyIndex)
					{
						key_state *KeyState = Input.KeyStates + KeyIndex;
						KeyState->TransitionCount = 0;
						KeyState->WasPressed = false;
						KeyState->WasReleased = false;
					}

					MSG Message;
					while(PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
					{
						TranslateMessage(&Message);
						if((Message.message == WM_KEYDOWN) ||
						   (Message.message == WM_KEYUP) ||
						   (Message.message == WM_SYSKEYDOWN) ||
						   (Message.message == WM_SYSKEYUP))
						{
		        			Win32ProcessKeyboardInput(&Message, &Input);
						}
						else if(!Win32ProcessMouseInputs(&Message, &Input, GlobalWindowSize))
						{
							DispatchMessage(&Message);
						}
					}

					END_BLOCK();

					if(Game.GameUpdate)
					{
						TIMED_BLOCK("Update");

						Game.GameUpdate(&GameMemory, &Input);
					}
					Accumulator -= Input.Simdt;
					++LoopCount;
				}

				if(Game.GameRender)
				{
					TIMED_BLOCK("Render");
					f32 StateBlend = Accumulator / Input.Simdt;
					Game.GameRender(&GameMemory, &Input, StateBlend);
				}

				//
				//
				//

#if PHYS2D_DEBUG
				BEGIN_BLOCK("Debug Frame End");

				b32 ExecutableNeedsReloading = false;
				if(Win32NewGameDLLAvailable(&Game, DLLName))
				{
					if(Win32DLLLockIsActive(LockName))
					{
						DEBUGSetRecording(false);
						ExecutableNeedsReloading = true;
					}
				}

				if(Game.DEBUGFrameEnd)
				{
					Game.DEBUGFrameEnd(&GameMemory, &Input, CountsPerSecond.QuadPart);

					for(u32 KeyIndex = 0;
					    KeyIndex < Key_Count;
					    ++KeyIndex)
					{
						key_state *KeyState = Input.KeyStates + KeyIndex;
						KeyState->DebugTransitionCount = 0;
						KeyState->DebugWasPressed = false;
						KeyState->DebugWasReleased = false;
					}
				}

				if(ExecutableNeedsReloading)
				{
					Win32UnloadGameDLL(&Game);
					Game = Win32LoadGameCode(DLLName, DLLNameTemp, LockName);
					DEBUGSetRecording(true);
					ExecutableNeedsReloading = false;
				}

				END_BLOCK();
#endif

				//
				//
				//

				BEGIN_BLOCK("OpenGL Render");
				OpenGLRender(GlobalOpenGLState, CommandBuffer);

				END_BLOCK();

				BEGIN_BLOCK("Wait for vBlank");

				DeviceContext = GetDC(Window);
				SwapBuffers(DeviceContext);
				ReleaseDC(Window, DeviceContext);

				glFinish();

				CommandBuffer->Size = 0;
				CommandBuffer->QuadCount = 0;

				END_BLOCK();

				//
				//
				//

				BEGIN_BLOCK("Frame Wait");

				r32 FrameTime = Win32GetTimeSince(CountsPerSecond, LastFrameEndTime);
#if 0

				if(FrameTime < dtForFrameTarget)
				{
					u32 MillisecondsToWait = (u32)((dtForFrameTarget - FrameTime)*1000);
					MillisecondsToWait -= (MillisecondsToWait % TimeCaps.wPeriodMin);
					// TODO: Sleep has problems, just rely on vsync
					Sleep(MillisecondsToWait);

					FrameTime = Win32GetTimeSince(CountsPerSecond, LastFrameEndTime);
				}
#endif

				LastFrameFPS = (1.0f / FrameTime);
				Input.LastFramedt_ = FrameTime;
				LARGE_INTEGER CurrentTimeCounts;
				QueryPerformanceCounter(&CurrentTimeCounts);
				LastFrameEndTime = CurrentTimeCounts;
				Accumulator += FrameTime;

				END_BLOCK();

				//
				//
				//

				u64 LastFrameCycleStart = FrameCycleStart;
				FrameCycleStart = __rdtsc();
				u64 FrameCycles = FrameCycleStart - LastFrameCycleStart;

				FRAME_MARK(FrameCycles);
			}

			timeEndPeriod(TimeCaps.wPeriodMin);
		}
	}

	return 0;
}
