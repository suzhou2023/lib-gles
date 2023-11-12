

/**
 *  author : sz
 *  date : 2023/9/1
 *  description : 
 */

#include "fbo_util.h"

#include "gl_util.h"
#include "android_log.h"


void fboUtil_createFbo(uint32_t width, uint32_t height, GLuint *fbo, GLuint *tex_2d) {

    genTex2D(tex_2d);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glGenFramebuffers(1, fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, *fbo);
    glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D,
            *tex_2d,
            0
    );

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        LOGE("glCheckFramebufferStatus error.");
        return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}























