//
// Created by 孙桢波 on 2018/5/7.
//

#include "Vec3.h"
#include <cstdio>

void Vec3::Normalize()
{
    float l = Length();
    if (l == 0.0) {
        printf("Length = 0, error!\n");
        return;
    }
    m_x /= l;
    m_y /= l;
    m_z /= l;
}
