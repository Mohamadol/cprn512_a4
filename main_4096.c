#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "vbx.h"
#include "matr.h"



void mm(int *A, int *B, int *C)
{
    int mat_size = N * N;
    int row_size = N;
    int num_rows = N;
    /*create vector scratchpads with the size of matrix for both*/
    /*operands and the result*/
	  vbx_word_t *a = vbx_sp_malloc(N * sizeof(vbx_word_t));
	  vbx_word_t *b = vbx_sp_malloc(N*N * sizeof(vbx_word_t));
	  vbx_word_t *c = vbx_sp_malloc(N * sizeof(vbx_word_t));
    /*transfering data from matrix arrays to vector scratchpads*/
    /*scratchpad ptr, host ptr, numBytes*/


    vbx_dma_to_vector(b, B, N*N*sizeof(vbx_word_t));
    vbx_sync();
    for(int i=0; i<N ; i++){
      //for(int j=0; j< N; j++){
        vbx_dma_to_vector(a, A+i*N, N*sizeof(vbx_word_t));
        vbx_sync();
        vbx_set_vl(row_size, num_rows);
        /*stride -> num_rows, incDest, incSrcA*/
        vbx_set_2D(1*4, 0, N*4);
        /*vector-vector, words size, unsigned, dest->c, sources->a, b*/
        vbx_acc(VVWWWUUU, VMUL, (c), (a), b);
        vbx_sync();
        vbx_dma_to_host(C+i*N, c, N*sizeof(vbx_word_t));
        vbx_sync();
      //}
    }
    //}
    vbxsim_print_stats_extended();
    //vbx_sp_free();
	  return;
}



int main(){

  vbxsim_init(
      4,      /* numb_lanes*/
      18*1024,     /*scratchpad capacity*/
      256,    /*max_masked_waves*/
      6,      /*fxp_word_frac_bits*/
      5,      /*fxp_byte_frac_bits*/
      4,
      0,
      0
  );
  clock_t end, begin;
  float datalayout_time;
  int *A = create_matrix();
  int *B_org = create_matrix();
  int *B = create_matrix();
  int *C = create_matrix();
  /*randomly initialize A and B*/
  initialize_matrix(A, 1, 0);
  initialize_matrix(B_org, 1, 0);


  //change the rows and columns of matrix for ease of access
  begin = clock();
  for(int i=0; i<N; i++)
    for(int j=0; j<N; j++)
      B[i*N+j] = B_org[j*N+i];
  end = clock();
  datalayout_time = end - begin;
  mm(A, B, C);
  printf("memory layout fixing time %.6f\n", datalayout_time / 1000000.0);

  /*
  //comment out below for result check with
  smaller matrcies
  print_matrix(A);
  printf("\n\n");
  print_matrix(B_org);
  printf("\n\n");
  print_matrix(C);
  printf("\n\n");
  */

  return 0;
}
