/**
 *  author : sz
 *  date : 2023/7/18
 *  description : 
 */


#ifndef ANDROIDBOILERPLATE_SHADER_TEX_H
#define ANDROIDBOILERPLATE_SHADER_TEX_H


static const char V_SHADER_TEX[] =
        "#version 300 es\n"
        "layout(location = 0)\n"
        "in vec4 vPosition;\n"

        "layout(location = 1)\n"
        "in vec2 texCoord;\n"

        "out vec2 fTexCoord;\n"

        "void main()\n"
        "{\n"
        "   gl_Position = vPosition;\n"
        "   //fTexCoord = texCoord;\n"
        "   fTexCoord = vec2(texCoord.x, 1.0 - texCoord.y);\n"
        "}\n";


static const char F_SHADER_TEX[] =
        "#version 300 es\n"
        "precision mediump float;\n"

        "in vec2 fTexCoord;\n"
        "out vec4 color;\n"

        "uniform sampler2D layer;\n"

        "void main()\n"
        "{\n"
        "    color = texture(layer, fTexCoord);\n"
        "}\n";


static const char F_SHADER_TEX2[] =
        "#version 300 es\n"
        "precision mediump float;\n"
        "in vec2 fTexCoord;\n"
        "out vec4 color;\n"
        "uniform sampler2D layer;\n"
        "void main()\n"
        "{\n"
        "    vec4 rgb = texture(layer, fTexCoord);\n"
        "    float gray = rgb.r * 0.2125 + rgb.g * 0.7154 + rgb.b * 0.0721;\n"
        "    color = vec4(gray, gray, gray, 1.0);\n"
        "}";


#endif //ANDROIDBOILERPLATE_SHADER_TEX_H
