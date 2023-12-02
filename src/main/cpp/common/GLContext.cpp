/**
 *  author : suzhou
 *  date : 2023/9/3 
 *  description : 
 */


#include "GLContext.h"
#include <GLES2/gl2ext.h>
#include "asset_util.h"
#include "egl_util.h"
#include "gl_util.h"
#include "shader_util.h"


// 创建EGLSurface
bool GLContext::createEGLSurface(JNIEnv *env, jobject surface, jint index) {
    EGLBoolean ret = egl_createSurface(env, this, surface, index);
    if (ret == EGL_TRUE) {
        egl_makeCurrent(this, eglSurface[index]);
        return true;
    }
    return false;
}


// 创建着色器程序
int GLContext::createProgram(JNIEnv *env, jobject thiz, jstring v_name, jstring f_name, jint index) {
    // 转c字符串
    const char *v_name_str = env->GetStringUTFChars(v_name, nullptr);
    const char *f_name_str = env->GetStringUTFChars(f_name, nullptr);

    GLubyte *buf_v = asset_readFile(assetManager, v_name_str);
    GLubyte *buf_f = asset_readFile(assetManager, f_name_str);

    // 释放字符串
    env->ReleaseStringUTFChars(v_name, v_name_str);
    env->ReleaseStringUTFChars(f_name, f_name_str);

    GLuint program = shader_createProgram(reinterpret_cast<const char *>(buf_v),
                                          reinterpret_cast<const char *>(buf_f));
    // 释放字符数组
    delete buf_v;
    delete buf_f;

    if (program <= 0) return -1;
    this->program[index] = program;
    // 使用program
    glUseProgram(program);
    LOGI("GLContext::createProgram success.");
    return 0;
}


// 加载顶点坐标和纹理坐标
void GLContext::loadVertices() {
    float vertices[] = {
            // 前3个图元顶点坐标，后两个纹理坐标
            1.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top right
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom right
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom left
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
    };
    // 顶点属性索引
    GLuint indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
    };
    // vbo, ebo
    GLuint vbo, ebo;
    gl_genBuffer(&vbo, vertices, sizeof(vertices));
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * 4));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    gl_genIndexBuffer(&ebo, indices, sizeof(indices));
    this->vbo[0] = vbo;
    this->ebo[0] = ebo;
    LOGI("GLContext::loadVertices success.");
}


// 加载顶点坐标和纹理坐标
// vertices - 顶点属性数组
// vNum - 顶点个数
// lineLen - 每行的长度
// indices - 索引数组
// iNum - 索引个数
void GLContext::loadVertices2(float vertices[], int vNum, int lineLen, GLuint indices[], int iNum) {
    gl_genBuffer(&vbo[0], vertices, vNum * lineLen * sizeof(float));
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, lineLen * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, lineLen - 3, GL_FLOAT, GL_FALSE, lineLen * sizeof(float), (void *) (3 * 4));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    gl_genIndexBuffer(&ebo[0], indices, sizeof(GLuint) * iNum);
    LOGI("GLContext::loadVertices2 success.");
}


// 创建oes纹理
int GLContext::createOesTexture() {
    GLuint texture;
    glGenTextures(1, &texture);
    // todo: OES纹理似乎是永久绑定？
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, texture);
    // 着色器变量赋值
    glUseProgram(program[0]);
    // OES纹理单元(图层)赋值。
    // 不赋值的话，后面绘制的时候openGL会默认帮你激活图层0，我们再激活其它图层也是多余的。
    // 但是你一旦赋值，后面要激活的图层必须是这个值。
    // 配合FBO使用的时候，貌似只能设置为图层0。(目前测试结果，有待确认)
    glUniform1i(glGetUniformLocation(program[0], "oes_tex"), 0);

    oesTexture = texture;
    LOGI("GLContext::createOesTexture success.");
    return texture;
}


// 创建fbo
void GLContext::createFbo(int width, int height, int index) {
    GLuint fbo, texture;
    gl_genTex2D(&texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D,
            texture,
            0
    );
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        LOGE("Frame buffer not complete.");
        return;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    this->fbo[index] = fbo;
    this->fboTexture[index] = texture;
    LOGI("GLContext::createFbo success.");
}


// 设置渲染窗口尺寸
void GLContext::setWindowSize(jint window_w, jint window_h) {
    windowW = window_w;
    windowH = window_h;
}


// 矩阵配置：根据图形和窗口的尺寸配置合适的矩阵(顶点坐标变换矩阵、纹理坐标变换矩阵)
void GLContext::configMatrix(
        int program_index, // 程序索引
        int frame_w, // 帧宽度
        int frame_h, // 帧高度
        int window_w, // 窗口宽度
        int window_h, // 窗口高度
        int scale_type, // 缩放类型：1 - crop, 2 - fit
        bool rotate // 是否旋转
) {
    LOGI("GLContext::configMatrix frame_w = %d", frame_w);
    LOGI("GLContext::configMatrix frame_h = %d", frame_h);
    LOGI("GLContext::configMatrix window_w = %d", window_w);
    LOGI("GLContext::configMatrix window_h = %d", window_h);

    // 激活程序
    glUseProgram(program[program_index]);
    // 顶点坐标变换矩阵变量名
    GLint m_location = glGetUniformLocation(program[program_index], "v_matrix");

    // 比例合适才旋转
    bool really_rotate = ((frame_w > frame_h && window_w < window_h) || (frame_w < frame_h && window_w > window_h)) && rotate;
    if (really_rotate) {
        // 顺时针旋转90度矩阵，列主序
        float m_rotate90[16] = {
                0.0, -1.0, 0.0, 0.0,
                1.0, 0.0, 0.0, 0.0,
                0.0, 0.0, 1.0, 0.0,
                0.0, 0.0, 0.0, 1.0
        };
        glUniformMatrix4fv(m_location, 1, false, m_rotate90);
    } else {
        // 单位矩阵
        float m_identity[16] = {
                1.0, 0.0, 0.0, 0.0,
                0.0, 1.0, 0.0, 0.0,
                0.0, 0.0, 1.0, 0.0,
                0.0, 0.0, 0.0, 1.0
        };
        glUniformMatrix4fv(m_location, 1, false, m_identity);
    }

    // 纹理坐标变换矩阵变量名
    GLint tex_location = glGetUniformLocation(program[program_index], "tex_matrix");
    // 根据是否旋转，ratio_x和ratio_y取值不一样
    float ratio_x, ratio_y;
    if (!really_rotate) {
        ratio_x = window_w / (float) frame_w;
        ratio_y = window_h / (float) frame_h;
    } else {
        ratio_x = window_h / (float) frame_w;
        ratio_y = window_w / (float) frame_h;
    }

    // 先搞清楚一种情况，其它很类似
    if (ratio_x < ratio_y && scale_type == 1 || ratio_y < ratio_x && scale_type == 2) {
        float scale_x = ratio_x / ratio_y; // 缩放系数
        LOGI("GLContext::configMatrix scale_x = %f", scale_x);
        // 缩放+平移得到矩阵(列主序)
        float m_tex[9] = {
                scale_x, 0.0, 0.0,
                0.0, 1.0, 0.0,
                static_cast<float>((1.0 - scale_x) / 2), 0.0, 1.0,
        };
        glUniformMatrix3fv(tex_location, 1, false, m_tex);
    }

    if (ratio_y < ratio_x && scale_type == 1 || ratio_x < ratio_y && scale_type == 2) {
        float scale_y = ratio_y / ratio_x; // 缩放系数
        LOGI("GLContext::configMatrix scale_y = %f", scale_y);
        float m_tex[9] = {
                1.0, 0.0, 0.0,
                0.0, scale_y, 0.0,
                0.0, static_cast<float>((1.0 - scale_y) / 2), 1.0,
        };
        glUniformMatrix3fv(tex_location, 1, false, m_tex);
    }
}




























