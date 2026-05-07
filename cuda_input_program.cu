#include <iostream>
#include <cuda_runtime.h>

using namespace std;

// Vector Addition Kernel
__global__ void vectorAdd(int *A, int *B, int *C)
{
    int i = threadIdx.x;

    if(i < 3)
        C[i] = A[i] + B[i];
}

// Matrix Multiplication Kernel
__global__ void matrixMul(int *A, int *B, int *C, int N)
{
    int row = threadIdx.y;
    int col = threadIdx.x;

    if(row < N && col < N)
    {
        int sum = 0;

        for(int k = 0; k < N; k++)
        {
            sum += A[row * N + k] * B[k * N + col];
        }

        C[row * N + col] = sum;
    }
}

int main()
{
    // ================= VECTOR ADDITION =================

    int A[3], B[3], C[3];

    cout << "Enter 3 elements of Vector A:\n";

    for(int i = 0; i < 3; i++)
        cin >> A[i];

    cout << "Enter 3 elements of Vector B:\n";

    for(int i = 0; i < 3; i++)
        cin >> B[i];

    int *d_A, *d_B, *d_C;

    cudaMalloc((void**)&d_A, 3 * sizeof(int));
    cudaMalloc((void**)&d_B, 3 * sizeof(int));
    cudaMalloc((void**)&d_C, 3 * sizeof(int));

    cudaMemcpy(d_A, A, 3 * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, 3 * sizeof(int), cudaMemcpyHostToDevice);

    vectorAdd<<<1, 3>>>(d_A, d_B, d_C);

    cudaDeviceSynchronize();

    cudaMemcpy(C, d_C, 3 * sizeof(int), cudaMemcpyDeviceToHost);

    cout << "\nVector Addition Result:\n";

    for(int i = 0; i < 3; i++)
        cout << C[i] << " ";

    cout << endl;

    // ================= MATRIX MULTIPLICATION =================

    int N = 2;

    int M1[4], M2[4], M3[4];

    cout << "\nEnter 4 elements of Matrix A (2x2):\n";

    for(int i = 0; i < 4; i++)
        cin >> M1[i];

    cout << "Enter 4 elements of Matrix B (2x2):\n";

    for(int i = 0; i < 4; i++)
        cin >> M2[i];

    int *d_M1, *d_M2, *d_M3;

    cudaMalloc((void**)&d_M1, 4 * sizeof(int));
    cudaMalloc((void**)&d_M2, 4 * sizeof(int));
    cudaMalloc((void**)&d_M3, 4 * sizeof(int));

    cudaMemcpy(d_M1, M1, 4 * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_M2, M2, 4 * sizeof(int), cudaMemcpyHostToDevice);

    dim3 threads(N, N);

    matrixMul<<<1, threads>>>(d_M1, d_M2, d_M3, N);

    cudaDeviceSynchronize();

    cudaMemcpy(M3, d_M3, 4 * sizeof(int), cudaMemcpyDeviceToHost);

    cout << "\nMatrix Multiplication Result:\n";

    for(int i = 0; i < 4; i++)
    {
        cout << M3[i] << " ";

        if((i + 1) % N == 0)
            cout << endl;
    }

    // Free Memory
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    cudaFree(d_M1);
    cudaFree(d_M2);
    cudaFree(d_M3);

    system("pause");

    return 0;
}

/*nvcc cuda_input_program.cu -o cuda_program*/
