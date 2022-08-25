
#include <iostream>
#include <vector>
#include <sstream>
#include "1_Tabela_de_frequencia/ftab.h"
#include "2_Arvore_de_prefixos/prefix.h"
#include "3_Tabela_de_equivalencia/eqtab.h"
#include "4_Fila_binaria/bqueue.h"

using namespace std;

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
    // Parte 01 - MONTANDO TABELA DE EQUIVALÊNCIA:
    char *file_in = argv[2];
    ftab tabFrequencia = ftab(file_in);

    // Parte 02 - MONTANDO ARVORE DE PREFIXOS:
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

    // Parte 03 - MONTANDO TABELA DE EQUIVALENCIA:
    eqtab tabEquivalencia;
    prefix_node *raiz = arvore.take_min_node();
    if (raiz->left == nullptr && raiz->right == nullptr)
    {
      tabEquivalencia.add("1", raiz->c);
    }
    else
    {
      string aux;
      PreOrdem(arvore.take_min_node(), aux, tabEquivalencia);
    }

    // Parte 04 - APLICANDO FILA BINÁRIA:
    // Abrindo o arquivo de saída:
    char *file_out = argv[3];
    FILE *fileFila = fopen(file_out, "wb");
    if (fileFila == NULL)
    {
      cout << "Error ao abrir o arquivo!\n";
    }
    else
    {
      // Criando a fila binária:
      bqueue filaBinaria = bqueue(fileFila);

      mychar esp = 0; // Null.

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
            mychar auxChar = 255;
            fwrite(&auxChar, sizeof(char), 1, fileFila);
            divd--;
          }
          if (resto > 0)
          {
            mychar auxChar2 = resto;
            fwrite(&auxChar2, sizeof(char), 1, fileFila);
          }
          fwrite(&esp, sizeof(char), 1, fileFila);
        }
      }
      fwrite(&esp, sizeof(char), 1, fileFila);

      // Gravando o arquivo compactado
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
      }

      int contResto{0};
      int restoStrBin = ((int)strBin.length() % 8);
      for (int i{0}; i < (8 - restoStrBin); i++)
      {
        contResto++;
        strBin += '0';
      }
      mychar contRestoC = contResto;
      fwrite(&contRestoC, sizeof(char), 1, fileFila);
      // cout << strBin << endl;
      filaBinaria.pushSring(strBin, true);

      fclose(file);
      fclose(fileFila);
    }
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
      mychar cArvore;
      mychar aux{0};
      myint freq{0};

      fread(&cArvore, sizeof(char), 1, fileEntrada);

      if (isZero == true && cArvore == 0)
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
      arvore.add_prefix(cArvore, freq);
    }
    while (arvore.take_min_node()->prxox != nullptr)
    {
      prefix_node *noFusao = arvore.take_min_node();
      prefix_node *noNovo = arvore.fusion(noFusao, noFusao->prxox);
    }

    // MONTANDO A TABELA DE EQUIVALÊNCIA:
    eqtab tabEquivalencia;
    prefix_node *raiz = arvore.take_min_node();
    if (raiz->left == nullptr && raiz->right == nullptr)
    {
      tabEquivalencia.add("1", raiz->c);
    }
    else
    {
      string auxStr;
      PreOrdem(arvore.take_min_node(), auxStr, tabEquivalencia);
    }

    vector<int> caracTab{}; // Representação decimal de cada char na Eqtab.
    vector<string> strTab{};
    for (int i{0}; i < 256; i++)
    {
      string teste = tabEquivalencia.getStr(i);
      if (teste != "")
      {
        caracTab.push_back(i);
        strTab.push_back(teste);
      }
    }

    mychar fim{};
    fread(&fim, sizeof(char), 1, fileEntrada);

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
    char *file_out = argv[3];
    FILE *fileFila = fopen(file_out, "wb");
    if (fileFila == NULL)
    {
      cout << "Error ao abrir o arquivo!\n";
    }
    else
    {
      string fluxo{};
      int strBinTam = (int)strBinaria.length();
      int caracTabSize = caracTab.size();
      for (int i{0}; i < strBinTam - (int)fim; i++)
      {
        fluxo += strBinaria[i];
        for (int i{0}; i < caracTabSize; i++)
        {
          if (strTab[i] == fluxo)
          {
            mychar temps = caracTab[i];
            fwrite(&temps, sizeof(char), 1, fileFila);
            fluxo = "";
            break;
          }
        }
      }
    }
    fclose(fileFila);

    // CONSTRUINDO O ARQUIVO DE SAIDA:
  }
  else
  {
    cout << "[ERROR!] Deve-se sinalizar, na linha de comando, '-c' para compressao ou '-d' para descompressao\n\n";
    cout << "A linha de comando consiste em: executavel.exe -c ou -d arquivo_de_entrada arquivo_de_saida\n";
    return 0;
  }

  return 0;
}