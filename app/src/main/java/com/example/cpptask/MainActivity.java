package com.example.cpptask;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;

public class MainActivity extends AppCompatActivity {

    MyGLSurfaceView mView;

    @Override protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mView = new MyGLSurfaceView(getApplication());
        setContentView(mView);
    }
}
