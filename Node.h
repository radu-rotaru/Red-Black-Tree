#pragma once

// enum pentru culoarea nodului din RED-BLACK tree
enum color {
    RED,
    BLACK
};

// Clasa Node reprezinta nodul unui arbore
class Node {
private:
    bool color;
    long long value;
    Node* parent;
    Node* left;         //copilul din stanga
    Node* right;        //din dreapta
    Node(long long val);      //constructor
    Node* bro();
    bool LR_kid();
public:
    friend void levelOrder(Node* x);
    friend class Multime;     //Vreau sa folosesc atributele private ale clasei Node in clasa Multime
};

