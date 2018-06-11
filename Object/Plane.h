//
// Created by 孙桢波 on 2018/5/29.
//

#ifndef LIGHT_PLANE_H
#define LIGHT_PLANE_H

#include "Object.h"

//require z > 0
class Plane : public Object
{
public:
    Plane(): Object() {}
    Plane(Vec3 normal, double d):Object(), m_normal(normal), m_d(d) { }
    virtual InterResult intersection(Ray &ray, double &dist);
    virtual Vec3 getNormal(Vec3 &point) { return m_normal; }
private:
    Vec3 m_normal;
    double m_d;
};


#endif //LIGHT_PLANE_H
