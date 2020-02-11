#include "ResizeManager.h"  

void ResizeManager::NormalCrop(Image& im, int w, int h) const {
	im = Image(im, 0, 0, w, h);
}

void ResizeManager::CenterCrop(Image& im, int w, int h) const {
	int x = im.w() / 2 - w / 2;
	int y = im.h() / 2 - h / 2;
	im = Image(im, x, y, w, h);
}

void ResizeManager::Resize(Image& im, int w, int h) const {
	int scalex = im.w() / w;
	int scaley = im.h() / h;
	Image tmp(w, h);

	std::array<float, 3> *filtered_row = new std::array<float, 3>[w];
    for (int r = 0; r < h; r++) {
		if (r * scaley < im.h()) {
			std::array<float, 3> *row = im[r * scaley];
			for (int c = 0; c < w; c++) {
				if (c * scalex < im.w()) {
					filtered_row[c] = row[c * scalex];
				} 
			}
			std::copy(filtered_row, filtered_row + w, tmp[r]);
		}
	}

	im = tmp;
}

void ResizeManager::ResizeCrop(Image& im, int w, int h) const {
	int scalex = im.w() / w;
	int scaley = im.h() / h;

	int diffx = im.w() - w;
	int diffy = im.h() - h;

	Image tmp(w, h);

    // On récupère la plus grande différence
	if (diffx < diffy) {
		int y = im.h() / 2 - h / 2;
		im = Image(im, 0, y, im.w(), h);
		scaley = 1;
	} else {
		int x = im.w() / 2 - w / 2;
		im = Image(im, x, 0, w, im.h());
		scalex = 1;
	}

	std::array<float, 3>* filtered_row = new std::array<float, 3>[w];
	for (int r = 0; r < h; r++) {
		if (r * scaley < im.h()) {
			std::array<float, 3> *row = im[r * scaley];
			for (int c = 0; c < w; c++) {
				filtered_row[c] = row[c * scalex];
			}
			std::copy(filtered_row, filtered_row + w, tmp[r]);
		}
	}
	
	im = tmp;
}