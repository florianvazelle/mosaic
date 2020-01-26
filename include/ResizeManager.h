#ifndef RESIZEMANAGER_H
#define RESIZEMANAGER_H

#include "Image.h"

class ResizeManager {
public:

    ResizeManager() { _resize = &ResizeManager::NormalCrop; };
    ResizeManager(const char* funcName) { 
        if (funcName == "NormalCrop") {
            _resize = &ResizeManager::NormalCrop;
        } else if (funcName == "CenterCrop") {
            _resize = &ResizeManager::CenterCrop;
        } else {
            _resize = &ResizeManager::NormalCrop;
        }
    };
    
    void resize(Image& im, int w, int h) { (this->*_resize)(im, w, h); };
    void NormalCrop(Image& im, int w, int h);
    void CenterCrop(Image& im, int w, int h);

private:
    void (ResizeManager::*_resize)(Image&, int, int);
};

#endif