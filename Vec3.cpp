//
// Created by 孙桢波 on 2018/5/7.
//

#include "Vec3.h"
#include <cstdio>

void Vec3::Normalize()
{
    float l = Length();
    if (l == 0.0) {
        //printf("Length = 0, error!\n");
        return;
    }
    m_x /= l;
    m_y /= l;
    m_z /= l;
}

Vec3 Vec3::rand_dir()
{
    float x, y, z;
    do {
        x = (float) (2 * ((double)rand() / RAND_MAX) - 1.0);
        y = (float) (2 * ((double)rand() / RAND_MAX) - 1.0);
        z = (float) (2 * ((double)rand() / RAND_MAX) - 1.0);
    } while (x * m_x + y * m_y + z * m_z > 0);
    Vec3 v = Vec3(x, y, z);
    v.Normalize();
    return v;
}

Vec3 Vec3::Cross(const Vec3 &v)
{
    return Vec3(m_y * v.m_z - m_z * v.m_y, m_z * v.m_x - m_x * v.m_z, m_x * v.m_y - m_y * v.m_x);
}

Vec3 Vec3::refl(const Vec3 &v)
{
    Vec3 p = (*this) * 2 * Dot(v) - v;
    p.Normalize();
    return p;
}
