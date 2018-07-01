//
// Created by 孙桢波 on 2018/5/13.
//

#ifndef LIGHT_CAMERA_H
#define LIGHT_CAMERA_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "Vec3.h"

class Ray
{
public:
    Ray() {}
    Ray(Vec3 ori, Vec3 dir) : m_ori(ori), m_dir(dir) { m_dir.Normalize(); m_is_refr = false; }
    Ray(Vec3 ori, Vec3 dir, bool is_refr): m_ori(ori), m_dir(dir), m_is_refr(is_refr) { m_dir.Normalize(); }
    void setOrigin(Vec3 ori) { m_ori = ori; }
    void setDirection(Vec3 dir) { m_dir = dir; }
    void setIsRefr(bool is_refr) { m_is_refr = is_refr; }
    void setRay(Vec3 ori, Vec3 dir) {
        m_ori = ori;
        m_dir = dir;
        m_dir.Normalize();
    }
    Vec3& GetOrigin() { return m_ori; }
    Vec3& GetDirection() { return m_dir; }
    bool is_refr() { return m_is_refr; }
private:
    Vec3 m_ori, m_dir;
    bool m_is_refr;
};


class Camera
{
public:
    Camera(int width, int height) : m_width(width), m_height(height), m_image(height, width, CV_32FC3) {
        m_is_depth = false;
    }
    void set_picSize(double width, double height) {
        m_picWidth = width; m_picHeight = height;
        m_wInv = width / m_width; m_hInv = height / m_height;
        m_Left = width / 2 - m_wInv / 2; m_Top = height / 2 - m_hInv / 2;
    }
    void set_eyePoint(Vec3 o) { m_o = o; }
    void set_color(int x, int y, Vec3 newcolor) {
        if (x >= m_width || x < 0 || y >= m_height || y < 0) return;
        cv::Vec3f color(newcolor.m_x, newcolor.m_y, newcolor.m_z);
        m_image.at<cv::Vec3f>(y, x) = color;
    }
    Ray GetRay(int x, int y) {
        Vec3 pix_pos(m_Left - x * m_wInv, m_Top - y * m_hInv, 0);
        return Ray(m_o, pix_pos - m_o);
    }
    
    Ray GetFocusRay(int x, int y) {
        Vec3 pix_pos(m_Left - x * m_wInv, m_Top - y * m_hInv, 0);
        Vec3 dir = pix_pos - m_o;
        dir.Normalize();
        Vec3 newd = m_o + dir * ((m_focus_dis - m_o.m_z) / dir.m_z);
        Vec3 neweye((2 * ((double)rand() / RAND_MAX) - 1) * m_aper, (2 * ((double)rand() / RAND_MAX) - 1) * m_aper, m_len_dis);
        neweye = neweye + pix_pos;
        return Ray(neweye, newd - neweye);
    }
    
    bool is_depth() { return m_is_depth; }
    void setFocus(double focusDis, double len_dis = 0.0) {
        m_is_depth = true;
        m_focus_dis = focusDis;
        m_aper = focusDis / 32.0;
        m_len_dis = 0.0;
    }
    Vec3 GetEyePoint() { return m_o; }
    Vec3 GetPixelCenter(int x, int y) { return Vec3(m_Left - x * m_wInv, m_Top - y * m_hInv, 0); }
    void show(std::string name) {
        cv::imshow(name, m_image);
        cv::waitKey(0);
    }
    
    void save(std::string name) {
        cv::Mat temp;
        m_image.convertTo(temp, CV_8UC3, 255.0);
        cv::imwrite(name.c_str(), temp);
    }
    
    int get_width() { return m_width; }
    int get_height() { return m_height; }
    double get_hInv() { return m_hInv; }
    double get_wInv() { return m_wInv; }
private:
    cv::Mat m_image;
    int m_width, m_height;
    bool m_is_depth;
    double m_focus_dis, m_aper, m_len_dis;
    double m_picWidth, m_picHeight, m_wInv, m_hInv, m_Left, m_Top;
    Vec3 m_o;
};


#endif //LIGHT_CAMERA_H
