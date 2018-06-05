//
// Created by 孙桢波 on 2018/5/13.
//

#include "Light.h"

Vec3 Light::GetEmitDir()
{
    float x, y, z, l;
    do {
        x = (float) (2 * ((double)rand() / RAND_MAX) - 1.0);
        y = (float) (2 * ((double)rand() / RAND_MAX) - 1.0);
        z = (float) (2 * ((double)rand() / RAND_MAX) - 1.0);
        l = x * x + y * y + z * z;
    } while (l != 0);
    Vec3 v = Vec3(z, y, z);
    v.Normalize();
    return v;
}
