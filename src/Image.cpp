#include "Image.h"

#include <SDL.h>
#include <SDL_image.h>
#include <stdexcept>

Image::Image(char const* filename, const std::string& type) {
    SDL_Surface* surface = IMG_Load(filename);
    if (surface) {
        _type = type;

        _h = surface->h;
        _w = surface->w;
        _pixels.resize(_h * _w);

        SDL_Surface* in_surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
        SDL_FreeSurface(surface);

        SDL_LockSurface(in_surface);
        auto pixels = static_cast<uint8_t*>(in_surface->pixels);
        for (int i = 0; i < _h * _w; ++i) {
            int j = 4 * i;
            _pixels[i][0] = pixels[j + 1];
            _pixels[i][1] = pixels[j + 2];
            _pixels[i][2] = pixels[j + 3];
        }
        if (_type == "HSV") rgb2hsv();

        SDL_UnlockSurface(in_surface);

        SDL_FreeSurface(in_surface);
    } else {
        throw std::runtime_error("Cannot load image");
    }
}

Image::Image(int w, int h) {
    _h = h;
    _w = w;
    _pixels.resize(w * h);
}

Image::Image(const Image& im, int x, int y, int w, int h) {
    _h = h;
    _w = w;
    _pixels.resize(w * h);

    Image::View v = im.view(x, y, w, h);

    for (int r = 0; r < h; ++r) {
        auto row = v[r];
        std::copy(&row[0][0], &row[0][0] + w * 3, &_pixels[r * w][0]);
    }
}

Image::Image(const std::vector<Image>& list_im, int row, int col) {
    int height = list_im[0].h();
    int width = list_im[0].w();

    _h = height * row;
    _w = width * col;
    _pixels.resize(_w * _h);

    int idx = 0;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            const Image& tmp = list_im[(i * col) + j];
            for (int l = 0; l < height; ++l) {
                auto row = tmp[l];
                idx = i * (height * _w) + j * width + l * _w;
                std::copy(&row[0][0], &row[0][0] + width * 3, &_pixels[idx][0]);
            }
        }
    }
}

void Image::save_png(char const* filename) {
    SDL_Surface* out_surface;
    out_surface = SDL_CreateRGBSurface(0, _w, _h, 32, 0, 0, 0, 0);
    out_surface = SDL_ConvertSurfaceFormat(out_surface, SDL_PIXELFORMAT_RGBA8888, 0);

    if (_type == "HSV") hsv2rgb();

    SDL_LockSurface(out_surface);
    auto pixels = static_cast<uint8_t*>(out_surface->pixels);
    for (int i = 0; i < _h * _w; ++i) {
        int j = 4 * i;
        pixels[j] = 255.0f;
        pixels[j + 1] = _pixels[i][0];
        pixels[j + 2] = _pixels[i][1];
        pixels[j + 3] = _pixels[i][2];
    }
    SDL_UnlockSurface(out_surface);

    IMG_SavePNG(out_surface, filename);
    SDL_FreeSurface(out_surface);
}

// https://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb-in-range-0-255-for-both
void Image::rgb2hsv() {
    for (std::array<float, 3> &p : _pixels) {

        std::array<float, 3> out;
        float min, max, delta;

        min = p[0] < p[1] ? p[0] : p[1];
        min = min < p[2] ? min : p[2];

        max = p[0] > p[1] ? p[0] : p[1];
        max = max > p[2] ? max : p[2];

        out[2] = max;
        delta = max - min;
        if (delta < 0.00001f) {
            out[1] = 0.0f;
            out[0] = 0.0f;
            p = out;
            continue;
        }

        if (max > 0.0f) {
            out[1] = delta / max;
        } else {
            out[1] = 0.0f;
            out[0] = NAN;
            p = out;
            continue;
        }

        if (p[0] >= max) {
            out[0] = (p[1] - p[2]) / delta;
        } else {
            if (p[1] >= max) {
                out[0] = 2.0f + (p[2] - p[0]) / delta;
            } else {
                out[0] = 4.0f + (p[0] - p[1]) / delta;
            }
        }

        out[0] *= 60.0f;

        if (out[0] < 0.0f) {
            out[0] += 360.0f;
        }

        p = out;
    }
}

void Image::hsv2rgb() {
    for (std::array<float, 3> &p : _pixels) {

        double hh, k, q, t, ff;
        long i;
        std::array<float, 3> out;

        if (p[1] <= 0.0) {
            out[0] = p[2];
            out[1] = p[2];
            out[2] = p[2];
            p = out;
            continue;
        }

        hh = p[0];
        if (hh >= 360.0)
            hh = 0.0;
        hh /= 60.0;
        i = (long)hh;
        ff = hh - i;
        k = p[2] * (1.0 - p[1]);
        q = p[2] * (1.0 - (p[1] * ff));
        t = p[2] * (1.0 - (p[1] * (1.0 - ff)));

        switch (i) {
            case 0:
                out[0] = p[2];
                out[1] = t;
                out[2] = k;
                break;
            case 1:
                out[0] = q;
                out[1] = p[2];
                out[2] = k;
                break;
            case 2:
                out[0] = k;
                out[1] = p[2];
                out[2] = t;
                break;

            case 3:
                out[0] = k;
                out[1] = q;
                out[2] = p[2];
                break;
            case 4:
                out[0] = t;
                out[1] = k;
                out[2] = p[2];
                break;
            case 5:
            default:
                out[0] = p[2];
                out[1] = k;
                out[2] = q;
                break;
        }

        p = out;
    }
}
