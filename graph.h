#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <map>
#include <climits>

class Graph {
    std::map<int, std::map<int, int>> adjList;       // Adjacency List
    std::vector<std::vector<int>> adjMatrix;         // Adjacency Matrix
    int TV;
    bool useMatrix;

public:
    Graph(int vertices, bool useMatrix = false);

    void setMode(bool useMatrixMode);

    void addEdge(int u, int v, int w);

    std::vector<int> dijkstra(int src, int dest, int& distance);
    void printAdjMatrix(const Graph& g, int size);

private:
    int minDistance(const std::vector<int>& dist, const std::vector<bool>& visited);
};

#endif // GRAPH_H