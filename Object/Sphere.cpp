//
// Created by 孙桢波 on 2018/5/13.
//

#include "Sphere.h"

InterResult Sphere::intersection(Ray &ray, double &dist)
{
    InterResult result = MISS;
    Vec3 v = m_origin - ray.GetOrigin();
    double a = v.Dot(ray.GetDirection());
    double b = m_radius * m_radius - v.SqrLength() + a * a;
    if (b > 0) {
        b = sqrt(b);
        if (a > b) {
            double dist_tmp = a - b;
            if (dist_tmp >= dist) result = MISS;
            else {
                result = HIT;
                dist = dist_tmp;
            }
        }
        else if (b > a && a > (-b)) {
            double dist_tmp = b + a;
            if (dist_tmp >= dist) result = MISS;
            else {
                result = INSIDE_HIT;
                dist = dist_tmp;
            }
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
