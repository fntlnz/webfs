#include <stdlib.h>

#ifndef TREE_H
#define TREE_H
enum node_type {
  TYPE_FOLDER,
  TYPE_FILE,
};

struct node_location {
  char *location;
};

typedef struct node_location node_location_t;

struct node {
  struct node *parent;
  struct node **children;
  size_t num_children;
  size_t size;
  char *name;
  enum node_type type;
  node_location_t **location;
};

typedef struct node node_t;

node_t *create_node();
#endif
