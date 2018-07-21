#include "FibonacciHeap.h"
#include "graph.h"
#include<time.h>

struct MSTnode
{
  int key;
  int parent;
};

typedef struct MSTnode MSTnode;

MSTnode* MST;
node* x, *y;
node** heap_access;

void calculate_MSTweight()
{
  int i;
  int MSTweight = 0;
  for(i=1; i<number_of_vertices; i++)
  {
    if(MST[i].parent == -1)
    {
      printf("Spanning tree is wrong!\n");
      break;
    }

    vertex* traverser = graph[i];
    while(traverser != NULL)
    {
      if(traverser->key != MST[i].parent)
      traverser = traverser->next;
      else
      break;
    }

    MSTweight += traverser->distance;
  }

  printf("Weight of MST = %d\n",MSTweight);
}

void MST_PRIM(int root)
{
  clock_t start,end;
  double cpu_time_used;

  FILE* fp_write;
  fp_write = fopen("output.txt","w");

  start = clock();
  int i;
  for(i=0; i<number_of_vertices; i++)
  {
    heap_access[i] = FIB_HEAP_INSERT(i);
    MST[i].key = i;
    MST[i].parent = -1;
  }

  x = heap_access[root];

  FIB_HEAP_DECREASE_KEY(x,0);

  int u;
  vertex* traverser;

  while(min_node != NULL)
  {
    u = FIB_HEAP_EXTRACT_MIN();
    heap_access[u] = NULL;

    traverser = graph[u];

    while(traverser != NULL)
    {
      x = heap_access[traverser->key];

      if(x != NULL && traverser->distance < x->distance)
      {
        MST[x->key].parent = u;
        FIB_HEAP_DECREASE_KEY(x,traverser->distance);
      }

      traverser = traverser->next;
    }
  }

  end = clock();

  for(i=0; i<number_of_vertices; i++)
  fprintf(fp_write,"%d\t%d\n",MST[i].key,MST[i].parent);

  printf("\nOutput file generated!\n");
  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  printf("Time taken: %f seconds\n\n",cpu_time_used);

  calculate_MSTweight();

}

int main()
{
  create_G();

  MST = malloc(number_of_vertices * sizeof(MSTnode));
  heap_access = malloc(number_of_vertices * sizeof(node*));
  //
  // printf("Choose a root vertex:");
  // int root;
  // scanf("%d",&root);

  MST_PRIM(0);
}
