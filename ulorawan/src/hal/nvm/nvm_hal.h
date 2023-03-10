/**
 * \file
 *
 * \brief The HAL prototypes for abstracting the NVM storage
 *
 * Copyright (c) 2023 Derek Goslin
 *
 * @author Derek Goslin
 *
 * \page License
 *
 * MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#ifndef NVM_HAL_H_
#define NVM_HAL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

//! No error occurred.
#define NVM_HAL_ERR_NONE 0
//! An error occurred reading nvm
#define NVM_HAL_ERR_FAIL -1

/**
 * \brief
 *
 * \param nonce
 *
 * \return Operation status.
 */
int32_t nvm_hal_read_join_nonce(uint16_t *const nonce);

/**
 * \brief
 *
 * \param nonce
 *
 * \return Operation status.
 */
int32_t nvm_hal_write_join_nonce(uint16_t nonce);

#ifdef __cplusplus
}
#endif

#endif /* NVM_HAL_H_ */
