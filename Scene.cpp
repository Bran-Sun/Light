//
// Created by 孙桢波 on 2018/5/13.
//

#include "Scene.h"

void Scene::init()
{
    m_emitNum = 15000000;
    
    double glass[5] = {0.0, 0.1, 0.0, 0.8, 1.6};
    double marble[5] = {0.6, 0.3, 0.9, 0.0, 0.0};
    double wall[5] = {0.6, 0.0, 0.0, 0.0, 0.0};
    double metal[5] = {0.15, 0.7, 0.0, 0.0, 0.0};
    
    std::vector<Vec2> points;
    points.push_back(Vec2(0.0, 0.0));
    points.push_back(Vec2(1.0, 0.0));
    points.push_back(Vec2(2.0, 1.5));
    points.push_back(Vec2(0.0, 2.5));
    points.push_back(Vec2(0.0, 4.5));
    
    Object *bezier = new Bezier(points, Vec3(0.0, -3.0, 7.0));
    bezier->setColor(Vec3(0.0, 1.0, 1.0));
    bezier->setMaterial(marble);
    m_objects.push_back(bezier);
    
    Object* s = new Sphere(Vec3(3.0, -4.2, 7.0), 1.5);
    s->setColor(Vec3( 0.9f, 0.7f, 0.9f ));
    s->setMaterial(metal);
    m_objects.push_back(s);
    
    Object* s2 = new Sphere(Vec3(-3.0, -4.2, 7.0), 1.5f);
    s2->setColor(Vec3( 0.9f, 0.9f, 0.6f));
    s2->setMaterial(glass);
    m_objects.push_back(s2);
    
    Object* s9 = new Sphere(Vec3(3.8, 0.0, 3.8), 1.8f);
    s9->setColor(Vec3(1.0f, 1.0f, 1.0f));
    s9->setMaterial(glass);
    m_objects.push_back(s9);
    
    Object* s3 = new Plane(Vec3(1.0, 0.0, 0.0), 6.0);
    s3->setColor(Vec3(0.5f, 0.5f, 0.75f ));
    s3->setMaterial(wall);
    m_objects.push_back(s3);
    
    Object* s4 = new Plane(Vec3(1.0, 0.0, 0.0), -6.0);
    s4->setColor(Vec3(0.5f, 0.75f, 0.5 ));
    s4->setMaterial(wall);
    m_objects.push_back(s4);
    
    Object* s5 = new Plane(Vec3(0.0, 1.0, 0.0), -6.0);
    s5->setColor(Vec3(0.75f, 0.75f, 0.75f ));
    s5->setMaterial(wall);
    m_objects.push_back(s5);
    
    Object* s6 = new Plane(Vec3(0.0, 1.0, 0.0), 6.0);
    //s6->setTexture(t);
    s6->setColor(Vec3(0.75f, 0.75f, 0.75f ));
    s6->setMaterial(wall);
    m_objects.push_back(s6);
    
    Texture *t = new Texture("../img/wall2.jpg");
    t->setBorder(-6.0, 6.0, -6.0, 6.0);
    Object* s7 = new Plane(Vec3(0.0, 0.0, 1.0), -10.0);
    s7->setTexture(t);
    s7->setMaterial(wall);
    m_objects.push_back(s7);
    
    Light* light1 = new CircleLight(Vec3(0.0, 6.0, 5.0), 1.5);
    light1->setColor(Vec3(1.0f, 1.0f, 1.0f));
    m_lights.push_back(light1);
    
    m_object_num = int(m_objects.size());
    m_light_num = int(m_lights.size());
}
