#include "Image.h"

class Histogram {
    public:
        Histogram(const Image& im);

        int r(int idx) const { return _r[idx]; };
        int g(int idx) const { return _g[idx]; };
        int b(int idx) const { return _b[idx]; };

    private:
        int _r[256] = { 0 };
        int _g[256] = { 0 };
        int _b[256] = { 0 };
};