//
// Created by valery on 08/09/2020.
//

#include "Huffman.h"

#include <utility>
#include <fstream>
#include <iostream>
#include <climits>

Huffman::Huffman(std::string file_path, Mode mode) : file_path(std::move(file_path)){
    for(int i=0; i<ASCII ; ++i){
        freq[i] = 0;
        dictionary[i] = nullptr;
    }
    if(mode == DECOMPRESS){
        return;
    }
    fill_hist();
    build_tree();
    auto* str = new std::string("");
    build_dict(freq_tree, str);
    delete str;
}

static void tree_del(Node* node){
    if(node == nullptr){
        return;
    }

    if(node->leaf){
        delete node;
        return;
    }
    tree_del(node->lchild);
    tree_del(node->rchild);
    delete node;
}

Huffman::~Huffman() {
    for(auto & i : dictionary){
        delete i;
    }
    tree_del(freq_tree);
}

void Huffman::fill_hist() {
    unsigned char in;

    std::ifstream input_file;

    input_file.open(file_path.c_str(), std::ios::in | std::ios::binary);
    if(!input_file.is_open()){
        throw fail_to_open();
    }

    //counting the number of appearances of each type of byte in the file
    while(input_file >> std::noskipws >> in){
        freq[in]++;
    }
}

void Huffman::build_tree() {
    Node* bytes_nodes_arr[ASCII];
    int count = 0;

    //creating leaf for every type of byte
    for(int i=0 ; i<ASCII ; ++i){
        if(freq[i] == 0){
            bytes_nodes_arr[i] = nullptr;
            continue;
        }
        Node* node = new Node(i, freq[i]);
        bytes_nodes_arr[i] = node;
        count++;
    }

    //if we don't have any bytes in the file then nothing more to do
    if(count == 0){
        return;
    }

    int min = 0, min_pos, second_min = 0, second_min_pos = 0;

    while(count > 1){
        for(int i = 0; i<ASCII ; ++i){
            if((bytes_nodes_arr[i] != nullptr) && (min == 0 || bytes_nodes_arr[i]->freq < min)){
                min = bytes_nodes_arr[i]->freq;
                min_pos = i;
            }
        }
        for(int i = 0; i<ASCII ; ++i){
            if((bytes_nodes_arr[i] != nullptr) && (i != min_pos) &&
                (second_min == 0 || bytes_nodes_arr[i]->freq < second_min)){
                second_min = bytes_nodes_arr[i]->freq;
                second_min_pos = i;
            }
        }
        Node* node = new Node(bytes_nodes_arr[min_pos], bytes_nodes_arr[second_min_pos]);
        /*
         * one of the nodes will be removed and the other will be assigned with
         * the new node, in the end of the process we will have only the
         * root in the array.
         */
        bytes_nodes_arr[min_pos] = node;
        bytes_nodes_arr[second_min_pos] = nullptr;
        count--;
        min = second_min = 0;
    }
    
    //min_pos contain the root of the tree
    freq_tree = bytes_nodes_arr[min_pos];
}

void Huffman::build_dict(const Node* node ,std::string* str) {
    if(node->leaf){
        dictionary[node->byte] = new std::string;
        *dictionary[node->byte] = *str;
        str->erase(str->length()-1, 1);
        return;
    }
    str->append("1");
    build_dict(node->lchild, str);
    str->append("0");
    build_dict(node->rchild, str);

    /*
     * if it's not the root then we want to erase the "step" we took to get to this
     * node
     */
    if(node != freq_tree){
        str->erase(str->length()-1, 1);
    }
}

void Huffman::compress() {
    std::ifstream input_file;
    input_file.open(file_path, std::ios::binary | std::ios::in);
    if(!input_file.is_open()){
        throw fail_to_open();
    }

    std::ofstream compressed_file;
    std::string compressed_file_path(file_path);
    //the compressed file will be named as the original file, but with .huf extension
    compressed_file_path += ".huf";
    compressed_file.open(compressed_file_path, std::ios::binary | std::ios::out);
    if(!compressed_file.is_open()){
        throw fail_to_open();
    }

    unsigned char in = 0;
    unsigned char out = 0;
    unsigned int bits_in_out = 0;

    //save the hist for the decompression process
    for(int i = 0; i<ASCII ; ++i){
        compressed_file.write((char *)&freq[i], sizeof(unsigned int));
    }

    /*
     * reads one byte at a time.
     * for each byte we write the corresponding Huffman code to "out".
     * once "out" is filled with 8 bits we flush it to the file.
     */
    while(input_file >> std::noskipws >> in){
        for(unsigned int i=0 ; i<dictionary[in]->length() ; ++i, ++bits_in_out){
            //if we filled 1 byte, we'll flush it to output file
            if(bits_in_out == 8){
                compressed_file << out;
                bits_in_out = 0;
                out = 0;
            }
            if((*dictionary[in])[i] == '1'){
                out = out | (0x01 << bits_in_out);
            }
            //if it's equal to 0, then nothing to do since out is already containing 0 in the
            //bit which corresponds to bits_in_out
        }
    }

    //flushing the rest of the bits if there are any
    if(bits_in_out){
        compressed_file << out;
    }

}

void Huffman::decompress() {
    restore_hist();
    build_tree();
    auto* str = new std::string("");

    std::ifstream compressed_file;
    compressed_file.open(file_path, std::ios::in | std::ios::binary);
    if(!compressed_file.is_open()){
        throw fail_to_open();
    }

    *str = "decompressed_file";
    std::ofstream decompressed_file;
    decompressed_file.open(str->c_str(), std::ios::binary | std::ios::out);
    if(!decompressed_file.is_open()){
        throw fail_to_open();
    }

    //not needed anymore
    delete str;

    //the ASCII*(sizeof(unsigned int)) first bytes are the freq data
    compressed_file.seekg(ASCII*(sizeof(unsigned int)));

    unsigned char in;
    Node* node = freq_tree;

    while(compressed_file >> std::noskipws >> in){
        for(int i = 0; i<CHAR_BIT ; ++i){
            if(in & 0b00000001){
                node = node->lchild;
                if(node->leaf){
                    if(!freq[node->byte]){
                        break;
                    }
                    decompressed_file << node->byte;
                    freq[node->byte]--;
                    node = freq_tree;
                }

            }
            else{
                node = node->rchild;
                if(node->leaf){
                    if(!freq[node->byte]){
                        break;
                    }
                    decompressed_file << node->byte;
                    freq[node->byte]--;
                    node = freq_tree;
                }
            }
            in = in >> 1;
        }

    }


}

void Huffman::restore_hist() {
    std::ifstream compressed_file;
    compressed_file.open(file_path, std::ios::in | std::ios::binary);
    if(!compressed_file.is_open()){
        throw fail_to_open();
    }

    //reconstruct frequencies
    for(int i = 0 ; i<ASCII ; ++i){
        compressed_file.read((char *)&freq[i], sizeof(unsigned int));
    }
}

std::string Huffman::get_file_path() const {
    return file_path;
}

const char * fail_to_open::what() const noexcept {
    return "failed to open file";
}