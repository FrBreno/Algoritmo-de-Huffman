#include <iostream>

using namespace std;

typedef unsigned char mychar;
typedef unsigned long long int myint;

class ftab
{
    pair<mychar, myint> tab[256]; // Guarda as frequênicas

public:
    ftab(const char *filename); // Recebe o arquivo de entrada

    void sort(); // Organiza o pair
    myint operator[](unsigned char key);
    void print();
};
