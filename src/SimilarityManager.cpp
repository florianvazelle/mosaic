#include <cmath>
#include <limits>
#include <iomanip>

#include "SimilarityManager.h"
#include "Image.h"

int SimilarityManager::diffHisto(const Histogram& main_histo, const std::vector<Image>& set) const {
    int idx_bin = -1;
    float min_distance = std::numeric_limits<float>::infinity();

    // Pour toute les images du set
    for (int idx = 0; idx < set.size(); idx++) {
        // On calcul son histogramme
        Histogram histo;
        set[idx].histo(histo);
        float distance = 0;

        // On regroupe les couleurs 16 par 16
        for (int i = 0; i < 16; i++) {
            float color1[3] = { 0, 0, 0 }, color2[3] = { 0, 0, 0 };
            for (int j = i * 16; j < 16 * (i + 1); j++) {

                // Pour les trois couleurs
                color1[0] += main_histo.r[j];
                color2[0] += histo.r[j];

                color1[1] += main_histo.g[j];
                color2[1] += histo.g[j];

                color1[2] += main_histo.b[j];
                color2[2] += histo.b[j];

            }
            // Et on calcul la distance entre l'histogramme d'origine et celui de l'image courante
            distance += sqrt(pow(color1[0] - color2[0], 2.0) + pow(color1[1] - color2[1], 2.0) + pow(color1[2] - color2[2], 2.0));
        }

        if (distance < min_distance) {
            idx_bin = idx;
            min_distance = distance;
        }

    }

    // On retourne l'index de l'image qui a la distance la plus petite
    return idx_bin;
}

int SimilarityManager::diffHistoZone(const Histogram& main_histo, const std::vector<Image>& set) const {
    int idx_bin = -1;
    float min_distance = std::numeric_limits<float>::infinity();

    for (int idx = 0; idx < set.size(); idx++) {
        const Image& im = set[idx];
        float distance = 0;

        // On determine en combien de zone on devrait decouper l'image
        int R, C;

        if (im.w() == 1) {
            C = 1;
        } else {
            C = (im.w() % 2 == 0) ? 2 : 3;
        }

        if (im.h() == 1) {
            R = 1;
        } else {
            R = (im.h() % 2 == 0) ? 2 : 3;
        }

        int w = im.w() / C;
        int h = im.h() / R;

        // On separe l'image en zone
        for (int i = 0; i < R; i++) {
            for (int j = 0; j < C; j++) {
                Image tmp(im, j * w, i * h, w, h);
                Histogram histo;
                tmp.histo(histo);
                
                for(int k = 0; k < 256; k++) {
                    distance += sqrt(pow(main_histo.r[k] - histo.r[k], 2.0) + pow(main_histo.g[k] - histo.g[k], 2.0) + pow(main_histo.b[k] - histo.b[k], 2.0));
                }

            }
        }

        if (distance < min_distance) {
            idx_bin = idx;
            min_distance = distance;
        }

    }

    return idx_bin; 
}
