//
// Created by 孙桢波 on 2018/6/4.
//

#include "PhotonMapping.h"
#include "PhotonMap.h"
#include <ctime>
#include <cstdlib>

void DrawWithPhtonMapping(Camera &camera, Scene& scene)
{
    PhotonMap photon_map;
    generate_photons(scene, photon_map);
    photon_map.build();
    
    
    for (int i = 0; i < camera.get_height(); i++)
        for (int j = 0; j < camera.get_width(); j++)
        {
            Ray ray = camera.GetRay(j, i);
            Vec3 color;
            PhotonTrace(ray);
            camera.set_color(j, i, color);
        }
}

void generate_photons(Scene &scene, PhotonMap &photon_map)
{
    srand(unsigned(time(0)));
    
    int photon_num = scene.getEmitNum();
    photon_map.setEmitNum(photon_num);
    
    int light_num = scene.getLightNum();
    float total_power = 0.0;
    for (int i = 0; i < light_num; i++)
        total_power += scene.getLight(i)->getPower();
    
    for (int i = 0; i < light_num; i++)
    {
        Light* light = scene.getLight(i);
        float light_power = scene.getLight(i)->getPower();
        int n_max = int(photon_num * light_power / total_power);
        
        int count = 0;
        Vec3 light_pos = light->getCenter();
        while (count < n_max) {
            Photon* photon = (Photon*)malloc(sizeof(Photon));
            Vec3 dir = light->GetEmitDir();
            photon->pos = light_pos;
            photon->dir = dir;
            photon->color = light->getColor();
            PhotonTrace(photon, scene, photon_map, 1);
            count++;
        }
    }
}

void PhotonTrace(Photon* photon, Scene& scene, PhotonMap &photon_map, int depth)
{
    if (depth > MAX_DEPTH) {
        free(photon);
        return;
    }
    
    Ray ray(photon->pos, photon->dir);
    InterResult  res = MISS, res_tmp;
    Object *min_ob = NULL;
    Vec3 intersectionPoint, N, V, L;
    float dist = 100.0;
    for (int i = 0; i < scene.getObjectNum(); i++)
    {
        Object *ob = scene.getObject(i);
        res_tmp = ob->intersection(ray, dist);
        if (res_tmp != MISS)
        {
            res = res_tmp;
            min_ob = ob;
        }
    }
    
    if (res == MISS) {
        free(photon);
        return;
    }
    
    intersectionPoint = ray.GetOrigin() + ray.GetDirection() * dist;
    N = min_ob->getNormal(intersectionPoint);
    N.Normalize();
    V = ray.GetOrigin() - intersectionPoint;
    if (N.Dot(V) < 0) N = -N;
    
    float diff = min_ob->getDiff();
    float refl = min_ob->getRef();
    float prob = (float) ((double)rand() / RAND_MAX);
    
    //whether store the photon
    if (diff > 1e-5) {
        Photon *storePhoton = (Photon*)malloc(sizeof(Photon));
        *storePhoton = *photon;
        storePhoton->pos = intersectionPoint;
        storePhoton->color *= min_ob->getColor();
        photon_map.store(storePhoton);
    }
    
    //Diffuse reflect
    if (prob < diff) {
        photon->pos = intersectionPoint;
        photon->color *= min_ob->getColor();
        photon->dir = N.rand_dir();
        PhotonTrace(photon, scene, photon_map, depth + 1);
    }
    else if (prob < (diff + refl)) {
        photon->pos = intersectionPoint;
        photon->color *= min_ob->getColor();
        photon->dir = N.refl(-photon->dir);
        PhotonTrace(photon, scene, photon_map, depth + 1);
    }
    else {
        free(photon);
        return;
    }
}
