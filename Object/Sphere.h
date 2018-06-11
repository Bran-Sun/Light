//
// Created by 孙桢波 on 2018/5/13.
//

#ifndef LIGHT_SPHERE_H
#define LIGHT_SPHERE_H

#include "Object.h"

class Sphere : public Object
{
public:
    Sphere():Object() {}
    Sphere(Vec3 origin, double radius) : Object(), m_origin(origin), m_radius(radius) {}
    virtual InterResult intersection(Ray &ray, double &dist);
    virtual Vec3 getNormal(Vec3 &point);
private:
    Vec3 m_origin;
    double m_radius;
};


#endif //LIGHT_SPHERE_H
