#include <stdlib.h>
#include <stdio.h>
#include "vbx.h"
#include "matr.h"



void mm(int *A, int *B, int *C)
{
    int mat_size = N * N;
    int row_size = N;
    int num_rows = N;
    /*create vector scratchpads with the size of matrix for both*/
    /*operands and the result*/
	  vbx_word_t *a = vbx_sp_malloc(N*N * sizeof(vbx_word_t));
	  vbx_word_t *b = vbx_sp_malloc(N*N * sizeof(vbx_word_t));
	  vbx_word_t *c = vbx_sp_malloc(N*N * sizeof(vbx_word_t));
    /*transfering data from matrix arrays to vector scratchpads*/
    /*scratchpad ptr, host ptr, numBytes*/

    vbx_dma_to_vector(a, A, N*N*sizeof(vbx_word_t));
    vbx_dma_to_vector(b, B, N*N*sizeof(vbx_word_t));
    vbx_sync();
    for(int i=0; i<N ; i++){
      //for(int j=0; j< N; j++){
        vbx_set_vl(row_size, num_rows);
        /*stride -> num_rows, incDest, incSrcA*/
        vbx_set_2D(1*4, 0, N*4);
        /*vector-vector, words size, unsigned, dest->c, sources->a, b*/
        vbx_acc(VVWWWUUU, VMUL, (c+i*N), (a+i*N), b);
        vbx_sync();
      //}
    }
    vbx_dma_to_host(C, c, N*N*sizeof(vbx_word_t));
    vbx_sync();
    //}
    //vbxsim_print_stats();
    //vbx_sp_free();
	  return;
}



int main(){

  vbxsim_init(
      4,      /* numb_lanes*/
      64,     /*scratchpad capacity*/
      256,    /*max_masked_waves*/
      6,      /*fxp_word_frac_bits*/
      5,      /*fxp_byte_frac_bits*/
      4,
      0,
      0
  );

  int *A = create_matrix();
  int *B_org = create_matrix();
  int *B = create_matrix();
  int *C = create_matrix();
  /*randomly initialize A and B*/
  initialize_matrix(A, 0, 1);
  initialize_matrix(B_org, 0, 2);

  A[12]=0;
  A[15]=3;
  B_org[7]=0;
  B_org[11]=3;

  for(int i=0; i<N; i++)
    for(int j=0; j<N; j++)
      B[i*N+j] = B_org[j*N+i];


  print_matrix(A);
  printf("\n\n");
  print_matrix(B);
  printf("\n\n");
  mm(A, B, C);

  print_matrix(C);
  printf("\n\n");


  return 0;
}
