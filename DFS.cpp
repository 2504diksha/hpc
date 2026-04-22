#include <iostream>
#include <vector>
#include <stack>
#include <omp.h>

using namespace std;

const int MAX = 100000;
vector<int> graph[MAX];
bool visited[MAX];

// Forward declaration
void dfs_task(int node);

// -------------------- Sequential DFS --------------------
void dfs_sequential(int node)
{
    stack<int> s;
    s.push(node);

    cout << "DFS Traversal: ";

    while (!s.empty())
    {
        int curr = s.top();
        s.pop();

        if (!visited[curr])
        {
            visited[curr] = true;
            cout << curr << " ";

            for (int i = graph[curr].size() - 1; i >= 0; i--)
            {
                int adj = graph[curr][i];
                if (!visited[adj])
                    s.push(adj);
            }
        }
    }
    cout << endl;
}

// -------------------- Parallel DFS --------------------
void dfs_parallel(int node)
{
    #pragma omp parallel
    {
        #pragma omp single
        {
            dfs_task(node);
        }
    }
}

void dfs_task(int node)
{
    if (visited[node])
        return;

    visited[node] = true;

    for (int i = 0; i < graph[node].size(); i++)
    {
        int adj = graph[node][i];

        if (!visited[adj])
        {
            #pragma omp task
            dfs_task(adj);
        }
    }
}

// -------------------- Main Function --------------------
int main()
{
    int n, m, start_node;

    cout << "Enter number of Nodes, Edges, and Starting Node:\n";
    cin >> n >> m >> start_node;

    cout << "Enter edges (u v):\n";
    for (int i = 0; i < m; i++)
    {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    // ---------- Print Graph Structure ----------
    cout << "\nGraph Structure (Adjacency List):\n";
    for (int i = 0; i < n; i++)
    {
        cout << i << ": ";
        for (auto adj : graph[i])
            cout << adj << " ";
        cout << endl;
    }

    // ---------- Sequential DFS ----------
    for (int i = 0; i < n; i++)
        visited[i] = false;

    double start_seq = omp_get_wtime();
    dfs_sequential(start_node);
    double end_seq = omp_get_wtime();

    double seq_time = end_seq - start_seq;

    // ---------- Parallel DFS ----------
    for (int i = 0; i < n; i++)
        visited[i] = false;

    double start_par = omp_get_wtime();
    dfs_parallel(start_node);
    double end_par = omp_get_wtime();

    double par_time = end_par - start_par;

    // ---------- Results ----------
    cout << "Sequential Time: " << seq_time << " seconds\n";
    cout << "Parallel Time:   " << par_time << " seconds\n";

    if (par_time > 0)
        cout << "Speedup:         " << seq_time / par_time << endl;

    return 0;
}

