#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "Vec3.h"

int main()
{
    Vec3 x(1.0, 1.0, 1.0);
    Vec3 y(2.0, 2.0, 2.0);
    Vec3 z;
    z = x + y;
    std::cout << x << std::endl;
    return 0;
}