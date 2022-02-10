#include <iostream>

using namespace std;

struct prefix_node
{
  prefix_node *prxox;
  prefix_node *left, *right;
  unsigned long freq;
  unsigned char c;
  prefix_node(unsigned char key, unsigned long freq);
};

class prefix
{
  prefix_node *list_root; // raiz

public:
  prefix();
  prefix_node *add_prefix(unsigned char key, unsigned long freq);
  prefix_node *take_min_node(); // pegar o menor no
  prefix_node *fusion(prefix_node *a, prefix_node *b);
};