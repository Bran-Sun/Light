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
    Object(float diff, float spec, float refl) {
        m_diff = diff; m_spec = spec; m_refl = refl; m_refr = 0.0; m_refractiveIndex = 0.0;
    }
    Object(float diff, float spec, float refl, float refr, float refractionIndex) {
        m_diff = diff; m_spec = spec; m_refl = refl; m_refr = refr; m_refractiveIndex = refractionIndex;
    }
    
    void setColor(Vec3 color) { m_color = color; }
    void setRef(float refl) { m_refl = refl; }
    void setDiff(float diff) { m_diff = diff; }
    void setSpec(float spec) { m_spec = spec; }
    void setMaterial(float a[]) {
        m_diff = a[0]; m_spec = a[1]; m_refl = a[2]; m_refr = a[3]; m_refractiveIndex = a[4];
    }
    float getDiff() { return m_diff; }
    float getRefl() { return m_refl; }
    float getSpec() { return m_spec; }
    float getRefr() { return m_refr; }
    float getRefractionIndex() { return m_refractiveIndex; }
    
    Vec3 getColor() { return m_color; }
    virtual Vec3 getNormal(Vec3 &point) = 0;
    virtual InterResult intersection(Ray& ray, float &dist) = 0;
private:
    float m_refl, m_diff, m_spec, m_refr, m_refractiveIndex;
    Vec3 m_color;
};

#endif //LIGHT_OBJECT_H