//
// Created by chen jy on 2022/4/30.
//

#include "PlayerShader.h"
#include "PlayerLog.h"
#include <GLES2/gl2.h>


//顶点着色器glsl
#define GET_STR(x) #x
static const char *vertexShader = GET_STR(
        attribute vec4 aPosition; //顶点坐标
        attribute vec2 aTexCoord; //材质顶点坐标
        varying vec2 vTexCoord;   //输出的材质坐标
        void main() {
            vTexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);
            gl_Position = aPosition;
        }
);

//片元着色器,软解码和部分x86硬解码
static const char *fragYUV420P = GET_STR(
        precision mediump float;    //精度
        varying vec2 vTexCoord;     //顶点着色器传递的坐标
        uniform sampler2D yTexture; //输入的材质（不透明灰度，单像素）
        uniform sampler2D uTexture;
        uniform sampler2D vTexture;
        void main() {
            vec3 yuv;
            vec3 rgb;
            yuv.r = texture2D(yTexture, vTexCoord).r;
            yuv.g = texture2D(uTexture, vTexCoord).r - 0.5;
            yuv.b = texture2D(vTexture, vTexCoord).r - 0.5;
            rgb = mat3(1.0, 1.0, 1.0,
                       0.0, -0.39465, 2.03211,
                       1.13983, -0.58060, 0.0) * yuv;
            //输出像素颜色
            gl_FragColor = vec4(rgb, 1.0);
        }
);

static GLuint initShader(const char *code, GLint type) {
    //创建shader
    GLuint sh = glCreateShader(type);
    if (sh == 0) {
        LOGE("glCreateShader %d failed!", type);
        return 0;
    }
    //加载shader
    glShaderSource(sh,
                   1,//shader数量
                   &code,//shader代码
                   0);//代码长度
    //编译shader
    glCompileShader(sh);
    //获取编译情况
    GLint status;
    glGetShaderiv(sh, GL_COMPILE_STATUS, &status);
    if (status == 0) {
        LOGE("glGetShaderiv failed");
        return 0;
    }
    LOGE("glGetShaderiv SUCCESS");
    return sh;
}

bool PlayerShader::init() {
    //顶点和片元shader初始化
    //顶点shader初始化
    vsh = initShader(vertexShader,GL_VERTEX_SHADER);
    if (vsh==0){
        LOGE("initShader GL_VERTEX_SHADER failed!");
        return false;
    }
    LOGD("initShader GL_VERTEX_SHADER success");
    //片元YUV420 shader初始化
    fsh = initShader(fragYUV420P,GL_FRAGMENT_SHADER);
    if (fsh==0){
        LOGE("initShader GL_FRAGMENT_SHADER failed!");
        return false;
    }
    LOGD("initShader GL_FRAGMENT_SHADER success");

    //创建渲染程序
    program = glCreateProgram();
    if (program==0){
        LOGE("glCreateProgram FAILED");
        return false;
    }
    //渲染程序中加入着色器
    glAttachShader(program,vsh);
    glAttachShader(program,fsh);
    //链接程序
    glLinkProgram(program);
    GLint status = 0;
    glGetProgramiv(program,GL_LINK_STATUS,&status);
    if (status!=GL_TRUE){
        LOGE("glGetProgramiv failed");
        return false;
    }
    glUseProgram(program);
    LOGD("glGetProgramiv SUCCESS");


    //加入三维顶点数据 两个三角形组成正方形
    static float vers[] = {
            1.0f,-1.0f,0.0f,
            -1.0f,-1.0f,0.0f,
            1.0f,1.0f,0.0f,
            -1.0f,1.0f,0.0f,
    };
    GLuint apos = glGetAttribLocation(program,"aPosition");
    glEnableVertexAttribArray(apos);
    //传递顶点
    glVertexAttribPointer(apos,3,GL_FLOAT,GL_FALSE,12,vers);

    //加入材质坐标数据
    static float txts[] = {
            1.0f,0.0f , //右下
            0.0f,0.0f,
            1.0f,1.0f,
            0.0,1.0
    };
    GLuint atex = glGetAttribLocation(program,"aTexCoord");
    glEnableVertexAttribArray(atex);
    glVertexAttribPointer(atex,2,GL_FLOAT,GL_FALSE,8,txts);

    //材质纹理初始化
    //设置纹理层
    glUniform1i( glGetUniformLocation(program,"yTexture"),0); //对于纹理第1层
    glUniform1i( glGetUniformLocation(program,"uTexture"),1); //对于纹理第2层
    glUniform1i( glGetUniformLocation(program,"vTexture"),2); //对于纹理第3层

    LOGD("初始化Shader成功！");

    return true;
}


void PlayerShader::draw()
{
    if(!program)
        return;
    //三维绘制
    //绘制存放的顶点信息
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
}

/***
 *
 * @param index
 * @param width
 * @param height
 * @param buf
 * getTexture会被调用3次，3次的结果就是材质
 */
void PlayerShader::getTexture(unsigned int index,int width,int height, unsigned char *buf)
{

    if(texts[index] == 0)
    {
        //材质初始化
        glGenTextures(1,&texts[index]);

        //设置纹理属性
        glBindTexture(GL_TEXTURE_2D,texts[index]);
        //缩小的过滤器
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        //设置纹理的格式和大小
        //如果是uv12会有区别
        glTexImage2D(GL_TEXTURE_2D,
                     0,           //细节基本 0默认
                     GL_LUMINANCE,//gpu内部格式 亮度，灰度图
                     width,height, //拉升到全屏
                     0,             //边框
                     GL_LUMINANCE,//数据的像素格式 亮度，灰度图 要与上面一致
                     GL_UNSIGNED_BYTE, //像素的数据类型
                     NULL                    //纹理的数据
        );
    }


    //激活第1层纹理,绑定到创建的opengl纹理
    glActiveTexture(GL_TEXTURE0+index);
    glBindTexture(GL_TEXTURE_2D,texts[index]);
    //替换纹理内容
    glTexSubImage2D(GL_TEXTURE_2D,0,0,0,width,height,GL_LUMINANCE,GL_UNSIGNED_BYTE,buf);

}