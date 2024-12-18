#include <stdio.h>
#include <cuda.h>


__global__ void gpu_histogram_non_strided(int* array, int* histogram, int size, int num_bins) {
    __shared__ int shared_histogram[256];

    int tid = threadIdx.x + blockIdx.x * blockDim.x;

    // Initialize shared histogram
    if (threadIdx.x < num_bins) {
        shared_histogram[threadIdx.x] = 0;
    }
    __syncthreads();

    // Update shared histogram
    if (tid < size) {
        atomicAdd(&shared_histogram[array[tid]], 1);
    }
    __syncthreads();

    // Write shared histogram to global memory
    if (threadIdx.x < num_bins) {
        atomicAdd(&histogram[threadIdx.x], shared_histogram[threadIdx.x]);
    }
}

// Function to initialize input array
void initialize_array(int* array, int size, int num_bins) {
    for (int i = 0; i < size; i++) {
        array[i] = rand() % num_bins; // Random values between 0 and num_bins-1
    }
}

int main() {
    // Parameters
    const int size = 1024 * 1024;  // Input array size
    const int num_bins = 256;      // Number of bins
    const int block_size = 256;    // Threads per block
    const int grid_size = (size + block_size - 1) / block_size; // Blocks per grid

    // Host memory allocation
    int* h_array = (int*)malloc(size * sizeof(int));
    int* h_histogram = (int*)malloc(num_bins * sizeof(int));

    // Initialize input array and histogram
    initialize_array(h_array, size, num_bins);
    for (int i = 0; i < num_bins; i++) {
        h_histogram[i] = 0;
    }

    // Device memory allocation
    int* d_array;
    int* d_histogram;
    cudaMalloc((void**)&d_array, size * sizeof(int));
    cudaMalloc((void**)&d_histogram, num_bins * sizeof(int));

    // Copy input data to device
    cudaMemcpy(d_array, h_array, size * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_histogram, h_histogram, num_bins * sizeof(int), cudaMemcpyHostToDevice);

    // Launch the kernel
    gpu_histogram_non_strided<<<grid_size, block_size>>>(d_array, d_histogram, size, num_bins);

    // Copy the result back to host
    cudaMemcpy(h_histogram, d_histogram, num_bins * sizeof(int), cudaMemcpyDeviceToHost);

    // Print results
    printf("Histogram:\n");
    for (int i = 0; i < num_bins; i++) {
        printf("Bin %d: %d\n", i, h_histogram[i]);
    }

    // Free memory
    free(h_array);
    free(h_histogram);
    cudaFree(d_array);
    cudaFree(d_histogram);

    return 0;
}
