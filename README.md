# Algoritmo de Huffman

&nbsp;

## 📃 Descrição

language: Brazilian Portuguese.

Projeto realizado durante a cadeira de Estrutura de Dados.

## 🛠️ Projeto

### 01. Objetivo

O objetivo deste projeto é implementar um algoritmo de compressão e descompressão de dados (Huffman).

---

### 02. Linguagem

- C++

---

### 03. Instruções

- Compilação - Linha de comando:

```
g++ 1_Tabela_de_frequencia/ftab.cpp 2_Arvore_de_prefixos/prefix.cpp 3_Tabela_de_equivalencia/eqtab.cpp 4_Fila_binaria/bqueue.cpp huff.cpp -o solver.exe
```

- Compactação - Linha de comando:

```
./solver.exe -c entrada.txt compactado.huff
```

- Descompactação - Linha de comando:

```
./solver.exe -d compactado.huff saida.txt
```

**OBS:** Você pode alterar o formato dos arquivos na linha de comando (.txt, .png, .pdf, etc.).
