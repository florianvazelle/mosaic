#include "GSImage.h"

#include <SDL.h>
#include <SDL_image.h>
#include <stdexcept>

GSImage::GSImage(char const* filename) {
    SDL_Surface* surface = IMG_Load(filename);
    if (!surface) {
        throw std::runtime_error("Cannot load image");
    }
    h_ = surface->h;
    w_ = surface->w;
    pixels_.resize(h_ * w_);

    SDL_Surface* in_surface =
        SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
    SDL_FreeSurface(surface);

    SDL_LockSurface(in_surface);
    auto pixels = static_cast<Uint8*>(in_surface->pixels);
    for (int i = 0; i < h_ * w_; ++i) {
        int j = 4 * i;
        pixels_[i] = (0.212671f * pixels[j + 3] + 0.715160f * pixels[j + 2] +
            0.072169f * pixels[j + 1]) /
            255.0f;
    }
    SDL_UnlockSurface(in_surface);

    SDL_FreeSurface(in_surface);
}

GSImage::GSImage(int w, int h) {
    h_ = h;
    w_ = w;
    pixels_.resize(w * h);
}

void GSImage::save_png(char const* filename) {
    SDL_Surface* out_surface;
    out_surface = SDL_CreateRGBSurface(0, w_, h_, 32, 0, 0, 0, 0);
    out_surface =
        SDL_ConvertSurfaceFormat(out_surface, SDL_PIXELFORMAT_RGBA8888, 0);

    SDL_LockSurface(out_surface);
    auto pixels = static_cast<Uint8*>(out_surface->pixels);
    for (int i = 0; i < h_ * w_; ++i) {
        int j = 4 * i;
        pixels[j] = 255.0f;
        pixels[j + 1] = pixels[j + 2] = pixels[j + 3] = pixels_[i] * 255.0f;
    }
    SDL_UnlockSurface(out_surface);

    IMG_SavePNG(out_surface, filename);
    SDL_FreeSurface(out_surface);
}