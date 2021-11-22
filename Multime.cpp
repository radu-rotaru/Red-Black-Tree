#include "Multime.h"
#include <iostream>
#include <queue>
#include <fstream>

using namespace std;


ofstream out("out.txt", ofstream::app);
/* Functia care primeste ca parametru valoarea ce trebuie stearsa.
   Daca multimea nu e vida si valoarea exista in multime,
   apeleaza functia care sterge nodul respectiv si scade cardinalul
   cu un element.
*/
void Multime::sterge(long long x) {
    if (root == nullptr) {
        out << "Valoarea " << x << " nu poate fi stearsa. Multimea este vida\n";
        return;
    }
    Node* y = search(x, root);
    if (y == nullptr) {
        out << "Nu exista nod cu valoarea " << x << '\n';
        return;
    }
    if (y->value == maxim)
        maxim = predecesor(y->value);

    if (y->value == minim)
        minim = succesor(y->value);

    card--;
    stergere_nod(y);
    out << "Valoarea " << x << " a fost stearsa cu succes.\n";
}

/*
   Functia care sterge nodul primit.
   Pentru stergere, exista mai multe cazuri.
 */
void Multime::stergere_nod(Node* x) {
    /* arborele ar ramane gol in locul unde e sters nodul,
       deci caut un nod care sa il inlocuiasca
       Nodul sters trebuie inlocuit pentru a pastra echilibrul,
       caut succesorul din subarbore.
    */
    Node* y = inlocuire(x);
    // Verific daca nodul ce trebuie sters si cel ce il inlocuieste au culoarea BLACK
    bool double_black = (y == nullptr || y->color == BLACK) && x->color == BLACK;
    // In cazul in care nodul ce trebuie sters este leaf, nu trebuie sa il inlocuim
    if (y == nullptr) {
        // Daca nodul sters e radacina, atribuim radacinii valoarea nullptr
        if (root == x)
            root = nullptr;
        else {
            // Daca nodul sters nu este radacina si nodul sters si cel ce il va inlocui
            // au culoarea BLACK, inseamna ca pe rutele ce contineau nodul sters vom
            // avea cu un nod BLACK mai putin decat pe restul. In acest caz trebuie sa
            // restabilim aceasta proprietate.
            if (double_black)
                double_black_case(x);
            else {
                if (x->bro() != nullptr) {
                    x->bro()->color = RED;
                }
            }
            // Verificam in ce parte a parintelui se afla nodul ce trebuie sters,
            // dupa care ii atribuim fiului respectiv al parintelui valoarea nullptr

            if (x->LR_kid())
                x->parent->left = nullptr;
            else
                x->parent->right = nullptr;
        }
        delete x;
        return;
    }
    // In cazul in care nodul ce trebuie sters are doar un copil, trebuie doar sa
    // stergem nodul si sa legam copilul acestuia de parinte.
    else if (x->left == nullptr or x->right == nullptr) {
        // In cazul in care nodul sters este radacina, nu avem parinte, copilul devine noua radacina.
        if (root == x) {
            x->value = y->value;
            x->left = x->right = nullptr;
            delete y;
        }
        else {
            // Verificam in ce parte a parintelui se afla nodul sters, copilul nodului sters
            // devine copilul parintelui nodului sters
            if (x->LR_kid())
                x->parent->left = y;
            else
                x->parent->right = y;

            y->parent = x->parent;

            delete x;

            // Daca nodul sters si copilul acestuia au culoarea BLACK, vom avea aceeasi problema,
            // rutele in care aparea nodul sters au cu un nod BLACK mai putin decat restul. Apelam
            // functia doubleBlackCase pentru a rezolva problema.
            if (double_black)
                double_black_case(y);
            // Daca nu avem aceasta problema, coloram copilul nodului sters BLACK. Facem acest lucru
            // pentru a evita cazul RED-RED, daca parintele si copilul nodului sters are avea culoarea
            // RED. In plus, acest lucru ar insemna ca nodul sters era BLACK, deci recolorarea mentine
            // numarul de noduri BLACK.
            // In cazul in care nodul sters era RED, parintele si copilul erau sigur BLACK, deci nu s-ar
            // schimba nimic.
            else
                y->color = BLACK;
        }
        return;
    }
    // Daca nodul ce trebuie sters are doi copii, punem valoarea din nodul ce ar fi trebuit sa il inlocuiasca
    // in el, dupa care incercam sa stergem acel nod.
    else {
        swap(x->value, y->value);
        stergere_nod(y);
    }
}
/*
   Functia care rezolva problema doubleBlack, atunci cand stergem un nod BLACK, iar cel ce il
   inlocuieste este tot BLACK iar rutele pe care se afla nodul sters ar avea un nod BLACK in minus.
 */
void Multime::double_black_case(Node* x) {
    if (x == root)
        return;
    // Avem nevoie de fratele unui nod pentru a reface RED-BLACK tree-ul.
    Node* brother = x->bro();
    // Daca nodul nu are frate, urcam problema in arbore
    if (brother == nullptr)
        double_black_case(x->parent);
    else {
        /*
          Daca fratele e RED, inseamna ca parintele e BLACK (nu putem avea doua noduri RED consecutive).
          Facem parintele RED si fratele BLACK, dupa care rotim in functie de ramura pe care se afla nodul
          sters. Cazul acesta nu rezolva problema, doar o rearanjeaza, pentru a fi rezolvata intr-unul din
          celelalte cazuri.
          Putea parea intuitiv sa lasam culoarea parintelui sa fie BLACK, si sa schimbam doar culoarea
          fratelui, dar dupa rotire, subarborele stang/drept al fratelui devine subarborele drept/stang
          al parintelui. Cum subarborele respectiv nu are un nod BLACK in minus si noi am adaugat un nou
          nod BLACK prin schimbarea culorii fratelui, am avea un nod BLACK in plus pe ramura aceea.
         */
        if (brother->color == RED) {
            x->parent->color = RED;
            brother->color = BLACK;
            if (x->LR_kid())
                left_rotation(root, x->parent);
            else
                right_rotation(root, x->parent);

            double_black_case(x);
        }
        else {
            // Daca fratele e BLACK si copilul din stanga al sau este RED, avem doua cazuri de rotire si recolorare.
            if (brother->left != nullptr and brother->left->color == RED) {
                /*
                 Cazul in care fratele e in stanga parintelui. In acest caz rutele din subarborele din dreapta au cu un
                 nod BLACK mai putin decat cele din stanga. Pentru a rezolva avem nevoie de recolorari si rotiri. Recolorarile
                 asaigura ca nu se pierde numarul de noduri BLACK pe cealalta ruta atunci cand incercam sa o refacem pe cea
                 care are mai putine. Stim ca dupa rotire parintele va apartine exclusiv subarborelui din dreapta, deci culoarea
                 acestuia trebuie sa fie BLACK pentru a reface numarul de noduri BLACK.
                 Fratele va ajunge in locul parintelui dupa rotire, pentru a nu strica numarul de noduri BLACK acesta trebuie sa aiba
                 aceeasi culoare pe care o avea parintele.
                 Fiul stang al fratelui va apartine exclusiv subarborelui stang si va lua locul fratelui, pentru a nu strica numarul
                 de noduri BLACK acesta trebuie sa aiba aaceeasi culoare pe care o avea fratele, deci BLACK.
                 */
                if (brother->LR_kid()) {
                    brother->left->color = brother->color;
                    brother->color = brother->parent->color;
                    right_rotation(root, x->parent);
                }
                /*
                  Daca fratele e in dreapta si copilul stang al lui e rosu, punem in copilul stang culoarea
                  parintelui, dupa care pune BLACK in parinte, pentru ca parintele o sa faca parte la final doar
                  din ruta din stanga, ruta care mai avea nevoie de un nod BLACK. Culoarea copilului din stanga
                  e schimbata in cea a parintelui pentru ca dupa rotiri, copilul din stanga ajunge pe pozitia
                  de intersectie a celor doua rute.

                 Cazul in care fratele e in dreapta parintelui. In acest caz rutele din subarborele din stanga au cu un
                 nod BLACK mai putin decat cele din dreapta. Pentru a rezolva avem nevoie de recolorari si rotiri. Recolorarile
                 asaigura ca nu se pierde numarul de noduri BLACK pe cealalta ruta atunci cand incercam sa o refacem pe cea
                 care are mai putine. Stim ca dupa rotire parintele va apartine exclusiv subarborelui din stanga, deci culoarea
                 acestuia trebuie sa fie BLACK pentru a reface numarul de noduri BLACK.
                 Fata de cazul precedent, aici facem o rotire in plus, rotire in urma careia fiul frateleui ajunge in locul fratelui.
                 Dupa inca o rotire fiul fratelui ajunge in locul parintelui, de aceea culoarea acestui nod trebuie sa fie cea pe care
                 o avea parintele. Culoarea fratelui va ramane aceeasi deoarece era BLACK si acesta ramane doar in subarborele din stanga.

                 */
                else {
                    brother->left->color = brother->parent->color;
                    right_rotation(root, brother);
                    left_rotation(root, x->parent);
                }
                x->parent->color = BLACK;
            }
            //Cazurile sunt asemanatoare cu cele de mai sus. Ideea este aceeasi, doar directiile sunt diferite.
            else if (brother->right != nullptr and brother->right->color == RED) {
                if (brother->LR_kid()) {
                    brother->right->color = brother->parent->color;
                    left_rotation(root, brother);
                    right_rotation(root, x->parent);
                }
                else {
                    brother->right->color = brother->color;
                    brother->color = brother->parent->color;
                    left_rotation(root, x->parent);
                }
                x->parent->color = BLACK;
            }
            /*
              Cazul in care fratele e BLACK si are ambii copii BLACK.
              Facem fratele RED si vrem sa facem parintele BLACK ca sa echilibram numarul de noduri BLACK.
              Daca parintele era RED, am adaugat un nod BLACK pe ruta pe care trebuia, si pe cealalta numarul a
              ramas la fel. Dar daca parintele era deja BLACK, nu am adaugat nimic, doar am urcat problema mai
              sus cu un nivel, acum apelam functia pentru parinte.
             */
            else {
                brother->color = RED;
                if (brother->parent->color == BLACK)
                    double_black_case(brother->parent);
                else
                    brother->parent->color = BLACK;
            }
        }
    }
}
// Functia care inlocuieste nodul sters
Node* Multime::inlocuire(Node* x) {
    /*
       Daca exista fiu stang si fiu drept, alegem fiul drept, dupa care mergem in fiul stang pana cand dam de nullptr. In acest mod,
       ajungem la succesorul din subarbore.
       Daca exista doar fiu drept, nu avem nevoie de succesor, inlocuim nodul cu fiul drept.
       Daca exista doar fiu stang, nu avem nevoie de succesor, inlocuim nodul cu fiul stang.
       Daca nu, nu avem cu ce sa inlocuim, returnam nullptr.
     */
    if (x->right != nullptr) {
        if (x->left != nullptr) {
            Node* z = x->right;
            while (z->left != nullptr)
                z = z->left;
            return z;
        }
        return x->right;
    }
    return x->left;
}
// Constructorul
Multime::Multime() {
    root = nullptr;
    card = 0;
}
// Functia de inserare
void Multime::insereaza(long long x) {
    /*
       Daca e primul element inserat, initializez si minimul si maximul.
       Fac acest lucru ca sa nu mai fie mereu nevoie sa il caut cand o se apeleze
       functiile pentru min si max.
    */
    if (card == 0) {
        minim = maxim = x;
    }
    // Daca nu e primul, verific daca este minim sau maxim
    else {
        if (x > maxim)
            maxim = x;
        if (x < minim)
            minim = x;
    }
    // Cresc cardinalul cu 1

    card++;

    Node* nd = new Node(x);
    // Inserez nodul in arbore ca intr-un binary search tree normal
    root = bst_insert(root, nd);

    //Daca nodul a fost inserat, trebuie sa verific ca toate regulile unui red-black tree sa fie respectate.
    if (nd != nullptr) {
        out << "Valoarea " << x << " a fost inserata cu succes.\n";
        balance(root, nd);
    }
}

// Inserare de tip binary search tree. Functia returneaza la final radacina arborelui sau sub-arborelui primit.
Node* Multime::bst_insert(Node* root, Node*& x) {
    // Daca radacina e nullptr, returna noua radacina, x
    if (root == nullptr)
        return x;
    // Daca valoarea din x e mai mica decat cea din root, inseamna ca x trebuie inserat in sub-arborele din stanga
    if (root->value > x->value) {
        root->left = bst_insert(root->left, x);
        root->left->parent = root;
    }
    // Daca e mai mare, in cel din dreapta
    else if (root->value < x->value) {
        root->right = bst_insert(root->right, x);
        root->right->parent = root;
    }
    // Daca e egala, inseamna ca exista deja in arbore si nu inseram nimic
    else if (root->value == x->value) {
        out << "Valoarea " << x->value << " se afla deja in multime.\n";
        delete x;
        x = nullptr;
        // Am incrementat cardinalul inainte sa intram in functie, de asta trebuie sa decrementam
        card--;
    }
    return root;
}
// Functia care rebalanseaza arborele.
void Multime::balance(Node*& root, Node*& x) {
    Node* parent_x = nullptr;
    Node* grandparent_x = nullptr;
    Node* uncle_x;
    // Cat timp x e diferit de root si nodul x si parintele au culoarea RED,
    // arborele trebuie rearanjat
    while (x != root && x->color != BLACK && x->parent->color == RED && x->parent->parent != nullptr) {
        parent_x = x->parent;
        grandparent_x = x->parent->parent;
        // Cazul in care parintele in dreapta, deci unchiul este in stanga.
        if (!parent_x->LR_kid()) {
            uncle_x = grandparent_x->left;

            // Cazul in care unchiul este nullptr sau are culoarea BLACK
            if (uncle_x == nullptr || uncle_x->color == BLACK) {

                /*
                 Cazul in care nodul inserat este fiul stang. Cum parintele se afla in dreapta,
                 avem cazul RL(right-left). In acest caz trebuie sa rotim parintele nodului
                 inserat spre dreapta, transfomand astfel cazul in RR(right-right). Nodul x ii ia
                 locul parintelui sau, iar parintele devine fiul sau drept.
                */
                if (x->LR_kid()) {
                    right_rotation(root, parent_x);
                    x = parent_x;
                    parent_x = x->parent;
                }
                /*
                   Dupa transformare sau daca aveam deja cazul RR, inseamna ca trebuie sa rotim spre stanga "bunicul"
                   nodului inserat. Dupa rotire, parintele ii ia locul bunicului si pentru a scapa de cazul RED-RED
                   trebuie sa schimbam intre ele culorile bunicului si parintelui.
                */
                left_rotation(root, grandparent_x);
                swap(parent_x->color, grandparent_x->color);
                x = parent_x;
            }
        }

        // Cazul in care parintele in dreapta, deci unchiul este in stanga.
        else {
            uncle_x = grandparent_x->right;

            // Cazul in care unchiul este nullptr sau are culoarea BLACK. Aceeasi idee ca mai sus, doar ca directiile difera
            if (uncle_x == nullptr || uncle_x->color == BLACK) {
                if (!x->LR_kid()) {
                    left_rotation(root, parent_x);
                    x = parent_x;
                    parent_x = x->parent;
                }
                right_rotation(root, grandparent_x);
                swap(parent_x->color, grandparent_x->color);
                x = parent_x;
            }
        }
        // Cazul in care unchiul are culoarea RED. In acest caz face parintele si unchiul nodului BLACK
        // pentru a nu pierde numarul de de noduri BLACK, dupa care facem nodul bunic RED, dupa care il
        // atribuim nodului x pentru a verifica in continuare cazul RED-RED pentru bunic.
        if (uncle_x != nullptr && uncle_x->color == RED) {
            parent_x->color = uncle_x->color = BLACK;
            grandparent_x->color = RED;
            x = grandparent_x;
        }
    }
    // Daca acest caz ajunge la radacina, facem radacina BLACK
    root->color = BLACK;
}

/*
  Functia care face rotirea spre dreapta a unui nod. Prin aceasta rotire, fiul stang ii ia locul
  nodului respectiv. Parintele nodului devine parintele fiului sau. Nodul ce trebuie rotit devine
  fiul drept al fiului stang pe care il avea inainte de rotire. Daca fiul stang avea un fiu drept,
  acela va fi noul fiu stang al nodului primit ca parametru dupa rotire.
*/
void Multime::right_rotation(Node*& root, Node*& x) {
    Node* y = x->left;
    x->left = y->right;
    if (x->left != nullptr)
        x->left->parent = x;
    y->parent = x->parent;
    if (x->parent == nullptr)
        root = y;
    else if (x->parent->left == x)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->right = x;
    x->parent = y;
}

//Functia care face rotirea spre stanga a unui nod. Aceeasi idee ca mai sus, directia difera.
void Multime::left_rotation(Node*& root, Node*& x) {
    Node* y = x->right;
    x->right = y->left;
    if (x->right != nullptr)
        x->right->parent = x;
    y->parent = x->parent;

    if (x->parent == nullptr)
        root = y;

    else if (x->parent->left == x)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}
/* Functia privata pentru aflarea celui de-al k-lea element in ordine crescatoare
   root - radacina arborelui
   index - indexul pe care il cautam
   currentIndex - indexul la care am ajuns
   val - valoare gasita la al k-lea element
*/
void Multime::sorted_tree(Node* root, unsigned long long index, unsigned long long& currentIndex, long long& val) {
    if (root != nullptr && index > currentIndex) {
        sorted_tree(root->left, index, currentIndex, val);
        currentIndex++;
        if (currentIndex == index)
            val = root->value;
        else
            sorted_tree(root->right, index, currentIndex, val);
    }
}
/*
  Functia publica pentru gasirea celui de-al k-lea element, pentru a respecta numarul de parametri din enunt.
  Mai verifica in plus daca exista k elemente in multime.
*/
long long Multime::k_element(unsigned long long k) {
    if (k < card) {
        Node* x = root;
        long long val;
        unsigned long long curInd = 0;
        sorted_tree(x, k, curInd, val);
        return val;
    }
    else {
        out << "Multimea are mai putin de " << k << " elemente.";
        return 0;
    }
}
// Functia publica pentru verificarea existentei unei valori
bool Multime::este_in(long long x) {
    Node* nd = root;
    nd = search(x, nd);
    return nd != nullptr;
}
// Functia privata care cauta valoarea respectiva
Node* Multime::search(long long x, Node* nd) {
    if (nd == nullptr || x == nd->value)
        return nd;
    if (x < nd->value)
        return search(x, nd->left);

    return search(x, nd->right);

}
// Functia ce returneaza cardinalul multimii
unsigned long long Multime::cardinal() {
    return card;
}

// Functia publica pentru gasirea succesorului
long long Multime::succesor(long long x) {
    Node* nd = root;
    long long suc = x;
    find_succesor(x, nd, suc);
    if (suc == x)
        out << "Elementul nu exista sau nu are succesor" << '\n';
    return suc;
}

// Functia privata care gaseste succesorul
void Multime::find_succesor(long long x, Node* nd, long long& succesor) {
    //Daca nodul gasit e nullptr, inseamna ca valoarea introdusa nu exista
    if (nd == nullptr) {
        succesor = x;
        return;
    }
    //Daca x e mai mic, il cautam in subarborele stang si nd ar fi succesorul pentru moment
    if (x < nd->value) {
        succesor = nd->value;
        find_succesor(x, nd->left, succesor);
        return;
    }
    //Daca x e mai mare, il cautam in sub-arborele drept
    if (x > nd->value) {
        find_succesor(x, nd->right, succesor);
        return;
    }
    /*
       Daca ajungem aici inseamna ca am gasit nodul cu valoarea x.
       In cazul acesta, stim ca daca nodul gasit e copilul din stanga al parintelui sau,
       e mai mic decat parintele. Parintele ar fi succesorul. Dar daca nodul gasit are un fiu in dreapta,
       inseamna ca fiul din dreapta e mai mare decat nodul gasit si mai mic decat parintele nodului gasit. Deci ar
       fi noul succesor. Daca fiul din dreapta are un fiu in stanga, acela ar avea o valoare mai mare decat nodul gasit
       si mai mica decat fiul din dreapta. Deci daca nodul gasit are fiu in dreapta, cautam fiul din stanga al fiului din dreapta
       si dupa fiul din stanga al fiului din stanga si tot asa pana dam de nullptr.
    */
    if (nd->right != nullptr) {
        nd = nd->right;
        while (nd != nullptr) {
            succesor = nd->value;
            nd = nd->left;
        }
    }
}
// Functia publica pentru predecesor
long long Multime::predecesor(long long x) {
    Node* nd = root;
    long long pred = x;
    find_predecesor(x, nd, pred);
    if (pred == x)
        out << "Elementul nu exista sau nu are succesor" << '\n';
    return pred;
}
// Functia privata pentru predecesor, ideea este aceeasi ca pentru succesor
void Multime::find_predecesor(long long x, Node* nd, long long& predecesor) {
    if (nd == nullptr) {
        predecesor = x;
        return;
    }
    if (x > nd->value) {
        predecesor = nd->value;
        find_predecesor(x, nd->right, predecesor);
        return;
    }
    if (x < nd->value) {
        find_predecesor(x, nd->left, predecesor);
        return;
    }
    if (nd->left != nullptr) {
        nd = nd->left;
        while (nd != nullptr) {
            predecesor = nd->value;
            nd = nd->right;
        }
    }
}
// Functia care returneaza maximul
long long Multime::max() {
    if (!card)
        out << "Nu exista elemente in multime\nValoarea returnata este irelevanta. ";

    return maxim;
}

//Functia care returneaza minimul
long long Multime::min() {
    if (!card)
        out << "Nu exista elemente in multime\nValoarea returnata este irelevanta. ";

    return minim;
}

