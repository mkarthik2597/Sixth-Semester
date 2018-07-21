#ifndef FIBONACCI_HEAP_H
#define FIBONACCI_HEAP_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define TRUE 1
#define FALSE 0

struct node
{
        /* Heap node attr */
        int key;
        int data;
        struct node *parent;
        struct node *child;
        struct node *left;
        struct node *right;
        int degree;
        int mark;

        /* MST attr */
        int distance;
};

typedef struct node node;

node *min_node = NULL;
int number_of_nodes = 0;

void add_to_root_list(node *new_node)
{
        if (min_node == NULL)
        {
                new_node->left = new_node->right = new_node;
        }

        else
        {
                new_node->left = min_node;
                new_node->right = min_node->right;
                min_node->right = new_node;
                new_node->right->left = new_node;
        }
}

void add_to_child_list(node *new_node, node *head)
{
        if (head == NULL)
        {
                new_node->left = new_node->right = new_node;
                return;
        }

        new_node->left = head;
        new_node->right = head->right;
        head->right = new_node;
        new_node->right->left = new_node;
}

node *remove_from_child_list(node *head)
{
        head->left->right = head->right;
        head->right->left = head->left;

        node *next_child;

        if (head->right != head)
                next_child = head->right;
        else
                next_child = NULL;

        head->left = head->right = head;

        return next_child;
}

void remove_from_root_list(node *head)
{
        head->left->right = head->right;
        head->right->left = head->left;

        head->left = head->right = head;
}

void swap(node **x, node **y)
{
        node *temp = *x;
        *x = *y;
        *y = temp;
}

struct queue
{
        node **array;
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
                if (Q.array[i] == NULL)
                        printf("NULL ");
                else
                        printf("%d ", Q.array[i]->key);

                i = (i + 1) % Q.capacity;
                count++;
        }
}

void enqueue(node *x)
{
        if (Q.count == Q.capacity)
        {
                printf("Error while enqueing %d\n", x->key);
                getchar();

                printf("Queue contents:\n");
                display_queue();
                return;
        }

        Q.array[Q.front] = x;
        Q.front = (Q.front + 1) % Q.capacity;
        Q.count++;
}

node *dequeue()
{
        if (Q.count == 0)
        {
                printf("Error while dequeing");
                getchar();
                return NULL;
        }

        node *x = Q.array[Q.rear];
        Q.rear = (Q.rear + 1) % Q.capacity;
        Q.count--;

        return x;
}

void init_queue(int size)
{
        Q.array = (node **)malloc(size * sizeof(node *));
        Q.front = 0;
        Q.rear = 0;
        Q.count = 0;
        Q.capacity = size;
}

void display_heap()
{
        init_queue(number_of_nodes);

        node *root_traverser = min_node;
        node *tree_traverser = root_traverser;

        if (tree_traverser == NULL)
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

                node *x;

                while (1)
                {
                        x = dequeue();

                        if (x->mark)
                                printf("%d* ", x->key);
                        else
                                printf("%d ", x->key);

                        if (x->child != NULL)
                        {
                                tree_traverser = x->child;
                                do
                                {
                                        enqueue(tree_traverser);
                                        tree_traverser = tree_traverser->right;
                                } while (tree_traverser != x->child);
                        }

                        if (Q.array[Q.rear] == NULL)
                        {
                                dequeue();

                                if (Q.count > 0)
                                {
                                        enqueue(NULL);
                                        printf("\nLevel %d:  ", ++level);
                                }
                                else
                                        break;
                        }
                }

                root_traverser = root_traverser->right;
        } while (root_traverser != min_node);

        free(Q.array);
}

node *FIB_HEAP_INSERT(int, int);
int FIB_HEAP_EXTRACT_MIN();
void CONSOLIDATE();
void FIB_HEAP_LINK(node *, node *);
void FIB_HEAP_DECREASE_KEY(node *, int);
void CUT(node *, node *);
void CASCADING_CUT(node *);
void FIB_HEAP_DELETE(node *);

node *FIB_HEAP_INSERT(int key, int data)
{
        node *new_node = (node *)malloc(sizeof(node));
        new_node->key = key;
        new_node->data = data;
        new_node->degree = 0;
        new_node->parent = NULL;
        new_node->child = NULL;
        new_node->mark = FALSE;
        new_node->distance = INT_MAX;

        add_to_root_list(new_node);

        if (min_node == NULL)
                min_node = new_node;

        else
        {
                if (new_node->distance < min_node->distance)
                        min_node = new_node;
        }

        number_of_nodes += 1;
        // printf("%p ",new_node);
        return new_node;
}

int FIB_HEAP_EXTRACT_MIN()
{
        node *z = min_node;
        int data;

        if (z == NULL)
                return -1;

        while (z->child != NULL)
        {
                node *new_child;

                new_child = remove_from_child_list(z->child);
                z->child->parent = NULL;
                add_to_root_list(z->child);

                z->child = new_child;
                z->degree -= 1;
        }

        if (z == z->right)
        {
                min_node = NULL;
                data = z->data;
                free(z);
        }
        else
        {
                min_node = z->right;

                remove_from_root_list(z);

                data = z->data;
                free(z);

                CONSOLIDATE();
        }

        number_of_nodes -= 1;
        return data;
}

void CONSOLIDATE()
{
        int max_degree = 100;

        node *hash_map[max_degree];
        int array_size = max_degree;

        int i;
        for (i = 0; i < array_size; i++)
                hash_map[i] = NULL;

        node *traverser = min_node;
        int degree;

        node *x, *y;
        do
        {
                x = traverser;
                degree = x->degree;

                while (hash_map[degree] != NULL && hash_map[degree] != x)
                {
                        y = hash_map[degree];

                        if (x->distance > y->distance)
                                swap(&x, &y);

                        FIB_HEAP_LINK(y, x);

                        min_node = x;

                        hash_map[degree] = NULL;

                        degree += 1;
                }

                hash_map[degree] = x;
                traverser = x->right;
        } while (traverser != min_node);

        for (i = 0; i < array_size; i++)
        {
                if (hash_map[i] != NULL)
                {
                        if (hash_map[i]->distance < min_node->distance)
                                min_node = hash_map[i];
                }
        }
}

void FIB_HEAP_LINK(node *y, node *x)
{
        remove_from_root_list(y);

        node *z = x->child;

        add_to_child_list(y, z);

        y->parent = x;
        x->child = y;
        x->degree += 1;
        y->mark = FALSE;
}

void FIB_HEAP_DECREASE_KEY(node *x, int k)
{
        if (x == NULL)
                return;

        if (k > x->distance)
                printf("Error! New distance is greater than current distance");

        x->distance = k;
        node *y = x->parent;

        if (y != NULL && x->distance < y->distance)
        {
                CUT(x, y);
                CASCADING_CUT(y);
        }

        if (x->distance < min_node->distance)
                min_node = x;
}

void CUT(node *x, node *y)
{
        node *new_child;
        new_child = remove_from_child_list(x);
        y->child = new_child;
        y->degree -= 1;

        x->parent = NULL;
        add_to_root_list(x);
        x->mark = FALSE;
}

void CASCADING_CUT(node *y)
{
        node *z = y->parent;

        if (z != NULL)
        {
                if (y->mark == FALSE)
                        y->mark = TRUE;

                else
                {
                        CUT(y, z);
                        CASCADING_CUT(z);
                }
        }
}

void FIB_HEAP_DELETE(node *x)
{
        if (x == NULL)
                return;

        FIB_HEAP_DECREASE_KEY(x, -INT_MAX);
        FIB_HEAP_EXTRACT_MIN();
}

node *FIB_HEAP_SEARCH(int data)
{
        node *root_traverser = min_node;
        node *tree_traverser = root_traverser;

        if (tree_traverser == NULL)
                return NULL;

        init_queue(number_of_nodes);

        do
        {
                enqueue(root_traverser);

                node *x;

                while (Q.count != 0)
                {
                        x = dequeue();
                        if (x->data == data)
                        {
                                while (Q.count != 0)
                                {
                                        dequeue();
                                }

                                free(Q.array);
                                Q.array = NULL;

                                return x;
                        }

                        if (x->child != NULL)
                        {
                                tree_traverser = x->child;
                                do
                                {
                                        enqueue(tree_traverser);
                                        tree_traverser = tree_traverser->right;
                                } while (tree_traverser != x->child);
                        }
                }

                root_traverser = root_traverser->right;
        } while (root_traverser != min_node);

        while (Q.count != 0)
        {
                dequeue();
        }

        free(Q.array);
        Q.array = NULL;

        return NULL;
}

class fibonacci_heap
{
      public:
        bool empty()
        {
                return min_node == NULL;
        }

        void insert(int, int);
        int extract_min();
        void decrease_key(int, int);
};

void fibonacci_heap::insert(int key, int data)
{
        FIB_HEAP_INSERT(key, data);

        return;
}

int fibonacci_heap::extract_min()
{
        return FIB_HEAP_EXTRACT_MIN();
}

void fibonacci_heap::decrease_key(int data, int key)
{
        FIB_HEAP_DECREASE_KEY(FIB_HEAP_SEARCH(data), key);

        return;
}

#endif // FIBONACCI_HEAP_H
