#include <jni.h>
#include <string>
#include <android/log.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cstdlib>

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

using namespace std;
static char gVertexShader[] =
        "attribute vec4 vPosition;\n"
        "void main() {\n"
        "  gl_Position = vPosition;\n"
        "}\n";
static char gFragmentShader[] = "precision mediump float;\n"
                         "void main()\n"
                          "{\n"
                          "  gl_FragColor = vec4(0.000000, 0.000000, 0.000000, 1.0);\n"
                          "}\n";

void createFragmentShader(){
    static float red=0.0f;
    static bool redIncrease = true;
    if(redIncrease){
        red += 0.001f;
    }
    else{
        red -= 0.001f;
    }
    if (red > 1.0f) {
        red = 1.0f;
        redIncrease = false;
    }
    else if( red<0.0f ){
        red = 0.0f;
        redIncrease = true;
    }

    static float green=0.0f;
    static bool greenIncrease = true;
    if(greenIncrease){
        green += 0.002f;
    }
    else{
        green -= 0.002f;
    }
    if (green > 1.0f) {
        green = 1.0f;
        greenIncrease = false;
    }
    else if( green<0.0f ){
        green = 0.0f;
        greenIncrease = true;
    }

    static float blue=0.0f;
    static bool blueIncrease = true;
    if(blueIncrease){
        blue += 0.003f;
    }
    else{
        blue -= 0.003f;
    }
    if (blue > 1.0f) {
        blue = 1.0f;
        blueIncrease = false;
    }
    else if( blue<0.0f ){
        blue = 0.0f;
        blueIncrease = true;
    }
    string NewFragmentShader = (string)"precision mediump float;\n" +
                               (string)"void main()\n" +
                               (string)"{\n" +
                               (string)"  gl_FragColor = vec4("+to_string(red)+", "+to_string(green)+", "+to_string(blue)+", 1.0);\n" +
                               (string)"}\n";

    strcpy(gFragmentShader, NewFragmentShader.c_str());
}

GLuint loadShader(GLenum shaderType, const char* pSource) {
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
        glShaderSource(shader, 1, &pSource, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = (char*) malloc(size_t(infoLen));
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}

GLuint createProgram(const char* pVertexSource, const char* pFragmentSource) {
    createFragmentShader();
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
    if (!vertexShader) {
        return 0;
    }

    GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
    if (!pixelShader) {
        return 0;
    }

    GLuint program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        glAttachShader(program, pixelShader);
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char* buf = (char*) malloc((size_t)bufLength);
                if (buf) {
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}

GLuint gProgram;
GLuint gvPositionHandle;
bool setupGraphics(int w, int h) {
    createFragmentShader();
    gProgram = createProgram(gVertexShader, gFragmentShader);
    if (!gProgram) {
        return false;
    }
    gvPositionHandle = glGetAttribLocation(gProgram, "vPosition");

    glViewport(0, 0, w, h);
    return true;
}


extern "C" JNIEXPORT void JNICALL Java_com_example_cpptask_MyGLRenderer_init(JNIEnv * env, jclass obj,  jint width, jint height)
{
    setupGraphics(width, height);
}

extern "C" JNIEXPORT void JNICALL Java_com_example_cpptask_MyGLRenderer_DrawPolygon(JNIEnv * env, jclass obj,  jfloatArray points)
{
    jfloat *inPoints = (env) -> GetFloatArrayElements(points,0);
    jsize length = (env) -> GetArrayLength(points);

    GLfloat gTriangleVertices[length];
    for (int i = 0; i < length; i++) {
        gTriangleVertices[i] = inPoints[i];
    }

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glUseProgram(gProgram);
    glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0,gTriangleVertices);
    glEnableVertexAttribArray(gvPositionHandle);
    glDrawArrays(GL_TRIANGLES, 0, length/2);

    gProgram = createProgram(gVertexShader, gFragmentShader);
}