//
// Created by 孙桢波 on 2018/5/29.
//

#include "Plane.h"

InterResult Plane::intersection(Ray &ray, float &dist)
{
    InterResult res = MISS;
    float t = m_normal.Dot(ray.GetDirection());
    //printf("t: %.6f\n", t);
    if (t == 0.0) return MISS;
    float v = - m_normal.Dot(ray.GetOrigin()) - m_d;
    //printf("v: %.6f\n", v);
    float dist_tmp = v / t;
    if (dist_tmp <= 0) return MISS;
    else if (dist_tmp >= dist) return MISS;
    else {
        dist = dist_tmp;
        return HIT;
    }
}