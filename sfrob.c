//
//  main.c
//  sfrob
//
//  Created by Sanchit Agarwal on 11/11/20.
//

#include <stdio.h>
#include <stdlib.h>

/* Prototype for the function that stores input
   in a 2*2 array */
char** readInput(int* rows);
int frobcmp(char const* a, char const* b);
char unOb(char obfuscated);
void printOutput(char** arr, int rows);
void freeMemory(char** arr, int rows);
int compare(const void* a, const void* b);

int main(int argc, const char * argv[]) {
    int* rows;
    int i = 0;
    rows = &i;
    char** arr = readInput(rows);
    qsort(arr, *rows, sizeof(char*), compare);
    printOutput(arr, *rows);
    freeMemory(arr, *rows);
}

char** readInput(int* rows)
{
    /* initialize a dynamic 2D array with one row and
       one cell in that row */
    char** input_array = (char**)malloc(sizeof(char*));
    *input_array = (char*)malloc(sizeof(char));
    
    /* report error if there is no space to allocate */
    if (input_array == NULL || *input_array == NULL)
    {
        fprintf(stderr, "Out of memory");
        exit(1);
    }
    
    /* some local variables */
    int current_char = getchar();
    int current_word = 0;
    int char_index = 0;
    
    /* Empty files */
    if(current_char == EOF)
    {
        return NULL;
    }
    
    int end_with_space = 0;
    while(current_char != EOF)
    {
        if((char)current_char == ' ')
        {
            // insert space at the end of current_word
            input_array[current_word][char_index] = ' ';
            // allocate a new row
            input_array = (char**)realloc(input_array, ((*rows + 1) * sizeof(char*)) + sizeof(char*));
            if (input_array == NULL)
            {
                fprintf(stderr, "Out of memory");
                exit(1);
            }
            *rows += 1;
            // modify local variables
            current_word += 1;
            char_index = 0;
            input_array[current_word] = (char*)malloc(sizeof(char));
            if (input_array[current_word] == NULL)
            {
                fprintf(stderr, "Out of memory");
                exit(1);
            }
            end_with_space = 1;
        }
        
        else
        {
            // make space for next char
            input_array[current_word] = (char*)realloc(input_array[current_word],
                                          char_index + 1 + sizeof(char));
            if (input_array[current_word] == NULL)
            {
                fprintf(stderr, "Out of memory");
                exit(1);
            }
            // insert the current char at the end of current word
            input_array[current_word][char_index] = (char)current_char;
            char_index++;
            end_with_space = 0;
        }
        current_char = getchar();
    }
    
    if(end_with_space == 0)
    {
        input_array[current_word][char_index] = ' ';
        *rows += 1;
    }
    else
    {
        free(input_array[current_word]);
    }
    return input_array;
}

int frobcmp(char const* a, char const* b)
{
    int forever = 1;
    int i = 0;
    while(forever)
    {
        if(unOb(a[i]) == unOb(b[i]))
        {
            if(a[i] == ' ')
                return 0;
            else
                i += 1;
        }
        
        else
        {
            if(unOb(a[i]) > unOb(b[i]))
                return 1;
            else
                return -1;
        }
    }
    return 0;
}

char unOb(char obfuscated)
{
    return obfuscated ^ 42;
}

void printOutput(char** arr, int rows)
{
    int i;
    int j;
    for(i = 0; i < rows; i++)
    {
        for(j = 0; arr[i][j] != ' '; j++)
        {
            putchar(arr[i][j]);
        }
        putchar(' ');
    }
}

void freeMemory(char** arr, int rows)
{
    int i;
    for(i = 0; i < rows; i++)
    {
        free((void*)arr[i]);
    }
    free((void*)arr);
}

int compare(const void* a, const void* b)
{
    return frobcmp(*(char**)a, *(char**)b);
}
