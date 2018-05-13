//
// Created by 孙桢波 on 2018/5/13.
//

#ifndef LIGHT_SCENE_H
#define LIGHT_SCENE_H

#include "Object/Object.h"
#include "Object/Sphere.h"
#include "Light.h"
#include <vector>


class Scene
{
public:
    Scene() {}
    void init();
    int getObjectNum() { return m_object_num; }
    int getLightNum() { return m_light_num; }
    Object* getObject(int id) { return m_objects[id]; }
    Light* getLight(int id) { return m_lights[id]; }
private:
    int m_object_num, m_light_num;
    std::vector<Object*> m_objects;
    std::vector<Light*> m_lights;
};


#endif //LIGHT_SCENE_H
