#include <iostream>
#include <queue>
#include <vector>
#include <omp.h>

using namespace std;

// -------------------- Node Definition --------------------
class Node
{
public:
    int data;
    Node* left;
    Node* right;

    Node(int val)
    {
        data = val;
        left = nullptr;
        right = nullptr;
    }
};

// -------------------- Tree Insertion (Level Order) --------------------
Node* insert(Node* root, int data)
{
    if (!root)
        return new Node(data);

    queue<Node*> q;
    q.push(root);

    while (!q.empty())
    {
        Node* temp = q.front();
        q.pop();

        if (!temp->left)
        {
            temp->left = new Node(data);
            return root;
        }
        else
            q.push(temp->left);

        if (!temp->right)
        {
            temp->right = new Node(data);
            return root;
        }
        else
            q.push(temp->right);
    }

    return root;
}

// -------------------- Print Tree Structure (Level Order) --------------------
void print_tree_structure(Node* root)
{
    if (!root) return;

    queue<Node*> q;
    q.push(root);

    cout << "\nTree Structure (Level Order):\n";

    while (!q.empty())
    {
        Node* curr = q.front();
        q.pop();
        cout << curr->data << ": ";
        if (curr->left) cout << "L->" << curr->left->data << " ";
        if (curr->right) cout << "R->" << curr->right->data << " ";
        cout << endl;

        if (curr->left) q.push(curr->left);
        if (curr->right) q.push(curr->right);
    }
}

// -------------------- Sequential BFS --------------------
void bfs_sequential(Node* root)
{
    if (!root) return;

    queue<Node*> q;
    q.push(root);

    cout << "BFS Traversal (Sequential): ";

    while (!q.empty())
    {
        Node* curr = q.front();
        q.pop();
        cout << curr->data << " ";

        if (curr->left) q.push(curr->left);
        if (curr->right) q.push(curr->right);
    }

    cout << endl;
}

// -------------------- Parallel BFS --------------------
void bfs_parallel(Node* root)
{
    if (!root) return;

    queue<Node*> q;
    q.push(root);

    cout << "BFS Traversal (Parallel): ";

    while (!q.empty())
    {
        int qSize = q.size();

        #pragma omp parallel for
        for (int i = 0; i < qSize; i++)
        {
            Node* curr;

            #pragma omp critical
            {
                curr = q.front();
                q.pop();
                cout << curr->data << " ";
            }

            #pragma omp critical
            {
                if (curr->left) q.push(curr->left);
                if (curr->right) q.push(curr->right);
            }
        }
    }

    cout << endl;
}

// -------------------- Main Function --------------------
int main()
{
    Node* root = nullptr;
    int data;
    char ans;

    // Insert nodes
    do
    {
        cout << "\nEnter node data: ";
        cin >> data;
        root = insert(root, data);

        cout << "Do you want to insert another node? (y/n): ";
        cin >> ans;

    } while (ans == 'y' || ans == 'Y');

    // Print tree structure
    print_tree_structure(root);

    // Sequential BFS
    double start_seq = omp_get_wtime();
    bfs_sequential(root);
    double end_seq = omp_get_wtime();
    double seq_time = end_seq - start_seq;

    // Parallel BFS
    double start_par = omp_get_wtime();
    bfs_parallel(root);
    double end_par = omp_get_wtime();
    double par_time = end_par - start_par;

    // Timing results
    cout << "Sequential Time: " << seq_time << " seconds\n";
    cout << "Parallel Time:   " << par_time << " seconds\n";

    if (par_time > 0)
        cout << "Speedup:         " << seq_time / par_time << endl;

    return 0;
}

