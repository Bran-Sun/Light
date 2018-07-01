//
// Created by 孙桢波 on 2018/6/10.
//

#ifndef LIGHT_TEXTURE_H
#define LIGHT_TEXTURE_H

#include <opencv2/highgui/highgui.hpp>
#include "../Vec3.h"
//assume it is a picture of rectangle
class Texture
{
public:
    Texture(std::string filename) {
        cv::Mat image = cv::imread(filename);
        image.convertTo(m_image, CV_32FC3, 1 / 255.0);
        //cv::imshow("floor", m_image);
        //cv::waitKey(0);
        //std::cout << image << std::endl;
        //std::cout << m_image << std::endl;
    }
    void setBorder(double x0, double x1, double y0, double y1) {
        m_x0 = x0; m_x1 = x1; m_y0 = y0; m_y1 = y1;
    }
    Vec3 getColor(const Vec3 &p) {
        double x = p.m_x;
        double y = p.m_y;
        if ((x > m_x1) || (x < m_x0) || (y < m_y0) || (y > m_y1)) return Vec3(0.6, 0.6, 0.6);
        double x_pixel = (m_x1 - x) / (m_x1 - m_x0) * (m_image.rows - 3) + 1;
        double y_pixel = (m_y1 - y) / (m_y1 - m_y0) * (m_image.cols - 3) + 1;
        cv::Vec3f color;
        int dx = (int) floor(x_pixel);
        int dy = (int) floor(y_pixel);
        color += m_image.at<cv::Vec3f>(dy, dx) * (x_pixel - dx) * (y_pixel - dy) + m_image.at<cv::Vec3f>(dy, dx + 1) * (1 + dx - x_pixel) * (y_pixel - dy);
        color += m_image.at<cv::Vec3f>(dy + 1, dx) * (x_pixel - dx) * (1 + dy - y_pixel) + m_image.at<cv::Vec3f>(dy + 1, dx + 1) * (1 + dx - x_pixel) * (1 + dy - y_pixel);
        //std::cout << color << std::endl;
        return Vec3(color.val[0], color.val[1], color.val[2]);
    }
private:
    cv::Mat m_image;
    double m_x0, m_x1, m_y0, m_y1;
};


#endif //LIGHT_TEXTURE_H
