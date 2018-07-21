struct queue
{
  node** array;
  int front;
  int rear;
  int count;
  int capacity;
};

typedef struct queue queue;
queue Q;

void display_queue()
{
  int i = Q.rear, count = 0;

  while (count != Q.capacity)
  {
    if(Q.array[i] == NULL)
    printf("NULL ");
    else
    printf("%d ",Q.array[i]->key);

    i = (i + 1) % Q.capacity;
    count++;
  }
}

void enqueue(node* x)
{
  if(Q.count == Q.capacity)
  {
    printf("Error while enqueing %d\n",x->key);
    getchar();

    printf("Queue contents:\n");
    display_queue();
    return;
  }

  Q.array[Q.front] = x;
  Q.front = (Q.front + 1) % Q.capacity;
  Q.count++;
}

node* dequeue()
{
  if(Q.count == 0)
  {
    printf("Error while dequeing");
    getchar();
    return NULL;
  }

  node* x = Q.array[Q.rear];
  Q.rear = (Q.rear + 1) % Q.capacity;
  Q.count--;

  return x;
}

void init_queue(int size)
{
  Q.array = malloc(size * sizeof(node*));
  Q.front = 0;
  Q.rear = 0;
  Q.count = 0;
  Q.capacity = size;
}

void display_heap()
{
  init_queue(number_of_nodes);

  node* root_traverser = min_node;
  node* tree_traverser = root_traverser;

  if(tree_traverser == NULL)
  {
    printf("\n\nHeap is empty\n");
    return;
  }

  int level;
  do
  {
    level = 0;
    printf("\n\nLevel 0:  ");

    enqueue(root_traverser);
    enqueue(NULL);

    node* x;

    while(1)
    {
      x = dequeue();

      if(x->mark)
      printf("%d* ",x->key);
      else
      printf("%d ",x->key);

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

      if(Q.array[Q.rear] == NULL)
      {
        dequeue();

        if(Q.count > 0)
        {
          enqueue(NULL);
          printf("\nLevel %d:  ",++level);
        }
        else
        break;
      }
    }

    root_traverser = root_traverser->right;
  }
  while(root_traverser != min_node);

  free(Q.array);
}
