#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// CPU Reduction: Sum
float cpu_reduction_sum(float* array, int size) {
    float sum = 0.0;
    for (int i = 0; i < size; i++) {
        sum += array[i];
    }
    return sum;
}

// CPU Reduction: Product
float cpu_reduction_product(float* array, int size) {
    float product = 1.0;
    for (int i = 0; i < size; i++) {
        product *= array[i];
    }
    return product;
}

// CPU Reduction: Min
float cpu_reduction_min(float* array, int size) {
    float min_value = array[0];
    for (int i = 1; i < size; i++) {
        if (array[i] < min_value) {
            min_value = array[i];
        }
    }
    return min_value;
}

// CPU Reduction: Max
float cpu_reduction_max(float* array, int size) {
    float max_value = array[0];
    for (int i = 1; i < size; i++) {
        if (array[i] > max_value) {
            max_value = array[i];
        }
    }
    return max_value;
}

// Helper function to initialize an array with random values
void initialize_array(float* array, int size) {
    for (int i = 0; i < size; i++) {
        array[i] = (float)(rand() % 100) / 10.0; // Random floats between 0.0 and 10.0
    }
}

// Benchmarking and testing all operators
int main() {
    // Set seed for reproducibility
    srand(time(NULL));

    // Test array sizes
    int sizes[] = {1000, 10000, 100000, 1000000, 10000000}; // Example sizes
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    // Loop through array sizes
    for (int i = 0; i < num_sizes; i++) {
        int size = sizes[i];
        float* array = (float*)malloc(size * sizeof(float));
        initialize_array(array, size);

        printf("\nArray Size: %d\n", size);

        // Measure CPU Reduction: Sum
        clock_t start = clock();
        float sum = cpu_reduction_sum(array, size);
        clock_t end = clock();
        printf("Sum: %f, Time: %f seconds\n", sum, (double)(end - start) / CLOCKS_PER_SEC);

        // Measure CPU Reduction: Product
        start = clock();
        float product = cpu_reduction_product(array, size);
        end = clock();
        printf("Product: %f, Time: %f seconds\n", product, (double)(end - start) / CLOCKS_PER_SEC);

        // Measure CPU Reduction: Min
        start = clock();
        float min_value = cpu_reduction_min(array, size);
        end = clock();
        printf("Min: %f, Time: %f seconds\n", min_value, (double)(end - start) / CLOCKS_PER_SEC);

        // Measure CPU Reduction: Max
        start = clock();
        float max_value = cpu_reduction_max(array, size);
        end = clock();
        printf("Max: %f, Time: %f seconds\n", max_value, (double)(end - start) / CLOCKS_PER_SEC);

        // Free memory
        free(array);
    }

    return 0;
}
