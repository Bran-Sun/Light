//
// Created by 孙桢波 on 2018/5/29.
//

#include "Plane.h"

InterResult Plane::intersection(Ray &ray, double &dist)
{
    InterResult res = MISS;
    double t = m_normal.Dot(ray.GetDirection());
    //printf("t: %.6f\n", t);
    if (t == 0.0) return MISS;
    double v = - m_normal.Dot(ray.GetOrigin()) - m_d;
    //printf("v: %.6f\n", v);
    double dist_tmp = v / t;
    if (dist_tmp <= 0) return MISS;
    else if (dist_tmp >= dist) return MISS;
    else {
        Vec3 point = ray.GetOrigin() + ray.GetDirection() * dist_tmp;
        if (point.m_z >= 0)
        {
            dist = dist_tmp;
            return HIT;
        }
        else
        {
            return MISS;
        }
    }
}
