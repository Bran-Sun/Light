#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "Vec3.h"
#include "Camera.h"
#include "Scene.h"
#include "PhotonMapping.h"

const int WINDOW_HEIGHT = 800;
const int WINDOW_WIDTH = 800;
const double PIC_HEIGHT = 11.0;
const double PIC_WIDTH = 11.0;


int main()
{
    Camera camera(WINDOW_WIDTH, WINDOW_HEIGHT);
    camera.set_picSize(PIC_WIDTH, PIC_HEIGHT);
    Vec3 eyePoint(0.0, 0.0, -12.0);
    camera.set_eyePoint(eyePoint);
    
    camera.setFocus(7.0);
    
    Scene scene;
    scene.init();
    
    //DrawWithRayTrace(camera, scene);
    DrawWithPhotonMapping(camera, scene);
    
    camera.save("RayTrace.png");
    camera.show("RayTrace");
    return 0;
}