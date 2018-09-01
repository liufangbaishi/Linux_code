/*************************************************************************
	> File Name: encry.h
	> Author: 
	> Mail: 
	> Created Time: Sat 01 Sep 2018 12:12:47 AM CST
 ************************************************************************/

#ifndef _ENCRY_H__
#define _ENCRY_H__

static inline char* XOR(char* buf, size_t len) 
{
    for(size_t i=0; i<len; i++) {
        buf[i] ^= 1;
    }
}

static inline void Decrypt(char* buf, size_t len)
{
    XOR(buf, len);
}

static inline void Encry(char* buf, size_t len)
{
    XOR(buf, len);
}

#endif
