#include "internal.h"
#include <stdint.h>

void oxidegl_platform_init();
void oxidegl_set_current_context(void *ctx);
void oxidegl_swap_buffers(void *ctx);
void *oxidegl_create_context(void *view, GLenum format, GLenum type,
                             GLenum depth_format, GLenum depth_type,
                             GLenum stencil_format, GLenum stencil_type);
void oxidegl_destroy_context(void *ctx);