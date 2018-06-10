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
        //std::cout << image.cols << std::endl;
        //std::cout << image.rows << std::endl;
    }
    void setBorder(double x0, double x1, double y0, double y1) {
        m_x0 = x0; m_x1 = x1; m_z0 = y0; m_z1 = y1;
    }
    Vec3 getColor(double x, double z) {
        double x_pixel = (x - m_x0) / (m_x1 - m_x0) * (m_image.rows - 1);
        double z_pixel = (z - m_z0) / (m_z1 - m_z0) * (m_image.cols - 1);
        cv::Vec3f color;
        int dx = (int) floor(x_pixel);
        int dz = (int) floor(z_pixel);
        color += m_image.at<cv::Vec3f>(dx, dz) * (x_pixel - dx) * (z_pixel - dz) + m_image.at<cv::Vec3f>(dx + 1, dz) * (1 + dx - x_pixel) * (z_pixel - dz);
        color += m_image.at<cv::Vec3f>(dx, dz + 1) * (x_pixel - dx) * (1 + dz - z_pixel) + m_image.at<cv::Vec3f>(dx + 1, dz + 1) * (1 + dx - x_pixel) * (1 + dz - z_pixel);
        return Vec3(color.val[0], color.val[1], color.val[2]);
    }
private:
    cv::Mat m_image;
    double m_x0, m_x1, m_z0, m_z1;
};


#endif //LIGHT_TEXTURE_H
