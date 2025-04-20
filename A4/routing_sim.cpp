#include <iostream>
#include <vector>
#include <limits>
#include <queue>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

const int INF = 9999;

void printDVRTable(int node, const vector<vector<int>>& table, const vector<vector<int>>& nextHop) {
    cout << "Node " << node << " Routing Table:\n";
    cout << "Dest\tCost\tNext Hop\n";
    for (int i = 0; i < table.size(); ++i) {
        cout << i << "\t";
        if (table[node][i] == INF) cout << "INF\t";
        else cout << table[node][i] << "\t";
        if (nextHop[node][i] == -1) cout << "-";
        else cout << nextHop[node][i];
        cout << "\n";
    }
    cout << "\n";
}

void simulateDVR(const vector<vector<int>>& graph) {
    int n = graph.size();
    vector<vector<int>> dist(n, vector<int>(n));
    vector<vector<int>> nextHop(n, vector<int>(n, -1));

    // --- Initialize distance and next-hop tables ---
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            dist[i][j] = graph[i][j];
            if (i != j && graph[i][j] != INF) {
                nextHop[i][j] = j;
            }
        }
    }

    // --- Iteratively exchange vectors until no update ---
    bool updated = true;
    int iteration = 0;
    while (updated) {
        updated = false;
        iteration++;
        // For each node u, consider each neighbor v
        for (int u = 0; u < n; ++u) {
            for (int v = 0; v < n; ++v) {
                if (u == v || graph[u][v] == INF) continue;
                // Try all destinations d through neighbor v
                for (int d = 0; d < n; ++d) {
                    if (dist[v][d] == INF) continue;
                    int viaCost = dist[u][v] + dist[v][d];
                    if (viaCost < dist[u][d]) {
                        dist[u][d] = viaCost;
                        nextHop[u][d] = nextHop[u][v];
                        updated = true;
                    }
                }
            }
        }
        // Print after this iteration
        cout << "--- DVR iteration " << iteration << " ---\n";
        for (int i = 0; i < n; ++i) {
            printDVRTable(i, dist, nextHop);
        }
    }

    // --- Final tables ---
    cout << "--- DVR Final Tables ---\n";
    for (int i = 0; i < n; ++i) {
        printDVRTable(i, dist, nextHop);
    }
}

void printLSRTable(int src, const vector<int>& dist, const vector<int>& prev) {
    cout << "Node " << src << " Routing Table:\n";
    cout << "Dest\tCost\tNext Hop\n";
    for (int i = 0; i < dist.size(); ++i) {
        if (i == src) continue;
        cout << i << "\t";
        if (dist[i] == INF) cout << "INF\t";
        else cout << dist[i] << "\t";
        // backtrack to find first hop
        int hop = i;
        if (prev[hop] == -1) {
            cout << "-\n";
            continue;
        }
        while (prev[hop] != src) {
            hop = prev[hop];
            if (hop == -1) break;
        }
        if (hop == -1) cout << "-\n";
        else cout << hop << "\n";
    }
    cout << "\n";
}

void simulateLSR(const vector<vector<int>>& graph) {
    int n = graph.size();
    cout << "--- Link State Routing Simulation ---\n";
    for (int src = 0; src < n; ++src) {
        vector<int> dist(n, INF);
        vector<int> prev(n, -1);
        vector<bool> visited(n, false);
        dist[src] = 0;

        // Standard Dijkstra
        for (int iter = 0; iter < n; ++iter) {
            // pick unvisited with smallest dist
            int u = -1, best = INF + 1;
            for (int i = 0; i < n; ++i) {
                if (!visited[i] && dist[i] < best) {
                    best = dist[i];
                    u = i;
                }
            }
            if (u == -1) break;
            visited[u] = true;
            // relax edges u->v
            for (int v = 0; v < n; ++v) {
                if (!visited[v] && graph[u][v] != INF) {
                    int alt = dist[u] + graph[u][v];
                    if (alt < dist[v]) {
                        dist[v] = alt;
                        prev[v] = u;
                    }
                }
            }
        }

        printLSRTable(src, dist, prev);
    }
}

vector<vector<int>> readGraphFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        exit(1);
    }
    
    int n;
    file >> n;
    vector<vector<int>> graph(n, vector<int>(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            file >> graph[i][j];

    file.close();
    return graph;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    vector<vector<int>> graph = readGraphFromFile(argv[1]);

    cout << "\n--- Distance Vector Routing Simulation ---\n";
    simulateDVR(graph);

    simulateLSR(graph);

    return 0;
}
