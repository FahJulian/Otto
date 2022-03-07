#include "graphics_api.h"

#include <glew/glew.h>

#include "otto/debug/log.h"

namespace otto
{
	static void openglMessageCallback(uint32 source, uint32 type, uint32 id, uint32 severity, 
		int32 length, const char8* message, const void* userParam)
	{
		if (id == 2)	// Fragment shader recompile due to state change
			return;

		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         Log::error(message, "[severity: HIGH; id: ", id, "]"); return;
		case GL_DEBUG_SEVERITY_MEDIUM:       Log::warn(message, "[severity: MEDIUM; id: ", id, "]"); return;
		case GL_DEBUG_SEVERITY_LOW:          Log::info(message, "[severity: LOW; id: ", id, "]"); return;
		case GL_DEBUG_SEVERITY_NOTIFICATION: Log::trace(message, "[severity: NOTIFICATION; id: ", id, "]"); return;
		default: 
			Log::warn("Unknown opengl message severity level!"); 
			return;
		}
	}

    bool8 GraphicsAPI::init()
    {   
#ifdef OTTO_DEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(openglMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//glEnable(GL_DEPTH_TEST);

        return true;
    }

	void GraphicsAPI::setViewport(uint32 x, uint32 y, uint32 width, uint32 height)
	{
		glViewport(x, y, width, height);
	}

	void GraphicsAPI::setClearColor(const Color& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void GraphicsAPI::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GraphicsAPI::drawIndexed(const VertexArray& vertexArray, uint32 indexCount)
	{
		glDrawElements(GL_TRIANGLES, indexCount != 0 ? indexCount : vertexArray.getSize(), GL_UNSIGNED_INT, nullptr);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
		glUseProgram(0);
	}

} // namespace otto
