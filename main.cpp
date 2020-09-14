#include <iostream>
#include "Huffman.h"
#include <cstring>


int main(int argc, char *argv[]) {
    if(argc < 3){
        std::cout << "Error, read README file for more information." << std::endl;
        return 0;
    }
    try {
        if (strcmp(argv[1], "-c") == 0) {
            Huffman c(argv[2], Huffman::COMPRESS);
            c.compress();
            std::cout << "file: " << c.get_file_path() << std::endl;
            std::cout << "compressed into: " << c.get_file_path() << ".huf" << std::endl;
        }
        if (strcmp(argv[1], "-d") == 0) {
            Huffman d(argv[2], Huffman::DECOMPRESS);
            d.decompress();
            std::cout << "file: " << d.get_file_path() << std::endl;
            std::cout << "decompressed into: " << "decompressed_file" << std::endl;
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
