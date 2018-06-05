//
// Created by 孙桢波 on 2018/6/3.
//

#include "PhotonMap.h"

void PhotonMap::build()
{
    m_head = balance(0, m_size, X);
}

PhNode *PhotonMap::balance(int l, int r, NodeType type)
{
    if (l == r) return nullptr;
    PhNode *node = (PhNode*)malloc(sizeof(PhNode));
    if (l == (r - 1)) {
        node->type = LEAF;
        node->lc = nullptr;
        node->rc = nullptr;
        node->photon = m_photons[l];
        return node;
    }
    int mid = (l + r) >> 1;
    split(l, r, mid, type);
    node->photon = m_photons[mid];
    node->lc = balance(l, mid, (type % 3) + 1);
    node->rc = balance(mid + 1, r, (type % 3) + 1);
    node->type = type;
    return node;
}

void PhotonMap::split(int st, int ed, int mid, NodeType type)
{
    while (st < ed) {
        int l = st, r = ed - 1;
        float key = get_pos_value(m_photons[r], type);
        do {
            while (get_pos_value(m_photons[l], type) < key) l++;
            while ((get_pos_value(m_photons[r], type) >= key) && (r > l)) r--;
            if (l < r) {
                swap(l, r);
                l++; r--;
            }
        } while (l < r);
        
    }
}

bool PhotonMap::comp(Photon *p1, Photon *p2, NodeType type)
{
    if (type == X) {
        return p1->pos.m_x < p2->pos.m_x;
    }
    if (type == Y) {
        return p1->pos.m_y < p2->pos.m_y;
    }
    if (type == Z) {
        return p1->pos.m_z < p2->pos.m_z;
    }
    return false;
}

float PhotonMap::get_mid(float p1, float p2, float p3)
{
    if (p1 < p2) {
        if (p2 < p3) return p2;
        else if (p1 < p3) return p3;
        else return p1;
    }
    else if (p2 > p3) return p2;
    else if (p1 > p3) return p3;
    else return p1;
}

bool PhotonMap::comp(Photon *p, float v, PhotonMap::NodeType type)
{
    if (type == X) return p->pos.m_x < v;
    if (type == Y) return p->pos.m_y < v;
    if (type == Z) return p->pos.m_z < v;
    return false;
}

float PhotonMap::get_pos_value(Photon *p, PhotonMap::NodeType type)
{
    if (type == X) return p->pos.m_x;
    if (type == Y) return p->pos.m_y;
    if (type == Z) return p->pos.m_z;
    return p->pos.m_x;
}

void PhotonMap::swap(int l, int r)
{
    Photon* tem = m_photons[l];
    m_photons[l] = m_photons[r];
    m_photons[r] = tem;
}


