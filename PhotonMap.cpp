//
// Created by 孙桢波 on 2018/6/3.
//

#include "PhotonMap.h"
#include <algorithm>

void PhotonMap::build()
{
    printf("building kd-tree with %d photon...\n", m_size);
    m_head = balance(0, m_size, X);
    printf("building finished!\n");
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
    sort(l, r, type);
    node->photon = m_photons[mid];
    node->lc = balance(l, mid, NodeType((type % 3) + 1));
    node->rc = balance(mid + 1, r, NodeType((type % 3) + 1));
    node->type = type;
    return node;
}

void PhotonMap::sort(int st, int ed, NodeType type)
{
    if (st >= (ed - 1)) return;
    swap(st, st + rand() % (ed - st));
    int lo = st, hi = ed - 1;
    float pivot = get_pos_value(m_photons[st], type);
    Photon *p = m_photons[lo];
    while (lo < hi) {
        while (lo < hi) {
            if (pivot < get_pos_value(m_photons[hi], type)) hi--;
            else {
                m_photons[lo++] = m_photons[hi];
                break;
            }
        }
        while (lo < hi) {
            if (get_pos_value(m_photons[lo], type) < pivot) lo++;
            else {
                m_photons[hi--] = m_photons[lo]; break;
            }
        }
    }
    m_photons[lo] = p;
    sort(st, lo, type);
    sort(lo + 1, ed, type);
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

bool PhotonMap::comp(Photon *p, float v, NodeType type)
{
    if (type == X) return p->pos.m_x < v;
    if (type == Y) return p->pos.m_y < v;
    if (type == Z) return p->pos.m_z < v;
    return false;
}

float PhotonMap::get_pos_value(Photon *p, NodeType type)
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

Vec3 PhotonMap::Nearest(Vec3 &point, Vec3 &N)
{
    m_color.set(0, 0, 0);
    m_point = point;
    m_norm = N;
    m_search = 0;
    
    cal_color(m_head);
    //std::cout << m_search << std::endl;
    m_color = m_color / (3.14 * m_r * m_r * m_whole_num);
    //std::cout << m_color << std::endl;
    
    return m_color;
}

void PhotonMap::cal_color(PhNode *node)
{
    // if is the leaf
    if (node->type == LEAF) {
        Vec3 d = m_point - node->photon->pos;
        if (d.Length() < m_r) {
            Photon *p = node->photon;
            float f = m_norm.Dot(-p->dir);
            if (f > 0.0) {
                m_color += p->color * f;
                m_search++;
            }
        }
        return;
    }
    
    Photon* p = node->photon;
    
    float b = dis(node, m_point);
    if (b > 0) {
        if (node->lc) cal_color(node->lc);
        Vec3 d = m_point - p->pos;
        if (d.Length() < m_r) {
            float f = m_norm.Dot(-p->dir);
            if (f > 0.0) {
                m_color += p->color * f;
                m_search++;
            }
        }
        if ((abs(b) < m_r) && node->rc) cal_color(node->rc);
    }
    else {
        if (node->rc) cal_color(node->rc);
        Vec3 d = m_point - p->pos;
        if (d.Length() < m_r) {
            float f = m_norm.Dot(-p->dir);
            if (f > 0.0) {
                m_color += p->color * f;
                m_search++;
            }
        }
        if ((abs(b) < m_r) && node->lc) cal_color(node->lc);
    }
}

float PhotonMap::dis(PhNode *node, const Vec3 &v)
{
    if (node->type == X) {
        return node->photon->pos.m_x - v.m_x;
    }
    
    if (node->type == Y) {
        return node->photon->pos.m_y - v.m_y;
    }
    if (node->type == Z) {
        return node->photon->pos.m_z - v.m_z;
    }
    return 0.0;
}


