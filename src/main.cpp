#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <filesystem>

#include "Image.h"
#include "FormUtilities.h"
#include "ResizeManager.h"
#include "SimilarityManager.h"

namespace fs = std::filesystem;

/* Retourne la liste d'image contenu dans le repertoire passer en parametre */
void getFilesInDirectory(std::vector<Image>& out, const std::string& directory) {

    for (const fs::directory_entry& p : fs::directory_iterator(directory)) {
        fs::path path = p.path();
        fs::path ext = path.extension();

        // On ne selectionne que les image PNG
        if (ext == ".png") {
            std::string path_str = path.string();

            // Au cas ou l'image provoque une erreur
            try {
                Image tmp(path_str.c_str());
                out.push_back(tmp);
            } catch (const std::exception& e) {
                std::cerr << "Caught " << e.what() << std::endl;
                std::cerr << "Type " << typeid(e).name() << std::endl;
            }

            if (out.size() >= 50) break;
        }

    }
}

void mosaic(Image& image, int R, int C, std::vector<Image> set, ResizeManager rm, SimilarityManager sm) {

    // Step 1 - Découpe de l'image en plusieurs vignettes
    std::vector<Image> vignettes;

    int x = 0;
    int y = 0;
    int w = image.w() / C;
    int h = image.h() / R;
    int s = R * C;

    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            vignettes.push_back(Image(image, x, y, w, h));
            y += h;
        }
        x += w;
        y = 0;
    }
    
    assert(vignettes.size() == R * C);

    // Step 2 - Mise a la resolution, l'ensemble des images du set
    // et calcul des histogrammes
    std::vector<Histogram> list_histo_set;
    int i = 0;
    for (Image& im : set) {
        rm.resize(im, w, h);
        Histogram out = im.histo();
        list_histo_set.push_back(out);
        
        //tmp
        std::string s = std::to_string(i);
        char const* pchar = s.c_str();
        im.save_png("../../assets/tmp.png");
        i++;
    }

    // Step 3 - Pour chaque vignette on determine quelle image du set est la plus ressemblante
    std::vector<Image> J;

    for (const Image& vignette : vignettes) {
        auto out = vignette.histo();
        int res = sm.sim(out, list_histo_set);
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
    std::string pathI; form("Path of I", pathI, "./assets/wiki.png", file_exist);
    std::string pathD; form("Directory with a set", pathD, "./assets/set", directory_exist);
    std::string row; form("Number of row", row, "2", is_number);
    std::string col; form("Number of col", col, "2", is_number);
    std::string funcResize; form("Methods for resize NormalCrop/CenterCrop", funcResize, "NormalCrop", resize_function_exist);
    std::string funcSim; form("Methods for similarity diffHisto", funcSim, "diffHisto", similarity_function_exist);

    // Creation de l'image principale
    Image image(pathI.c_str());

    // Creation de set
    std::vector<Image> set;

    // Remplissage de set
    getFilesInDirectory(set, pathD);

    // Autre variables
    int R = std::stoi(row);
    int C = std::stoi(col);

    ResizeManager rm(funcResize.c_str());
    SimilarityManager sm(funcSim.c_str());

    // Application du filtre mosaique
    mosaic(image, R, C, set, rm, sm);

    // Sauvegarde de l'image
    image.save_png("../../assets/out.png");

    return 0;
}
