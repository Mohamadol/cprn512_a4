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
#include "vbx_port.h"
#include "limits.h"

/** Reverses a vector of elements allocated *in the scratchpad*.
 *  @pre v_src contains the elements to reverse.
 *  @pre v_dst and v_src must not overlap.
 *  @post v_dst will contain the elements of v_src in reverse order.
 *
 *  @param[out] v_dst *in scratch*.
 *  @param[in]  v_src *in scratch*.
 *  @param[in]  N is number of elements to reverse.
 *  @returns    negative on error condition. See vbw_exit_codes.h
 *
 *  @todo       handle overlapping input/output
 */
template<typename vbx_sp_t>
int vbw_vec_reverse( vbx_sp_t *v_dst, vbx_sp_t *v_src, const unsigned int N )
{
  /* Note this used to be very complicated and rely on using VROT instructions to try to speed up
     reversing bytes.  This has been replaced with a simple 2D loop with the expectation that if
     vector reverse speed is important you can implement a custom instruction to reverse the 
     bytes/halves/words and then do a 2D instruction one wavefront at a time */
  vbx_set_vl( 1, N );
  vbx_set_2D( (int)-sizeof(vbx_sp_t), (int)sizeof(vbx_sp_t), 0 );
  vbxx(VMOV, v_dst+N-1, v_src);
  return VBW_SUCCESS;
}
extern "C" int vbw_vec_reverse_word( vbx_word_t *v_dst, vbx_word_t *v_src, const unsigned int N )
{return vbw_vec_reverse(v_dst,v_src,N );}
extern "C" int vbw_vec_reverse_uword( vbx_uword_t *v_dst, vbx_uword_t *v_src, const unsigned int N )
{return vbw_vec_reverse(v_dst,v_src,N );}
extern "C" int vbw_vec_reverse_half( vbx_half_t *v_dst, vbx_half_t *v_src, const unsigned int N )
{return vbw_vec_reverse(v_dst,v_src,N );}
extern "C" int vbw_vec_reverse_uhalf( vbx_uhalf_t *v_dst, vbx_uhalf_t *v_src, const unsigned int N )
{return vbw_vec_reverse(v_dst,v_src,N );}
extern "C" int vbw_vec_reverse_byte( vbx_byte_t *v_dst, vbx_byte_t *v_src, const unsigned int N )
{return vbw_vec_reverse(v_dst,v_src,N );}
extern "C" int vbw_vec_reverse_ubyte( vbx_ubyte_t *v_dst, vbx_ubyte_t *v_src, const unsigned int N )
{return vbw_vec_reverse(v_dst,v_src,N );}
