#include<stdio.h>
#include<stdlib.h>

#define ALLOC_INC 15

typedef struct str_line {
    char* line;
    int size;
    int limit;
} str_line;

void init_line(str_line* myLine); 
void resize_line(str_line* myLine);
void free_line(str_line* myLine);
int read_line(str_line* myLine, FILE* f);
void open_file(FILE** f, char* nameFile);
void close_file(FILE** f);
int find_line(str_line myLine, char* pattern);
int length(char* myStr);

enum { NO_END_LINE = 0, END_LINE = 1, END_FILE = 2, FIND = 1, NO_FIND = 0 };

int main(int argc, char *argv[])
{
    str_line line;
    FILE* f;

    open_file(&f, argv[2]);

    int finish_read = NO_END_LINE;
    int i = 0;
    while (finish_read != END_FILE){
        init_line(&line);
        finish_read = read_line(&line, f);
    
        if (find_line(line, argv[1])){
            printf("%s\n", line.line);
            i++;
        }
        free_line(&line);
    }
    close_file(&f);
    
    return i > 0 ? 0 : 1;
}

void init_line(str_line* myLine){
    myLine->line = (char*) malloc (sizeof(char));
    myLine->size = 0;
    myLine->limit = 1;
    myLine->line[0] = '\0';
}

void resize_line(str_line* myLine){
    myLine->limit += ALLOC_INC;
    myLine->line = realloc(myLine->line, myLine->limit * sizeof(char));
}

void free_line(str_line* myLine){
    if(myLine->line != NULL){
        free(myLine->line);
    }
}

int read_line(str_line* myLine, FILE* f){
    int finish_line = NO_END_LINE;
    while(!finish_line) {
        if(myLine->size + 2 >= myLine->limit) {
            resize_line(myLine);
        }

        char ch = fgetc(f);

        switch (ch)
        {
        case '\n':
            finish_line = END_LINE;
            break;

        case EOF:
            finish_line = END_FILE;
            break;
        
        default:
            myLine->line[myLine->size++] = ch;
            break;
        }
    }
    myLine->line[myLine->size++] = '\0';
    return finish_line;
}

int length(char* myStr){
    int tmp = 0;
    while(myStr[tmp]){
        tmp++;
    }
    return tmp;  
}

int find_line(str_line myLine, char* pattern){
    int len = length(pattern);
    if (len == 0){
        return FIND;
    }
    int i = 0;
    while(myLine.line[i]){
        if(myLine.line[i] == pattern[0]){
            int j = 1;
            while(j < len && i+j < myLine.size && myLine.line[(i+j)] == pattern[j]){
                j++;
            }
            if(j == len){
                return FIND;
            }
        }
        i++;
    }
    return NO_FIND;
}

void open_file(FILE** f, char* nameFile){
    *f = fopen(nameFile, "r");
    if (*f == NULL){
        exit(EXIT_FAILURE);
    }
}

void close_file(FILE** f){
    if (fclose(*f) == EOF){
        exit(EXIT_FAILURE);
    }
}