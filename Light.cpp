//
// Created by 孙桢波 on 2018/5/13.
//

#include "Light.h"
#define ran() ( double( rand() % RAND_MAX ) / RAND_MAX )

Ray SpotLight::GetEmitRay()
{
    float x, y, z, l;
    do {
        x = (float) (2 * ran() - 1.0);
        y = (float) (2 * ran() - 1.0);
        z = (float) (2 * ran() - 1.0);
        l = x * x + y * y + z * z;
    } while (l > 1 || l < 1e-5);
    Vec3 v = Vec3(x, y, z);
    return Ray(m_center, v);
}

Ray SquareLight::GetEmitRay()
{
    float x, y, z, l;
    float a, b;
    do {
        x = (float) (2 * ran() - 1.0);
        y = (float) (2 * ran() - 2.0);
        z = (float) (2 * ran() - 1.0);
        l = x * x + y * y + z * z;
    } while (l < 1e-5);
    Vec3  v = Vec3(x, y, z);
    a = (float)ran() * (m_x1 - m_x0) + m_x0;
    b = (float)ran() * (m_z1 - m_z0) + m_z0;
    return Ray(Vec3(a, m_y, b), v);
}

InterResult SquareLight::intersection(Ray &ray, float &dist)
{
    float d_y = m_y - ray.GetOrigin().m_y;
    float dir_y = ray.GetDirection().m_y;
    if (dir_y == 0) return MISS;
    
    float d = d_y / dir_y;
    if (d <= 0) return MISS;
    if (d > dist) return MISS;
    Vec3 point = ray.GetOrigin() + ray.GetDirection() * d;
    if ((point.m_x < m_x1) && (point.m_x > m_x0) && (point.m_z > m_z0) && (point.m_z < m_z1)) {
        dist = d;
        return HIT;
    }
    return MISS;
}

