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
    Ray(Vec3 ori, Vec3 dir) : m_ori(ori), m_dir(dir) { m_dir.Normalize(); }
    Vec3& GetOrigin() { return m_ori; }
    Vec3& GetDirection() { return m_dir; }
private:
    Vec3 m_ori, m_dir;
};


class Camera
{
public:
    Camera(int width, int height) : m_width(width), m_height(height), m_image(height, width, CV_32FC3) {}
    void set_picSize(float width, float height) {
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
    
    void show(std::string name) {
        cv::imshow(name, m_image);
        cv::waitKey(0);
    }
private:
    cv::Mat m_image;
    int m_width, m_height;
    float m_picWidth, m_picHeight, m_wInv, m_hInv, m_Left, m_Top;
    Vec3 m_o;
};


#endif //LIGHT_CAMERA_H
