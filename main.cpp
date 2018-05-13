#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "Vec3.h"
#include "Camera.h"
#include "Scene.h"

const int WINDOW_HEIGHT = 800;
const int WINDOW_WIDTH = 800;
const float PIC_HEIGHT = 10.0;
const float PIC_WIDTH = 10.0;
const float MIN_WEIGHT = 0.0001;
const int MAX_DEPTH = 4;

void RayTrace(Ray ray, int depth, float weight, Vec3 &color, Scene &scene)
{
    color.set(0.0, 0.0, 0.0);  //also background color
    
    //end condition
    if ( weight < MIN_WEIGHT ) return;
    if ( depth >= MAX_DEPTH ) return;
    
    //whether there are intersections
    float dist = 10000.0, dist_tmp = 0.0;
    InterResult res = MISS, res_tmp;
    Object *min_object = NULL;
    Vec3 intersectPoint, N, V, ob_color;
    
    for ( int i = 0; i < scene.getObjectNum(); i++ )
    {
        Object *ob = scene.getObject(i);
        res_tmp = ob->intersection(ray, dist_tmp);
        if ( res_tmp != MISS && dist_tmp < dist )
        {
            res = res_tmp;
            dist = dist_tmp;
            min_object = ob;
        }
    }
    
    if ( res == MISS ) return;
    else {
        intersectPoint = ray.GetOrigin() + ray.GetDirection() * dist;
        //std::cout << intersectPoint << std::endl;
        N = min_object->getNormal(intersectPoint);
        //std::cout << N << std::endl;
        N.Normalize();
        V = ray.GetOrigin() - intersectPoint;
        V.Normalize();
        ob_color = min_object->getColor();
    }
    
    //whethter is in the shade and calculate the color
    for (int i = 0; i < scene.getLightNum(); i++)
    {
        bool isShade = false;
        
        Light* light = scene.getLight(i);
        Vec3 newdir = light->getCenter() - intersectPoint;
        Vec3 L = newdir;
        L.Normalize();
        Ray newray(light->getCenter(), newdir);
        
        for (int j = 0; j < scene.getObjectNum(); j++)
        {
            Object* ob = scene.getObject(j);
            float d;
            InterResult r = ob->intersection(newray, d);
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
            float dot = L.Dot(N);
            if (dot > 0) {
                color = color + v * dot * min_object->getDiff() * ob_color;
            }
            
            //specular component;
            float spec = (float)1.0 - min_object->getDiff();
            dot = V.Dot(R);
            if (dot > 0) {
                color = color + v * spec * powf(dot, 2) * ob_color;
            }
        }
    }
    
    float ref = min_object->getRef();
    if (ref > 0)
    {
        Vec3 newdir = N * 2 * N.Dot(V) - V;
        Ray newray((intersectPoint + newdir * 0.0001), newdir);
        Vec3 RefColor;
        RayTrace(newray, depth + 1, weight * ref, RefColor, scene);
        color = color + (RefColor * ref) * ob_color;
    }
}

void DrawPicture(Camera &camera, Scene &scene) {
    
    for (int i = 0; i < WINDOW_HEIGHT; i++)
        for (int j = 0; j < WINDOW_WIDTH; j++)
        {
            Ray ray = camera.GetRay(j, i);
            Vec3 color;
            RayTrace(ray, 1, 1.0, color, scene);
            camera.set_color(j, i, color);
        }
    /*
    Ray ray = camera.GetRay(299, 399);
    std::cout << ray.GetDirection() << std::endl;
    Vec3 color;
    RayTrace(ray, 1, 1.0, color, scene);
    camera.set_color(349, 399, color);
    std::cout << color << std::endl;*/
}

int main()
{
    Camera camera(WINDOW_WIDTH, WINDOW_HEIGHT);
    camera.set_picSize(PIC_WIDTH, PIC_HEIGHT);
    Vec3 eyePoint(0.0, 0.0, float(- 5.0));
    camera.set_eyePoint(eyePoint);
    
    Scene scene;
    scene.init();
    
    DrawPicture(camera, scene);
    camera.show("RayTrace");
    return 0;
}