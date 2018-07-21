#ifndef BINOMIAL_HEAP_H
#define BINOMIAL_HEAP_H

#include <list>
#include <limits>

using namespace std;

class binomial_heap
{
      private:
        struct binomial_tree_node
        {
                int key;
                int data;
                int degree;
                binomial_tree_node *parent;
                binomial_tree_node *child;
                binomial_tree_node *sibling;

                binomial_tree_node()
                {
                        key = 0;
                        degree = 0;
                        parent = nullptr;
                        child = nullptr;
                        sibling = nullptr;
                }

                ~binomial_tree_node()
                {
                        key = 0;
                        degree = 0;
                        parent = nullptr;
                        child = nullptr;
                        sibling = nullptr;
                }
        };

        static bool binomial_tree_compare(binomial_tree_node *, binomial_tree_node *);
        static binomial_tree_node *binomial_tree_merge(binomial_tree_node *&, binomial_tree_node *&);
        static binomial_tree_node *binomial_tree_search(binomial_tree_node *, int);
        static void binomial_tree_destroy(binomial_tree_node *&);

        list<binomial_tree_node *> binomial_tree_list;

        static list<binomial_tree_node *> binomial_tree_list_merge(list<binomial_tree_node *> &, list<binomial_tree_node *> &);

        binomial_tree_node *search(int);

      public:
        bool empty()
        {
                return binomial_tree_list.empty();
        }

        void insert(int, int);
        int extract_min();
        void decrease_key(int, int);

        ~binomial_heap()
        {
                list<binomial_tree_node *>::iterator iterator;

                for (iterator = binomial_tree_list.begin(); iterator != binomial_tree_list.end(); iterator++)
                {
                        binomial_tree_destroy(*iterator);
                }

                binomial_tree_list.erase(binomial_tree_list.begin(), binomial_tree_list.end());
        }
};

bool binomial_heap::binomial_tree_compare(binomial_tree_node *root_1, binomial_tree_node *root_2)
{
        if (root_1 == nullptr || root_2 == nullptr)
        {
                return false;
        }

        if (root_1->degree <= root_2->degree)
        {
                return true;
        }

        return false;
}

binomial_heap::binomial_tree_node *binomial_heap::binomial_tree_merge(binomial_tree_node *&root_1, binomial_tree_node *&root_2)
{
        binomial_heap::binomial_tree_node *root_3;

        if (root_1 == nullptr || root_2 == nullptr || root_1->degree != root_2->degree)
        {
                return nullptr;
        }

        if (root_1->key <= root_2->key)
        {
                root_2->parent = root_1;
                root_2->sibling = root_1->child;
                root_1->child = root_2;
                root_1->degree++;

                root_3 = root_1;
        }
        else
        {
                root_1->parent = root_2;
                root_1->sibling = root_2->child;
                root_2->child = root_1;
                root_2->degree++;

                root_3 = root_2;
        }

        root_1 = nullptr;
        root_2 = nullptr;

        return root_3;
}

binomial_heap::binomial_tree_node *binomial_heap::binomial_tree_search(binomial_tree_node *root, int data)
{
        binomial_tree_node *node = nullptr;

        if (root == nullptr)
        {
                return nullptr;
        }

        if (root->data == data)
        {
                return root;
        }

        node = binomial_tree_search(root->sibling, data);

        if (node != nullptr)
        {
                return node;
        }

        return binomial_tree_search(root->child, data);
}

void binomial_heap::binomial_tree_destroy(binomial_tree_node *&root)
{
        if (root == nullptr)
        {
                return;
        }

        binomial_tree_destroy(root->sibling);
        binomial_tree_destroy(root->child);

        delete root;
        root = nullptr;

        return;
}

list<binomial_heap::binomial_tree_node *> binomial_heap::binomial_tree_list_merge(list<binomial_tree_node *> &binomial_tree_list_1, list<binomial_tree_node *> &binomial_tree_list_2)
{
        list<binomial_tree_node *> binomial_tree_list_3;
        list<binomial_tree_node *>::iterator previous_iterator;
        list<binomial_tree_node *>::iterator current_iterator;
        list<binomial_tree_node *>::iterator next_iterator;
        list<binomial_tree_node *>::iterator temporary;

        if (binomial_tree_list_1.empty() && binomial_tree_list_2.empty())
        {
                return binomial_tree_list_3;
        }

        binomial_tree_list_3.merge(binomial_tree_list_1, binomial_tree_compare);
        binomial_tree_list_3.merge(binomial_tree_list_2, binomial_tree_compare);

        previous_iterator = binomial_tree_list_3.begin();

        while (next(previous_iterator) != binomial_tree_list_3.end())
        {
                current_iterator = next(previous_iterator);
                next_iterator = next(current_iterator);

                if ((*previous_iterator)->degree == (*current_iterator)->degree)
                {
                        if (next_iterator != binomial_tree_list_3.end() && (*current_iterator)->degree == (*next_iterator)->degree)
                        {
                                temporary = binomial_tree_list_3.insert(current_iterator, binomial_tree_merge(*current_iterator, *next_iterator));

                                binomial_tree_list_3.erase(current_iterator);
                                binomial_tree_list_3.erase(next_iterator);
                        }
                        else
                        {
                                temporary = binomial_tree_list_3.insert(current_iterator, binomial_tree_merge(*previous_iterator, *current_iterator));

                                binomial_tree_list_3.erase(previous_iterator);
                                binomial_tree_list_3.erase(current_iterator);
                        }

                        previous_iterator = temporary;
                }
                else
                {
                        previous_iterator++;
                }
        }

        return binomial_tree_list_3;
}

binomial_heap::binomial_tree_node *binomial_heap::search(int data)
{
        binomial_tree_node *node = nullptr;
        list<binomial_tree_node *>::iterator iterator;

        for (iterator = binomial_tree_list.begin(); iterator != binomial_tree_list.end(); iterator++)
        {
                node = binomial_tree_search(*iterator, data);

                if (node != nullptr)
                {
                        return node;
                }
        }

        return nullptr;
}

void binomial_heap::insert(int key, int data)
{
        binomial_tree_node *new_node = nullptr;
        list<binomial_tree_node *> temporary;

        new_node = new binomial_tree_node;

        new_node->key = key;
        new_node->data = data;

        temporary.push_back(new_node);

        binomial_tree_list = binomial_tree_list_merge(binomial_tree_list, temporary);

        return;
}

int binomial_heap::extract_min()
{
        int data;
        binomial_tree_node *old_node = nullptr;
        binomial_tree_node *temporary_node = nullptr;
        list<binomial_tree_node *> temporary_list;
        list<binomial_tree_node *>::iterator iterator;
        list<binomial_tree_node *>::iterator minimum_iterator;

        if (binomial_tree_list.empty())
        {
                return numeric_limits<int>::max();
        }

        minimum_iterator = binomial_tree_list.begin();

        for (iterator = next(binomial_tree_list.begin()); iterator != binomial_tree_list.end(); iterator++)
        {
                if ((*iterator)->key < (*minimum_iterator)->key)
                {
                        minimum_iterator = iterator;
                }
        }

        old_node = *minimum_iterator;

        data = old_node->data;

        while (old_node->child != nullptr)
        {
                temporary_node = old_node->child->sibling;
                old_node->child->parent = nullptr;
                old_node->child->sibling = nullptr;

                temporary_list.push_front(old_node->child);

                old_node->child = temporary_node;
        }

        delete old_node;
        old_node = nullptr;

        binomial_tree_list.erase(minimum_iterator);

        binomial_tree_list = binomial_tree_list_merge(binomial_tree_list, temporary_list);

        return data;
}

void binomial_heap::decrease_key(int data, int key)
{
        int temporary;
        binomial_tree_node *node = nullptr;

        node = search(data);

        if (node == nullptr || node->key <= key)
        {
                return;
        }

        node->key = key;

        while (node->parent != nullptr && node->parent->key > node->key)
        {
                temporary = node->key;
                node->key = node->parent->key;
                node->parent->key = temporary;

                temporary = node->data;
                node->data = node->parent->data;
                node->parent->data = temporary;

                node = node->parent;
        }

        return;
}

#endif // BINOMIAL_HEAP_H
