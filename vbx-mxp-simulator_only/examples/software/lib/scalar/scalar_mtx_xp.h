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
/**@{*/

#ifndef __SCALAR_MTX_XP_H
#define __SCALAR_MTX_XP_H

#include <stdio.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C"{
#endif
void scalar_mtx_xp_byte(int8_t *out, int8_t *in, const int32_t INROWS, const int32_t INCOLS);

void scalar_mtx_xp_half(int16_t *out, int16_t *in, const int32_t INROWS, const int32_t INCOLS);

void scalar_mtx_xp_word(int32_t *out, int32_t *in, const int32_t INROWS, const int32_t INCOLS);

void scalar_mtx_xp_ubyte(uint8_t *out, uint8_t *in, const int32_t INROWS, const int32_t INCOLS);

void scalar_mtx_xp_uhalf(uint16_t *out, uint16_t *in, const int32_t INROWS, const int32_t INCOLS);

void scalar_mtx_xp_uword(uint32_t *out, uint32_t *in, const int32_t INROWS, const int32_t INCOLS);
#ifdef __cplusplus
}
#endif
#endif // __SCALAR_MTX_XP_H
/**@}*/
