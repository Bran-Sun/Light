//
// Created by 孙桢波 on 2018/5/13.
//

#include "Scene.h"

void Scene::init()
{
    m_emitNum = 10000000;
    Object* s = new Sphere(Vec3(3, 0, 4.0), 1.5);
    s->setColor(Vec3( 0.7f, 0.7f, 0.7f ));
    s->setRef(0.9f);
    s->setDiff(0.5f);
    s->setSpec(0.3f);
    m_objects.push_back(s);
    
    Object* s2 = new Sphere(Vec3(-3.0, 0.0, 4.0), 1.5f);
    s2->setColor(Vec3( 1.0f, 1.0f, 1.0f));
    float glass[5] = {0.0, 0.1, 0.0, 0.9, 1.5};
    s2->setMaterial(glass);
    m_objects.push_back(s2);
    
    Object* s3 = new Plane(Vec3(1.0, 0.0, 0.0), 6.0);
    s3->setColor(Vec3(0.25f, 0.25f, 0.75f ));
    s3->setRef(0.4);
    s3->setDiff(0.80);
    s3->setSpec(0.0);
    m_objects.push_back(s3);
    
    Object* s4 = new Plane(Vec3(1.0, 0.0, 0.0), -6.0);
    s4->setColor(Vec3(0.75f, 0.25f, 0.25f ));
    s4->setRef(0.4);
    s4->setDiff(0.8);
    s4->setSpec(0.0);
    m_objects.push_back(s4);
    
    Object* s5 = new Plane(Vec3(0.0, 1.0, 0.0), -6.0);
    s5->setColor(Vec3(0.75f, 0.75f, 0.75f ));
    s5->setRef(0.4);
    s5->setDiff(0.8);
    s5->setSpec(0.0);
    m_objects.push_back(s5);
    
    Object* s6 = new Plane(Vec3(0.0, 1.0, 0.0), 6.0);
    s6->setColor(Vec3(0.75f, 0.75f, 0.75f ));
    s6->setRef(0.5);
    s6->setDiff(0.75);
    s6->setSpec(0.0);
    m_objects.push_back(s6);
    
    Object* s7 = new Plane(Vec3(0.0, 0.0, 1.0), -10.0);
    s7->setColor(Vec3(0.75f, 0.75f, 0.75f ));
    s7->setRef(0.4);
    s7->setDiff(0.8);
    s7->setSpec(0.0);
    m_objects.push_back(s7);
    
    Light* light1 = new SquareLight(6.0, -1.5, 1.5, 2.5, 5.5);
    light1->setColor(Vec3(1.0f, 1.0f, 1.0f));
    m_lights.push_back(light1);
    
    m_object_num = int(m_objects.size());
    m_light_num = int(m_lights.size());
}
