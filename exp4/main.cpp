#include "GraphAlgorithms.h"
#include <iostream>

using namespace std;

// 检查负权边
bool checkNegativeWeights(const Graph& graph) {
    for (int u = 0; u < graph.vertices; ++u) {
        for (auto& neighbor : graph.adjList[u]) {
            if (neighbor.second < 0) {
                cerr << "Error: Negative weights detected. Dijkstra algorithm does not support graphs with negative weights." << endl;
                return true;
            }
        }
    }
    return false;
}

// 打印最短路径距离
void printDistances(const vector<int>& distances) {
    for (int distance : distances) {
        if (distance == INT_MAX)
            cout << "INF ";
        else
            cout << distance << " ";
    }
    cout << endl;
}

int main() {
    // 创建一个图
    Graph graph(6);
    graph.addEdge(0, 1, 4);
    graph.addEdge(0, 2, 1);
    graph.addEdge(2, 1, 2);
    graph.addEdge(1, 3, 5);
    graph.addEdge(2, 3, 8);
    graph.addEdge(3, 4, 3);
    graph.addEdge(4, 5, 6);

    // 1. BFS
    cout << "BFS starting from node 0: ";
    vector<int> bfs_result = BFS(graph, 0);
    for (int node : bfs_result) cout << node << " ";
    cout << endl;

    // 2. DFS
    cout << "DFS starting from node 0: ";
    vector<int> dfs_result = DFS(graph, 0);
    for (int node : dfs_result) cout << node << " ";
    cout << endl;

    // 3. Dijkstra's Algorithm
    cout << "Dijkstra distances from node 0: ";
    if (!checkNegativeWeights(graph)) { // 检查是否有负权边
        vector<int> dijkstra_result = Dijkstra(graph, 0);
        printDistances(dijkstra_result);
    }

    // 4. Kruskal's Algorithm
    cout << "Edges in the Minimum Spanning Tree: ";
    vector<tuple<int, int, int>> mst = Kruskal(graph);
    for (const auto& edge : mst) {
        cout << "(" << get<1>(edge) << ", " << get<2>(edge) << ") ";
    }
    cout << endl;

    // 测试非连通图
    Graph disconnectedGraph(6);
    disconnectedGraph.addEdge(0, 1, 4);
    disconnectedGraph.addEdge(0, 2, 1);
    disconnectedGraph.addEdge(2, 1, 2);

    cout << "Dijkstra distances from node 0 in disconnected graph: ";
    if (!checkNegativeWeights(disconnectedGraph)) { // 检查是否有负权边
        vector<int> dijkstra_disconnected = Dijkstra(disconnectedGraph, 0);
        printDistances(dijkstra_disconnected);
    }

    // 测试负权边
    Graph negativeWeightGraph(4);
    negativeWeightGraph.addEdge(0, 1, 4);
    negativeWeightGraph.addEdge(1, 2, -5); // 负权边
    negativeWeightGraph.addEdge(2, 3, 2);

    cout << "Dijkstra distances from node 0 with negative weights: ";
    if (!checkNegativeWeights(negativeWeightGraph)) { // 检查是否有负权边
        vector<int> dijkstra_negative = Dijkstra(negativeWeightGraph, 0);
        printDistances(dijkstra_negative);
    }

    return 0;
}

