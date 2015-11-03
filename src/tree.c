#include "tree.h"
#include <malloc.h>

node_t *create_node()
{
  node_t *node = malloc(sizeof(node_t));
  return node;
}

