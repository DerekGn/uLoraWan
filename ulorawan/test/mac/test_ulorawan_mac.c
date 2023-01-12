/**
 * \file
 *
 * \brief 
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

#include "unity.h"
#include "ulorawan_mac.h"
#include "ulorawan_mac_frame.h"

void test_ulorawan_mac_write_mhdr_index_error()
{
    // Arrange
    struct ulorawan_mac_frame_context context;
    context.eof = 10;

    union ulorawan_mac_mhdr mhdr;

    // Act
    int32_t result = ulorawan_mac_write_mhdr(&context, &mhdr);
    
    // Assert
    TEST_ASSERT_EQUAL_HEX8(ULORAWAN_MAC_ERR_INDEX, result);
}

void test_ulorawan_mac_write_mhdr_success()
{
    // Arrange
    struct ulorawan_mac_frame_context context;
    context.eof = 0;
    
    union ulorawan_mac_mhdr mhdr;
    mhdr.value = 0x55;

    // Act
    int32_t result = ulorawan_mac_write_mhdr(&context, &mhdr);
    
    // Assert
    uint8_t expected[] = {0x55};

    TEST_ASSERT_EQUAL_HEX8(ULORAWAN_MAC_ERR_NONE, result);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected, context.buf, sizeof(union ulorawan_mac_mhdr));
}


void test_ulorawan_mac_write_fhdr_index_error()
{
    // Arrange
    struct ulorawan_mac_frame_context context;
    context.eof = 10;

    struct ulorawan_mac_fhdr fhdr;

    // Act
    int32_t result = ulorawan_mac_write_fhdr(&context, &fhdr);
    
    // Assert
    TEST_ASSERT_EQUAL_HEX8(ULORAWAN_MAC_ERR_INDEX, result);
}