#include <cmath>
#include <limits>
#include <iomanip>

#include "SimilarityManager.h"
#include "Image.h"
#include "Histogram.h"

int SimilarityManager::diffVal(const Image& im, const std::vector<Image>& set) const {
    int idx_bin = -1;
    float min_distance = std::numeric_limits<float>::infinity();

    const auto& row_im = im.pixels();

    // Pour toute les images du set
    for (int idx = 0; idx < set.size(); idx++) {
        float distance = 0;

        const auto& row_set = set[idx].pixels();

        // On parcours l'ensemble des pixels, chaque image est sensé avoir la meme taille
        for (int i = 0; i < im.h() * im.w(); i++) {
            // Et on calcul la distance entre le pixel d'origine et celui de l'image courante
            distance += sqrt(pow(row_im[i][0] - row_set[i][0], 2.0) + pow(row_im[i][1] - row_set[i][1], 2.0) + pow(row_im[i][2] - row_set[i][2], 2.0));
        }

        if (distance < min_distance) {
            idx_bin = idx;
            min_distance = distance;
        }

    }

    // On retourne l'index de l'image qui a la distance la plus petite
    return idx_bin;
}

int SimilarityManager::diffHisto(const Image& im, const std::vector<Image>& set) const {
    int idx_bin = -1;
    float min_distance = std::numeric_limits<float>::infinity();

    const Histogram main_histo(im);

    // On regroupe les couleurs de l'image principale 16 par 16
    float bin[16][3];
    for (int i = 0; i < 16; i++) {
        bin[i][0] = 0;
        bin[i][1] = 0;
        bin[i][2] = 0;
        for (int j = i * 16; j < 16 * (i + 1); j++) {
            bin[i][0] += main_histo.r(j);
            bin[i][1] += main_histo.g(j);
            bin[i][2] += main_histo.b(j);
        }
    }

    // Pour toute les images du set
    for (int idx = 0; idx < set.size(); idx++) {
        // On calcul son histogramme
        const Histogram histo(set[idx]);
        float distance = 0;

        // On regroupe les couleurs 16 par 16
        for (int i = 0; i < 16; i++) {
            float color[3] = { 0, 0, 0 };
            for (int j = i * 16; j < 16 * (i + 1); j++) {

                // Pour les trois couleurs
                color[0] += histo.r(j);
                color[1] += histo.g(j);
                color[2] += histo.b(j);

            }
            // Et on calcul la distance entre l'histogramme d'origine et celui de l'image courante
            distance += sqrt(pow(bin[i][0] - color[0], 2.0) + pow(bin[i][1] - color[1], 2.0) + pow(bin[i][2] - color[2], 2.0));
        }

        if (distance < min_distance) {
            idx_bin = idx;
            min_distance = distance;
        }
    }

    // On retourne l'index de l'image qui a la distance la plus petite
    return idx_bin;
}

int SimilarityManager::diffHistoZone(const Image& im, const std::vector<Image>& set) const {
    int idx_bin = -1;
    float min_distance = std::numeric_limits<float>::infinity();

    const Histogram main_histo(im);

    for (int idx = 0; idx < set.size(); idx++) {
        const Image& other = set[idx];
        float distance = 0;

        // On determine en combien de zone on devrait decouper l'image
        int R, C;

        if (other.w() == 1) {
            C = 1;
        } else {
            C = (other.w() % 2 == 0) ? 2 : 3;
        }

        if (other.h() == 1) {
            R = 1;
        } else {
            R = (other.h() % 2 == 0) ? 2 : 3;
        }

        int w = other.w() / C;
        int h = other.h() / R;

        // On separe l'image en zone
        for (int i = 0; i < R; i++) {
            for (int j = 0; j < C; j++) {
                const Image tmp(other, j * w, i * h, w, h);
                const Histogram histo(Image(other, j * w, i * h, w, h));
                
                for(int k = 0; k < 256; k++) {
                    distance += sqrt(pow(main_histo.r(k) - histo.r(k), 2.0) + pow(main_histo.g(k) - histo.g(k), 2.0) + pow(main_histo.b(k) - histo.b(k), 2.0));
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
