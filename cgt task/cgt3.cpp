#include <iostream>
#include <vector>
#include <limits.h>
#include <queue>
#include <algorithm>

using namespace std;

class Graph {
public:
    int V;
    vector<vector<int>> adj; 

    Graph(int V);
    void addEdge(int u, int v);
    int vertexConnectivity();
    int edgeConnectivity();
    bool bfs(const vector<vector<int>>& rGraph, int s, int t, vector<int>& parent);
    int fordFulkerson(vector<vector<int>>& graph, int s, int t);
    void printGraph(); 
};


Graph::Graph(int V) {
    this->V = V;
    adj.resize(V);
}

void Graph::addEdge(int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u); // For undirected graph
}
bool Graph::bfs(const vector<vector<int>>& rGraph, int s, int t, vector<int>& parent) {
    int n = rGraph.size();
    vector<bool> visited(n, false);
    queue<int> q;
    q.push(s);
    visited[s] = true;
    parent[s] = -1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 0; v < n; v++) {
            if (!visited[v] && rGraph[u][v] > 0) {
                if (v == t) {
                    parent[v] = u;
                    return true;
                }
                q.push(v);
                parent[v] = u;
                visited[v] = true;
            }
        }
    }
    return false;
}
int Graph::fordFulkerson(vector<vector<int>>& graph, int s, int t) {
    int n = graph.size();
    vector<vector<int>> rGraph = graph;
    vector<int> parent(n);
    int max_flow = 0;

    while (bfs(rGraph, s, t, parent)) {
        int path_flow = INT_MAX;
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            path_flow = min(path_flow, rGraph[u][v]);
        }

        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            rGraph[u][v] -= path_flow;
            rGraph[v][u] += path_flow;
        }

        max_flow += path_flow;
    }

    return max_flow;
}

int Graph::vertexConnectivity() {
    if (V <= 1) return 0;
    if (V == 2) return adj[0].size() > 0 ? 1 : 0;

    int min_cut = INT_MAX;
    for (int s = 0; s < V - 1; s++) {
        for (int t = s + 1; t < V; t++) {
            vector<vector<int>> graph(V + 2, vector<int>(V + 2, 0));
            int source = V, sink = V + 1;

            for (int i = 0; i < V; i++) {
                if (i != s && i != t) {
                    graph[source][i] = 1;
                    graph[i][sink] = 1;
                }
                for (int j : adj[i]) {
                    if (i != s && i != t && j != s && j != t) {
                        graph[i][j] = 1;
                    }
                }
            }

            int cut = fordFulkerson(graph, source, sink);
            min_cut = min(min_cut, cut);
        }
    }
    return min_cut;
}

int Graph::edgeConnectivity() {
    vector<vector<int>> graph(V, vector<int>(V, 0));
    for (int u = 0; u < V; u++) {
        for (int v : adj[u]) {
            graph[u][v] = 1;
        }
    }

    int min_cut = INT_MAX;
    for (int s = 0; s < V - 1; s++) {
        for (int t = s + 1; t < V; t++) {
            min_cut = min(min_cut, fordFulkerson(graph, s, t));
        }
    }
    return min_cut;
}


void Graph::printGraph() {
    for (int i = 0; i < V; i++) {
        cout << "Vertex " << i << ":";
        for (int neighbor : adj[i]) {
            cout << " " << neighbor;
        }
        cout << endl;
    }
}

int main() {
    Graph g(4); 
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(0, 3);
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 3);

    cout << "Graph structure:" << endl;
    g.printGraph();

    int vertexConn = g.vertexConnectivity();
    cout << "Vertex Connectivity: " << vertexConn << endl;

    int edgeConn = g.edgeConnectivity();
    cout << "Edge Connectivity: " << edgeConn << endl;

    int k = min(vertexConn, edgeConn);
    cout << "The graph is " << k << "-connected." << endl;

    return 0;
}