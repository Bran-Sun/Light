//
// Created by 孙桢波 on 2018/6/9.
//

#ifndef LIGHT_BEZIER_H
#define LIGHT_BEZIER_H

#include "Object.h"

/*assume the curve is around y axis*/

struct Vec2
{
    Vec2(double xx, double yy) : x(xx), y(yy) {}
    double x, y;
};

class Bezier: public Object
{
public:
    Bezier(std::vector<Vec2> points, Vec3 pos): Object() {
        m_points = points;
        m_pos = pos;
        m_deg = (int) m_points.size() - 1;
        
        m_r = 0.0;
        m_mid = 0.0;
        gen_poly();
        /*Vec3 o(0, 0, -5);
        Ray ray(o, Vec3(0.0, 0.1, 0) - o);
        
        double dist = 100.0;
        InterResult res = intersection(ray, dist);
        if (res == HIT) printf("success!\n");
        else printf("fail!\n");
        exit(0);*/
    }
    virtual InterResult intersection(Ray &ray, double &dist, Vec3 &N);
    virtual Vec3 getNormal(Vec3 &point);
    
private:
    void gen_poly();
    void cal_xy(double &x, double &y, double t);
    void cal_dxy(double &dx, double &dy, double t);
    bool cal_arg(Ray &ray, Vec3 &st);
    bool cal_inverse_matrix(std::vector<double> &F);
    inline int next(int i);
    
private:
    std::vector<Vec2> m_points;
    std::vector<double> x_poly, y_poly, x_derive, y_derive;
    Vec3 m_pos, m_arg;
    double m_r, m_mid;
    int m_deg;
};


#endif //LIGHT_BEZIER_H
