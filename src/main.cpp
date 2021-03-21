#include <CLI/CLI.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <cassert>
#include <cstdio>

#include "Image.h"
#include "ResizeManager.h"
#include "SimilarityManager.h"

namespace fs = std::filesystem; // g++ -v >= 9

/* Retourne la liste d'image contenu dans le repertoire passer en parametre */
void getFilesInDirectory(std::vector<Image>& out, const std::string& directory, const std::string& type) {

    for (const fs::directory_entry& p : fs::directory_iterator(directory)) {
        fs::path path = p.path();
        fs::path ext = path.extension();

        // On ne selectionne que des image
        if (ext == ".png" || ext == ".jpg" || ext == ".bmp") {
            std::string path_str = path.string();

            // Au cas ou l'image provoque une erreur
            try {
                Image tmp(path_str.c_str(), type);
                out.push_back(tmp);
            } catch (const std::exception& e) {
                std::cerr << "Caught " << e.what() << std::endl;
                std::cerr << "Type " << typeid(e).name() << std::endl;
            }

            // if (out.size() >= 15) break;
        }

    }
}

void mosaic(Image& image, int R, int C, std::vector<Image> set, const ResizeManager& rm, const SimilarityManager& sm) {

    // Step 1 - Découpe de l'image en plusieurs vignettes
    std::vector<Image> vignettes;

    int w = image.w() / C;
    int h = image.h() / R;

    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            vignettes.push_back(Image(image, j * w, i * h, w, h));
        }
    }

    assert(vignettes.size() == (size_t)R * C);

    // Step 2 - Mise a la resolution de l'ensemble des images du set
    for (Image& im : set) {
        rm.resize(im, w, h);
    }

    // Step 3 - Pour chaque vignette on determine quelle image du set est la plus ressemblante
    std::vector<Image> J;

    for (const Image& vignette : vignettes) {
        int res = sm.sim(vignette, set);
        if (res == -1) {
            std::cout << "Error : no image found" << std::endl;
        } else {
            J.push_back(set[res]);
        }
    }

    assert(J.size() == vignettes.size());
    image = Image(J, R, C);
}

int main(int argc, char** argv) {
    // Demande a l'utilisateur les valeurs en entree
    CLI::App app("A C++ program for creating photomosaic images.\n");

    std::string pathI;
    app.add_option("PNG", pathI, "Path of a base image")->required()->check(CLI::ExistingFile);

    std::string pathD;
    app.add_option("DIR", pathD, "Directory with a set")->required()->check(CLI::ExistingDirectory);

    std::string pathOut = "./out.png";
    app.add_option("-o,--output", pathOut, "Path of the output");


    int row = 20, col = 20;
    app.add_option("-r,--row", row, "Number of row")->check(CLI::PositiveNumber);
    app.add_option("-c,--col", col, "Number of col")->check(CLI::PositiveNumber);

    enum class ResizeMethod : int { NormalCrop, CenterCrop, Resize, ResizeCrop };

    ResizeMethod resize{ ResizeMethod::NormalCrop };

    std::map<std::string, ResizeMethod> mapResize{
        {"NormalCrop", ResizeMethod::NormalCrop},
        {"CenterCrop", ResizeMethod::CenterCrop},
        {"Resize", ResizeMethod::Resize},
        {"ResizeCrop", ResizeMethod::ResizeCrop},
    };

    app.add_option("--resize", resize, "Resize Method settings")
        ->transform(CLI::CheckedTransformer(mapResize, CLI::ignore_case));

    enum class SimilarityMethod : int { diffVal, diffHisto, diffHistoZone };

    SimilarityMethod similarity{ SimilarityMethod::diffVal };

    std::map<std::string, SimilarityMethod> mapSimilarity{
        {"diffVal", SimilarityMethod::diffVal},
        {"diffHisto", SimilarityMethod::diffHisto},
        {"diffHistoZone", SimilarityMethod::diffHistoZone},
    };

    app.add_option("--similarity", similarity, "Resize Method settings")
        ->transform(CLI::CheckedTransformer(mapSimilarity, CLI::ignore_case));

    enum class Channel : int { None, H, S };

    Channel channel{Channel::None};

    std::map<std::string, Channel> mapChannel{
        {"H", Channel::H},
        {"S", Channel::S},
    };

    app.add_option("--hsv", channel,
                   "By default, image comparisons are done in rgb but you can "
                   "specify a channel to do this on hue or saturation")
        ->transform(CLI::CheckedTransformer(mapChannel, CLI::ignore_case));

    CLI11_PARSE(app, argc, argv);

    std::string funcResize;
    for (auto &it : mapResize) {
      if (it.second == resize) {
        funcResize = it.first;
        break;
      }
    }

    std::string funcSim;
    for (auto &it : mapSimilarity) {
      if (it.second == similarity) {
        funcSim = it.first;
        break;
      }
    }

    std::string type = (channel == Channel::None) ? "RGB" : "HSV";
    std::string strchannel = (channel == Channel::None) ? "RGB" : (channel == Channel::H) ? "H" : "S";

    // Creation de l'image principale
    Image image(pathI.c_str(), type);

    // Creation de set
    std::vector<Image> set;

    // Remplissage de set
    getFilesInDirectory(set, pathD, type);

    // Autre variables
    int R = row;
    int C = col;

    ResizeManager rm(funcResize);
    SimilarityManager sm(funcSim, strchannel);

    // Application du filtre mosaique
    mosaic(image, R, C, set, rm, sm);

    // Sauvegarde de l'image
    image.save_png(pathOut.c_str());

    return 0;
}
