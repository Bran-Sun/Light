//
// Created by 孙桢波 on 2018/5/13.
//

#include "Light.h"
#define ran() ( double( rand() % RAND_MAX ) / RAND_MAX )

Ray SpotLight::GetEmitRay()
{
    double x, y, z, l;
    do {
        x = (2 * ran() - 1.0);
        y = (2 * ran() - 1.0);
        z = (2 * ran() - 1.0);
        l = x * x + y * y + z * z;
    } while (l > 1 || l < 1e-5);
    Vec3 v = Vec3(x, y, z);
    return Ray(m_center, v);
}

Ray SquareLight::GetEmitRay()
{
    double x, y, z, l;
    double a, b;
    do {
        x = (2 * ran() - 1.0);
        y = (2 * ran() - 2.0);
        z = (2 * ran() - 1.0);
        l = x * x + y * y + z * z;
    } while (l < 1e-5);
    Vec3  v = Vec3(x, y, z);
    a = ran() * (m_x1 - m_x0) + m_x0;
    b = ran() * (m_z1 - m_z0) + m_z0;
    return Ray(Vec3(a, m_y, b), v);
}

InterResult SquareLight::intersection(Ray &ray, double &dist)
{
    double d_y = m_y - ray.GetOrigin().m_y;
    double dir_y = ray.GetDirection().m_y;
    if (dir_y == 0) return MISS;
    
    double d = d_y / dir_y;
    if (d <= 0) return MISS;
    if (d > dist) return MISS;
    Vec3 point = ray.GetOrigin() + ray.GetDirection() * d;
    if ((point.m_x < m_x1) && (point.m_x > m_x0) && (point.m_z > m_z0) && (point.m_z < m_z1)) {
        dist = d;
        return HIT;
    }
    return MISS;
}


Ray CircleLight::GetEmitRay()
{
    double x, y, z, l;
    double a, b;
    do {
        x = (2 * ran() - 1.0);
        y = (2 * ran() - 2.0);
        z = (2 * ran() - 1.0);
        l = x * x + y * y + z * z;
    } while (l < 1e-5);
    Vec3 v(x, y, z);
    do {
        a = 2 * ran() - 1.0;
        b = 2 * ran() - 1.0;
    } while ((a * a + b * b) >= (m_r2));
    return Ray(Vec3(m_center.m_x + a, m_center.m_y, m_center.m_z + b), v);
}

InterResult CircleLight::intersection(Ray &ray, double &dist)
{
    double d_y = m_center.m_y - ray.GetOrigin().m_y;
    double dir_y = ray.GetDirection().m_y;
    if (dir_y == 0) return MISS;
    
    double d = d_y / dir_y;
    if (d <= 0) return MISS;
    if (d > dist) return MISS;
    Vec3 point = ray.GetOrigin() + ray.GetDirection() * d;
    double dx = point.m_x - m_center.m_x;
    double dz = point.m_z - m_center.m_z;
    if (dx * dx + dz * dz < m_r2) {
        dist = d;
        return HIT;
    }
    return MISS;
}
