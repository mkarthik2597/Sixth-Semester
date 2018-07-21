#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include "binary_heap.h"
#include "binomial_heap.h"
#include "fibonacci_heap.h"
#include "graph.h"

map<string, int> vertex_id;
map<int, string> vertex_name;

graph *get_graph_from_file(const char *filename)
{
        int u;
        int v;
        graph *g = nullptr;
        string input;
        fstream file;
        adjacency_list_node node;

        file.open(filename, fstream::in);

        if (!file)
        {
                cout << "ERROR: " << filename << " not found" << endl;

                exit(1);
        }

        g = new graph;

        while (true)
        {
                file >> input;

                if (file.eof() || input.compare("#") == 0)
                {
                        break;
                }

                vertex_name[g->number_of_vertices] = input;
                vertex_id[input] = g->number_of_vertices;

                g->number_of_vertices++;
        }

        g->adjacency_list.resize(g->number_of_vertices);

        while (true)
        {
                file >> input;

                if (file.eof())
                {
                        break;
                }

                u = vertex_id[input];

                file >> input;

                if (file.eof())
                {
                        break;
                }

                v = vertex_id[input];

                file >> input;

                if (file.eof())
                {
                        break;
                }

                node.uv_weight = atoi(input.c_str());

                node.v = v;

                g->adjacency_list[u].push_back(node);

                node.v = u;

                g->adjacency_list[v].push_back(node);

                g->number_of_edges++;
        }

        file.close();

        return g;
}

void graph_display_edge_list(graph *g)
{
        int u;
        int v;
        int cost;
        list<adjacency_list_node>::iterator iterator;

        if (g->number_of_edges == 0)
        {
                cout << "No edges" << endl;

                return;
        }

        cost = 0;

        for (u = 0; u < g->number_of_vertices; u++)
        {
                for (iterator = g->adjacency_list[u].begin(); iterator != g->adjacency_list[u].end(); iterator++)
                {
                        cost += (*iterator).uv_weight;
                }
        }

        cout << "Total cost: " << cost << endl
             << endl;

        return;
}

int main(int argc, char **argv)
{
        graph *g = nullptr;
        graph minimum_spanning_forest;
        double start;
        double finish;
        double time_elapsed;

        if (argc == 1)
        {
                cout << "ERROR: Enter graph source filename as a program argument" << endl;

                return 1;
        }

        g = get_graph_from_file(argv[1]);

        cout << "File Reading Complete!" << endl;

        start = clock();
        minimum_spanning_forest = minimum_spanning_forest_prim<binary_heap>(g);
        finish = clock();

        time_elapsed = (finish - start) * 1e-6;

        cout << endl
             << "Time taken for Prim's algorithm using a Binary Heap: " << time_elapsed << " seconds" << endl;
        graph_display_edge_list(&minimum_spanning_forest);

        minimum_spanning_forest.number_of_edges = 0;
        minimum_spanning_forest.number_of_vertices = 0;
        minimum_spanning_forest.adjacency_list.clear();

        start = clock();
        minimum_spanning_forest = minimum_spanning_forest_prim<binomial_heap>(g);
        finish = clock();

        time_elapsed = (finish - start) * 1e-6;

        cout << endl
             << "Time taken for Prim's algorithm using a Binomial Heap: " << time_elapsed << " seconds" << endl;
        graph_display_edge_list(&minimum_spanning_forest);

        minimum_spanning_forest.number_of_edges = 0;
        minimum_spanning_forest.number_of_vertices = 0;
        minimum_spanning_forest.adjacency_list.clear();

        start = clock();
        minimum_spanning_forest = minimum_spanning_forest_prim<fibonacci_heap>(g);
        finish = clock();

        time_elapsed = (finish - start) * 1e-6;

        cout << endl
             << "Time taken for Prim's algorithm using a Fibonacci Heap: " << time_elapsed << " seconds" << endl;
        graph_display_edge_list(&minimum_spanning_forest);

        minimum_spanning_forest.number_of_edges = 0;
        minimum_spanning_forest.number_of_vertices = 0;
        minimum_spanning_forest.adjacency_list.clear();

        delete g;

        return 0;
}
