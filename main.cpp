#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "Vec3.h"
#include "Camera.h"
#include "Scene.h"
#include "PhotonMapping.h"

const int WINDOW_HEIGHT = 800;
const int WINDOW_WIDTH = 800;
const float PIC_HEIGHT = 10.0;
const float PIC_WIDTH = 10.0;


int main()
{
    Camera camera(WINDOW_WIDTH, WINDOW_HEIGHT);
    camera.set_picSize(PIC_WIDTH, PIC_HEIGHT);
    Vec3 eyePoint(0.0, 0.0, float(- 5.0));
    camera.set_eyePoint(eyePoint);
    
    Scene scene;
    scene.init();
    
    //DrawWithRayTrace(camera, scene);
    DrawWithPhotonMapping(camera, scene);
    
    camera.save("RayTrace.png");
    camera.show("RayTrace");
    return 0;
}