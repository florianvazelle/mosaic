#include <iostream>
#include "GSImage.h"

int main(int argc, char const* argv[]) {
    if (argc != 4) {
        std::cout << "\n\tUsage: " << argv[0] << " <I> <dir> <R> <C> <resize> <sim>\n"
            << std::endl;
        exit(1);
    }

    GSImage image(argv[1]);
    //image.save_png(argv[2]);

    return 0;
}
