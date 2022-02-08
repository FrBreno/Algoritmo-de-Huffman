#include "prefix.h"

prefix_node::prefix_node(unsigned char key, unsigned long freq)
{
  this->c = key;
  this->freq = freq;
  this->prxox = nullptr;
  this->left = nullptr;
  this->right = nullptr;
}

prefix::prefix()
{
  this->list_root = nullptr;
}

prefix_node *prefix::add_prefix(unsigned char key, unsigned long freq)
{
  prefix_node *novoNo = new prefix_node(key, freq);
  prefix_node *pos_atual, *pos_anterior;

  if (!this->list_root)
  {
    this->list_root = novoNo;
    return novoNo;
  }

  // Adicionando e ordenando (ordem crescente):
  for (pos_anterior = nullptr, pos_atual = this->list_root; pos_atual; pos_anterior = pos_atual, pos_atual = pos_atual->prxox)
  {
    if (freq < pos_atual->freq)
    {
      // Posição encontrada
      break;
    }
  }

  // Número inserido como primeiro:
  if (!pos_anterior)
  {
    novoNo->prxox = pos_atual;
    this->list_root = novoNo;
  }
  else if (pos_atual == nullptr) // Número inserido na última posição:
  {
    pos_anterior->prxox = novoNo;
  }
  else
  {
    pos_anterior->prxox = novoNo;
    novoNo->prxox = pos_atual;
  }
  return novoNo;
}

prefix_node *prefix::take_min_node()
{
  return this->list_root;
}

prefix_node *prefix::fusion(prefix_node *a, prefix_node *b)
{
  unsigned long freqTotal = a->freq + b->freq;
  this->list_root = b->prxox;

  prefix_node *novoNo = add_prefix(0, freqTotal);

  novoNo->left = a;
  novoNo->right = b;
  a->prxox = nullptr;
  b->prxox = nullptr;

  return novoNo;
}
