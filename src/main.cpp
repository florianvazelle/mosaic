#include <iostream>
#include "GSImage.h"

int main(int argc, char const* argv[]) {
    /* if (argc != 4) {
        std::cout << "\n\tUsage: " << argv[0] << " <I> <dir> <R> <C> <resize> <sim>\n"
            << std::endl;
        exit(1);
    } */

    GSImage image("./assets/wiki.png");
    image.save_png("../../assets/out.png");

    return 0;
}
