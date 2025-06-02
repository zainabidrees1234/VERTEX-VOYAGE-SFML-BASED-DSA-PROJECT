#include "graph.h"
#include<iostream>


Graph::Graph(int vertices, bool useMatrix)
    : TV(vertices), useMatrix(useMatrix) {
    adjMatrix.resize(vertices, std::vector<int>(vertices, INT_MAX));
}

void Graph::setMode(bool useMatrixMode) {
    useMatrix = useMatrixMode;
}

void Graph::addEdge(int u, int v, int w) {
    adjList[u][v] = adjList[v][u] = w;
    adjMatrix[u][v] = adjMatrix[v][u] = w;
}

int Graph::minDistance(const std::vector<int>& dist, const std::vector<bool>& visited) {
    int min = INT_MAX, minIdx = -1;
    for (int i = 0; i < TV; ++i)
        if (!visited[i] && dist[i] <= min)
            min = dist[i], minIdx = i;
    return minIdx;
}
void Graph::printAdjMatrix(const Graph& g, int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            int val = g.adjMatrix[i][j];
            if (val == INT_MAX) std::cout << "INF ";
            else std::cout << val << " ";
        }
        std::cout << "\n";
    }
}


std::vector<int> Graph::dijkstra(int src, int dest, int& distance) {
    std::vector<int> dist(TV, INT_MAX), prev(TV, -1);
    std::vector<bool> visited(TV, false);
    dist[src] = 0;

    for (int i = 0; i < TV - 1; ++i) {
        int u = minDistance(dist, visited);
        if (u == -1) break;
        visited[u] = true;

        for (int v = 0; v < TV; ++v) {
            int weight = useMatrix ? adjMatrix[u][v] :
                (adjList[u].count(v) ? adjList[u][v] : INT_MAX);
            if (!visited[v] && weight != INT_MAX && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u;
            }
        }
    }

    distance = dist[dest];
    std::vector<int> path;
    for (int at = dest; at != -1; at = prev[at])
        path.insert(path.begin(), at);
    return path;
}