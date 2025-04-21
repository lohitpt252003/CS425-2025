#include <iostream>
#include <vector>
#include <limits>
#include <queue>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

const int INF = 9999;

// Read adjacency matrix; off-diagonal zeros become INF
vector<vector<int>> readGraph(const string &fname) {
    ifstream in(fname);
    if (!in) {
        cerr << "Error: cannot open " << fname << "\n";
        exit(1);
    }
    int n; 
    in >> n;
    vector<vector<int>> g(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int v; in >> v;
            if (i != j && v == 0) g[i][j] = INF;
            else                  g[i][j] = v;
        }
    }
    return g;
}

// Print DVR—direct only
void simulateDVR(const vector<vector<int>>& g) {
    int n = g.size();
    cout << "\n--- Distance Vector Routing Simulation ---\n";
    for (int i = 0; i < n; ++i) {
        cout << "Node " << i << " Routing Table:\n";
        cout << "Dest Metric Next Hop\n";
        for (int j = 0; j < n; ++j) {
            // cost
            int cost = g[i][j];
            if (i == j) cost = 0;
            // next hop
            cout << j << " " << cost << " ";
            if (i == j || g[i][j] == INF) 
                cout << "-\n";
            else
                cout << j << "\n";
        }
    }
}

// Print LSR—Dijkstra per node
void simulateLSR(const vector<vector<int>>& g) {
    int n = g.size();
    cout << "\n--- Link State Routing Simulation ---\n";
    cout << "Instructor: Adithya Vadapalli TA Incharge: Rishit and Yugul\n";
    cout << "CS425: Computer Networks A4: Routing Protocols (DVR and LSR)\n";
    
    for (int src = 0; src < n; ++src) {
        // Dijkstra init
        vector<int> dist(n, INF), prev(n, -1);
        vector<bool> used(n, false);
        dist[src] = 0;

        for (int it = 0; it < n; ++it) {
            int u = -1, best = INF + 1;
            for (int x = 0; x < n; ++x) {
                if (!used[x] && dist[x] < best) {
                    best = dist[x];
                    u = x;
                }
            }
            if (u < 0) break;
            used[u] = true;
            for (int v = 0; v < n; ++v) {
                if (!used[v] && g[u][v] != INF) {
                    int alt = dist[u] + g[u][v];
                    if (alt < dist[v]) {
                        dist[v] = alt;
                        prev[v] = u;
                    }
                }
            }
        }

        // print
        cout << "Node " << src << " Routing Table:\n";
        cout << "Dest Metric Next Hop\n";
        for (int d = 0; d < n; ++d) {
            if (d == src) continue;
            cout << d << " ";
            if (dist[d] == INF) cout << "INF ";
            else                 cout << dist[d] << " ";
            // backtrack to first hop
            int hop = d;
            while (prev[hop] != -1 && prev[hop] != src)
                hop = prev[hop];
            if (prev[hop] == -1) cout << "-\n";
            else                  cout << hop << "\n";
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }
    auto graph = readGraph(argv[1]);
    simulateDVR(graph);
    simulateLSR(graph);
    return 0;
}