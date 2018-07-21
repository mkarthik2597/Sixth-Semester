#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H

#include <vector>

using namespace std;

class binary_heap
{
      private:
        struct binary_heap_node
        {
                int key;
                int data;
        };

        vector<binary_heap_node> heap;

        void min_heapify(int);
        int search(int);

      public:
        bool empty()
        {
                return heap.empty();
        }

        void insert(int, int);
        int extract_min();
        void decrease_key(int, int);
};

void binary_heap::min_heapify(int current_index)
{
        int left_child_index;
        int right_child_index;
        int smallest_key_index;
        binary_heap_node temporary;

        if (current_index >= heap.size())
        {
                return;
        }

        left_child_index = 2 * current_index + 1;
        right_child_index = left_child_index + 1;

        smallest_key_index = current_index;

        if (left_child_index < heap.size() && heap[left_child_index].key < heap[current_index].key)
        {
                smallest_key_index = left_child_index;
        }

        if (right_child_index < heap.size() && heap[right_child_index].key < heap[smallest_key_index].key)
        {
                smallest_key_index = right_child_index;
        }

        if (smallest_key_index != current_index)
        {
                temporary = heap[smallest_key_index];
                heap[smallest_key_index] = heap[current_index];
                heap[current_index] = temporary;

                min_heapify(smallest_key_index);
        }

        return;
}

int binary_heap::search(int data)
{
        int index;

        for (index = 0; index < heap.size(); index++)
        {
                if (heap[index].data == data)
                {
                        return index;
                }
        }

        return -1;
}

void binary_heap::insert(int key, int data)
{
        int current_index;
        int parent_index;
        binary_heap_node new_node;
        binary_heap_node temporary;

        new_node.key = key;
        new_node.data = data;

        heap.push_back(new_node);

        if (heap.size() == 1)
        {
                return;
        }

        current_index = heap.size() - 1;
        parent_index = (current_index - 1) / 2;

        while (heap[current_index].key < heap[parent_index].key)
        {
                temporary = heap[current_index];
                heap[current_index] = heap[parent_index];
                heap[parent_index] = temporary;

                current_index = parent_index;
                parent_index = (current_index - 1) / 2;
        }

        return;
}

int binary_heap::extract_min()
{
        int data;

        data = heap[0].data;

        heap.erase(heap.begin());

        min_heapify(0);

        return data;
}

void binary_heap::decrease_key(int data, int key)
{
        int current_index;
        int parent_index;
        binary_heap_node temporary;

        current_index = search(data);

        if (current_index != -1 && heap[current_index].key <= key)
        {
                return;
        }

        heap[current_index].key = key;

        if (current_index == 0)
        {
                return;
        }

        parent_index = (current_index - 1) / 2;

        while (heap[current_index].key < heap[parent_index].key)
        {
                temporary = heap[current_index];
                heap[current_index] = heap[parent_index];
                heap[parent_index] = temporary;

                current_index = parent_index;
                parent_index = (current_index - 1) / 2;
        }

        return;
}

#endif // BINARY_HEAP_H
