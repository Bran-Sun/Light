//
// Created by 孙桢波 on 2018/6/4.
//

#include "PhotonMapping.h"
#include "PhotonMap.h"
#include <ctime>
#include <cstdlib>
#include <omp.h>

#define Square 512

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
    
    int i;
    
    if (camera.is_depth()) {
#pragma omp parallel for num_threads(2) schedule(dynamic, 40)
        for ( i = 0; i < camera.get_height(); i++ )
        {
            for ( int j = 0; j < camera.get_width(); j++ )
            {
                Vec3 sumColor(0, 0, 0);
                for (int k = 0; k < 15; k++)
                {
                    Ray ray = camera.GetFocusRay(j, i);
                    Vec3 color(0, 0, 0);
                    PhotonRayTrace(ray, 1, color, scene, photon_map);
                    sumColor += color;
                }
                sumColor /= 15;
                //std::cout << sumColor << std::endl;
                camera.set_color(j, i, sumColor * Square);
            }
            if ( i % 40 == 0 ) printf("finish line %d\n", i);
        }
    }
    else
    {
#pragma omp parallel for num_threads(2)
        for ( i = 0; i < camera.get_height(); i++ )
        {
            for ( int j = 0; j < camera.get_width(); j++ )
            {
                Vec3 sumColor(0, 0, 0);
                Vec3 pos = camera.GetPixelCenter(j, i);
    
                /*for (int k = -1; k < 2; k++)
                    for (int t = -1; t < 2; t++)
                    {
                        Vec3 new_pos(pos.m_x + k * wInv / 4, pos.m_y + t * hInv / 4, pos.m_z);
                        Ray ray(eye_point, new_pos - eye_point, false);
                        Vec3 color;
                        PhotonRayTrace(ray, 1, color, scene, photon_map);
                        //std::cout << color << std::endl;
                        sumColor += color * weight[k + 1][t + 1];
                    }*/
                Ray ray(eye_point, pos - eye_point);
                PhotonRayTrace(ray, 1, sumColor, scene, photon_map);
                //std::cout << sumColor << std::endl;
                camera.set_color(j, i, sumColor * Square);
            }
            if ( i % 40 == 0 ) printf("finish line %d\n", i);
        }
    }
    /*Ray ray = camera.GetRay(273, 390);
    Vec3 color;
    PhotonRayTrace(ray, 1, color, scene, photon_map);
    camera.set_color(273, 390, color);
    std::cout << color * Square << std::endl;*/
    
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
#pragma omp parallel for num_threads(2)
        for (count = 0; count < n_max; count++)
        {
            Photon* photon = (Photon*)malloc(sizeof(Photon));
            Ray ray = light->GetEmitRay();
            photon->pos = ray.GetOrigin();
            photon->dir = ray.GetDirection();
            photon->color = light->getColor();
            PhotonTrace(photon, scene, photon_map, 1, false);
            if (count % 200000 == 0) printf("emit %d photons\n", count);
        }
    }
}

void PhotonTrace(Photon* photon, Scene& scene, PhotonMap &photon_map, int depth, bool is_refr)
{
    if (depth > MAX_DEPTH) {
        free(photon);
        return;
    }
    
    Ray ray(photon->pos, photon->dir, is_refr);
    InterResult  res = MISS, res_tmp;
    Object *min_ob = NULL;
    Vec3 intersectionPoint, N, V, L, tem_N;
    double dist = 10000.0;
    for (int i = 0; i < scene.getObjectNum(); i++)
    {
        Object *ob = scene.getObject(i);
        res_tmp = ob->intersection(ray, dist, tem_N);
        if (res_tmp != MISS)
        {
            res = res_tmp;
            min_ob = ob;
            N = tem_N;
        }
    }
    
    if (res == MISS) {
        free(photon);
        return;
    }
    
    intersectionPoint = ray.GetOrigin() + ray.GetDirection() * dist;
    N.Normalize();
    V = ray.GetOrigin() - intersectionPoint;
    V.Normalize();
    if (N.Dot(V) < 0) N = -N;
    
    double diff = min_ob->getDiff();
    double spec = min_ob->getSpec();
    double refr = min_ob->getRefr();
    double prob = ((double)(rand() % RAND_MAX) / RAND_MAX);
    Vec3 ob_color(0, 0, 0);
    Texture *t = min_ob->getTexture();
    if (t == nullptr) ob_color = min_ob->getColor();
    else ob_color = t->getColor(intersectionPoint);
    //whether store the photon
    if (diff > 1e-5) {
        Photon *storePhoton = (Photon*)malloc(sizeof(Photon));
        *storePhoton = *photon;
        storePhoton->pos = intersectionPoint;
        //std::cout << intersectionPoint << std::endl;
        //storePhoton->color *= min_ob->getColor() / min_ob->getColor().Power();
#pragma omp critical
        photon_map.store(storePhoton);
    }
    
    //Diffuse reflect
    if (prob < diff) {
        photon->dir = N.rand_dir();
        photon->pos = intersectionPoint + photon->dir * 0.00001;
        photon->color *= ob_color / ob_color.Power();
        PhotonTrace(photon, scene, photon_map, depth + 1, is_refr);
    }//specular reflect
    else if (prob < (diff + spec)) {
        photon->dir = N.refl(-photon->dir);
        photon->pos = intersectionPoint + photon->dir * 0.00001;
        photon->color *= ob_color / ob_color.Power();
        PhotonTrace(photon, scene, photon_map, depth + 1, is_refr);
    }
    else if (prob < (diff + spec + refr)){
        double eta;
        if (res == INSIDE_HIT) eta = (1.0 / min_ob->getRefractionIndex());
        else eta = min_ob->getRefractionIndex();
        double cos1 = N.Dot(V);
        double cos2 = sqrt(1 - 1 / (eta * eta) * (1 - cos1 * cos1));
        Vec3 newdir = - V * 1.0 / eta - N * (cos2 - 1.0 / eta * cos1);
        newdir.Normalize();
        photon->dir = newdir;
        photon->pos = intersectionPoint + photon->dir * 0.00001;
        photon->color *= ob_color / ob_color.Power();
        PhotonTrace(photon, scene, photon_map, depth + 1, !is_refr);
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
    Vec3 intersectionPoint, N, V, L, tem_N;
    double dist = 100.0;
    for (int i = 0; i < ob_num; i++)
    {
        Object *ob = scene.getObject(i);
        res_tmp = ob->intersection(ray, dist, tem_N);
        if (res_tmp != MISS)
        {
            res = res_tmp;
            min_ob = ob;
            N = tem_N;
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
        color.set( (1.0 / Square), (1.0 / Square), (1.0 / Square));
        return;
    }
    if (res == MISS) return;
    intersectionPoint = ray.GetOrigin() + ray.GetDirection() * dist;
    
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
    else {
        ob_color = t->getColor(intersectionPoint);
        //std::cout << ob_color << std::endl;
    }
    //diff
#pragma omp critical
    if (diff > 1e-5)
        color += photon_map.Nearest(intersectionPoint, N) * ob_color * diff;
        //std::cout << depth << " diff " << color << std::endl;
    //spec
    if (spec > 1e-5) {
        Vec3 newdir = N * 2 * N.Dot(V) - V;
    
        Ray newray(intersectionPoint + newdir * 0.00001, newdir, ray.is_refr());
        Vec3 refcolor;
        PhotonRayTrace(newray, depth + 1, refcolor, scene, photon_map);
        color += refcolor * spec * ob_color;
        //std::cout << depth << " spec " << refcolor << std::endl;
    }
    //refraction
    if (refr > 1e-5) {
        double eta;
        if (res == INSIDE_HIT) eta = (1.0 / min_ob->getRefractionIndex());
        else eta = min_ob->getRefractionIndex();
        double cos1 = N.Dot(V);
        double cos2 = sqrt(1 - 1 / (eta * eta) * (1 - cos1 * cos1));
        Vec3 newdir = - V * 1.0 / eta - N * (cos2 - 1.0 / eta * cos1);
        newdir.Normalize();
        Ray newray(intersectionPoint + newdir * 0.00001, newdir, !ray.is_refr());
        Vec3 refrcolor;
        PhotonRayTrace(newray, depth + 1, refrcolor, scene, photon_map);
        color += refrcolor * refr * ob_color;
        //std::cout << depth << " refra " << refrcolor << std::endl;
    }
    //std::cout << depth << " sum " <<  color << std::endl;
}