#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const double epsilon = 0.00000001d;

double dabs(double a) {
    return (a >= 0) ? a : -a;
}

int dcomp(const void *a, const void *b) {
    if (*(double*)a == *(double*)b) return 0;

    return (*(double*)a > *(double*)b) ? 1 : -1;
}

void reverse(double *arr, int n) {
    for (int i = 0; i < n / 2; i++) {
        double temp = arr[i];
        arr[i] = arr[n - 1 - i];
        arr[n - 1 - i] = temp;
    }
}

double *arrgen(int type, int n) {
    double *arr = malloc(sizeof(double) * n);

    for (int i = 0; i < n; i++) {
        arr[i] = ((double)rand() / (double)RAND_MAX) * 200000000 - 100000000;
    }

    switch (type) {         // 3 types of generation: 0 - random, 1 - ascending, -1 - descending order
    case 1:                 // ascending order generation
        qsort(arr, n, sizeof(double), dcomp);
        break;

    case -1:                // descending order generation
        qsort(arr, n, sizeof(double), dcomp);
        reverse(arr, n);
        break;
    }

    return arr;
}

void shell_sort(double *arr, int n) {

    int comp_count = 0;
    int swap_count = 0;

    for (int gap = n / 2; gap > 0; gap /= 2) {

        for (int i = 0; i < n; i++) {
            double key = arr[i];
            int j = i - gap;

            while (j >= 0 && (comp_count++, dabs(arr[j]) > dabs(key))) {
                arr[j + gap] = arr[j];
                swap_count++;
                j -= gap;
            }

            arr[j + gap] = key;
            swap_count++;
        }
    }

    printf("\tShell sort:\n");
    printf("\tAmount of swaps:\t%d\n", swap_count);
    printf("\tAmount of comparisons:\t%d\n\n", comp_count);

    return 0;
}


void swap(double *a, double *b) {
    double temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(int i, double *arr, int n, int *swap_count, int *comp_count) {
    int root = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < n && ((*comp_count)++, dabs(arr[l]) > dabs(arr[root]))) {
        root = l;
    }

    if (r < n && ((*comp_count)++, dabs(arr[r]) > dabs(arr[root]))) {
        root = r;
    }

    if (root != i) {
        swap(arr + i, arr + root);
        (*swap_count)++;

        heapify(root, arr, n, swap_count, comp_count);
    }
}

void pyramid_sort(double *arr, int n) {

    int comp_count = 0;
    int swap_count = 0;

    for (int i = n / 2 - 1; i >= 0 ; i--) {
        heapify(i, arr, n, &swap_count, &comp_count);
    }

    for (int i = n - 1; i >= 0; i--) {
        swap(arr, arr + i);
        swap_count++;

        heapify(0, arr, i, &swap_count, &comp_count);
    }

    printf("\tPyramid sort:\n");
    printf("\tAmount of swaps:\t%d\n", swap_count);
    printf("\tAmount of comparisons:\t%d\n\n", comp_count);
}

int main()
{
    unsigned int seed = time(NULL);
    printf("Seed : %d\n\n", seed);
    srand(seed);

    for (int n = 10; n <= 10000; n *= 10) {

        printf("N = %d\n\n", n);

        printf("1) (best case)\n");

        double *arr1 = arrgen(1, n);
        double *arr2 = malloc(n * sizeof(double));

        memcpy(arr2, arr1, n * sizeof(double));

        shell_sort(arr1, n);
        pyramid_sort(arr2, n);

        free(arr1);
        free(arr2);

        printf("2) (worst case)\n");

        arr1 = arrgen(-1, n);
        arr2 = malloc(n * sizeof(double));

        memcpy(arr2, arr1, n * sizeof(double));

        shell_sort(arr1, n);
        pyramid_sort(arr2, n);

        free(arr1);
        free(arr2);

        printf("3)\n");

        arr1 = arrgen(0, n);
        arr2 = malloc(n * sizeof(double));

        memcpy(arr2, arr1, n * sizeof(double));

        shell_sort(arr1, n);
        pyramid_sort(arr2, n);

        free(arr1);
        free(arr2);

        printf("4)\n");

        arr1 = arrgen(0, n);
        arr2 = malloc(n * sizeof(double));

        memcpy(arr2, arr1, n * sizeof(double));

        shell_sort(arr1, n);
        pyramid_sort(arr2, n);

        free(arr1);
        free(arr2);
    }

    return 0;
}
