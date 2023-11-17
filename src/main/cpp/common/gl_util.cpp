/**
 *  author : suzhou
 *  date : 2023/7/30 
 *  description : 
 */

#include "gl_util.h"
#include "android_log.h"


/**
 * opengl配置矩阵(顶点坐标变换、纹理坐标变换)
 * @param program - 着色器程序
 * @param frame_w - 帧宽度
 * @param frame_h - 帧高度
 * @param window_w - 窗口宽度
 * @param window_h - 窗口高度
 * @param scale_type - 缩放类型：1 - crop, 2 - fit
 * @param rotate - 是否旋转
 */
void gl_setMatrix(GLuint program, int frame_w, int frame_h, int window_w, int window_h,
                  int scale_type, bool rotate) {

    LOGE("frame_w = %d", frame_w);
    LOGE("frame_h = %d", frame_h);
    LOGE("window_w = %d", window_w);
    LOGE("window_h = %d", window_h);

    // 顶点坐标变换矩阵变量名
    GLint m_location = glGetUniformLocation(program, "v_matrix");
    // 是否旋转
    bool rotate2 = ((frame_w > frame_h && window_w < window_h) || (frame_w < frame_h && window_w > window_h)) && rotate;
    if (rotate2) {
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
    GLint tex_location = glGetUniformLocation(program, "tex_matrix");
    // 根据是否旋转，ratio_x和ratio_y取值不一样
    float ratio_x, ratio_y;
    if (rotate2) {
        ratio_x = (float) frame_h / window_w;
        ratio_y = (float) frame_w / window_h;
    } else {
        ratio_x = (float) frame_w / window_w;
        ratio_y = (float) frame_h / window_h;
    }
    LOGE("ratio_x = %f", ratio_x);
    LOGE("ratio_y = %f", ratio_y);
    LOGE("scale_type = %d", scale_type);
    // 先搞清楚一种情况，其它很类似
    if (ratio_x > ratio_y && scale_type == 1 || ratio_x < ratio_y && scale_type == 2) {
        float max_x = ratio_x / ratio_y; // 归一化的纹理坐标x最大值(缩放系数)
        // 缩放+平移得到矩阵(列主序)
        float m_tex[9] = {
                max_x, 0.0, 0.0,
                0.0, 1.0, 0.0,
                static_cast<float>(-(max_x - 1.0) / 2), 0.0, 1.0,
        };
        glUniformMatrix3fv(tex_location, 1, false, m_tex);
    }
    if (ratio_x > ratio_y && scale_type == 2 || ratio_x < ratio_y && scale_type == 1) {
        float max_y = ratio_y / ratio_x; // 归一化的纹理坐标y最大值(缩放系数)
        float m_tex[9] = {
                1.0, 0.0, 0.0,
                0.0, max_y, 0.0,
                0.0, static_cast<float>((1.0 - max_y) / 2), 1.0,
        };
        glUniformMatrix3fv(tex_location, 1, false, m_tex);
    }
}


/**
     * 创建顶点缓冲对象并填充数据
     * @param vbo
     * @param data
     * @param size
     */
void gl_genBuffer(GLuint *vbo, float data[], GLuint size) {
    // 创建缓冲对象
    glGenBuffers(1, vbo);
    // 绑定缓冲对象
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    // 为缓冲对象创建存储，大小为size，数据初始化为data指向的数据
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

/**
 * 创建顶点索引缓冲对象并填充数据
 * @param ebo
 * @param data
 * @param size
 */
void gl_genIndexBuffer(GLuint *ebo, GLuint data[], GLuint size) {
    // 创建缓冲对象
    glGenBuffers(1, ebo);
    // 绑定索引缓冲对象
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);
    // 为索引缓冲对象创建存储，并利用data进行初始化
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

/**
 * 创建2d纹理对象，绑定和配置
 * @param tex_2d
 */
void gl_genTex2D(GLuint *tex_2d) {
    // 创建纹理对象
    glGenTextures(1, tex_2d);
    // 绑定一个纹理对象
    glBindTexture(GL_TEXTURE_2D, *tex_2d);
    // 横坐标环绕配置
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // 纵坐标环绕配置
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // 纹理分辨率大于图元分辨率，即纹理需要被缩小的过滤配置
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // 纹理分辨率小于图元分辨率，即纹理需要被放大的过滤配置
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

/**
 * 加载bitmap到当前绑定的2D纹理
 * @param env
 * @param bitmap
 */
void gl_texImage2D(JNIEnv *env, jobject bitmap, uint32_t *width, uint32_t *height) {
    // 获取bitmap的信息和数据的指针
    AndroidBitmapInfo bitmapInfo;
    if (AndroidBitmap_getInfo(env, bitmap, &bitmapInfo) < 0) {
        LOGE("Get bitmap info failed.");
        return;
    }
    // 返回尺寸信息
    *width = bitmapInfo.width;
    *height = bitmapInfo.height;

    void *bitmapPixels;
    AndroidBitmap_lockPixels(env, bitmap, &bitmapPixels);
    // todo: 加载图片到纹理（通过这个函数把图片数据加载到显卡了？）
    glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            bitmapInfo.width,
            bitmapInfo.height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            bitmapPixels
    );
    AndroidBitmap_unlockPixels(env, bitmap);
}

/**
 * 绘制
 * @param vertexNum
 */
void gl_drawElements(GLuint vertexNum) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, vertexNum, GL_UNSIGNED_INT, (void *) 0);
}




































