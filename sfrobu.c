//
//  main.c
//  sfrob
//
//  Created by Sanchit Agarwal on 11/11/20.
//
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

// Global Variable
int f_true = 0;

/* Prototype for the function that stores input
   in a 2*2 array */
char** readInput(int* rows, char* source, int bytes);
int frobcmp(char const* a, char const* b);
char unOb(char obfuscated);
void printOutput(char** arr, int rows);
void freeMemory(char** arr, int rows);
int compare(const void* a, const void* b);
char* inToBuff(int* total_bytes);
void memError(void);
void readError(void);

int main(int argc, const char * argv[]) {
    int* rows;
    int* bytes;
    int i = 0;
    int j = 0;
    rows = &i;
    bytes = &j;

    // change global variable if fflag present
    if(argc == 2 && strcmp(argv[1],"-f") == 0)
        f_true = 1;
    else if(argc > 2 || (argc == 2 && strcmp(argv[1],"-f") != 0))
    {
        fprintf(stderr, "Incorrect arguments!");
        exit(1);
    }
    char* buffer = inToBuff(bytes);
    char** arr = readInput(rows, buffer, *bytes);
    qsort(arr, *rows, sizeof(char*), compare);
    printOutput(arr, *rows);
    freeMemory(arr, *rows);
    free((void*)buffer);
}

void memError()
{
    fprintf(stderr, "Out of memory");
    exit(1);
}

void readError()
{
    fprintf(stderr, "read/write error!");
    exit(1);
}

char* inToBuff(int* total_bytes)
{
    struct stat fileData;
    char* readBuffer;
    int bytes_read = 0;
    ssize_t r_err;

    // populate filedata struct with file info and report errors
    if(fstat(STDIN_FILENO, &fileData) < 0) 
    {
        fprintf(stderr, "file error");
        exit(1);
    }

    if(S_ISREG(fileData.st_mode))
    {
        int size = fileData.st_size;
        readBuffer = malloc(size);
        if(readBuffer == NULL)
            memError();
        int i = 0;
        while((r_err = read(STDIN_FILENO, readBuffer + i, size - i)) > 0)
        {
            if(fileData.st_size > r_err)
            {
                readBuffer = realloc(readBuffer, fileData.st_size);
                if(readBuffer == NULL)
                    memError();
                i = r_err;
                size = fileData.st_size;
            }
            readBuffer = realloc(readBuffer, fileData.st_size + 1);
        }
        bytes_read = size;
    }

    else 
    {
        readBuffer = malloc(8192);
        if(readBuffer == NULL)
            memError();
        int i = 0;
        int size = 8192;
        while((r_err = read(STDIN_FILENO, readBuffer + i, 1)) > 0)
        {
            if(i >= size)
            {
                size *= 2;
                readBuffer = realloc(readBuffer, size);
                if(readBuffer == NULL)
                    memError();
            }
            i += 1;
        }
        bytes_read = i;
    }
    
    if(r_err < 0)
        readError();
    //check for trailing space
    if((bytes_read >= 1 && readBuffer[bytes_read - 1] != ' ') ||
       (bytes_read >= 2 && readBuffer[bytes_read - 1] == ' ' && readBuffer[bytes_read - 2] == ' ') ||
       (bytes_read == 1 && readBuffer[bytes_read - 1] == ' '))
    {
        readBuffer[bytes_read] = ' ';
        bytes_read += 1;
    }
    *total_bytes = bytes_read;
    return readBuffer;
}

char** readInput(int* rows, char* source, int bytes)
{
    if(bytes == 0)
    {
        return NULL;
    }
    /* initialize a dynamic 2D array with one row and
       one cell in that row */
    char** input_array = (char**)malloc(sizeof(char*));
    *input_array = (char*)malloc(sizeof(char));

    /* some local variables */
    int current_word = 0;
    int char_index = 0;
    int i = 0;
    
    int end_with_space = 0;
    while(i < bytes)
    {
        if(source[i] == ' ')
        {
            // insert space at the end of current_word
            input_array[current_word][char_index] = ' ';
            // allocate a new row
            input_array = (char**)realloc(input_array, ((*rows + 1) * sizeof(char*)) + sizeof(char*));
            if (input_array == NULL)
                memError();
            *rows += 1;
            // modify local variables
            current_word += 1;
            char_index = 0;
            input_array[current_word] = (char*)malloc(sizeof(char));
            if (input_array[current_word] == NULL)
                memError();
            // end_with_space = 1;
        }
        
        else
        {
            // make space for next char
            input_array[current_word] = (char*)realloc(input_array[current_word],
                                          char_index + 1 + sizeof(char));
            // insert the current char at the end of current word
            input_array[current_word][char_index] = source[i];
            if (input_array[current_word] == NULL)
                memError();
            char_index += 1;
            // end_with_space = 0;
        }
        i++;
    }
    free(input_array[current_word]);
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
    char unobfuscated = obfuscated ^ 42;
    if(f_true == 1)
        return toupper((unsigned char)unobfuscated);
    return obfuscated ^ 42;
}

void printOutput(char** arr, int rows)
{
    int i;
    int j;
    ssize_t w_err = 0;
    for(i = 0; i < rows; i++)
    {
        for(j = 0; arr[i][j] != ' '; j++)
        {
            w_err = write(STDOUT_FILENO, &arr[i][j], 1);
            if(w_err < 0)
                readError();
        }
        w_err = write(STDOUT_FILENO, &arr[i][j], 1);
        if(w_err < 0)
            readError();
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
