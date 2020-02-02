#ifndef SIMILARITYMANAGER_H
#define SIMILARITYMANAGER_H

#include <vector>
#include "Image.h"

class SimilarityManager {
public:

    SimilarityManager() { _sim = &SimilarityManager::diffHisto; };
    SimilarityManager(const std::string& funcName) {
        if (funcName == "diffHisto") {
            _sim = &SimilarityManager::diffHisto;
        } else if (funcName == "diffHistoZone") {
            _sim = &SimilarityManager::diffHistoZone;
        } else {
            // Valeur par defaut
            _sim = &SimilarityManager::diffHisto;
        }
    };

    int sim(const Histogram& main_histo, const std::vector<Image>& set) const { return (this->*_sim)(main_histo, set); };

    int diffHisto(const Histogram& main_histo, const std::vector<Image>& set) const;
    int diffHistoZone(const Histogram& main_histo, const std::vector<Image>& set) const;

private:
    int (SimilarityManager::*_sim)(const Histogram&, const std::vector<Image>&) const;
};

#endif