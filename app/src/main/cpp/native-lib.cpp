#include <jni.h>
#include <string>
#include <GLES2/gl2.h>
#include <chrono>

char gVertexShader[] =
        "attribute vec4 vPosition;\n"
        "void main() {\n"
        "  gl_Position = vPosition;\n"
        "}\n";

char gFragmentShader[] =
        "precision mediump float;\n"
        "uniform vec4 color;"
        "void main()\n"
        "{\n"
        "  gl_FragColor = color;\n"
        "}\n";

auto t_start = std::chrono::high_resolution_clock::now();
float rgb[] = {0.0f,0.0f,0.0f};

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
GLint gvPositionHandle;
GLint color;
bool setupGraphics(int w, int h) {
    gProgram = createProgram(gVertexShader, gFragmentShader);
    if (!gProgram) {
        return false;
    }
    color = glGetUniformLocation(gProgram, "color");
    gvPositionHandle = glGetAttribLocation(gProgram, "vPosition");

    glViewport(0, 0, w, h);
    return true;
}

void ChangeRGB(){
    auto t_now = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
    rgb[0] = (sin(time * 1.0f) + 1.0f) /  2.0f;
    rgb[1] = (sin(time * 2.0f) + 1.0f) /  2.0f;
    rgb[2] = (sin(time * 3.0f) + 1.0f) /  2.0f;
    glUniform4f(color, rgb[0], rgb[1], rgb[2], 1.0f);
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

    //Changing the color of the drawn polygon depending on time.
    ChangeRGB();

    glVertexAttribPointer((GLuint)gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0,gTriangleVertices);
    glEnableVertexAttribArray((GLuint)gvPositionHandle);
    glDrawArrays(GL_TRIANGLES, 0, length/2);
}