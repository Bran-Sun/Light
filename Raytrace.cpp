//
// Created by 孙桢波 on 2018/6/4.
//

#include "RayTrace.h"

void RayTrace(Ray ray, int depth, double weight, Vec3 &color, Scene &scene)
{
    color.set(0.0, 0.0, 0.0);  //also background color
    
    //end condition
    if ( weight < MIN_WEIGHT ) return;
    if ( depth >= MAX_DEPTH ) return;
    
    //whether there are intersections
    double dist = 10000.0;
    InterResult res = MISS, res_tmp;
    Object *min_object = NULL;
    Vec3 intersectPoint, N, V, ob_color, tem_N;
    
    for ( int i = 0; i < scene.getObjectNum(); i++ )
    {
        Object *ob = scene.getObject(i);
        res_tmp = ob->intersection(ray, dist, tem_N);
        if ( res_tmp != MISS)
        {
            res = res_tmp;
            min_object = ob;
            N = tem_N;
        }
    }
    
    if ( res == MISS ) return;
    else {
        intersectPoint = ray.GetOrigin() + ray.GetDirection() * dist;
        N.Normalize();
        V = ray.GetOrigin() - intersectPoint;
        V.Normalize();
        if (N.Dot(V) < 0) N = -N;
        ob_color = min_object->getColor();
    }
    
    color = color +  min_object->getColor() * Vec3(0.1, 0.1, 0.1);
    
    //whethter is in the shade and calculate the color
    for (int i = 0; i < scene.getLightNum(); i++)
    {
        bool isShade = false;
        
        Light* light = scene.getLight(i);
        Vec3 newdir = light->getCenter() - intersectPoint;
        double d = newdir.Length();
        Vec3 L = newdir;
        L.Normalize();
        Ray newray(intersectPoint + newdir * 0.0001, newdir);
        
        for (int j = 0; j < scene.getObjectNum(); j++)
        {
            Object* ob = scene.getObject(j);
            InterResult r = ob->intersection(newray, d, tem_N);
            if (r != MISS) {
                isShade = true;
                break;
            }
        }
        if (!isShade) {
            //std::cout << "great!" << std::endl;
            Vec3 v = light->getColor();
            Vec3 R = N * 2 * N.Dot(L) - L;
            //diffuse component
            double dot = L.Dot(N);
            if (dot > 0) {
                color = color + v * dot * min_object->getDiff() * ob_color;
            }
            
            //specular component;
            double spec = (double)1.0 - min_object->getDiff();
            dot = V.Dot(R);
            if (dot > 0) {
                color = color + v * spec * powf(dot, 2) * ob_color;
            }
        }
    }
    
    double ref = min_object->getRefl();
    if (ref > 0)
    {
        Vec3 newdir = N * 2 * N.Dot(V) - V;
        Ray newray((intersectPoint + newdir * 0.0001), newdir);
        Vec3 RefColor;
        RayTrace(newray, depth + 1, weight * ref, RefColor, scene);
        color = color + (RefColor * ref) * ob_color;
    }
}

void DrawWithRayTrace(Camera &camera, Scene &scene) {
    
    for (int i = 0; i < camera.get_height(); i++)
        for (int j = 0; j < camera.get_width(); j++)
        {
            Ray ray = camera.GetRay(j, i);
            Vec3 color;
            RayTrace(ray, 1, 1.0, color, scene);
            camera.set_color(j, i, color);
        }
    /*
    Ray ray = camera.GetRay(399, 399);
    std::cout << ray.GetDirection() << std::endl;
    Vec3 color;
    RayTrace(ray, 1, 1.0, color, scene);
    camera.set_color(399, 399, color);
    std::cout << color << std::endl;*/
}