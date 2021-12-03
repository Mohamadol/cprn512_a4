#include <stdlib.h>
#include <stdio.h>
#include "vbx.h"
#include "matr.h"



void mm(float *A, float *B, float *C)
{
    /*create vector scratchpads with the size of matrix for both*/
    /*operands and the result*/
	  vbx_word_t *a = vbx_sp_malloc(N * N * sizeof(vbx_word_t));
	  vbx_word_t *b = vbx_sp_malloc(N * N * sizeof(vbx_word_t));
	  vbx_word_t *c = vbx_sp_malloc(N * N * sizeof(vbx_word_t));
    /*transfering data from matrix arrays to vector scratchpads*/
    /*scratchpad ptr, host ptr, numBytes*/
    vbx_dma_to_vector(a, A, N * N * sizeof(vbx_word_t));
    vbx_dma_to_vector(b, B, N * N * sizeof(vbx_word_t));
    vbx_sync();
    /*setting vector length register*/
    /*num lanes=N, num_rows=N*/
    vbx_set_vl(N, N);
    /*stride -> num_rows, incDest, incSrcA*/
    vbx_set_2D(N, N, N);
    /*vector-vector, words size, unsigned, dest->c, sources->a, b*/
    vbx(VVWWWUUU, VMUL, c, a, b);
  	vbx_sync();
    vbx_dma_to_host(C, c, N * N * sizeof(vbx_word_t));
    vbx_sync();
    vbxsim_print_stats();
    vbx_sp_free();
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

  float *A = create_matrix();
  float *B = create_matrix();
  float *C = create_matrix();
  /*randomly initialize A and B*/
  initialize_matrix(A, 1, 0);
  initialize_matrix(B, 1, 0);
  print_matrix(A);
  print_matrix(B);
  mm(A, B, C);
  print_matrix(C);

  return 0;
}
