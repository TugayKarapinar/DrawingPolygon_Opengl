package com.example.cpptask;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import android.opengl.GLSurfaceView;
public class MyGLRenderer implements GLSurfaceView.Renderer {
    static {
        System.loadLibrary("cpptask");
    }

    float points[] = {-0.9f,0.0f,
            -0.6f,0.3f,
            -0.3f, 0.0f,
            0.9f, 0.0f,
            0.6f,0.3f,
            0.3f,0.0f
    };


    public static native void init(int width, int height);
    public static native void DrawPolygon(float points[]);

    public void onSurfaceCreated(GL10 unused, EGLConfig config) {
    }

    public void onDrawFrame(GL10 unused) {
        DrawPolygon(points);
    }

    public void onSurfaceChanged(GL10 unused, int width, int height) {
        init(width,height);
    }
}