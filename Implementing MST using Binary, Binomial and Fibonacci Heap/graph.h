#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

#include <list>
#include <limits>

using namespace std;

struct adjacency_list_node
{
        int v;
        int uv_weight;

        adjacency_list_node()
        {
                v = 0;
                uv_weight = 0;
        }

        ~adjacency_list_node()
        {
                v = 0;
                uv_weight = 0;
        }
};

struct graph
{
        int number_of_edges;
        int number_of_vertices;

        vector<list<adjacency_list_node>> adjacency_list;

        graph()
        {
                number_of_edges = 0;
                number_of_vertices = 0;
        }

        ~graph()
        {
                number_of_edges = 0;
                number_of_vertices = 0;
        }
};

template <typename heap>
graph minimum_spanning_forest_prim(graph *g)
{
        int u;
        int v;
        int uv_weight;
        adjacency_list_node node;
        heap h;
        graph minimum_spanning_forest;
        vector<int> key;
        vector<int> parent;
        vector<bool> in_minimum_spanning_forest;
        list<adjacency_list_node>::iterator iterator;

        key.insert(key.end(), g->number_of_vertices, numeric_limits<int>::max());
        parent.insert(parent.end(), g->number_of_vertices, -1);
        in_minimum_spanning_forest.insert(in_minimum_spanning_forest.end(), g->number_of_vertices, false);

        for (v = 0; v < g->number_of_vertices; v++)
        {
                h.insert(key[v], v);
        }

        while (!h.empty())
        {
                u = h.extract_min();

                in_minimum_spanning_forest[u] = true;

                for (iterator = g->adjacency_list[u].begin(); iterator != g->adjacency_list[u].end(); iterator++)
                {
                        v = (*iterator).v;
                        uv_weight = (*iterator).uv_weight;

                        if (!in_minimum_spanning_forest[v] && uv_weight < key[v])
                        {
                                key[v] = uv_weight;
                                parent[v] = u;

                                h.decrease_key(v, key[v]);
                        }
                }
        }

        minimum_spanning_forest.number_of_vertices = g->number_of_vertices;
        minimum_spanning_forest.number_of_edges = 0;
        minimum_spanning_forest.adjacency_list.resize(minimum_spanning_forest.number_of_vertices);

        for (v = 0; v < g->number_of_vertices; v++)
        {
                u = parent[v];

                if (u == -1)
                {
                        continue;
                }

                node.uv_weight = key[v];

                node.v = v;

                minimum_spanning_forest.adjacency_list[u].push_back(node);

                node.v = u;

                minimum_spanning_forest.adjacency_list[v].push_back(node);

                minimum_spanning_forest.number_of_edges++;
        }

        return minimum_spanning_forest;
}

#endif // GRAPH_H
