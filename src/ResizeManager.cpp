#include "ResizeManager.h"  

void ResizeManager::NormalCrop(Image& im, int w, int h) {
	std::cout << "Execute : NormalCrop" << std::endl;
	im = Image(im, 0, 0, w, h);
}

void ResizeManager::CenterCrop(Image& im, int w, int h) {
	std::cout << "Execute : CenterCrop" << std::endl;
	int x = im.w() / 2 - w / 2;
	int y = im.h() / 2 - h / 2;
	im = Image(im, x, y, w, h);
}
