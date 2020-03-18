#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mpi.h>


#define D           3
#define N_NODES     (int)pow(2,D)

#define FILE_NAME   "datos.dat"

#define TRUE        1
#define FALSE       0

/* For data checks */
#define LENGTH_MSG "length"
#define SIZE_MSG   "size"

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define XOR(a, b) (a ^ b)



void get_data(double *data, int *length);
void check_data(int var, char *type);
void send_data(double *data);
void check_data(int var, char *type);
void send_data(double *data);
void hypercube_neighbors(int rank, int neighbors[]);
void calculate_max(int rank, double num);


int end = FALSE;

int main(int argc, char *argv[]){
    double *data = malloc(N_NODES * sizeof(double));
    int length;
    int rank, size;
    double num;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    if (rank == 0){
        get_data(data, &length);

        check_data(length, LENGTH_MSG);

        if (!end) check_data(size, SIZE_MSG);

        if (!end) send_data(data);
    }

    /* Get confirmation to continue from first node */
    MPI_Bcast(&end,1,MPI_INT,0,MPI_COMM_WORLD);
        
    if(!end){
        /* Wait the number */
        MPI_Recv(&num, 1, MPI_DOUBLE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, NULL);
        calculate_max(rank, num);
    }
    
    MPI_Finalize();

    return EXIT_SUCCESS;
}


void get_data(double *data, int *length){
    /* For load data from datos.dat */
    
    FILE *file;
    char *aux = malloc(1024 * sizeof(char));
    char *n;

    if ((file = fopen(FILE_NAME, "r")) == NULL){
        fprintf(stderr, "Error opening file\n");
        end = TRUE; 
    }else{
        *length = 0;

        fscanf(file, "%s", aux);
        fclose(file);

        data[(*length)++] = atof(strtok(aux,","));

        while((n = strtok(NULL, ",")) != NULL)
            data[(*length)++] = atof(n);

    }

    free(aux);
    
}

void check_data(int var, char *type){
    /* For check length or size */

    if (var != N_NODES){
        fprintf(stderr, "Error in data %s\n", type);
        end = TRUE;
    }
}

void send_data(double *data){
    /* Send numbers to all the nodes */
    
    double buff_num;
    int i;

    for(i=0; i < N_NODES; i++){
        buff_num = data[i];
        MPI_Send(&buff_num, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
        printf("%.2f sended to %d node\n",buff_num, i);
    }

    free(data);
}

void calculate_max(int rank, double num){
    ///* Calculate the maximum number of all the nodes 

    double his_num;
    int neighbor, i;

    for(i=0; i < D; i++){
        neighbor = XOR(rank, (int)pow(2,i));
        MPI_Send(&num, 1, MPI_DOUBLE, neighbor, 1, MPI_COMM_WORLD);
        MPI_Recv(&his_num, 1, MPI_DOUBLE, neighbor, 1,MPI_COMM_WORLD, NULL);
        num = MAX(num, his_num);
    }

    if(rank == 0) printf("\nThe maximum number is: %.2f\n",num);
    
}