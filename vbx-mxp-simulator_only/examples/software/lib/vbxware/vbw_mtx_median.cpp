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

/**@file*/
#include "vbx.h"
#include "vbw_exit_codes.h"
/** VBX median filter using bubble sort.
 *  Does essentially the same thing as the scalar median, but on a vbx worth of pixels at a time.
 *  @pre  minimum free scratchpad space is the greater of 6 vbx widths or 2 vbx widths + filter size.
 *  @pre  output and input must not overlap.
 *  @pre  filter_height <= image_height.
 *  @pre  filter_width <= image_width.
 *  @post output will contain the median values taken from input with a sliding window filter.
 *
 *  @param[out] output *in memory*.
 *  @param[in]  input *in memory*.
 *  @param[in]  filter_height Number of rows in filter
 *  @param[in]  filter_width Number of columns in filter
 *  @param[in]  image_height Number of rows in input image
 *  @param[in]  image_width  Number of columns in input image
 *  @param[in]  image_pitch  Number of pixels between start of rows. If there is no padding, this should be same as image_width
 *  @returns    negative on error condition. See vbw_exit_codes.h
 *
 *  @todo       add an internal version
 */
template<typename vbx_mm_t>
int vbw_mtx_median_ext( vbx_mm_t *output, vbx_mm_t *input, const int filter_height, const int filter_width,
                           const int image_height, const int image_width, const int image_pitch )
{
	typedef vbx_mm_t vbx_sp_t;

	const int FREE_BYTES = vbx_sp_getfree();
	int l,k;
	int filter_mid, filter_size;
	int rows_per_l,vl,temp_vl;
	int j,i;
	int partial_row = 0;

	filter_size = filter_height*filter_width;
	filter_mid = filter_size/2;

	vbx_mxp_t *this_mxp = VBX_GET_THIS_MXP();
	const int VBX_WIDTH_BYTES = this_mxp->scratchpad_alignment_bytes;

	// Could possibly check for low SP here (less than 6*VBX_WIDTH_BYTES) and assign vl differently

	// During allocation, max additional SP bytes needed due to alignment is one VBX_WIDTH_BYTES per vector
	// Taking that off the top simplifies calculation and will always be correct, but sacrifices a little SP space
	vl = (FREE_BYTES-3*VBX_WIDTH_BYTES)/((filter_size+2)*sizeof(vbx_sp_t));


	if(vl < image_width){
		rows_per_l = 1;
		partial_row = 1;
	} else {
		rows_per_l = vl/image_width;
		vl = image_width*rows_per_l;
	}

	vbx_sp_push();

	vbx_sp_t *v_input = (vbx_sp_t *)vbx_sp_malloc(filter_size*vl*sizeof(vbx_sp_t));
	vbx_sp_t *v_sub   = (vbx_sp_t *)vbx_sp_malloc(vl*sizeof(vbx_sp_t));
	vbx_sp_t *v_temp  = (vbx_sp_t *)vbx_sp_malloc(vl*sizeof(vbx_sp_t));
	vbx_sp_t *v_min,*v_max;
	if(v_temp == NULL){
		vbx_sp_pop();
		return VBW_ERROR_SP_ALLOC_FAILED;
	}
	for(l = 0; l < image_height-filter_height; l+= rows_per_l){
		// detect last pass
		if(l+rows_per_l > image_height-filter_height){
			rows_per_l = (image_height-filter_height)-l;
			vl = image_width*rows_per_l;
		}
		temp_vl = vl;
		for(k = 0; k < image_width; k += temp_vl){
			if(partial_row){
				if(k + temp_vl > image_width){
					temp_vl = image_width - k;
				}
			}

			for(j = 0; j < filter_height; j++){
				vbx_dma_to_vector_2D(v_input+temp_vl*j,
									 input+(l+j)*image_pitch+k,
									 temp_vl/rows_per_l*sizeof(vbx_mm_t),
									 rows_per_l,
									 image_width*sizeof(vbx_mm_t),
									 image_pitch*sizeof(vbx_mm_t));
			}
			vbx_set_vl(temp_vl);

			for(j = 1; j < filter_height; j++){
				for(i = 0; i < filter_width; i++){
					vbxx(VMOV,v_input+(j*filter_height+i)*temp_vl,
								  v_input+i*temp_vl+j);
				}
			}

			//Do the bubble sort up to the filter_size/2^th element on each vbx
			for(j = 0; j < filter_mid; j++){
				v_min = v_input+j*temp_vl;

				for(i = j+1; i < filter_size; i++){
					v_max = v_input+i*temp_vl;

					vbxx(VMOV,v_temp,v_min);
					vbxx(VSUB,v_sub,v_max,v_min);
					vbxx(VCMV_LTZ,v_min,v_max,v_sub);
					vbxx(VCMV_LTZ,v_max,v_temp,v_sub);
				}
			}

			// grab next smallest value, the median, don't sort the rest
			v_min = v_input+filter_mid*temp_vl;
			for(i = filter_mid+1; i < filter_size; i++){
				v_max = v_input+i*temp_vl;
				vbxx(VSUB,v_sub,v_max,v_min);
				vbxx(VCMV_LTZ,v_min,v_max,v_sub);
			}

			// dma out median value
			// back to pixels
			vbx_dma_to_host_2D(output+(l*image_pitch)+k,
			                   v_input+temp_vl*filter_mid,
			                   (temp_vl/rows_per_l-filter_width)*sizeof(vbx_mm_t),
			                   rows_per_l,
			                   image_pitch*sizeof(vbx_mm_t),
			                   image_width*sizeof(vbx_mm_t));
		}
	}
	vbx_sp_pop();
	vbx_sync();
	return VBW_SUCCESS;
}

extern "C"
int vbw_mtx_median_ext_word( vbx_word_t *output, vbx_word_t *input, const int filter_height, const int filter_width,
                             const int image_height, const int image_width, const int image_pitch )
{
	return vbw_mtx_median_ext<vbx_word_t>(output,input,filter_height,filter_width,image_height,image_width,image_pitch);
}
extern "C"
int vbw_mtx_median_ext_uword( vbx_uword_t *output, vbx_uword_t *input, const int filter_height, const int filter_width,
                              const int image_height, const int image_width, const int image_pitch )
{
	return vbw_mtx_median_ext<vbx_uword_t>(output,input,filter_height,filter_width,image_height,image_width,image_pitch);
}
extern "C"
int vbw_mtx_median_ext_half( vbx_half_t *output, vbx_half_t *input, const int filter_height, const int filter_width,
                             const int image_height, const int image_width, const int image_pitch )
{
	return vbw_mtx_median_ext<vbx_half_t>(output,input,filter_height,filter_width,image_height,image_width,image_pitch);
}
extern "C"
int vbw_mtx_median_ext_uhalf( vbx_uhalf_t *output, vbx_uhalf_t *input, const int filter_height, const int filter_width,
                              const int image_height, const int image_width, const int image_pitch )
{
	return vbw_mtx_median_ext<vbx_uhalf_t>(output,input,filter_height,filter_width,image_height,image_width,image_pitch);
}
extern "C"
int vbw_mtx_median_ext_byte( vbx_byte_t *output, vbx_byte_t *input, const int filter_height, const int filter_width,
                             const int image_height, const int image_width, const int image_pitch )
{
	return vbw_mtx_median_ext<vbx_byte_t>(output,input,filter_height,filter_width,image_height,image_width,image_pitch);
}
extern "C"
int vbw_mtx_median_ext_ubyte( vbx_ubyte_t *output, vbx_ubyte_t *input, const int filter_height, const int filter_width,
                              const int image_height, const int image_width, const int image_pitch )
{
	return vbw_mtx_median_ext<vbx_ubyte_t>(output,input,filter_height,filter_width,image_height,image_width,image_pitch);
}
