//
// Created by 孙桢波 on 2018/5/13.
//

#ifndef LIGHT_OBJECT_H
#define LIGHT_OBJECT_H

#include "../Vec3.h"
#include "../Camera.h"

enum InterResult
{
    MISS = 0,
    HIT,
    INSIDE_HIT,
};

class Object
{
public:
    Object() {}
    void setColor(Vec3 color) { m_color = color; }
    void setRef(float ref) { m_ref = ref; }
    float getRef() { return m_ref; }
    virtual Vec3 getNormal(Vec3 &point) = 0;
    virtual InterResult intersection(Ray& ray, float &dist) = 0;
private:
    float m_ref;
    Vec3 m_color;
};

#endif //LIGHT_OBJECT_H
