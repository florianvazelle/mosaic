#include <cmath>
#include <limits>
#include <iomanip>

#include "SimilarityManager.h"

int SimilarityManager::diffHisto(const Histogram& main_histo, const std::vector<Image>& set) const {
    int idx_bin = -1;
    float min_distance = std::numeric_limits<float>::infinity();

    // Pour toute les images du set
    for (int idx = 0; idx < set.size(); idx++) {
        // On calcul son histogramme
        Histogram histo = set[idx].histo();
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
    return 0;
}
