#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

void free_arrays(int** arr, int ammount);

void matrix_addition(int** arr_one, int rows_one, int columns_one, int** arr_two, int rows_two, int columns_two, int pos_fir, int j, int* ret, int i);

void matrix_substraction(int** arr_one, int rows_one, int columns_one, int** arr_two, int rows_two, int columns_two, int pos_fir, int j, int* ret, int i);

int* matrix_multiplication(int* arr_one, int rows_one, int columns_one, int* arr_two, int* rows_two, int columns_two, int* ret, int i);

void print_matrix(int* arr, int rows, int columns);

enum {INPUT_ERROR = 100, MEMORY_ERROR = -1, SUCCESS = 0};

int main()
{
    int ret = SUCCESS;
    int i = 0;
    int rows[99] = { 0 }, cols[99] = { 0 };
    char operation[99] = { 0 };
    int SIZE_LIMIT = 99;
    int** mats = (int**)malloc(SIZE_LIMIT * sizeof(int*));
    if (mats == NULL){
        fprintf(stderr, "Error while allocation of dynamic array\n");
        free(mats);
        exit(MEMORY_ERROR);
    }
    // Step by step input: allocating memory, writing input, checking input.
    while(scanf("%d %d", &rows[i], &cols[i]) == 2) {
		mats[i] = (int *)malloc((rows[i] * cols[i]) * sizeof(int));
        if (mats == NULL){
            fprintf(stderr, "Error while allocation of dynamic array\n");
            free_arrays(mats, i);
            exit(MEMORY_ERROR);
        }
        for (int j = 0; j < (rows[i] * cols[i]); j++) {
            if (scanf("%d", &mats[i][j]) != 1) {
                fprintf(stderr, "Error: Chybny vstup!\n");
                free_arrays(mats, (i+1));
                exit(INPUT_ERROR);
            }
        }
        if (scanf("\n%c", &operation[i]) == 0) {
            fprintf(stderr, "Error: Chybny vstup!\n");
            free_arrays(mats, (i+1));
            exit(INPUT_ERROR);
        }
        i++;
    }

    // 99 is the maximum ammount of matrixes, 98 is the maximum ammount of operations.
    // performs multiplication by calling the mult. function each time it sees a '*'.
    // Mult product is stored at matrix B ( A*B=C ) and matrix A is marked as skippable.
    int skip_mat[99]= { 0 };
    for (int j = 0; j < 98; j++) { 
        if (operation[j] == '*' && (cols[j] == rows[(j+1)])) {
                mats[(j+1)] = matrix_multiplication(&mats[j][0], rows[j], cols[j], &mats[(j+1)][0], &rows[(j+1)], cols[(j+1)], &ret, i);
                skip_mat[(j)] = 1;
                if(ret != SUCCESS) break;
        } else if (operation[j] == '*' && (cols[j] != rows[(j+1)])){
            fprintf(stderr, "Error: Chybny vstup!\n");
            free_arrays(mats, (i));
            exit(INPUT_ERROR);
        }
    }

    // Marks the first unskippable matrix.
    int pos_fir;
    if(ret == SUCCESS){
        for (int j = 0; j < 98; j++) {
            if(skip_mat[j] == 0){
                pos_fir = j;
                break;
            }
        }
    }
    // Performs all remaining '+' and '-' operations on the first unskippable matrix.
    if(ret == SUCCESS){
        for (int j = 0; j < 98; j++) {
            if(operation[j] == '+'){
                for (int k = 1; k < 98; k++) {
                    if (skip_mat[(j+k)] == 0) {
                        matrix_addition(mats, rows[pos_fir], cols[pos_fir], mats, rows[(j+k)], cols[(j+k)], pos_fir, (j+k), &ret, i);
                        break;
                    }
                }
            } else if(operation[j] == '-') {
                for (int k = 1; k < 98; k++) {
                    if (skip_mat[(j+k)] == 0) {
                        matrix_substraction(mats, rows[pos_fir], cols[pos_fir], mats, rows[(j+k)], cols[(j+k)], pos_fir, (j+k), &ret, i);
                        break;
                    }
                }
            } else if(operation[j] == 0) break;
        }
    }
    // Prints the correct output.
    if(ret == SUCCESS){
    for (int j = 0; j < i; j++) {
        if(skip_mat[j] == 0 && j < i){
            print_matrix(mats[j], rows[j], cols[j]);
            break;
        }
    }
    }
    free_arrays(mats, i);
    return ret;
}

void print_matrix(int* arr, int rows, int columns){
    bool first_cycle = true;
    printf("%d %d\n", rows, columns);
    for (int i = 0; i < rows*columns; i++){
        if(!first_cycle) printf(" ");
        first_cycle = false;
        printf("%d", *arr);
        if ( (i+1) % columns == 0){
            printf("\n");
            first_cycle = true;
        }
        arr++;
    }
}

void matrix_addition(int** arr_one, int rows_one, int columns_one, int** arr_two, int rows_two, int columns_two, int pos_fir, int j, int* ret, int i){
    if(rows_one != rows_two || columns_one != columns_two){
        fprintf(stderr, "Error: Chybny vstup!\n");
        free_arrays(arr_one, i);
        exit(INPUT_ERROR);
    }
    if (*ret == SUCCESS) {
        for (int i = 0; i < rows_one*columns_one; i++) {
            arr_one[pos_fir][i] = arr_one[pos_fir][i] + arr_two[j][i];
        }
    }
}

void matrix_substraction(int** arr_one, int rows_one, int columns_one, int** arr_two, int rows_two, int columns_two, int pos_fir, int j, int* ret, int i){
    if(rows_one != rows_two || columns_one != columns_two){
        fprintf(stderr, "Error: Chybny vstup!\n");
        free_arrays(arr_one, i);
        exit(INPUT_ERROR);
    }
    if (*ret == SUCCESS) {
        for (int i = 0; i < rows_one*columns_one; i++) {
            arr_one[pos_fir][i] = arr_one[pos_fir][i] - arr_two[j][i];
        }
    }
}

// Creates a temporary result matrix to which the result is being stored, then setting the answer as matrix B ( A*B = C). 
int* matrix_multiplication(int* arr_one, int rows_one, int columns_one, int* arr_two, int* rows_two, int columns_two, int* ret, int i){
    int* res_matrix = calloc( (rows_one * columns_two), sizeof(int));
    if (res_matrix == NULL) {
        fprintf(stderr, "Error while allocation of dynamic array\n");
        free(res_matrix);
        *ret = MEMORY_ERROR;
    }

    if(*ret == SUCCESS) {
        for(int k = 0; k < rows_one; k++) {
            for(int i = 0; i < columns_two; i++) {
                for (int j = 0; j < *rows_two; j++) {
                    res_matrix[(k * columns_two)+ i] += *(arr_one + (k*columns_one) + j) * *(arr_two + (j*columns_two) + i); 
                }
            }
        }
        *rows_two = rows_one;
        free(arr_two);
        arr_two = NULL;
        arr_two = realloc(res_matrix, (rows_one * columns_two) * sizeof(int));
        return arr_two;
    } else return NULL;
}

void free_arrays(int** arr, int ammount){
    for (int i = 0; i < ammount; i++)
		free(arr[i]);
	free(arr);
}
