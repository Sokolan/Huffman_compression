//
// Created by valery on 08/09/2020.
//

#include "Node.h"

Node::Node(Node *lchid, Node *rchild) : lchild(lchid), rchild(rchild), leaf(false), byte(0) {
    freq = lchid->freq + rchild->freq;
}

Node::Node(unsigned char byte, int freq) : lchild(nullptr), rchild(nullptr), byte(byte), freq(freq), leaf(true) {}