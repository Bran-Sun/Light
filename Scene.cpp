//
// Created by 孙桢波 on 2018/5/13.
//

#include "Scene.h"

void Scene::init()
{
    Object* s = new Sphere(Vec3(3, 0, 3), 1.5);
    s->setColor(Vec3( 0.7f, 0.7f, 0.7f ));
    s->setRef(0.4f);
    s->setDiff(0.2f);
    m_objects.push_back(s);
    
    Object* s2 = new Sphere(Vec3(-3.0, 0.0, 3.0), 1.5f);
    s2->setColor(Vec3( 0.7f, 0.7f, 1.0f));
    s2->setRef(0.3f);
    s2->setDiff(0.1f);
    m_objects.push_back(s2);
    
    Object* s3 = new Plane(Vec3(1.0, 0.0, 0.0), 6.0);
    s3->setColor(Vec3(0.4f, 0.3f, 0.3f ));
    s3->setRef(0.0);
    s3->setDiff(1.0);
    m_objects.push_back(s3);
    
    Object* s4 = new Plane(Vec3(1.0, 0.0, 0.0), -6.0);
    s4->setColor(Vec3(0.4f, 0.3f, 0.3f ));
    s4->setRef(0.0);
    s4->setDiff(1.0);
    m_objects.push_back(s4);
    
    Object* s5 = new Plane(Vec3(0.0, 1.0, 0.0), -6.0);
    s5->setColor(Vec3(0.4f, 0.3f, 0.3f ));
    s5->setRef(0.0);
    s5->setDiff(1.0);
    m_objects.push_back(s5);
    
    Object* s6 = new Plane(Vec3(0.0, 1.0, 0.0), 6.0);
    s6->setColor(Vec3(0.4f, 0.3f, 0.3f ));
    s6->setRef(0.0);
    s6->setDiff(1.0);
    m_objects.push_back(s6);
    
    Object* s7 = new Plane(Vec3(0.0, 0.0, 1.0), -10.0);
    s7->setColor(Vec3(0.4f, 0.3f, 0.3f ));
    s7->setRef(0.0);
    s7->setDiff(1.0);
    m_objects.push_back(s7);
    
    Light* light1 = new Light(Vec3(0.0, 5.0, 3.0));
    light1->setColor(Vec3(1.0f, 1.0f, 1.0f));
    m_lights.push_back(light1);
    
    m_object_num = int(m_objects.size());
    m_light_num = int(m_lights.size());
}
