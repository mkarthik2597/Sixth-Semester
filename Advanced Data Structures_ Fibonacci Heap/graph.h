#include<stdio.h>
#include<stdlib.h>

struct vertex
{
  int key;
  int distance;
  struct vertex* next;
};
typedef struct vertex vertex;

vertex** graph;
int number_of_vertices;

void create_G()
{
  FILE* fp_read;
  fp_read = fopen("input.txt","r");

  int input_array[500],count=0;
  int x;

  while(fscanf(fp_read,"%d",&x),x != -999)
  input_array[count++] = x;

  number_of_vertices = count;

  graph = malloc(number_of_vertices * sizeof(vertex*));

  int i;
  for(i=0; i<number_of_vertices; i++)
  graph[i]=NULL;

  int v1,v2,weight;
  while(fscanf(fp_read,"%d %d %d",&v1,&v2,&weight) != EOF)
  {
    vertex* temp = graph[v1];
    graph[v1] = malloc(sizeof(vertex));
    graph[v1]->next = temp;
    graph[v1]->key = v2;
    graph[v1]->distance = weight;

    temp = graph[v2];
    graph[v2] = malloc(sizeof(vertex));
    graph[v2]->next = temp;
    graph[v2]->key = v1;
    graph[v2]->distance = weight;
  }
}

void print_G()
{
  int i;
  printf("\n\n");

  for(i=0; i<number_of_vertices; i++)
  {
    printf("%d->",i);

    vertex* traverser = graph[i];
    while(traverser != NULL)
    {
      printf("%d(%d) ",traverser->key, traverser->distance);
      traverser = traverser->next;
    }
    printf("\n");
  }
  printf("\n\n");
}
//
// int main()
// {
//   create_G();
//   print_G();
// }
