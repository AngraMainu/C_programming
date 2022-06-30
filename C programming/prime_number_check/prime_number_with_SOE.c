#include<stdio.h>
#include<math.h>
#include<stdint.h>
#include<stdlib.h>
#include<stdbool.h>

#define PRIME 0
#define NOT_PRIME 1
#define ERAT_LIMIT 1000000

int64_t ReadInput(int64_t *n, int *ret);
void Eratosthenes(int *prime_arr); 
void primeFactor(int *prime_arr, int64_t number);


enum {SUCCESS = 0, INPUT_ERROR = 100, MEMORY_ERROR = -1};

int main()
{
    int ret = SUCCESS;
    int64_t number;
    int *prime_arr = (int*) calloc(ERAT_LIMIT, sizeof(*prime_arr));
    if (prime_arr == NULL) {
        fprintf(stderr, "Error while allocation of dynamic array\n");
        return MEMORY_ERROR;
    }

    Eratosthenes(prime_arr);

    while(ReadInput(&number, &ret) > 0){
        primeFactor(prime_arr, number);
    }
    if (number < 0)
    {
        fprintf(stderr, "Error: Chybny vstup!\n");
        ret = INPUT_ERROR;
    }
    
    free(prime_arr);
    prime_arr = NULL;

    return ret;
}

int64_t ReadInput(int64_t *number, int *ret){
    if(scanf("%ld", number) != 1 ){
        *number = 0;
        fprintf(stderr, "Error: Chybny vstup!\n");
        *ret = INPUT_ERROR;
    }
    return *number;
}

void Eratosthenes(int *prime_arr)
{
    prime_arr[0] = NOT_PRIME;
    prime_arr[1] = NOT_PRIME;
    for (int i = 2; i < ERAT_LIMIT; i++) {
        if (prime_arr[i] == PRIME){
            for (int j = 2; (i*j) < ERAT_LIMIT; j++) { 
                // Every prime number multiplied by a 
                // natural number up to the ERAT_LIMIT
                // is not a prime number.
                prime_arr[(i*j)] = NOT_PRIME;
            }
        }
    }
}

void primeFactor(int *prime_arr, int64_t number)
{
    int number_root = sqrt(number);
    int64_t n_origin = number;
    bool first_iteration = true;
    int count;
    printf("Prvociselny rozklad cisla %ld je:\n", number);
    for (int i = 2; i <= number_root; i++) {
        if(prime_arr[i] == PRIME && number % i == 0) {
            count = 0;
            while (number % i == 0){
                // Counting the ammount
                // of times a number is
                // divisible by the same
                // number "i".
                number = number / i;
                count++;
            }
            if(first_iteration == false) printf(" x ");
            if(count > 1) printf("%d^%d", i, count);
            else printf("%d", i);
            first_iteration = false;
        } else continue;
    }
    if(n_origin == number) printf("%ld\n", number);
    else if(number != 1) printf(" x %ld\n", number);
    else printf("\n");
}
