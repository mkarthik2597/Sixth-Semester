#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

void generate_graph(int number_of_vertices)
{
        int i, u, v, weight;
        fstream file;

        file.open("graph.txt", ios::out);

        for (v = 0; v < number_of_vertices; v++)
        {
                file << v << "\n";
        }

        file << "#\n";

        srand(time(nullptr));

        for (u = 0; u < number_of_vertices; u++)
        {
                for (i = 0; i < 100; i++)
                {
                        v = rand() % number_of_vertices;

                        if (u == v)
                        {
                                i--;

                                continue;
                        }

                        weight = rand() % 99 + 1;

                        if (weight > 0)
                        {
                                file << u << "\t" << v << "\t" << weight << "\n";
                        }
                }
        }

        file << endl;

        file.close();

        return;
}

int main(int argc, char **argv)
{
        if (argc == 1)
        {
                cout << "ERROR: Enter the number of vertices for the graph as a program argument" << endl;

                return 1;
        }

        generate_graph(atoi(argv[1]));

        return 0;
}