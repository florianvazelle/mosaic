#include <cmath>
#include <limits>
#include <iomanip>

#include "SimilarityManager.h"

int SimilarityManager::diffHisto(const Histogram& main_histo, const std::vector<Histogram>& list_histo) {
    int idx_bin = -1;
    float max_distance = std::numeric_limits<float>::infinity();

    // Pour tout les histogrammes
    for (int idx = 0; idx < list_histo.size(); idx++) {
        Histogram histo2 = list_histo[idx];
        float distance = 0;

        // On calcul pour un ensemble de 16 par 16
        for (int i = 0; i < 16; i++) {
            float color1[3] = { 0, 0, 0 }, color2[3] = { 0, 0, 0 };
            for (int j = i * 16; j < 16 * (i + 1); j++) {

                // Pour les trois couleurs
                color1[0] += main_histo.r[j];
                color2[0] += histo2.r[j];

                color1[1] += main_histo.g[j];
                color2[1] += histo2.g[j];

                color1[2] += main_histo.b[j];
                color2[2] += histo2.b[j];

            }
            distance += sqrt(pow(color1[0] - color2[0], 2.0) + pow(color1[1] - color2[1], 2.0) + pow(color1[2] - color2[2], 2.0));
        }

        if (distance < max_distance) {
            idx_bin = idx;
            max_distance = distance;
        }

    }

    return idx_bin;
}
