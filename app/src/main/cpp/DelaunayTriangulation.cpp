#include <jni.h>
#include "delaunator.h"
#include <iostream>
#include <vector>
#include <math.h>
using namespace std;

extern "C" JNIEXPORT jfloatArray JNICALL Java_com_example_cpptask_MyGLRenderer_DelaunayTriangulation(JNIEnv * env, jclass obj,  jfloatArray points)
{
    jfloat *inPoints = (env) -> GetFloatArrayElements(points,0);
    jsize length = (env) -> GetArrayLength(points);

    vector<float> coords;

    for (int i = 0; i < length; i++) {
        coords.push_back(inPoints[i]);
    }

    //triangulation happens here
    delaunator::Delaunator d(coords);

    jfloat *outPoints = new jfloat[d.triangles.size()*2];
    jfloatArray triangulatedPoints =  env->NewFloatArray(d.triangles.size()*2);

    int indexOutPoints = 0;
    for (std::size_t i = 0; i < d.triangles.size(); i += 3) {
        outPoints[indexOutPoints] = (d.coords[2 * d.triangles[i]]);
        indexOutPoints++;
        outPoints[indexOutPoints] = d.coords[2 * d.triangles[i] + 1];
        indexOutPoints++;
        outPoints[indexOutPoints] = d.coords[2 * d.triangles[i + 1]];
        indexOutPoints++;
        outPoints[indexOutPoints] = d.coords[2 * d.triangles[i + 1] + 1];
        indexOutPoints++;
        outPoints[indexOutPoints] = d.coords[2 * d.triangles[i + 2]];
        indexOutPoints++;
        outPoints[indexOutPoints] = d.coords[2 * d.triangles[i + 2] + 1];
        indexOutPoints++;
    }

    env->SetFloatArrayRegion(triangulatedPoints, 0, d.triangles.size()*2, outPoints);

    return triangulatedPoints;
}
