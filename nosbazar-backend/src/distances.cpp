#include "distances.h"

float nosbazar::distances::chebyshev(double x1, double y1, double x2, double y2)
{
    return std::max(std::abs(x2 - x1), std::abs(y2 - y1));
}
