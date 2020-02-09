#include "Histogram.h"

#include <array>

Histogram::Histogram(const Image& im) {
    const std::array<float, 3> * pixel = im.pixels();
    for (int i = 0; i < im.w() * im.h(); i++) {
        _r[int(pixel[i][0])] += 1;
        _g[int(pixel[i][1])] += 1;
        _b[int(pixel[i][2])] += 1;
    }
}