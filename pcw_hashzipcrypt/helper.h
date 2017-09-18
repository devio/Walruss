/*
 * Copyright (c) 2017
 *    Thorsten Schroeder <ths at ccc dot de>
 *
 * All rights reserved.
 *
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Thorsten Schroeder <ths at modzero dot ch> wrote this file. As long as you 
 * retain this notice you can do whatever you want with this stuff. If we meet 
 * some day, and you think this stuff is worth it, you can buy me a beer in 
 * return. Thorsten Schroeder.
 *
 * NON-MILITARY-USAGE CLAUSE
 * Redistribution and use in source and binary form for military use and 
 * military research is not permitted. Infringement of these clauses may
 * result in publishing the source code of the utilizing applications and 
 * libraries to the public. As this software is developed, tested and
 * reviewed by *international* volunteers, this clause shall not be refused 
 * due to the matter of *national* security concerns.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE DDK PROJECT BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * File: helper.h
 * Description: Declaration & Documentation of helper functions.
 */

#ifndef _HELPER_H
#define _HELPER_H

#include <stdint.h>

/*!
 * print buffer as hex-dump. The input buffer is printed as hex-dump, with
 * an offset column first. The second column is the hexadecimal dump of the binary data.
 * Third column is the ascii representation of the input data.
 * Example:
 * 00000   47 45 54 20 2f 20 48 54  54 50 2f 31 2e 31 0d 0a   GET / HTTP/1.1..
 *
 * @param[in] s source buffer
 * @param[in] l source buffer length
 * @param[in] o offset
*/
void print_hex_ascii_line(const unsigned char *s, int l, int o);

/*!
 * print byte as hex. Prints input byte b as hex string.
 * @param[in]  b    byte
*/
void print_hex_byte(char b);

/*!
 * print 16bit word as hex. Prints input word w as hex string.
 * @param[in]  w word
*/
void print_hex_word(int16_t w);

/*!
 * print 32bit dword as hex. Prints input dword d as hex string.
 * @param[in]  d    dword
*/
void print_hex_dword(int32_t d);

/*!
 * print buffer as hex. Prints input byte buffer s as hex string.
 * @param[in] s source buffer
 * @param[in] l source buffer length
*/
void print_hex_buf(char *s, uint32_t l);

int decode_nibble(char n);

#endif