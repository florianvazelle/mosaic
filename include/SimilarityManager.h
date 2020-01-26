#ifndef SIMILARITYMANAGER_H
#define SIMILARITYMANAGER_H

#include <vector>
#include "Image.h"

class SimilarityManager {
public:

    SimilarityManager() { _sim = &SimilarityManager::diffHisto; };
    SimilarityManager(const char* funcName) {
        if (funcName == "diffHisto") {
            _sim = &SimilarityManager::diffHisto;
        } else {
            _sim = &SimilarityManager::diffHisto;
        }
    };

    int sim(const Histogram& main_histo, const std::vector<Histogram>& list_histo) { return (this->*_sim)(main_histo, list_histo); };

    int diffHisto(const Histogram& main_histo, const std::vector<Histogram>& list_histo);

private:
    int (SimilarityManager::*_sim)(const Histogram&, const std::vector<Histogram>&);
};

#endif