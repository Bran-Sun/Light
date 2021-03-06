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

enum NodeType
{
    LEAF = 0,
    X = 1,
    Y = 2,
    Z = 3
};

struct PhNode
{
    Photon *photon;
    PhNode *lc, *rc;
    NodeType type;
};

class PhotonMap
{
public:
    
    PhotonMap() {
        m_size = 0;
        m_r = 0.08;
        m_head = NULL;
    }
    void setEmitNum(int num) { m_whole_num = num; }
    void setRadius(double r) { m_r = r; }
    void build(); //build kd_tree
    void sort(int l, int r, int mid, NodeType type);
    bool comp(Photon* p1, Photon* p2, NodeType type);
    bool comp(Photon *p, double v, NodeType type);
    double get_pos_value(Photon *p, NodeType type);
    Vec3 Nearest(Vec3 &point, Vec3 &N);
    void cal_color(PhNode *node);
    double dis(PhNode* node, const Vec3 &v);
    
    PhNode* balance(int l, int r, NodeType type);
    
    void store(Photon* p) {
        m_photons.push_back(p);
        m_size++;
    }
    void swap(int l, int r);
    
private:
    std::vector<Photon*> m_photons;
    int m_size, m_whole_num, m_search;
    PhNode *m_head;
    Vec3 m_point, m_color, m_norm;
    int build_node;
    double m_r;
};


#endif //LIGHT_PHOTONMAP_H
