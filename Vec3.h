//
// Created by 孙桢波 on 2018/5/7.
//

#ifndef LIGHT_VEC3_H
#define LIGHT_VEC3_H

#include <cmath>
#include <iostream>

class Vec3
{
public:
    Vec3(float x, float y, float z) : m_x(x), m_y(y), m_z(z) {}
    Vec3() : m_x(0.0), m_y(0.0), m_z(0.0) {}
    
    void Normalize();
    void GetXYZ(float &x, float&y, float &z) {
        x = m_x; y = m_y; z = m_z;
    }
    float Dot(Vec3 &m) {
        return m_x * m.m_x + m_y * m.m_y + m_z * m.m_z;
    }
    float Length() {
        return sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
    }
    float SqrLength() {
        return m_x * m_x + m_y * m_y + m_z * m_z;
    }
    void operator += (Vec3 &m) {
        m_x += m.m_x; m_y += m.m_y; m_z += m.m_z;
    }
    void operator -= (Vec3 &m) {
        m_x -= m.m_x; m_y -= m.m_y; m_z -= m.m_z;
    }
    Vec3 operator- () const { return Vec3( -m_x, -m_y, -m_z ); }
    
    friend Vec3 operator + (const Vec3 &a, const Vec3 &b) { return Vec3(a.m_x + b.m_x, a.m_y + b.m_y, a.m_z + b.m_z); }
    friend Vec3 operator - (const Vec3 &a, const Vec3 &b) { return Vec3(a.m_x - b.m_x, a.m_y - b.m_y, a.m_z - b.m_z); }
    friend std::ostream& operator << (std::ostream &os, const Vec3 &m) { os << "x: " << m.m_x << ", y: " << m.m_y << ", z: " << m.m_z << " "; return os;}
private:
    float m_x, m_y, m_z;
};


#endif //LIGHT_VEC3_H
