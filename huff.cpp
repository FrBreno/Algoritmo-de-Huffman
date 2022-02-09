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
  08. Fazer lógica de adiciona zero a direita na última parte da string na comparação com a tabela de equivalência --> Descompactação
*/

#include <iostream>
#include <vector>
#include <sstream>
#include "1_Tabela de frequencia/ftab.h"
#include "2_Arvore de prefixos/prefix.h"
#include "3_Tabela de equivalencia/eqtab.h"
#include "4_Fila binaria/bqueue.h"

using namespace std;
// Linha de comando: executável.exe -c/-d arq_in arq_out

void decimalParaStr(int x, bool final, string &str)
{
  string fluxo{};
  int aux{0};

  for (int i{0}; i < 8; i++)
  {
    if (x >= 1)
    {
      aux = (x % 2);
      x /= 2;

      if (aux == 0)
      {
        fluxo += "0";
      }
      else
      {
        fluxo += "1";
      }
    }
    else if (x == 0 && !final)
    {
      fluxo += "0";
    }
  }

  if (final)
  {
    string aux = str;
    str = "";
    for (int i{0}; i < (int)aux.length() - 8; i++)
    {
      str += aux[i];
    }
  }

  for (int i = (int)fluxo.length() - 1; i >= 0; i--)
  {
    str += fluxo[i];
  }
  return;
}

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

      mychar esp = 0;
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

      FILE *file = fopen(file_in, "rb");
      string strBin{};
      mychar c;
      while (!feof(file))
      {
        fread(&c, sizeof(char), 1, file);
        if (!feof(file))
        {
          strBin += tabEquivalencia.getStr(c);
        }
        else if (feof(file))
        {
          string aux{};
          aux = tabEquivalencia.getStr(c);

          for (int i{0}; i < (int)aux.length(); i++)
          {
            fwrite(&aux[i], sizeof(char), 1, fileFila);
          }
        }
      }

      // cout << strBin << endl;
      fwrite(&esp, sizeof(char), 1, fileFila);
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
    char *file_in = argv[2];
    FILE *fileEntrada = fopen(file_in, "rb");

    // MONTANDO A ÁRVORE (A tabela de frequência já vem no arquivo):
    prefix arvore;
    bool isZero{false};
    while (true)
    {
      mychar c;
      mychar aux{0};
      myint freq{0};

      fread(&c, sizeof(char), 1, fileEntrada);

      if (isZero == true && c == 0)
      {
        break;
      }

      while (true)
      {
        fread(&aux, sizeof(char), 1, fileEntrada);
        if ((int)aux == 0)
        {
          isZero = true;
          break;
        }
        else
        {
          isZero = false;
          freq += (int)aux;
        }
      }
      arvore.add_prefix(c, freq);
    }
    while (arvore.take_min_node()->prxox != nullptr)
    {
      prefix_node *noFusao = arvore.take_min_node();
      prefix_node *noNovo = arvore.fusion(noFusao, noFusao->prxox);
    }

    // MONTANDO A TABELA DE EQUIVALÊNCIA:
    eqtab tabEquivalencia;
    string aux;
    vector<int> caracTab{};
    PreOrdem(arvore.take_min_node(), aux, tabEquivalencia);

    for (int i{0}; i < 256; i++)
    {
      string teste = tabEquivalencia.getStr(i);
      if (teste != "")
      {
        caracTab.push_back(i);
      }
    }

    // for (int i{0}; i < 256; i++)
    // {
    //   string teste = tabEquivalencia.getStr(i);
    //   if (teste != "")
    //   {
    //     cout << (char)i << " : " << teste << endl;
    //   }
    // }

    string fim{};
    while (true)
    {
      mychar c{};
      fread(&c, sizeof(char), 1, fileEntrada);
      if (c == 0)
      {
        break;
      }
      fim += c;
    }

    // LENDO OS BYTES DO ARQUIVO COMPACTADO:
    string strBinaria{};
    mychar c;
    while (!feof(fileEntrada))
    {
      fread(&c, sizeof(char), 1, fileEntrada);
      if (!feof(fileEntrada)) // Adc pós
      {
        decimalParaStr((int)c, false, strBinaria);
      }
      else if (feof(fileEntrada))
      {
        decimalParaStr((int)c, true, strBinaria);
      }
    }
    // cout << strBinaria << endl;
    fclose(fileEntrada);

    // MONTANDO A FILA BINÁRIA:
    string fluxo{};
    string finalizada{};
    for (int i{0}; i < (int)strBinaria.length(); i++)
    {
      fluxo += strBinaria[i];
      for (int i{0}; i < (int)caracTab.size(); i++)
      {
        if (tabEquivalencia.getStr(caracTab[i]) == fluxo)
        {
          finalizada += tabEquivalencia.getChar(fluxo);
          // cout << tabEquivalencia.getChar(fluxo);
          fluxo = "";
          break;
        }
      }
    }
    // cout << endl;
    // cout << fluxo << endl;
    // fluxo += "1101";

    int size = (int)fim.size();
    if ((int)fluxo.size() < size && (int)fluxo.size() != 0)
    {
      fluxo = fim;
    }
    else
    {
      for (int i = (int)fluxo.length() - 1; i >= 0; i--)
      {
        size--;
        if (size == 0)
        {
          string aux{};
          for (int j{0}; j < i; j++)
          {
            aux += fluxo[j];
          }
          aux += fim;
          fluxo = aux;
          break;
        }
      }
    }

    // cout << endl;
    // cout << finalizada << endl;
    string aux2{};
    for (int i{0}; i < (int)fluxo.length(); i++)
    {
      aux2 += fluxo[i];
      for (int i{0}; i < (int)caracTab.size(); i++)
      {
        if (tabEquivalencia.getStr(caracTab[i]) == aux2)
        {
          finalizada += tabEquivalencia.getChar(aux2);
          // cout << tabEquivalencia.getChar(aux2);
          aux2 = "";
          break;
        }
      }
    }
    // cout << endl;
    // cout << finalizada << endl;
    // cout << fluxo << endl;
    // cout << fim;

    // CONSTRUINDO O ARQUIVO DE SAIDA:
    char *file_out = argv[3];
    FILE *fileFila = fopen(file_out, "wb");
    if (fileFila == NULL)
    {
      cout << "Error ao abrir o arquivo!\n";
    }
    else
    {
      for (int i{0}; i < (int)finalizada.length(); i++)
      {
        mychar temp = finalizada[i];
        fwrite(&temp, sizeof(char), 1, fileFila);
      }
    }
    fclose(fileFila);
  }
  else
  {
    cout << "[ERROR!] Deve-se sinalizar, na linha de comando, '-c' para compressao ou '-d' para descompressao\n\n";
    cout << "A linha de comando consiste em: executavel.exe -c ou -d arquivo_de_entrada arquivo_de_saida\n";
    return 0;
  }

  return 0;
}