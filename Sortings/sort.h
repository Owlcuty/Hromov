#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

typedef int errno_t;

errno_t sort_bubble(double *arr, int arr_size, int *n_cmp, int *n_move);
errno_t sort_select(double *arr, int arr_size, int *n_cmp, int *n_move);
errno_t sort_insert(double *arr, int arr_size, int *n_cmp, int *n_move);
errno_t sort_qsort(double *arr, int arr_size, int *n_cmp, int *n_move);
errno_t my_qsort(double *arr, int first, int last, int *n_cmp, int *n_move);
errno_t generation(FILE* fout);
errno_t write_csv(double* arr, int arr_size, FILE* fcsv);
errno_t sort(double* arr, int arr_size, FILE* fcsv);
errno_t run(FILE* fdata, FILE* fcsv);

extern errno_t (*sortings[4])(double *arr, int arr_size, int *n_cmp, int *n_move);

extern const size_t NumOfArray;
