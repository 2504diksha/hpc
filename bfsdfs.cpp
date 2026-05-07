#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
using namespace std;

vector<vector<int>> g;

// -------- BFS --------
void BFS(int s, int n)
{
    vector<bool> vis(n, false);
    queue<int> q;
    q.push(s);
    vis[s] = true;

    cout << "\nBFS: ";
    while (!q.empty())
    {
        int node = q.front();
        q.pop();
        cout << node << " ";

#pragma omp parallel for
        for (int i = 0; i < g[node].size(); i++)
        {
            int adj = g[node][i];
            if (!vis[adj])
            {
#pragma omp critical
                if (!vis[adj])
                {
                    vis[adj] = true;
                    q.push(adj);
                }
            }
        }
    }
}

// -------- DFS --------
void DFS(int node, vector<bool> &vis)
{
    cout << node << " ";
    vis[node] = true;

#pragma omp parallel for
    for (int i = 0; i < g[node].size(); i++)
    {
        int adj = g[node][i];
        if (!vis[adj])
        {
#pragma omp critical
            if (!vis[adj])
                DFS(adj, vis);
        }
    }
}

int main()
{
    int n, e, u, v, start;
    cout << "Nodes & edges: ";
    cin >> n >> e;

    g.resize(n);

    cout << "Enter edges:\n";
    for (int i = 0; i < e; i++)
    {
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    cout << "Start node: ";
    cin >> start;

    BFS(start, n);

    vector<bool> vis(n, false);
    cout << "\nDFS: ";
    DFS(start, vis);

    return 0;
}

/*
Input-
Enter nodes & edges: 5 4
Enter edges:
0 1
0 2
1 3
2 4
Start node: 0

Output-
BFS: 0 1 2 3 4
DFS: 0 1 3 2 4

-------------------------------------------------------------
Time complexity
BFS and DFS both have a time complexity of O(V + E), where V is the number of vertices and E is the number of edges in the graph.

*/