//
// Created by valery on 08/09/2020.
//

#ifndef HUFFMAN_COMPRASOR_HUFFMAN_H
#define HUFFMAN_COMPRASOR_HUFFMAN_H
#include "Node.h"
#include <string>

class fail_to_open : public std::exception{
public:
    const char * what() const noexcept override;
};

class Huffman {
    static const int ASCII = 256;
    unsigned int freq[ASCII]{};
    Node* freq_tree{};
    std::string file_path;
    std::string* dictionary[ASCII]{};

    void fill_hist();
    void build_tree();
    void build_dict(const Node* node ,std::string* str);
    void restore_hist();

public:
    enum Mode{COMPRESS, DECOMPRESS};

    explicit Huffman(std::string file_path, Mode mode);
    ~Huffman();
    void compress();
    void decompress();

    std::string get_file_path() const;
};


#endif //HUFFMAN_COMPRASOR_HUFFMAN_H
