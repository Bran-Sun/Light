//
// Created by 孙桢波 on 2018/5/13.
//

#include "Sphere.h"

InterResult Sphere::intersection(Ray &ray, float &dist)
{
    InterResult result = MISS;
    Vec3 v = m_origin - ray.GetOrigin();
    float a = v.Dot(ray.GetDirection());
    float b = m_radius * m_radius - v.SqrLength() + a * a;
    if (b > 0) {
        b = sqrt(b);
        if (a > b) {
            result = HIT;
            dist = a - b;
        }
        else if (b > a && a > (-b)) {
            result = INSIDE_HIT;
            dist = b + a;
        }
        else {
            result = MISS;
        }
    }
    return result;
}

Vec3 Sphere::getNormal(Vec3 &point)
{
    return point - m_origin;
}
