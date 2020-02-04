#ifndef RESIZEMANAGER_H
#define RESIZEMANAGER_H

#include "Image.h"

class ResizeManager {
public:

    ResizeManager() { _resize = &ResizeManager::NormalCrop; };
    ResizeManager(const std::string& funcName) { 
        if (funcName == "NormalCrop") {
            _resize = &ResizeManager::NormalCrop;
        } else if (funcName == "CenterCrop") {
            _resize = &ResizeManager::CenterCrop;
        } else if (funcName == "Resize") {
            _resize = &ResizeManager::Resize;
        } else if (funcName == "Resize") {
            _resize = &ResizeManager::ResizeCrop;
        } else {
            // Valeur par defaut
            _resize = &ResizeManager::NormalCrop;
        }
    };
    
    void resize(Image& im, int w, int h) const { (this->*_resize)(im, w, h); };
    void NormalCrop(Image& im, int w, int h) const;
    void CenterCrop(Image &im, int w, int h) const;
    void Resize(Image &im, int w, int h) const;
    void ResizeCrop(Image &im, int w, int h) const;

  private:
    void (ResizeManager::*_resize)(Image&, int, int) const;
};

#endif