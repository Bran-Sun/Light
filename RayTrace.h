//
// Created by 孙桢波 on 2018/6/4.
//

#ifndef LIGHT_RAYTRACE_H
#define LIGHT_RAYTRACE_H

#include "Scene.h"
#include "Camera.h"

const double MIN_WEIGHT = 0.0001;
const int MAX_DEPTH = 5;

void RayTrace(Ray ray, int depth, double weight, Vec3 &color, Scene &scene);
void DrawWithRayTrace(Camera &camera, Scene &scene);

#endif //LIGHT_RAYTRACE_H
