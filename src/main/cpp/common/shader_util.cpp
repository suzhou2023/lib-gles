/**
 *  author : suzhou
 *  date : 2023/7/16 
 *  description : 
 */

#include "shader_util.h"
#include "android_log.h"


GLuint shader_loadShader(const char *source, GLuint type) {
    // 创建shader
    GLuint shader = glCreateShader(type);
    if (shader == 0) {
        LOGE("glCreateShader failed, type = %d", type);
        return -1;
    }
    // 加载shader
    // 代码长度传0则读到字符串结尾
    glShaderSource(shader, 1, &source, 0);
    // 编译shader
    glCompileShader(shader);
    // 打印编译错误信息
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status <= 0) {
        LOGE("Shader compile failed, type = %d", type);
        LOGE("Error shader source:\n%s", source);
        auto *infoLog = new GLchar[2048];
        GLsizei length;
        glGetShaderInfoLog(shader, 2048, &length, infoLog);
        LOGE("Shader compile info log:\n%s", infoLog);
        return -1;
    }
    LOGI("Shader compile success, type = %d", type);
    return shader;
}

GLuint shader_createProgram(const char *source_vShader, const char *source_fShader) {
    GLuint vShader = shader_loadShader(source_vShader, GL_VERTEX_SHADER);
    GLuint fShader = shader_loadShader(source_fShader, GL_FRAGMENT_SHADER);
    // 创建渲染程序
    GLuint program = glCreateProgram();
    if (program <= 0) {
        LOGE("glCreateProgram failed.");
        return -1;
    }
    // 向渲染程序中加入着色器
    glAttachShader(program, vShader);
    glAttachShader(program, fShader);
    // 链接程序
    glLinkProgram(program);
    GLint status = -1;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status <= 0) {
        LOGE("GL link program failed.");
        return -1;
    }
    LOGI("GL link program success.");
    glDeleteShader(vShader);
    glDeleteShader(fShader);
    return program;
}
































