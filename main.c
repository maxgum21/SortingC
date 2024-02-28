#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int comp_count, swap_count;

double dabs(double a) {
    return (a >= 0) ? a : -a;
}

int dcomp(const void *a, const void *b) { // comparison func for qsort in array generation
    if (dabs(*(double*)a) == dabs(*(double*)b)) return 0;

    return (dabs(*(double*)a) > dabs(*(double*)b)) ? 1 : -1;
}

void reverse(double *arr, int n) {      //function to reverse arrays
    for (int i = 0; i < n / 2; i++) {
        double temp = arr[i];
        arr[i] = arr[n - 1 - i];
        arr[n - 1 - i] = temp;
    }
}

double *arrgen(int type, int n) {
    double *arr = malloc(sizeof(double) * n);

    for (int i = 0; i < n; i++) { // generates random doubles from -100000000 to 100000000
        arr[i] = ((double)rand() / (double)RAND_MAX) * 200000000 - 100000000;
    }

    switch (type) {         // 3 types of generation: 1 - ascending, -1 - descending order and random by default
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

void swap(double *a, double *b) {
    double temp = *a;
    *a = *b;
    *b = temp;
}

void shell_sort(double *arr, int n) {

    for (int gap = n / 2; gap > 0; gap /= 2) { // I used one of the most common ways to get gap length for shell sort

        // do insertion sort but with step equal to 'gap' instead of 1
        for (int i = 0; i < n; i++) {
            for (int j = i - gap; j >= 0 && (comp_count++, dabs(arr[j]) > dabs(arr[j + gap])); j -= gap) {
                swap(arr + j, arr + j + gap);
                swap_count++;
            }
        }
    }
}



void heapify(int i, double *arr, int n) {
    int root = i;               // get the root and its 2 children
    int l = 2 * i + 1;          // and swap the root with a child if it's bigger
    int r = 2 * i + 2;

    if (l < n && (comp_count++, dabs(arr[l]) > dabs(arr[root]))) {
        root = l;
    }

    if (r < n && (comp_count++, dabs(arr[r]) > dabs(arr[root]))) {
        root = r;
    }

    if (root != i) {
        swap(arr + i, arr + root);
        swap_count++;

        heapify(root, arr, n);
    }
}

void pyramid_sort(double *arr, int n) {
    for (int i = n / 2 - 1; i >= 0 ; i--) { // turn the input into a pyramid array
        heapify(i, arr, n);
    }

    for (int i = n - 1; i >= 0; i--) { // get the root element, which is the biggest,
        swap(arr, arr + i);            // and put it after the whole pyramid and do this
        swap_count++;                  // till the pyramid is of length 0

        heapify(0, arr, i);
    }
}

void print_arr(double *arr, int n) {
    for (int i = 0; i < n; i++) {
        printf("(%d, %lf)\n", i+1, arr[i]);
    }
    printf("\n");
}

void show_results(int n, int type, char *message) {
    double *arr1 = arrgen(type, n);
    double *arr2 = malloc(n * sizeof(double));

    printf("%s \n", message);

    //printf("Original array:\n");
    //print_arr(arr1, 10);

    memcpy(arr2, arr1, n * sizeof(double));

    swap_count = comp_count = 0;
    shell_sort(arr1, n);
    printf("\tShell sort:\n");
    printf("\tAmount of swaps:\t%d\n", swap_count);
    printf("\tAmount of comparisons:\t%d\n\n", comp_count);

    //printf("Shell sort array:\n");
    //print_arr(arr1, 10);

    swap_count = comp_count = 0;
    pyramid_sort(arr2, n);
    printf("\tPyramid sort:\n");
    printf("\tAmount of swaps:\t%d\n", swap_count);
    printf("\tAmount of comparisons:\t%d\n\n", comp_count);

    //printf("Pyramid sort array:\n");
    //print_arr(arr2, 10);

    free(arr1);
    free(arr2);
}

void test_generation(int n) {
    double *arr = arrgen(0, n);

    print_arr(arr, n);

    free(arr);
}

void verbosinator(int a, int b, const char *s, double *arr) {
    printf("%d & %d & ", comp_count, swap_count);

    for (int i = 0; i < 5; i++) {
        if (i == a || i == b) printf("%s{%.1lf}", s, arr[i]);
        else                  printf("%.1lf", arr[i]);

        if (i != 4) printf(" & ");
    }
    printf(" \\\\ \n");
}

void verbose_heapify(int i, double *arr, int n) {
    int root = i;               // get the root and its 2 children
    int l = 2 * i + 1;          // and swap the root with a child if it's bigger
    int r = 2 * i + 2;



    if (l < n && (comp_count++, dabs(arr[l]) > dabs(arr[root]))) {
        root = l;
    }
    if (l < n) verbosinator(l, i, "\\cmp", arr);

    if (r < n && (comp_count++, dabs(arr[r]) > dabs(arr[root]))) {
        verbosinator(root, r, "\\cmp", arr);
        root = r;
    }else if (r < n) verbosinator(root, r, "\\cmp", arr);


    if (root != i) {
        swap_count++;
        verbosinator(i, root, "\\swap", arr);
        swap(arr + i, arr + root);

        verbose_heapify(root, arr, n);
    }
}

void verbose_pyramid_sort(double *arr, int n) {
    for (int i = n / 2 - 1; i >= 0 ; i--) { // turn the input into a pyramid array
        verbose_heapify(i, arr, n);
    }

    printf("\\hline \\hline\n \\multicolumn{10}{c}{Сортировка} \\\\ \n \\hline \\hline\n");

    for (int i = n - 1; i >= 0; i--) { // get the root element, which is the biggest,
        swap_count++;                  // till the pyramid is of length 0
        verbosinator(0, i, "\\swap", arr);

        swap(arr, arr + i);            // and put it after the whole pyramid and do this


        verbose_heapify(0, arr, i);
    }
}



int main()
{
    unsigned int seed = 1709061327; //time(NULL);
    printf("Seed : %d\n\n", seed);
    srand(seed);

    /*test_generation(10000);

    
    for (int n = 10; n <= 100000; n *= 10) { // creates arrays with lengths of increasing magnitude

        printf("N = %d\n\n", n);

        // creates an array and copies it
        // to be sorted by 2 different sorting algorithms

        // Best case array is in the order we need

        show_results(n, 1, "1) (best case)");

        // Worst case array is an array in the order that is reverse to the needed

        show_results(n, -1, "2) (worst case)");

        // Random array ¹1

        show_results(n, 0, "3)");

        // Random array ¹2

        show_results(n, 0, "4)");


    }
    */

    double arr[5] = {3.1, 1.0, 6.4, 3.5, 7.6};


    swap_count = comp_count = 0;

    //verbose_pyramid_sort(arr, 5);

    pyramid_sort(arr, 5);
    printf("%d %d", comp_count, swap_count);

    return 0;
}
