﻿#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <cassert>
#include <cstdio>

#include "Image.h"
#include "FormUtilities.h"
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

int main() {
    // Demande a l'utilisateur les valeurs en entree
    std::string pathI; form("Path of I", pathI, "./assets/test.png", file_exist);
    std::string pathD; form("Directory with a set", pathD, "./assets/set", directory_exist);
    std::string row; form("Number of row", row, "20", is_number);
    std::string col; form("Number of col", col, "20", is_number);
    std::string funcResize; form("Methods for resize NormalCrop/CenterCrop/Resize/ResizeCrop", funcResize, "NormalCrop", resize_function_exist);
    std::string type; form("RGB or HSV ? RGB/HSV", type, "RGB", type_exist);
    std::string channel = "RGB";
    if(type == "HSV") {
        form("Channel H/V", channel, "H", channel_exist);
    }
    std::string funcSim; form("Methods for similarity diffVal/diffHisto/diffHistoZone", funcSim, "diffHisto", similarity_function_exist);

    // Creation de l'image principale
    Image image(pathI.c_str(), type);

    // Creation de set
    std::vector<Image> set;

    // Remplissage de set
    getFilesInDirectory(set, pathD, type);

    // Autre variables
    int R = std::stoi(row);
    int C = std::stoi(col);

    ResizeManager rm(funcResize);
    SimilarityManager sm(funcSim, channel);

    // Application du filtre mosaique
    mosaic(image, R, C, set, rm, sm);

    // Sauvegarde de l'image
    char buff[100];
    sprintf(buff, "../../assets/out-%s-%s-%s-%dx%d.png", funcResize.c_str(), funcSim.c_str(), channel.c_str(), R, C);
    image.save_png(buff);

    return 0;
}
