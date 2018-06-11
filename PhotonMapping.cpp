//
// Created by 孙桢波 on 2018/6/4.
//

#include "PhotonMapping.h"
#include "PhotonMap.h"
#include <ctime>
#include <cstdlib>

#define Square 600

void DrawWithPhotonMapping(Camera &camera, Scene& scene)
{
    PhotonMap photon_map;
    generate_photons(scene, photon_map);
    photon_map.build();
    
    printf("starting ray tracing...\n");
    
    double hInv, wInv;
    hInv = camera.get_wInv();
    wInv = camera.get_hInv();
    double weight[3][3] = {0.0625, 0.125, 0.0625, 0.125, 0.25, 0.125, 0.0625, 0.125, 0.0625};
    Vec3 eye_point = camera.GetEyePoint();
    
    for (int i = 0; i < camera.get_height(); i++)
    {
        for ( int j = 0; j < camera.get_width(); j++ )
        {
            Vec3 sumColor(0, 0, 0);
            Vec3 pos = camera.GetPixelCenter(j, i);
            
            for (int k = -1; k < 2; k++)
                for (int t = -1; t < 2; t++)
                {
                    Vec3 new_pos(pos.m_x + k * wInv / 4, pos.m_y + t * hInv / 4, pos.m_z);
                    Ray ray(eye_point, new_pos - eye_point);
                    Vec3 color;
                    PhotonRayTrace(ray, 1, color, scene, photon_map);
                    //std::cout << color << std::endl;
                    sumColor += color * weight[k + 1][t + 1];
                }
            //Ray ray(eye_point, pos - eye_point);
            //PhotonRayTrace(ray, 1, sumColor, scene, photon_map);
            //std::cout << sumColor << std::endl;
            camera.set_color(j, i, sumColor * Square);
        }
        if (i % 40 == 0) printf("finish line %d\n", i);
    }
    /*Ray ray = camera.GetRay(400, 400);
    Vec3 color;
    PhotonRayTrace(ray, 1, color, scene, photon_map);
    camera.set_color(400, 400, color);
    std::cout << color * 512.0 << std::endl;*/
    
    printf("tracing finished!\n");
}

void generate_photons(Scene &scene, PhotonMap &photon_map)
{
    printf("generating photons...\n");
    srand(unsigned(time(NULL)));
    
    int photon_num = scene.getEmitNum();
    photon_map.setEmitNum(photon_num);
    
    int light_num = scene.getLightNum();
    double total_power = 0.0;
    for (int i = 0; i < light_num; i++)
        total_power += scene.getLight(i)->getPower();
    
    for (int i = 0; i < light_num; i++)
    {
        Light* light = scene.getLight(i);
        double light_power = scene.getLight(i)->getPower();
        int n_max = int(photon_num * light_power / total_power);
        
        int count = 0;
        Vec3 light_pos = light->getCenter();
        while (count < n_max) {
            Photon* photon = (Photon*)malloc(sizeof(Photon));
            Ray ray = light->GetEmitRay();
            photon->pos = ray.GetOrigin();
            photon->dir = ray.GetDirection();
            photon->color = light->getColor();
            PhotonTrace(photon, scene, photon_map, 1);
            count++;
            if (count % 200000 == 0) printf("emit %d photons\n", count);
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
    double dist = 24.0;
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
    V.Normalize();
    if (N.Dot(V) < 0) N = -N;
    
    double diff = min_ob->getDiff();
    double spec = min_ob->getSpec();
    double refr = min_ob->getRefr();
    double prob = (double) ((double)(rand() % RAND_MAX) / RAND_MAX);
    Vec3 ob_color(0, 0, 0);
    Texture *t = min_ob->getTexture();
    if (t == nullptr) ob_color = min_ob->getColor();
    else ob_color = t->getColor(intersectionPoint.m_x, intersectionPoint.m_z);
    //whether store the photon
    if (diff > 1e-5) {
        Photon *storePhoton = (Photon*)malloc(sizeof(Photon));
        *storePhoton = *photon;
        storePhoton->pos = intersectionPoint;
        //std::cout << intersectionPoint << std::endl;
        //storePhoton->color *= min_ob->getColor() / min_ob->getColor().Power();
        photon_map.store(storePhoton);
    }
    
    //Diffuse reflect
    if (prob < diff) {
        photon->dir = N.rand_dir();
        photon->pos = intersectionPoint + photon->dir * 0.00001;
        photon->color *= ob_color;
        PhotonTrace(photon, scene, photon_map, depth + 1);
    }//specular reflect
    else if (prob < (diff + spec)) {
        photon->dir = N.refl(-photon->dir);
        photon->pos = intersectionPoint + photon->dir * 0.00001;
        photon->color *= ob_color;
        PhotonTrace(photon, scene, photon_map, depth + 1);
    }
    else if (prob < (diff + spec + refr)){
        double eta;
        if (res == INSIDE_HIT) eta = (double) (1.0 / min_ob->getRefractionIndex());
        else eta = min_ob->getRefractionIndex();
        double cos1 = N.Dot(V);
        double cos2 = sqrt(1 - 1 / (eta * eta) * (1 - cos1 * cos1));
        Vec3 newdir = - V * 1.0 / eta - N * (cos2 - 1.0 / eta * cos1);
        newdir.Normalize();
        photon->dir = newdir;
        photon->pos = intersectionPoint + photon->dir * 0.00001;
        photon->color *= ob_color;
        PhotonTrace(photon, scene, photon_map, depth + 1);
    }
    else {
        free(photon);
        return;
    }
}

void PhotonRayTrace(Ray ray, int depth, Vec3 &color, Scene &scene, PhotonMap &photon_map)
{
    color.set(0, 0, 0);
    if (depth > MAX_DEPTH) return;
    
    int ob_num = scene.getObjectNum();
    InterResult  res = MISS, res_tmp;
    Object *min_ob = nullptr;
    Vec3 intersectionPoint, N, V, L;
    double dist = 100.0;
    for (int i = 0; i < ob_num; i++)
    {
        Object *ob = scene.getObject(i);
        res_tmp = ob->intersection(ray, dist);
        if (res_tmp != MISS)
        {
            res = res_tmp;
            min_ob = ob;
        }
    }
    
    int light_num = scene.getLightNum();
    InterResult res_light = MISS, res_tmp_light;
    double dist_light = 100.0;
    for (int i = 0; i < light_num; i++)
    {
        Light *ob = scene.getLight(i);
        res_tmp_light = ob->intersection(ray, dist_light);
        if (res_tmp_light != MISS)
        {
            res_light = res_tmp_light;
        }
    }
    if (res_light != MISS && dist_light <= dist) {
        color.set((double) (1.0 / Square), (double) (1.0 / Square), (double) (1.0 / Square));
        return;
    }
    if (res == MISS) return;
    intersectionPoint = ray.GetOrigin() + ray.GetDirection() * dist;
    
    N = min_ob->getNormal(intersectionPoint);
    N.Normalize();
    V = ray.GetOrigin() - intersectionPoint;
    V.Normalize();
    if (N.Dot(V) < 0) N = -N;
    
    double refl = min_ob->getRefl();
    double diff = min_ob->getDiff();
    double spec = min_ob->getSpec();
    double refr = min_ob->getRefr();
    Vec3 ob_color(0, 0, 0);
    Texture *t = min_ob->getTexture();
    if (t == nullptr) ob_color = min_ob->getColor();
    else ob_color = t->getColor(intersectionPoint.m_x, intersectionPoint.m_z);
    //diff
    if (diff > 1e-5)
        color += photon_map.Nearest(intersectionPoint, N) * ob_color * diff;
    //spec
    if (spec > 1e-5) {
        Vec3 newdir = N * 2 * N.Dot(V) - V;
    
        Ray newray(intersectionPoint + newdir * 0.00001, newdir);
        Vec3 refcolor;
        PhotonRayTrace(newray, depth + 1, refcolor, scene, photon_map);
        color += refcolor * spec * ob_color;
        //std::cout << depth << " spec " << refcolor << std::endl;
    }
    //refraction
    if (refr > 1e-5) {
        double eta;
        if (res == INSIDE_HIT) eta = (double) (1.0 / min_ob->getRefractionIndex());
        else eta = min_ob->getRefractionIndex();
        double cos1 = N.Dot(V);
        double cos2 = sqrt(1 - 1 / (eta * eta) * (1 - cos1 * cos1));
        Vec3 newdir = - V * 1.0 / eta - N * (cos2 - 1.0 / eta * cos1);
        newdir.Normalize();
        Ray newray(intersectionPoint + newdir * 0.00001, newdir);
        Vec3 refrcolor;
        PhotonRayTrace(newray, depth + 1, refrcolor, scene, photon_map);
        color += refrcolor * refr *ob_color;
        //std::cout << depth << " refra " << refrcolor << std::endl;
    }
    //std::cout << refcolor << std::endl;
}