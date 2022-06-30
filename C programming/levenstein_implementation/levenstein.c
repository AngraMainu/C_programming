#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

char* input(int* dyn_arr_size, int* arr_len, int* ret);

int decipher(char* coded, char* heard, int* arr_len);

void print_deciphed(char* coded, int* arr_len, int shift_amm);

int LevenDist(char* coded, char* heard, int arr_len_coded, int arr_len_heard, int* ret);

int minimum(int fst, int scd, int trd);

enum {SUCCESS = 0, INPUT_ERROR = 100, INPUT_LEN_ERROR = 101, MEMORY_ERROR = -1, NUM_OF_CHARS = 52};

int main(int argc, char *argv[])
{    
    int arr_len_coded = 0, arr_len_heard = 0, ret = SUCCESS;
    int dyn_arr_size = 100;
    char *string_coded, *string_heard;
    bool optional = argc > 1 ? strcmp(argv[1], "-prp-optional") == 0 : false;
    string_heard = string_coded = NULL;

    // "If" construction for correct error representation.
    string_coded = input(&dyn_arr_size, &arr_len_coded, &ret);
    if (ret == SUCCESS) {
        string_heard = input(&dyn_arr_size, &arr_len_heard, &ret);

        if((arr_len_coded == arr_len_heard) && ret == SUCCESS && optional == false) {
            int max = 0, current = 0, shift_amm = 0;

            for (int i = 0; i < NUM_OF_CHARS; i++) {
                current = decipher(string_coded, string_heard, &arr_len_coded);

                if (current > max) {
                    max = current;
                    shift_amm = i+1;
                }
            }
            print_deciphed(string_coded, &arr_len_coded, shift_amm);

        } else if(arr_len_coded != arr_len_heard && optional == false) {
            fprintf(stderr, "Error: Chybna delka vstupu!\n");
            ret = INPUT_LEN_ERROR;

        } else if (ret == SUCCESS && optional == true) {
            int current = 0, needed = __INT_MAX__, shift_amm = 0, max = 0;

            for (int i = 0; i < NUM_OF_CHARS; i++) {

                if(arr_len_coded == arr_len_heard) {
                    current = decipher(string_coded, string_heard, &arr_len_coded);
                } else {
                    decipher(string_coded, string_heard, &arr_len_coded);
                    current = LevenDist(string_coded, string_heard, arr_len_coded, arr_len_heard, &ret);
                    if (ret != SUCCESS) break;
                }
                if (((arr_len_coded == arr_len_heard) && current > max) || 
                    ((arr_len_coded != arr_len_heard) && current < needed)) {
                    max = current;
                    needed = current;
                    shift_amm = i+1;
                }
            }
            if(ret == SUCCESS) print_deciphed(string_coded, &arr_len_coded, shift_amm);
        }
    }
    free(string_coded);
    free(string_heard);
    return ret;
}

//Pseudocode made into working function.
int LevenDist(char* coded, char* heard, int row, int column, int* ret) {
    int subst_cost;
    int* dist = calloc( ((row+1) * (column+1)) , sizeof(int));
    if (dist == NULL){
        fprintf(stderr, "Memory allocation error!");
        *ret = MEMORY_ERROR;
        return -1;
    }
    for (int i = 1; i <= row; i++) {
        dist[(column+1) * i] = i;
    }

    for (int j = 1; j <= column; j++) {
        dist[j] = j;
    }
    
    for (int j = 1; j < column+1; j++) {
        for (int i = 1; i < row+1; i++) {
            if (coded[i] == heard[j]) subst_cost = 0;
            else subst_cost = 1;

            dist[((column+1) * i) + j] = minimum(dist[((column+1) * (i-1)) + j] + 1, //[i-1][j]
                                 dist[((column+1) * i) + (j-1)] + 1,                 //[i][j-1]
                                 dist[((column+1) * (i-1)) + (j-1)] + subst_cost);   //[i-1][j-1]
        }
    }
    int ret_value = dist[(row+1)*(column+1) - 1];
    free(dist);
    return ret_value;
}

int minimum(int fst, int scd, int trd) {
    int min = fst < scd ?  (fst < trd ? fst : trd):
                           (scd < trd ? scd : trd);
    return min;                       
    
}
// Dynamic allocation, read input, realloc if needed.
// Counting length of string.
char* input(int* dyn_arr_size, int* arr_len, int* ret) {
    int ch;
    char* str = malloc(*dyn_arr_size * sizeof(char));
    if (str == NULL){
        fprintf(stderr, "Error while reallocation of dynamic array\n");
        *ret = MEMORY_ERROR;
    }
    if (*dyn_arr_size > 100) *dyn_arr_size = 100;
    while((ch = getchar()) != '\n' && ch != EOF) {
        if ((ch < 'A' || ( (ch > 'Z') && (ch < 'a') ) || ch > 'z'))  {
           fprintf(stderr, "Error: Chybny vstup!\n");
           *ret = INPUT_ERROR;
           break;
        }
        if (*arr_len == *dyn_arr_size) {
            *dyn_arr_size *= 2;
            char* str_new = realloc(str, *dyn_arr_size);
            if (str_new == NULL) {
                free(str);
                fprintf(stderr, "Error while reallocation of dynamic array\n");
                *ret = MEMORY_ERROR;
                break;
            }
            str = str_new;
        }
        str[(*arr_len)++] = ch;
    }
    str[(*arr_len)] = '\0';
    return str;
}

// Simply shifts the string's elements 52 times and checks for alignment.
int decipher(char* coded, char* heard, int *arr_len) {
    int same_chars = 0;
    for (int i = 0; i < *arr_len; i++) {
        if (coded[i] == 'Z') {
            coded[i] = 'a';
        } else if (coded[i] == 'z') {
            coded[i] = 'A';
        } else {
            coded[i] = (int)coded[i] + 1;
        }
        if (coded[i] == heard[i]) same_chars++;
    }
    return same_chars;
}

//almost a copy of the decipher function, but gives out the final result.
void print_deciphed(char* coded, int* arr_len, int shift_amm){
    for (int i = 0; i < *arr_len; i++) {
        for (int j = 0; j != shift_amm; j++) {
            if (coded[i] == 'Z') {
                coded[i] = 'a';
            } else if (coded[i] == 'z') {
                coded[i] = 'A';
            } else {
                coded[i] = (int)coded[i] + 1;
            }
        }
    }
    printf("%s\n", coded);
}
