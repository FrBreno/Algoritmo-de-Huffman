#include "bqueue.h"

bqueue::bqueue(FILE *_file_)
{
  fout = _file_;
}

void bqueue::pushSring(string s)
{
  int sizeStr = (int)s.length();
  for (int i{0}; i < sizeStr; i++)
  {
    fila.push(s[i]);

    if (fila.size() == 8 || i == sizeStr - 1)
    {
      unsigned char c{0};
      for (int j{0}; j < 8 && fila.size() != 0; j++)
      {
        // SEM CONVERSÃO:
        // c = (int)fila.front();
        // fila.pop();
        // putc(c, fout);

        // COM CONVERSÃO:
        c <<= 1;
        c += fila.front() - '0';
        fila.pop();
      }
      putc(c, fout); // Usar com conversão.
    }
  }

  return;
}