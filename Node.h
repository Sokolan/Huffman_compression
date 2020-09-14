//
// Created by valery on 08/09/2020.
//

#ifndef HUFFMAN_COMPRESSOR_NODE_H
#define HUFFMAN_COMPRESSOR_NODE_H

class Node{
public:
    Node* lchild;
    Node* rchild;
    unsigned char byte{};
    int freq;
    bool leaf;

    //constructor for parents
    Node(Node* lchid, Node* rchild);

    //constructor for leaves
    Node(unsigned char byte, int freq);
};


#endif //HUFFMAN_COMPRESSOR_NODE_H
