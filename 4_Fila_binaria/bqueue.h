#include <iostream>
#include <string>
#include <queue>

using namespace std;

class bqueue
{
    queue<char> fila;
    FILE *fout;

public:
    bqueue(FILE *_file_);
    void pushSring(string s, bool resto);
};

//"bool resto" não vai ser útil nos trabalhos 5 e 6, ele foi aplicado para gaantir que, nos testes da fila binária, não vai restar bits dentro da fila (já que não tem como prever se as strings que serão inseridas terão tamanho 8 ou múltiplos de 8).

// Se as strings que forem inseridas na estrutura tiverem tamanho 8 ou tamanho múltiplo de 8, não há a necessidade de sinalizar resto para true. Logo, a entrada do método seria fila.pushSring(string, false);

// Se for inserida, por exemplo, uma string de 9 caracteres binários, resto tem que ser sinalizado como true. Se for sinalizado como false, apenas os 8 caracteres iniciais serão "lidos" e escritos, o outro restante ficará preso na estrutura.