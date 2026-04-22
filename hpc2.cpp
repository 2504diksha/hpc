//Tejas Pandhare
#include <iostream>
#include <omp.h>
using namespace std;

void merge(int a[], int l, int m, int r)
{
    int t[1000], i = l, j = m + 1, k = 0;
    while (i <= m && j <= r)
        t[k++] = (a[i] < a[j]) ? a[i++] : a[j++];
    while (i <= m) t[k++] = a[i++];
    while (j <= r) t[k++] = a[j++];
    for (i = l, j = 0; i <= r; i++, j++) a[i] = t[j];
}

void seq(int a[], int l, int r)
{
    if (l < r)
    {
        int m = (l + r) / 2;
        seq(a, l, m);
        seq(a, m + 1, r);
        merge(a, l, m, r);
    }
}

void par(int a[], int l, int r)
{
    if (l < r)
    {
        int m = (l + r) / 2;
        #pragma omp parallel sections
        {
            #pragma omp section
            par(a, l, m);
            #pragma omp section
            par(a, m + 1, r);
        }
        merge(a, l, m, r);
    }
}

// Sequential Bubble
void bubbleSeq(int a[], int n)
{
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (a[j] > a[j + 1])
                swap(a[j], a[j + 1]);
}

// Parallel Bubble (safe version - odd-even)
void bubblePar(int a[], int n)
{
    for (int i = 0; i < n; i++)
    {
        #pragma omp parallel for
        for (int j = (i % 2); j < n - 1; j += 2)
        {
            if (a[j] > a[j + 1])
                swap(a[j], a[j + 1]);
        }
    }
}

void print(int a[], int n)
{
    for (int i = 0; i < n; i++) cout << a[i] << " ";
    cout << endl;
}

int main()
{
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    int a[n];
    cout << "Enter elements: ";
    for (int i = 0; i < n; i++) cin >> a[i];

    int ch;
    do
    {
        cout << "\n1.Merge Sort\n2.Bubble Sort\n3.Exit\nEnter choice: ";
        cin >> ch;

        if (ch == 1)
        {
            int b[n], c[n];
            for (int i = 0; i < n; i++) b[i] = c[i] = a[i];

            double s = omp_get_wtime();
            seq(b, 0, n - 1);
            double e = omp_get_wtime();

            double s2 = omp_get_wtime();
            par(c, 0, n - 1);
            double e2 = omp_get_wtime();

            cout << "\nSorted Array:\n";
            print(c, n);

            cout << "Sequential Time: " << (e - s) * 1e6 << " us\n";
            cout << "Parallel Time: " << (e2 - s2) * 1e6 << " us\n";
        }

        else if (ch == 2)
        {
            int b[n], c[n];
            for (int i = 0; i < n; i++) b[i] = c[i] = a[i];

            double s = omp_get_wtime();
            bubbleSeq(b, n);
            double e = omp_get_wtime();

            double s2 = omp_get_wtime();
            bubblePar(c, n);
            double e2 = omp_get_wtime();

            cout << "\nSorted Array:\n";
            print(c, n);

            cout << "Sequential Time: " << (e - s) * 1e6 << " us\n";
            cout << "Parallel Time: " << (e2 - s2) * 1e6 << " us\n";
        }

    } while (ch != 3);

    return 0;
}
