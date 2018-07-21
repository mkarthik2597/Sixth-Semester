#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>


#define TRUE 1
#define FALSE 0

struct node
{
  /* Heap node attr */
  int key;
  struct node* parent;
  struct node* child;
  struct node* left;
  struct node* right;
  int degree;
  int mark;

  /* MST attr */
  int distance;
};

typedef struct node node;

node* min_node = NULL;
int number_of_nodes = 0;

#include "LinkedList.h"
#include "Queue.h"

node* FIB_HEAP_INSERT(int);
int FIB_HEAP_EXTRACT_MIN();
void CONSOLIDATE();
void FIB_HEAP_LINK(node*,node*);
void FIB_HEAP_DECREASE_KEY(node*,int);
void CUT(node*,node*);
void CASCADING_CUT(node*);
void FIB_HEAP_DELETE(node*);


node* FIB_HEAP_INSERT(int key)
{
  node* new_node = malloc(sizeof(node));
  new_node->key = key;
  new_node->degree = 0;
  new_node->child = NULL;
  new_node->mark = FALSE;
  new_node->distance = INT_MAX;

  add_to_root_list(new_node);

  if(min_node == NULL)
  min_node = new_node;

  else
  {
    if(new_node->distance < min_node->distance)
    min_node = new_node;
  }

  number_of_nodes+=1;
  // printf("%p ",new_node);
  return new_node;
}

int FIB_HEAP_EXTRACT_MIN()
{
  node* z = min_node;
  int key;

  if(z == NULL)
  return -1;

  while(z->child != NULL)
  {
    node* new_child;

    new_child = remove_from_child_list(z->child);
    z->child->parent = NULL;
    add_to_root_list(z->child);

    z->child = new_child;
    z->degree -= 1;
  }

  if(z == z->right)
  {
    min_node = NULL;
    key = z->key;
    free(z);
  }
  else
  {
    min_node = z->right;

    remove_from_root_list(z);

    key = z->key;
    free(z);

    CONSOLIDATE();
  }

  number_of_nodes -= 1;
  return key;
}

void CONSOLIDATE()
{
  int max_degree = log2(number_of_nodes) + 1;

  node* hash_map[max_degree];
  int array_size = max_degree;

  int i;
  for(i=0; i<array_size; i++)
  hash_map[i] = NULL;

  node* traverser = min_node;
  int degree;

  node* x, *y;
  do
  {
    x = traverser;
    degree = x->degree;

    while(hash_map[degree] != NULL && hash_map[degree] != x)
    {
      y = hash_map[degree];

      if(x->distance > y->distance)
      swap(&x,&y);

      FIB_HEAP_LINK(y,x);

      min_node = x;

      hash_map[degree] = NULL;

      degree += 1;
    }

    hash_map[degree] = x;
    traverser = x->right;
  }
  while(traverser != min_node);

  for(i=0; i < array_size; i++)
  {
    if(hash_map[i] != NULL)
    {
      if(hash_map[i]->distance < min_node->distance)
      min_node = hash_map[i];
    }
  }
}

void FIB_HEAP_LINK(node* y, node* x)
{
  remove_from_root_list(y);

  node* z =x->child;

  add_to_child_list(y,z);

  y->parent = x;
  x->child = y;
  x->degree += 1;
  y->mark = FALSE;
}

void FIB_HEAP_DECREASE_KEY(node* x, int k)
{
  if(x == NULL)
  return;

  if(k > x->distance)
  printf("Error! New distance is greater than current distance");

  x->distance = k;
  node* y = x->parent;

  if(y != NULL && x->distance < y->distance)
  {
    CUT(x,y);
    CASCADING_CUT(y);
  }

  if(x->distance < min_node->distance)
  min_node = x;
}

void CUT(node* x, node* y)
{
  node* new_child;
  new_child = remove_from_child_list(x);
  y->child = new_child;
  y->degree -= 1;

  x->parent = NULL;
  add_to_root_list(x);
  x->mark = FALSE;
}

void CASCADING_CUT(node* y)
{
  node* z = y->parent;

  if(z != NULL)
  {
    if(y->mark == FALSE)
    y->mark = TRUE;

    else
    {
      CUT(y,z);
      CASCADING_CUT(z);
    }
  }
}

void FIB_HEAP_DELETE(node* x)
{
  if(x == NULL)
  return;

  FIB_HEAP_DECREASE_KEY(x,-INT_MAX);
  FIB_HEAP_EXTRACT_MIN();
}

node* FIB_HEAP_SEARCH(int key)
{
  init_queue(number_of_nodes);

  node* root_traverser = min_node;
  node* tree_traverser = root_traverser;

  if(tree_traverser == NULL)
  return NULL;

  do
  {
    enqueue(root_traverser);

    node* x;

    while(Q.count != 0)
    {
      x = dequeue();
      if(x->key == key)
      return x;

      if(x->child != NULL)
      {
        tree_traverser = x->child;
        do
        {
          enqueue(tree_traverser);
          tree_traverser = tree_traverser->right;
        }
        while(tree_traverser != x->child);

      }
    }

    root_traverser = root_traverser->right;
  }
  while(root_traverser != min_node);

  return NULL;
}
//
// int main()
// {
// repeat:
//   printf("\n\nEnter\n1 to insert\n2 to extract min\n3 to decrease key\n4 to delete\n5 to exit:");
//   int i;
//   scanf("%d",&i);
//
//   int key;
//   node* x;
//
//   switch(i)
//   {
//     case 1: printf("\nEnter a series of keys(-999 to exit):");
//
//             while(scanf("%d",&key),key != -999)
//             FIB_HEAP_INSERT(key);
//
//             break;
//
//     case 2: FIB_HEAP_EXTRACT_MIN();
//             break;
//
//     case 3: printf("\nEnter key to decrease:");
//             scanf("%d",&key);
//             x = FIB_HEAP_SEARCH(key);
//             printf("\nEnter the new key value:");
//             scanf("%d",&key);
//             FIB_HEAP_DECREASE_KEY(x,key);
//             break;
//
//     case 4: printf("\nEnter key to delete:");
//             scanf("%d",&key);
//             x = FIB_HEAP_SEARCH(key);
//             FIB_HEAP_DELETE(x);
//   }
//
//   if(i != 5)
//   {
//     printf("Final call:");
//     display_heap();
//     goto repeat;
//   }
// }
