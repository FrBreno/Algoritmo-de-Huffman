#include "bqueue.h"

bqueue::bqueue(FILE *_file_)
{
  fout = _file_;
}

void bqueue::pushSring(string s, bool resto)
{
  int sizeStr = (int)s.length();
  for (int i{0}; i < sizeStr; i++)
  {
    fila.push(s[i]);

    if (fila.size() == 8)
    {
      unsigned char c{0};
      for (int j{0}; j < 8 && fila.size() != 0; j++)
      {
        c <<= 1;
        c += fila.front() - '0';
        fila.pop();
      }
      fwrite(&c, sizeof(char), 1, fout);
    }
  }
  if (resto && fila.size() < 8) // Explicação no .h.
  {
    unsigned char c{0};
    for (int j{0}; fila.size() != 0; j++)
    {
      c <<= 1;
      c += fila.front() - '0';
      fila.pop();
    }
    fwrite(&c, sizeof(char), 1, fout);
  }

  return;
}