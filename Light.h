//
// Created by 孙桢波 on 2018/5/13.
//

#ifndef LIGHT_LIGHT_H
#define LIGHT_LIGHT_H

#include "Vec3.h"
#include "Object/Object.h"

class Light
{
public:
    Light() {}
    Light(Vec3 center, Vec3 color) : m_center(center), m_color(color) {}
    Light(Vec3 center) : m_center(center) {}
    void setColor(Vec3 color) { m_color = color; }
    Vec3& getCenter() { return m_center; }
    Vec3& getColor() { return m_color; }
    double getPower() { return m_color.Power(); }
    virtual Ray GetEmitRay() = 0;
    virtual InterResult intersection(Ray &ray, double &dist) = 0;
protected:
    Vec3 m_center, m_color;
};

class SpotLight: public Light
{
public:
    SpotLight(Vec3 center) : Light(center) {}
    virtual Ray GetEmitRay();
    virtual InterResult intersection(Ray &ray, double &dist) {
        return MISS;
    }
};


//require horizon
class SquareLight: public Light
{
public:
    SquareLight(double y, double x0, double x1, double z0, double z1) : Light() {
        m_norm.set(0, 1.0, 0);
        m_x0 = x0; m_x1 = x1; m_z0 = z0; m_z1 = z1;
        m_y = y;
    }
    
    virtual Ray GetEmitRay();
    void setBorder(int x0, int x1, int z0, int z1) {
        m_x0 = x0; m_x1 = x1; m_z0 = z0; m_z1 = z1;
    }
    void sety(double y) { m_y = y; }
    virtual InterResult intersection(Ray &ray, double &dist);
private:
    Vec3 m_norm;
    double m_x0, m_x1, m_z0, m_z1, m_y;
};


#endif //LIGHT_LIGHT_H
