//
// Created by 孙桢波 on 2018/6/4.
//

#ifndef LIGHT_PHOTONMAPPING_H
#define LIGHT_PHOTONMAPPING_H

#include "Scene.h"
#include "PhotonMap.h"

const int MAX_DEPTH = 6;
const float MIN_WEIGHT = 0.0001;

void DrawWithPhtonMapping(Camera &camera, Scene& scene);
void generate_photons(Scene &scene, PhotonMap &photon_map);

#endif //LIGHT_PHOTONMAPPING_H
