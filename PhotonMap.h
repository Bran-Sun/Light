//
// Created by 孙桢波 on 2018/6/3.
//

#ifndef LIGHT_PHOTONMAP_H
#define LIGHT_PHOTONMAP_H

#include "Vec3.h"
#include "Scene.h"
#include <vector>

struct Photon
{
    Vec3 pos, dir, color;
};

struct PhNode
{
    Photon *photon;
    PhNode *lc, *rc;
    PhotonMap::NodeType type;
};

class PhotonMap
{
public:
    enum NodeType
    {
        LEAF = 0,
        X,
        Y,
        Z
    };
    
    PhotonMap() {
        m_size = 0;
        m_r = 0.0;
        m_head = NULL;
    }
    void setEmitNum(int num) { m_whole_num = num; }
    void setRadius(float r) { m_r = r; }
    void build(); //build kd_tree
    void split(int l, int r, int mid, NodeType);
    bool comp(Photon* p1, Photon* p2, NodeType type);
    bool comp(Photon *p, float v, NodeType type);
    float get_pos_value(Photon *p, NodeType type);
    
    float get_mid(float p1, float p2, float p3);
    
    PhNode* balance(int l, int r, NodeType type);
    
    void store(Photon* p) {
        m_photons.push_back(p);
        m_size++;
    }
    void swap(int l, int r);
    
private:
    std::vector<Photon*> m_photons;
    int m_size, m_whole_num;
    PhNode *m_head;
    float m_r;
};


#endif //LIGHT_PHOTONMAP_H
