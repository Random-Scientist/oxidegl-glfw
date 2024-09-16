#include "GLFW/glfw3.h"
#include "internal.h"
#include "oxidegl_ctx.h"

#include <math.h>
#include <stdio.h>
#include <unistd.h>

#define GL_BGRA 0x80E1
#define GL_UNSIGNED_INT_8_8_8_8_REV 0x8367
#define GL_DEPTH_COMPONENT 0x1902
#define GL_FLOAT 0x1406

static void makeContextCurrentOxideGL(_GLFWwindow *window) {
  if (window) {
    _glfw.oxidegl.make_context_current(window->context.oxidegl.ctx);
  } else {
    _glfw.oxidegl.make_context_current(NULL);
  }
  _glfwPlatformSetTls(&_glfw.contextSlot, window);
}

static void swapBuffersOxideGL(_GLFWwindow *window) {
  _glfw.oxidegl.swap_buffers(window->context.oxidegl.ctx);
}

static void swapIntervalOxideGL(int interval) {}

static int extensionSupportedOxideGL(const char *extension) {
  return GLFW_FALSE;
}

static GLFWglproc getProcAddressOxideGL(const char *procname) {
  GLFWproc symbol;
  assert(_glfw.oxidegl.handle);
  symbol = _glfwPlatformGetModuleSymbol(_glfw.oxidegl.handle, procname);
  return symbol;
}
static void destroyContextOxideGL(_GLFWwindow *window) {}

//////////////////////////////////////////////////////////////////////////
//////                       GLFW internal API                      //////
//////////////////////////////////////////////////////////////////////////

// Initialize OpenGL support
//
GLFWbool _glfwInitOxideGL(void) {
  if (_glfw.oxidegl.handle)
    return GLFW_TRUE;
  _glfw.oxidegl.handle = _glfwPlatformLoadModule("liboxidegl.dylib");
  assert(_glfw.oxidegl.handle);

  if (_glfw.oxidegl.handle == NULL) {
    _glfwInputError(GLFW_API_UNAVAILABLE,
                    "OxideGL: Failed to load liboxidegl.dylib");
    return GLFW_FALSE;
  }
  _glfw.oxidegl.platform_init = getProcAddressOxideGL("oxidegl_platform_init");
  assert(_glfw.oxidegl.platform_init);

  _glfw.oxidegl.make_context_current =
      (void (*)(void *))getProcAddressOxideGL("oxidegl_set_current_context");
  assert(_glfw.oxidegl.make_context_current);

  _glfw.oxidegl.swap_buffers =
      (void (*)(void *))getProcAddressOxideGL("oxidegl_swap_buffers");
  assert(_glfw.oxidegl.swap_buffers);

  _glfw.oxidegl.create_context =
      (OXIDEGLCREATECTXPROC)getProcAddressOxideGL("oxidegl_create_context");
  assert(_glfw.oxidegl.create_context);
  _glfw.oxidegl.platform_init();
  return GLFW_TRUE;
}

void _glfwTerminateOxideGL(void) {}

GLFWbool _glfwCreateContextOxideGL(_GLFWwindow *window,
                                   const _GLFWctxconfig *ctxconfig,
                                   const _GLFWfbconfig *fbconfig) {
  if (ctxconfig->client == GLFW_OPENGL_ES_API) {
    _glfwInputError(GLFW_API_UNAVAILABLE,
                    "OxideGL: oxideGL does not support GLES");
    return GLFW_FALSE;
  }

  if (ctxconfig->major != 4) {
    _glfwInputError(GLFW_VERSION_UNAVAILABLE,
                    "OxideGL supports OpenGL 4.6 Core only");
    return GLFW_FALSE;
  }

  if (ctxconfig->minor != 6) {
    _glfwInputError(GLFW_VERSION_UNAVAILABLE,
                    "OxideGL supports OpenGL 4.6 Core only");
    return GLFW_FALSE;
  }
  if (ctxconfig->share) {
    _glfwInputError(GLFW_API_UNAVAILABLE,
                    "OxideGL does not support shared contexts");
  }

  window->context.oxidegl.ctx =
      _glfw.oxidegl.create_context(window->ns.view, GL_BGRA, GL_UNSIGNED_INT,
                                   GL_DEPTH_COMPONENT, GL_FLOAT, 0, 0);

  if (window->context.oxidegl.ctx == 0) {
    _glfwInputError(GLFW_VERSION_UNAVAILABLE,
                    "Failed to create OxideGL Context");
    return GLFW_FALSE;
  }

  window->context.makeCurrent = makeContextCurrentOxideGL;
  window->context.swapBuffers = swapBuffersOxideGL;
  window->context.swapInterval = swapIntervalOxideGL;
  window->context.extensionSupported = extensionSupportedOxideGL;
  window->context.getProcAddress = getProcAddressOxideGL;
  window->context.destroy = destroyContextOxideGL;

  return GLFW_TRUE;
}

//////////////////////////////////////////////////////////////////////////
//////                        GLFW native API                       //////
//////////////////////////////////////////////////////////////////////////

GLFWAPI void *glfwGetOxideGLContext(GLFWwindow *handle) {
  _GLFWwindow *window = (_GLFWwindow *)handle;
  _GLFW_REQUIRE_INIT_OR_RETURN(nil);

  if (_glfw.platform.platformID != GLFW_PLATFORM_COCOA) {
    _glfwInputError(GLFW_PLATFORM_UNAVAILABLE,
                    "OxideGL: Platform not initialized");
    return nil;
  }

  if (window->context.source != GLFW_NATIVE_CONTEXT_API) {
    _glfwInputError(GLFW_NO_WINDOW_CONTEXT, NULL);
    return nil;
  }

  return &window->context.oxidegl.ctx;
}
