//
// Created by 孙桢波 on 2018/6/9.
//

#include "Bezier.h"

InterResult Bezier::intersection(Ray &ray, double &dist)
{
    bool find = false;
    Vec3 best_arg(1000.0, 0.0, 0.0);
    std::vector<Vec2> cand;
    double x, y;
    double raydx = ray.GetDirection().m_x;
    double raydz = ray.GetDirection().m_z;
    double rayox = m_pos.m_x - ray.GetOrigin().m_x;
    double rayoz = m_pos.m_z - ray.GetOrigin().m_z;
    
    
    for (double i = 0.0; i <= 1.0; i += 0.1)
    {
        cal_xy(x, y, i);
        if ( fabs(ray.GetDirection().m_y) > 1e-5 )
        {
            double t1 = (-ray.GetOrigin().m_y + m_pos.m_y + y) / ray.GetDirection().m_y;
            if ( t1 > 1e-5  ) cand.push_back(Vec2(t1, i));
        }
        double v = sqrt(raydx * raydx + raydz * raydz);
        if ( v > 1e-5 )
        {
            double tem = x * x - rayox * rayox - rayoz * rayoz +
                         (rayox * raydx + rayoz * raydz) * (rayox * raydx + rayoz * raydz);
            if ( tem > 0 )
            {
                double t = (rayox * raydx + rayoz * raydz - sqrt(tem)) / v;
                if ( t > 1e-5 ) cand.push_back(Vec2(t, i));
            }
        }
    }
    int size = cand.size();
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            Vec3 st(cand[i].x, cand[i].y, 1.57 * j + atan2(ray.GetOrigin().m_x + ray.GetDirection().m_x * cand[i].x - m_pos.m_x,
                                                ray.GetOrigin().m_z + ray.GetDirection().m_z * cand[i].x - m_pos.m_z));
            if ( cal_arg(ray, st))
            {
                if ((st.m_x < best_arg.m_x) && (st.m_x > 0) && (st.m_y >= 0.0) && (st.m_y <= 1.0))
                {
                    best_arg = st;
                }
            }
        }
    }
    
    if (best_arg.m_x >= dist) return MISS;
    else {
        m_arg = best_arg;
        dist = m_arg.m_x;
        return HIT;
    }
}

void Bezier::gen_poly()
{
    x_poly.assign(m_deg + 1, 0.0);
    y_poly.assign(m_deg + 1, 0.0);
    x_derive.assign(m_deg, 0.0);
    y_derive.assign(m_deg, 0.0);
    
    double *s = (double*)malloc((m_deg + 1) * sizeof(double));
    double *v = (double*)malloc((m_deg + 1) * sizeof(double));
    s[0] = 1;
    double c = 1;
    for ( int i = 0; i <= m_deg; i++) {
        double sign = 1.0;
        for (int j = 0; j <= i; j++) {
            x_poly[m_deg - i + j] += c * m_points[i].x * sign * s[j];
            y_poly[m_deg - i + j] += c * m_points[i].y * sign * s[j];
            sign = -sign;
        }
        if (i < m_deg)
        {
            v[0] = 1;
            for ( int j = 1; j <= i; j++ )
                v[j] = s[j] + s[j - 1];
            v[i + 1] = 1;
            for (int j = 0; j <= i + 1; j++)
                s[j] = v[j];
            c = c * (m_deg - i) / (i + 1);
        }
    }
    for (int j = 0; j < m_deg; j++) {
        x_derive[j] = (j + 1) * x_poly[j + 1];
        y_derive[j] = (j + 1) * y_poly[j + 1];
    }
    
    for (int i = 0; i <= m_deg; i++) {
        if (m_mid < (m_points[i].y / 2)) m_mid = m_points[i].y / 2;
    }
    for (int i = 0; i <= m_deg; i++)
    {
        double r = sqrt(m_points[i].x * m_points[i].x + (m_points[i].y - m_mid) * (m_points[i].y * m_mid));
        if (m_r < r) m_r = r;
    }
}

void Bezier::cal_xy(double &x, double &y, double t)
{
    double u = 1.0;
    x = 0.0; y = 0.0;
    
    for (int i = 0; i <= m_deg; i++) {
        x += u * x_poly[i];
        y += u * y_poly[i];
        u *= t;
    }
}

Vec3 Bezier::getNormal(Vec3 &point)
{
    double dx, dy;
    cal_dxy(dx, dy, m_arg.m_y);
    return Vec3(dy * cos(m_arg.m_z), -dx, dy * sin(m_arg.m_z));
}

bool Bezier::cal_arg(Ray &ray, Vec3 &st)
{
    Vec3 pre_ans(0.0, 0.0, 0.0);
    double x, y, dx, dy;
    
    for ( int i = 0; i < 8; i++ ) {
        cal_xy(x, y, st.m_y);
        cal_dxy(dx, dy, st.m_y);
        F[0][0] = ray.GetDirection().m_x;
        F[1][0] = ray.GetDirection().m_y;
        F[2][0] = ray.GetDirection().m_z;
        F[0][1] = - cos(st.m_z) * dx;
        F[0][2] = x * sin(st.m_z);
        F[1][1] = - dy;
        F[1][2] = 0.0;
        F[2][1] = - dx * sin(st.m_z);
        F[2][2] = - x * cos(st.m_z);
        if (!cal_inverse_matrix()) return false;
        F_pre[0] = ray.GetOrigin().m_x + ray.GetDirection().m_x * st.m_x - x * cos(st.m_z) - m_pos.m_x;
        F_pre[1] = ray.GetOrigin().m_y + ray.GetDirection().m_y * st.m_x - y - m_pos.m_y;
        F_pre[2] = ray.GetOrigin().m_z + ray.GetDirection().m_z * st.m_x - x * sin(st.m_z) - m_pos.m_z;
        
        pre_ans = st;
        for (int j = 0; j < 3; j++) {
            st.m_x -= F[0][j] * F_pre[j];
            st.m_y -= F[1][j] * F_pre[j];
            st.m_z -= F[2][j] * F_pre[j];
        }
    }
    pre_ans = pre_ans - st;
    return fabs(pre_ans.m_x + pre_ans.m_y + pre_ans.m_z) <= 0.0001;
}

bool Bezier::cal_inverse_matrix()
{
    
    /*for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
            printf("%.6f\t", F[i][j]);
        printf("\n");
    }*/
    double det = F[0][0] * (F[1][1] * F[2][2] - F[1][2] * F[2][1]) - F[0][1] * (F[1][0] * F[2][2] - F[2][0] * F[1][2]) + F[0][2] * (F[1][0] * F[2][1] - F[2][0] * F[1][1]);
    if (det == 0.0) return false;
    //printf("det: %.6f\n", det);
    for (int i = 0; i < 3; i++)
    {
        for ( int j = 0; j < 3; j++ )
        {
            F_tem[i][j] = 1.0 / det * (F[next(j + 1)][next(i + 1)] * F[next(j + 2)][next(i + 2)] - F[next(j + 1)][next(i + 2)] * F[next(j + 2)][next(i + 1)]);
        }
    }
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            F[i][j] = F_tem[i][j];
    /*for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
            printf("%.6f\t", F[i][j]);
        printf("\n");
    }*/
    return true;
}

int Bezier::next(int i)
{
    if (i >= 3) return i - 3;
    else return i;
}

void Bezier::cal_dxy(double &dx, double &dy, double t)
{
    dx = 0.0; dy = 0.0;
    double u = 1.0;
    for (int i = 0; i < m_deg; i++) {
        dx += u * x_derive[i];
        dy += u * y_derive[i];
        u = u * t;
    }
}

