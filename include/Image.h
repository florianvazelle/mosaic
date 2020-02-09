#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <iostream>
#include <string>
#include <array>

class Image {
public:
    Image(char const* filename, const std::string& type);
    Image(int w, int h);
    Image(const Image& im, int x, int y, int w, int h);
    Image(const std::vector<Image>& list_im, int row, int col);

    auto operator[](int r) { return _pixels.data() + r * w(); }
    auto operator[](int r) const { return _pixels.data() + r * w(); }

    void save_png(char const* filename);
    auto pixels() { return _pixels.data(); }
    auto pixels() const { return _pixels.data(); }
    int w() const { return _w; }
    int h() const { return _h; }

    class View {
    public:
        auto operator[](int r) const { return image.pixels() + (y + r) * image.w() + x; }

        int x;
        int y;
        int w;
        int h;
        const Image& image;
    };

    auto view(int x, int y, int w, int h) const { return View{ x, y, w, h, *this }; }

    void rgb2hsv(void);
    void hsv2rgb(void);

  private:
    std::vector<std::array<float, 3>> _pixels;
    std::string _type;
    int _w;
    int _h;
};

#endif