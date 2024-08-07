#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>

#include "common.h"


void md5(const void *inbuf, const size_t inlen, void *outbuf){
    MD5_CTX ctx_md5;
    unsigned char hash[MD5_DIGEST_LENGTH] = {'\0'};
    if(inbuf && outbuf){
        MD5_Init(&ctx_md5);
        MD5_Update(&ctx_md5, inbuf, inlen);
        MD5_Final(hash, &ctx_md5);
        sprintf(outbuf,"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x", hash[0],
                                                                                         hash[1],
                                                                                         hash[2],
                                                                                         hash[3],
                                                                                         hash[4],
                                                                                         hash[5],
                                                                                         hash[6],
                                                                                         hash[7],
                                                                                         hash[8],
                                                                                         hash[9],
                                                                                         hash[10],
                                                                                         hash[11],
                                                                                         hash[12],
                                                                                         hash[13],
                                                                                         hash[14],
                                                                                         hash[15]);
    }
}