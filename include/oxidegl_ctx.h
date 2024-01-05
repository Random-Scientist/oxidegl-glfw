#include <_types/_uint32_t.h>
#include <stdint.h>
typedef uint32_t GLenum;

void oxidegl_set_current_context(uint32_t ctxid);
void oxidegl_swap_buffers(uint32_t ctxid);
uint32_t oxidegl_create_context(void *view, GLenum format, GLenum type,
                                GLenum depth_format, GLenum depth_type,
                                GLenum stencil_format, GLenum stencil_type);