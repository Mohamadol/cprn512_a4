#ifndef MATR_H
#define MATR_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


#if !defined(N)
#define N 4096
#endif

#if !defined(PARALLEL)
#define PARALLEL 0
#endif



int *create_matrix(){
  int *matrix = (int *)malloc(sizeof(int) * N * N);
  return matrix;
}


void initialize_matrix(int *data, int random_init, int init_val){
  float max_random_init = 1000.0;
  int max_random_init_int = 500;

  for(int i=0 ; i<N ; i++){
    for(int j=0 ; j<N ; j++){
      if(random_init)
        data[i * N + j] = (int)(rand() / (RAND_MAX/max_random_init) + 1);
      else
        data[i * N + j] = init_val;
    }
  }
}


void initialize_matrix_from_another_matrix(float *dest, float *source){
  for(int i=0 ; i<N ; i++){
    for(int j=0 ; j<N ; j++){
      dest[i * N + j] = source[i * N + j];
    }
  }
}


void print_matrix(int *data){
  for (int i=0 ; i<N ; i++){
    for(int j=0 ; j<N ; j++)
      printf("%d  ", (data)[i*N+j]);
    printf("\n");
  }
}



void free_matrix(int *data){
  free(data);
}

#endif
