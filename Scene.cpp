//
// Created by 孙桢波 on 2018/5/13.
//

#include "Scene.h"

void Scene::init()
{
    Object* s = new Sphere(Vec3(4, 0, 3), 2.0);
    s->setColor(Vec3( 0.7f, 0.7f, 0.7f ));
    s->setRef(0.6f);
    s->setDiff(0.2f);
    m_objects.push_back(s);
    
    Object* s2 = new Sphere(Vec3(-4.0, 0.0, 3.0), 2.0f);
    s2->setColor(Vec3( 0.7f, 0.7f, 1.0f));
    s2->setRef(0.3f);
    s2->setDiff(0.1f);
    m_objects.push_back(s2);
    
    Light* light1 = new Light(Vec3(0.0, 6.0, 3.0));
    light1->setColor(Vec3(1.0f, 1.0f, 1.0f));
    m_lights.push_back(light1);
    
    m_object_num = int(m_objects.size());
    m_light_num = int(m_lights.size());
}
