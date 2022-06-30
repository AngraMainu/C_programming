#include<stdio.h>

#define MIN 3
#define MAX 69

void print_roof(int columns);
void print_unfilled_square(int rows, int columns);
void print_filled_square(int rows, int columns, int fence_size);
void print_fence(int rows, int i, int fence_size, int columns, int j);

int main()
{
    int columns, rows, fence_size = 0;
    int opt_check;
    if(scanf("%d %d", &columns, &rows) == 2) {

        if ((MIN > columns) || (columns > MAX) ||
        (MIN > rows) || (rows > MAX)) {
            fprintf(stderr, "Error: Vstup mimo interval!\n");
            return 101;
        } 
        if (columns % 2 == 0)  {
            fprintf(stderr, "Error: Sirka neni liche cislo!\n");
            return 102;
        }
    } else {
        fprintf(stderr, "Error: Chybny vstup!\n");
        return 100;
    }
    opt_check = scanf("%d", &fence_size); 
    // EOF = -1, character as input = 0, viable input = 1

    if (opt_check == 1){ //print pretty house
        if(fence_size < rows && fence_size > 0){
            print_roof(columns);
            print_filled_square(rows, columns, fence_size);
        } else {
            fprintf(stderr, "Error: Neplatna velikost plotu!\n");
            return 103;
        }
    } else if (opt_check == 0) { 
        fprintf(stderr, "Error: Chybny vstup!\n");
        return 100;
    } else { //print boring house
        print_roof(columns);
        print_unfilled_square(rows, columns);
    }

    return 0;
}

void print_unfilled_square(int rows, int columns)
{
    for (int i = 1; i <= rows; i++)
    {
        for (int j = 1; j <= columns; j++)
        {
            if (i == 1 || i == rows || j == 1 || j == columns) printf("X");
            else printf(" ");
        }
        printf("\n");
    }
}

void print_roof(int columns)
{
    int mid_line = columns/2;

    for (int i = 0; i < mid_line; i++)
    {
        for (int j = 0; j <= mid_line+i; j++)
        {
            if (j == mid_line - i  || j == mid_line + i) printf("X");
            else printf(" ");
        }
        printf("\n");
    }
}

void print_filled_square(int rows, int columns, int fence_size)
{
    for (int i = 1; i <= rows; i++)
    {
        for (int j = 1; j <= columns; j++) 
        //this cycle is responsible for building filled square
        {
            if (i == 1 || i == rows || j == 1 || j == columns) printf("X");
            else if((j+i) % 2 == 0) printf("o");
            else printf("*");
        }
        if(rows - i < fence_size)
        //this cycle is responsible for building fence
        {
            for (int j = 1; j <= fence_size; j++)
            {
                print_fence(rows, i, fence_size, columns, j);
            }
        }
        printf("\n");
    }
}

void print_fence(int rows, int i, int fence_size, int columns, int j)
{
    int starting_fence_row = rows-i+1;
    int rows_left_to_print = rows-i;

    if (fence_size % 2 == 0){ // "-|-|-|" pattern for even fence sizes
        if ((rows_left_to_print == 0) || (starting_fence_row == fence_size)) {
            if(j % 2 != 0) printf("-");
            else printf("|");
        }
        else {
            if(j % 2 != 0) printf(" ");
            else printf("|");
        }
    }
    else { // "|-|-|-|" pattern for odd fence sizes
        if ((rows - i == 0) || ((rows-i)+1 == fence_size)) {
            if(j % 2 != 0) printf("|");
            else printf("-");
        }
        else {
            if(j % 2 != 0) printf("|");
            else printf(" ");
        }
    }
}


