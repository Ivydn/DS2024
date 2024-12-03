#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <algorithm>
#include <climits>

using namespace std;

class Graph {
public:
    int vertices;
    vector<vector<pair<int, int>>> adjList;

    Graph(int v) : vertices(v) {
        adjList.resize(v);
    }

    void addEdge(int u, int v, int weight = 1) {
        adjList[u].push_back(make_pair(v, weight));
        adjList[v].push_back(make_pair(u, weight)); // 无向图
    }
};

// BFS
vector<int> BFS(const Graph& graph, int start) {
    vector<bool> visited(graph.vertices, false);
    queue<int> q;
    vector<int> result;

    q.push(start);
    visited[start] = true;

    while (!q.empty()) {
        int current = q.front();
        q.pop();
        result.push_back(current);

        for (auto& neighbor : graph.adjList[current]) {
            if (!visited[neighbor.first]) {
                visited[neighbor.first] = true;
                q.push(neighbor.first);
            }
        }
    }

    return result;
}

// DFS
void DFSUtil(const Graph& graph, int node, vector<bool>& visited, vector<int>& result) {
    visited[node] = true;
    result.push_back(node);

    for (auto& neighbor : graph.adjList[node]) {
        if (!visited[neighbor.first]) {
            DFSUtil(graph, neighbor.first, visited, result);
        }
    }
}

vector<int> DFS(const Graph& graph, int start) {
    vector<bool> visited(graph.vertices, false);
    vector<int> result;
    DFSUtil(graph, start, visited, result);
    return result;
}

// Dijkstra
vector<int> Dijkstra(const Graph& graph, int start) {
    vector<int> distances(graph.vertices, INT_MAX);
    distances[start] = 0;

    // 优先队列，最小优先级队列
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push(make_pair(0, start));
    
    // 检查是否有负权边
    for (int u = 0; u < graph.vertices; ++u) {
        for (auto& neighbor : graph.adjList[u]) {
            if (neighbor.second < 0) {
                cerr << "Error: Dijkstra algorithm does not support graphs with negative weights." << endl;
                return distances; // 直接返回当前结果，避免死循环
            }
        }
    }


    while (!pq.empty()) {
        int currentDistance = pq.top().first;
        int currentNode = pq.top().second;
        pq.pop();

        if (currentDistance > distances[currentNode]) continue;

        for (auto& neighbor : graph.adjList[currentNode]) {
            int nextNode = neighbor.first;
            int weight = neighbor.second;

            if (distances[currentNode] + weight < distances[nextNode]) {
                distances[nextNode] = distances[currentNode] + weight;
                pq.push(make_pair(distances[nextNode], nextNode));
            }
        }
    }

    return distances;
}

// Kruskal
class UnionFind {
public:
    vector<int> parent, rank;

    UnionFind(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; ++i) {
            parent[i] = i;
        }
    }

    int find(int node) {
        if (parent[node] != node) {
            parent[node] = find(parent[node]);
        }
        return parent[node];
    }

    void unite(int u, int v) {
        int rootU = find(u);
        int rootV = find(v);

        if (rootU != rootV) {
            if (rank[rootU] > rank[rootV]) {
                parent[rootV] = rootU;
            } else if (rank[rootU] < rank[rootV]) {
                parent[rootU] = rootV;
            } else {
                parent[rootV] = rootU;
                ++rank[rootU];
            }
        }
    }
};

vector<tuple<int, int, int>> Kruskal(const Graph& graph) {
    vector<tuple<int, int, int>> edges;

    for (int u = 0; u < graph.vertices; ++u) {
        for (const auto& neighbor : graph.adjList[u]) {
            int v = neighbor.first;
            int weight = neighbor.second;
            if (u < v) edges.emplace_back(make_tuple(weight, u, v));
        }
    }

    sort(edges.begin(), edges.end());
    UnionFind uf(graph.vertices);
    vector<tuple<int, int, int>> mst;

    for (const auto& edge : edges) {
        int weight = get<0>(edge);
        int u = get<1>(edge);
        int v = get<2>(edge);

        if (uf.find(u) != uf.find(v)) {
            uf.unite(u, v);
            mst.push_back(make_tuple(weight, u, v));
        }
    }

    return mst;
}
