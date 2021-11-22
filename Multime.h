#pragma once
#include "Node.h"

//Clasa Multime, RED-BLACK tree-ul in care sunt pastrate datele
class Multime {
private:
    long long minim;
    long long maxim;
    unsigned long long card;           //cardinalul multimii
    Node* root; //radacina arborelui
    Node* bst_insert(Node* root, Node*& x);
    void balance(Node*& root, Node*& x);
    void right_rotation(Node*& root, Node*& x);
    void left_rotation(Node*& root, Node*& x);
    void sorted_tree(Node* root, unsigned long long index, unsigned long long& currentIndex, long long& val);
    Node* search(long long x, Node* nd);
    void find_succesor(long long x, Node* nd, long long& succesor);
    void find_predecesor(long long x, Node* nd, long long& predecesor);
    void stergere_nod(Node* x);
    Node* inlocuire(Node* x);
    void double_black_case(Node* x);
public:
    Multime();
    void insereaza(long long x);
    void sterge(long long x);
    long long k_element(unsigned long long k);
    bool este_in(long long x);
    unsigned long long cardinal();
    long long succesor(long long x);
    long long predecesor(long long x);
    long long min();
    long long max();
};


