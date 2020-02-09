#ifndef SIMILARITYMANAGER_H
#define SIMILARITYMANAGER_H

#include <vector>
#include "Image.h"

class SimilarityManager {
public:

    SimilarityManager() { _sim = &SimilarityManager::diffHisto; _channel = "RGB"; };
    SimilarityManager(const std::string& funcName, const std::string& channel) {
        _channel = channel;

        if (funcName == "diffVal") {
            _sim = &SimilarityManager::diffVal;
        } else if (funcName == "diffHisto") {
            _sim = &SimilarityManager::diffHisto;
        } else if (funcName == "diffHistoZone") {
            _sim = &SimilarityManager::diffHistoZone;
        } else {
            // Valeur par defaut
            _sim = &SimilarityManager::diffHisto;
        }
    };

    int sim(const Image& im, const std::vector<Image>& set) const { return (this->*_sim)(im, set); };

    int diffVal(const Image& im, const std::vector<Image>& set) const;
    int diffHisto(const Image& im, const std::vector<Image>& set) const;
    int diffHistoZone(const Image& im, const std::vector<Image>& set) const;

private:
    int (SimilarityManager::*_sim)(const Image&, const std::vector<Image>&) const;
    std::string _channel;
};

#endif