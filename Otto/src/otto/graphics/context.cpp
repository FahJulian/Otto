#include "context.h"

#include <glew/glew.h>
#include <glew/wglew.h>

#include "otto/debug/log/log.h"

namespace otto
{
    static HDC sDeviceContext;
    static HGLRC sContext;

    bool8 Context::init(ptr64 windowHandle)
    {
        sDeviceContext = GetDC(reinterpret_cast<HWND>(windowHandle));
        if (sDeviceContext == NULL)
        {
            Log::error("Error fetching device context");
            return false;
        }

        PIXELFORMATDESCRIPTOR pfd =
        {
            sizeof(PIXELFORMATDESCRIPTOR),
            1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
            PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
            32,                   // Colordepth of the framebuffer.
            0, 0, 0, 0, 0, 0,
            0,
            0,
            0,
            0, 0, 0, 0,
            24,                   // Number of bits for the depthbuffer
            8,                    // Number of bits for the stencilbuffer
            0,                    // Number of Aux buffers in the framebuffer.
            PFD_MAIN_PLANE,
            0,
            0, 0, 0
        };

        INT pixelFormat = ChoosePixelFormat(sDeviceContext, &pfd);
        if (pixelFormat == 0)
        {
            Log::error("Error choosing pixel format");
            return false;
        }

        SetPixelFormat(sDeviceContext, pixelFormat, &pfd);

        HGLRC dummyContext = wglCreateContext(sDeviceContext);
        wglMakeCurrent(sDeviceContext, dummyContext);

        const int32 attribList[] =
        {
            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
            WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
            WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
            WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
            WGL_COLOR_BITS_ARB, 32,
            WGL_DEPTH_BITS_ARB, 24,
            WGL_STENCIL_BITS_ARB, 8,
            0,
        };

        if (glewInit() != GLEW_OK)
            return false;

        UINT numFormats;
        if (!wglChoosePixelFormatARB(sDeviceContext, attribList, NULL, 1, &pixelFormat, &numFormats))
        {
            Log::error("Error choosing pixel format");
            return false;
        }

        SetPixelFormat(sDeviceContext, pixelFormat, &pfd);

        sContext = wglCreateContext(sDeviceContext);
        wglMakeCurrent(sDeviceContext, sContext);

        wglDeleteContext(dummyContext);

        return true;
    }

    void Context::swapBuffers()
    {
        SwapBuffers(sDeviceContext);
    }

} // namespace otto
