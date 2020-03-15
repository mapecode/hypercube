#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define FILE_NAME "datos.dat"
#define MAX  1000
#define MIN -1000

void check_argv(char *argv[],int argc);
FILE *open_file(const char * path, const char *mode);
void write_numbers(FILE *file, int max);

int main(int argc, char *argv[]){
    int max;
    FILE *file;

    check_argv(argv,argc);
    file = open_file(FILE_NAME, "w+");
    write_numbers(file, atoi(argv[1]));

    return EXIT_SUCCESS;
}

void check_argv(char *argv[],int argc){
    if(argc!=2){
        fprintf(stderr,"Error in argv. Use: ./gen_data <NUM>\n");
        exit(EXIT_FAILURE);
    }

    for(int i = 0, n = strlen(argv[1]); i < n; i++){
        if(!isdigit(argv[1][i])){
            fprintf(stderr,"Error in argv. Use: ./gen_data <NUM>\n");
            exit(EXIT_FAILURE);
        }
    }
}

FILE *open_file(const char * path, const char *mode){
    FILE *file;
    if ((file = fopen(path, mode)) == NULL)
    {
        fprintf(stderr, "Error opening file.\n");        
        exit(EXIT_FAILURE);
    }
    return file;
}

void write_numbers(FILE *file, int max){
    int i;
    srand(time(NULL));

    for(i = 0; i < max-1; i++)
        fprintf(file,"%.2f,", ((rand() % (MIN - MAX + 1) + MIN) * (drand48() + 1.0)));
    
    fprintf(file,"%.2f", ((rand() % (MIN - MAX + 1) + MIN) * (drand48() + 1.0)));

    fclose(file);
}