#include "Node.h"

// Constructorul cu parametru, nodul nu e inca inserat, deci nu are parent, left sau right.
// Culoarea unui nod inserat trebuie sa fie mereu RED.
Node::Node(long long val) {
    value = val;
    left = right = parent = nullptr;
    color = RED;
}
// Functie ce verifica daca nodul este fiul stang sau fiul drept al parintelui
// Returneaza true daca nodul este fiul stang si false daca este cel drept
bool Node::LR_kid() {
    return this == this->parent->left;
}
//Functie ce returneaza fratele unui nod din tree
Node* Node::bro() {
    if (parent == nullptr)
        return nullptr;

    if (LR_kid())
        return parent->right;

    return parent->left;
}