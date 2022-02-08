/*
  ANOTAÇÕES
  01. Tratar casos em que a árvore terá apenas um nó (nó raiz) --> Fazer um if na função que lerá os caminhos da árvore
  02. Verifcar se realmente compactou o negocio
  03. Verificar se descompactou
  04. Criar um filtro que aceita apenas as estradas: -c/-d arq_entrada arq_saida
    --> Caso contrário, exibir uma mensagem de erro explicando a entrada para compilação
  05. Converter argv[] para str sem usar sstream.
  06. Mudar o escirta de putc para fwrite na bqueue.cpp
  07. Fazer tratamento de falha ao abrir o arquivo de entrada em ftab.cpp
*/
/*
  DÚVIDA:
  É normal a compactação de arquivos peuqenos, guardando a tabela de frequência, resultar em um arquivo maior do que o arquivo de entrada?
*/

#include <iostream>
#include <sstream>
#include "1_Tabela de frequencia/ftab.h"
#include "2_Arvore de prefixos/prefix.h"
#include "3_Tabela de equivalencia/eqtab.h"
#include "4_Fila binaria/bqueue.h"

using namespace std;
// Linha de comando: executável.exe -c/-d arq_in arq_out

void PreOrdem(prefix_node *noAtual, string &a, eqtab &tabEquivalencia)
{
  if (noAtual != NULL)
  {
    // std::cout << noAtual->c << ":";
    // std::cout << noAtual->freq << std::endl;

    if (noAtual->left == nullptr && noAtual->right == nullptr)
    {
      // cout << noAtual->c << ":" << noAtual->freq << " E uma folha\n";
      // cout << a << endl;
      tabEquivalencia.add(a, noAtual->c);

      string s = a;
      a = "";
      for (int i{0}; i < (int)s.length() - 1; i++)
      {
        a += s[i];
      }
      return;
    }
    a += "0";
    PreOrdem(noAtual->left, a, tabEquivalencia);
    a += "1";
    PreOrdem(noAtual->right, a, tabEquivalencia);
    string s = a;
    a = "";
    for (int i{0}; i < (int)s.length() - 1; i++)
    {
      a += s[i];
    }
  }
  return;
}

int main(int argc, char *argv[])
{
  if (argc < 4)
  {
    cout << "[ERROR!] A linha de comando consiste em: executavel.exe -c ou -d arquivo_de_entrada arquivo_de_saida\n";
    return 0;
  }
  stringstream ss(argv[1]);
  string operacao;
  ss >> operacao;

  if (operacao == "-c")
  {
    // MONTANDO TABELA DE EQUIVALÊNCIA:
    char *file_in = argv[2];
    ftab tabFrequencia = ftab(file_in);
    // tabFrequencia.print();

    // MONTANDO ARVORE DE PREFIXOS:
    prefix arvore;
    for (int i{0}; i < 256; i++)
    {
      if (tabFrequencia[i] > 0)
      {
        arvore.add_prefix(i, tabFrequencia[i]);
      }
    }

    while (arvore.take_min_node()->prxox != nullptr)
    {
      prefix_node *noFusao = arvore.take_min_node();
      prefix_node *noNovo = arvore.fusion(noFusao, noFusao->prxox);
    }

    // MONTANDO TABELA DE EQUIVALENCIA:
    eqtab tabEquivalencia;
    string aux;
    PreOrdem(arvore.take_min_node(), aux, tabEquivalencia);
    // for (int i{0}; i < 256; i++)
    // {
    //   string teste = tabEquivalencia.getStr(i);
    //   if (teste != "")
    //   {
    //     cout << teste << endl;
    //   }
    // }

    // APLICANDO FILA BINÁRIA:
    char *file_out = argv[3];
    FILE *fileFila = fopen(file_out, "wb");
    if (fileFila == NULL)
    {
      cout << "Error ao abrir o arquivo!\n";
    }
    else
    {
      bqueue filaBinaria = bqueue(fileFila);

      mychar esp = ' ';
      // Gravando a tabela de frequencia no arquivo:
      for (int i{0}; i < 256; i++)
      {
        if (tabFrequencia[i] > 0)
        {
          fwrite(&i, sizeof(char), 1, fileFila);
          myint freq = tabFrequencia[i];
          myint divd = freq / 255;
          myint resto = freq % 255;

          while (divd > 0)
          {
            mychar aux = 255;
            fwrite(&aux, sizeof(char), 1, fileFila);
            divd--;
          }
          mychar aux = resto;
          fwrite(&aux, sizeof(char), 1, fileFila);
          fwrite(&esp, sizeof(char), 1, fileFila);
        }
      }
      fwrite(&esp, sizeof(char), 1, fileFila);

      FILE *file = fopen(file_in, "r");
      string strBin{};
      mychar c;
      while (!feof(file))
      {
        fread(&c, sizeof(char), 1, file);
        if (!feof(file))
        {
          strBin += tabEquivalencia.getStr(c);
        }
      }

      // cout << strBin << endl;
      filaBinaria.pushSring(strBin);

      fclose(file);
      fclose(fileFila);
    }
    // FILE *teste = fopen("saida.huff", "r");
    // mychar z;
    // while (!feof(teste))
    // {
    //   fread(&z, sizeof(char), 1, teste);
    //   if (!feof(teste))
    //   {
    //     cout << (int)z << " ";
    //   }
    // }
    // cout << endl;

    // fclose(teste);
  }
  else if (operacao == "-d")
  {
  }
  else
  {
    cout << "[ERROR!] Deve-se sinalizar, na linha de comando, '-c' para compressao ou '-d' para descompressao\n\n";
    cout << "A linha de comando consiste em: executavel.exe -c ou -d arquivo_de_entrada arquivo_de_saida\n";
    return 0;
  }

  return 0;
}