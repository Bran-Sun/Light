//
// Created by 孙桢波 on 2018/5/13.
//

#ifndef LIGHT_OBJECT_H
#define LIGHT_OBJECT_H

#include "../Vec3.h"
#include "../Camera.h"
#include "Texture.h"

enum InterResult
{
    MISS = 0,
    HIT,
    INSIDE_HIT,
};

class Object
{
public:
    Object() { m_texture = nullptr; }
    Object(double diff, double spec, double refl) {
        m_diff = diff; m_spec = spec; m_refl = refl; m_refr = 0.0; m_refractiveIndex = 0.0; m_texture = nullptr;
    }
    Object(double diff, double spec, double refl, double refr, double refractionIndex) {
        m_diff = diff; m_spec = spec; m_refl = refl; m_refr = refr; m_refractiveIndex = refractionIndex; m_texture = nullptr;
    }
    
    void setColor(Vec3 color) { m_color = color; }
    void setRef(double refl) { m_refl = refl; }
    void setDiff(double diff) { m_diff = diff; }
    void setSpec(double spec) { m_spec = spec; }
    void setMaterial(double a[]) {
        m_diff = a[0]; m_spec = a[1]; m_refl = a[2]; m_refr = a[3]; m_refractiveIndex = a[4];
        m_texture = nullptr;
    }
    void setTexture(Texture *t) { m_texture = t; }
    Texture* getTexture() { return m_texture; }
    double getDiff() { return m_diff; }
    double getRefl() { return m_refl; }
    double getSpec() { return m_spec; }
    double getRefr() { return m_refr; }
    double getRefractionIndex() { return m_refractiveIndex; }
    
    Vec3 getColor() { return m_color; }
    virtual Vec3 getNormal(Vec3 &point) = 0;
    virtual InterResult intersection(Ray& ray, double &dist) = 0;
private:
    double m_refl, m_diff, m_spec, m_refr, m_refractiveIndex;
    Texture *m_texture;
    Vec3 m_color;
};

#endif //LIGHT_OBJECT_H