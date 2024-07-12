#include "internal.h"
#include <_types/_uint32_t.h>
#include <stdint.h>

void oxidegl_platform_init();
void oxidegl_set_current_context(void *ctxid);
void oxidegl_swap_buffers(void *ctxid);
void *oxidegl_create_context(void *view, GLenum format, GLenum type,
                             GLenum depth_format, GLenum depth_type,
                             GLenum stencil_format, GLenum stencil_type);