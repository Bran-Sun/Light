//
// Created by 孙桢波 on 2018/5/13.
//

#ifndef LIGHT_LIGHT_H
#define LIGHT_LIGHT_H

#include "Vec3.h"

class Light
{
public:
    Light() {}
    Light(Vec3 center, Vec3 color) : m_center(center), m_color(color) {}
    Light(Vec3 center) : m_center(center) {}
    void setColor(Vec3 color) { m_color = color; }
    Vec3& getCenter() { return m_center; }
    Vec3& getColor() { return m_color; }
    float getPower() { return m_color.Power(); }
    Vec3 GetEmitDir();
private:
    Vec3 m_center, m_color;
};


#endif //LIGHT_LIGHT_H
