/* VECTORBLOX MXP SOFTWARE DEVELOPMENT KIT
 *
 * Copyright (C) 2012-2018 VectorBlox Computing Inc., Vancouver, British Columbia, Canada.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 *     * Neither the name of VectorBlox Computing Inc. nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This agreement shall be governed in all respects by the laws of the Province
 * of British Columbia and by the laws of Canada.
 *
 * This file is part of the VectorBlox MXP Software Development Kit.
 *
 */


#include "vbx_copyright.h"
VBXCOPYRIGHT( test_vec_add )
#include <stdio.h>
#include "vbx.h"
#include "vbx_test.h"


/**
 * These test are not meant to be exhaustive, They are simply meant
 * To check if a board can do basic MXP functions
 */

int test_sp_alloc(){

	int errors=0;
	printf("testing scratchpad allocation ... ");fflush(stdout);
	vbx_sp_push();
	vbx_word_t* word=vbx_sp_malloc(16);
	vbx_sp_pop();
	errors+=word == NULL;
	printf("%s\n",(errors==0?"Passed":"Failed"));
	return errors;
}
#if ARM_LINUX
#include "xil_mmu.h"
int test_sp_access(){

	int errors=0;

	Xil_SetTlbAttributes(XPAR_VECTORBLOX_MXP_0_S_AXI_BASEADDR,DEVICE_MEMORY);
	printf("testing scratchpad write ... ");fflush(stdout);
	vbx_sp_push();
	volatile vbx_word_t* word=vbx_sp_malloc(16);
	word[0]=0xFABDAD;
	printf("testing read ... ");fflush(stdout);
	vbx_sp_pop();
	errors+=word[0] != 0xFABDAD;
	printf("%s\n",(errors==0?"Passed":"Failed"));
	return errors;
}
#else
int test_sp_access(){

	int errors=0;
	printf("testing scratchpad write ... ");fflush(stdout);
	vbx_sp_push();
	volatile vbx_word_t* word=vbx_sp_malloc(16);
	word[0]=0xFABDAD;
	printf("testing read ... ");fflush(stdout);
	vbx_sp_pop();
	errors+=word[0] != 0xFABDAD;
	printf("%s\n",(errors==0?"Passed":"Failed"));
	return errors;
}
#endif

int test_vbx_sync(){
	printf("testing vbx_sync ... ");fflush(stdout);
	vbx(SVW,VMUL,0,0,0);vbx_sync();
	//Passes if it doesn't hang
	printf("Passed\n");
	return 0;

}
int test_set_vl()
{
	int errors=0;
	printf("testing set_vl ... ");fflush(stdout);
	int vl,vl2d,vl3d;
	vbx_set_vl(3,4,5);
	printf ("testing get_vl ... ");fflush(stdout);
	vbx_get_vl(&vl,&vl2d,&vl3d);
	errors+= vl !=3;
	errors+= vl2d !=4;
	errors+= vl3d !=5;
	printf("%s\n",(errors==0?"Passed":"Failed"));
	return errors;
}
int main(void)
{
	printf("\n\nInitializing test ... ");fflush(stdout);
	vbx_test_init();
	printf("done\n");fflush(stdout);

	if(test_sp_alloc()){return 1;}
	if(test_sp_access()){return 1;}
	if(test_vbx_sync()){return 1;}
	if(test_set_vl()){return 1;}


	return 0;
}
