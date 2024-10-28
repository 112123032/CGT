#include <set>
#include <functional>

using namespace std;


struct Edge {
    int src, dest, weight;
};


int findParent(int i, vector<int>& parent) {
    if (parent[i] == -1)
        return i;
    return findParent(parent[i], parent);
}


void unionSets(int x, int y, vector<int>& parent) {
    parent[x] = y;
}


void primMST(vector<vector<int>>& graph) {
    int V = graph.size();
    vector<int> parent(V, -1);
    vector<bool> inMST(V, false);
    vector<int> key(V, INT_MAX);
    
    key[0] = 0;

    for (int count = 0; count < V - 1; count++) {
        int minKey = INT_MAX, minIndex;
        for (int v = 0; v < V; v++)
            if (!inMST[v] && key[v] < minKey) {
                minKey = key[v];
                minIndex = v;
            }

        inMST[minIndex] = true;

        for (int v = 0; v < V; v++)
            if (graph[minIndex][v] && !inMST[v] && graph[minIndex][v] < key[v]) {
                parent[v] = minIndex;
                key[v] = graph[minIndex][v];
            }
    }

    
    cout << "Prim's Algorithm MST:\n";
    cout << "Edge \tWeight\n";
    int totalWeightPrim = 0;
    for (int i = 1; i < V; i++) {
        cout << parent[i] << " - " << i << "\t" << graph[i][parent[i]] << endl;
        totalWeightPrim += graph[i][parent[i]];
    }
    cout << "Total Weight: " << totalWeightPrim << endl;

    
    cout << "\nFundamental Cutsets:\n";
    for (int i = 1; i < V; i++) {
        cout << "Cutset for edge " << parent[i] << " - " << i << ": { ";
        bool first = true; // Flag to handle comma placement
        for (int j = 0; j < V; j++) {
            if (graph[parent[i]][j] > 0 && j != i) {
                if (!first) {
                    cout << ", "; // Add comma before subsequent edges
                }
                cout << parent[i] << " - " << j;
                first = false; // After the first edge, set flag to false
            }
        }
        cout << " }\n";
    }
}

// Kruskal's Algorithm to find MST
void kruskalMST(vector<Edge>& edges, int V) {
    sort(edges.begin(), edges.end(), [](Edge a, Edge b) { return a.weight < b.weight; });
    
    vector<int> parent(V, -1);
    
    cout << "\nKruskal's Algorithm MST:\n";
    cout << "Edge \tWeight\n";
    
    int totalWeightKruskal = 0;
    
    for (const auto& edge : edges) {
        int x = findParent(edge.src, parent);
        int y = findParent(edge.dest, parent);

        if (x != y) {
            cout << edge.src << " - " << edge.dest << "\t" << edge.weight << endl;
            unionSets(x, y, parent);
            totalWeightKruskal += edge.weight;
        }
    }
    
    cout << "Total Weight: " << totalWeightKruskal << endl;

   
    cout << "\nFundamental Circuits:\n";
    
    for (const auto& edge : edges) {
        int x = findParent(edge.src, parent);
        int y = findParent(edge.dest, parent);

      
        if (x == y) {
            cout << "Circuit formed by adding edge: " << edge.src << " - " << edge.dest << endl;

          
            vector<int> circuitVertices;
            set<int> visited;

           
            function<void(int)> dfs = [&](int v) {
                visited.insert(v);
                circuitVertices.push_back(v);
                for (const auto& e : edges) {
                    if ((e.src == v || e.dest == v) && visited.find(e.dest) == visited.end() && visited.find(e.src) == visited.end()) {
                        dfs(e.dest);
                    }
                }
            };

            dfs(edge.src); // Start DFS from one end of the added edge
            
            cout << "{ ";
            for (size_t i = 0; i < circuitVertices.size(); ++i) {
                cout << circuitVertices[i];
                if (i < circuitVertices.size() - 1) { // Add comma if not last element
                    cout << ", ";
                }
            }
            cout << ", " << edge.src << ", " << edge.dest; // Include the added edge vertices
            cout << " }\n";
        }
    }
}

int main() {
   
    vector<vector<int>> graph = {
        {0, 2, 0, 6, 0},
        {2, 0, 3, 8, 5},
        {0, 3, 0, 0, 7},
        {6, 8, 0, 0, 9},
        {0, 5, 7, 9, 0}
    };
    
   
    vector<Edge> edges = {
        {0, 1 ,2}, {1 ,2 ,3}, {0 ,3 ,6}, 
        {1 ,3 ,8}, {1 ,4 ,5}, {2 ,4 ,7}, 
        {3 ,4 ,9}
    };
    
    primMST(graph);
    
    int V = graph.size(); // Number of vertices
    kruskalMST(edges,V);

    return 0;
}