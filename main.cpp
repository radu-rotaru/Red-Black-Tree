#include "Multime.h"
#include <chrono>
#include <iostream>
#include <fstream>

using namespace std;

ofstream output("out.txt", ofstream::app);

int main() {
    //get the start time of main
    auto start = chrono::high_resolution_clock::now();
    ifstream in("in.txt");
    Multime x;
    int nr_inserari;
    int nr_stergeri;
    int is_in;
    int k_el;
    long long val;
    unsigned long long k;
    in >> nr_inserari;
    for (int i = 1; i <= nr_inserari; i++) {
        in >> val;
        if (nr_inserari % i == 0) {
            output << "Maxim: " << x.max() << '\n';
            output << "Minim: " << x.min() << '\n';
        }
        x.insereaza(val);
    }

    in >> is_in;
    for (int i = 1; i <= is_in; i++) {
        in >> val;
        bool a = x.este_in(val);
        output << "Este in multime " << val << "?\n" << a << '\n';
        if (a) {
            output << "Predecesor: " << x.predecesor(val) << '\n';
            output << "Succesor: " << x.succesor(val) << '\n';
        }
    }

    in >> k_el;
    for (int i = 1; i <= k_el; i++) {
        in >> k;
        output << "Al " << k << "-lea element: " << x.k_element(k) << '\n';
    }
    output << "Cardinal: " << x.cardinal() << '\n';

    in >> nr_stergeri;
    for (int i = 1; i <= nr_stergeri; i++) {
        in >> val;
        x.sterge(val);
    }

    output << "Cardinal: " << x.cardinal() << '\n';

    in >> is_in;
    for (int i = 1; i <= is_in; i++) {
        in >> val;
        bool a = x.este_in(val);
        output << "Este in multime " << val << "?\n" << a << '\n';
        if (a) {
            output << "Predecesor: " << x.predecesor(val) << '\n';
            output << "Succesor: " << x.succesor(val) << '\n';
        }
    }

    output << "\n\n\n\n";
    //get the end time of the program and output in in microsecond, miliseconds and seconds
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    output << duration.count() << " microseconds\n";
    output << duration.count() / 1000 << " milliseconds\n";
    output << duration.count() / 1000 / 1000 << " seconds\n";


    return 0;
}
