//
//  hbase64.c
//  C Library
//
//  Created by hailong zhang on 11-5-27.
//  Copyright 2011年 hailong.org. All rights reserved.
//

#include "hconfig.h"
#include "hbase64.h"

#undef hbase64_encode

#undef hbase64_decode

typedef struct{
    hbyte byte0;
    hbyte byte1;
    hbyte byte2;
}hbase64_token_t;

typedef union{
    hbase64_token_t token;
    hint32 value;
}hbase64_value_t;

static hchar hbase64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

#define IS_BASE_BIT(c)  (((c) >='A' && (c) <= 'Z') || ((c) >= 'a' && (c) <= 'z') || ((c) >= '0' && (c) <= '9') || (c) == '+' || (c) == '/')

#define HBASE64_WIDTH 19

hbool hbase64_encode(hbyte * data,hint32 length,hbuffer_t buffer,InvokeTickDeclare){
    if(data && length >0 && buffer){
        hbase64_value_t value ;
        hchar t[5] = {0};
        hint32 len = length / 3;
        hint32 d = length % 3;
        hint32 i,n;
        
        for(i=0;i<len;i++){
            value.value = 0;
#ifdef BITS_LOW
            value.token.byte0 = data[i *3 + 2];
            value.token.byte1 = data[i *3 + 1];
            value.token.byte2 = data[i *3 + 0];
#else
            value.token.byte0 = data[i *3 + 0];
            value.token.byte1 = data[i *3 + 1];
            value.token.byte2 = data[i *3 + 2];
#endif
            if(i && (i % HBASE64_WIDTH) ==0){
                buffer_append_str(buffer, "\n");
            }
            for(n=0;n<4;n++){
                t[3-n] = hbase64_table[(value.value & 0x003f)];
                value.value = value.value >> 6;
            }
            buffer_append(buffer, t, 4);
        }
        if(d){
            value.value = 0;
            if(d == 1){
#ifdef BITS_LOW
                value.token.byte2 = data[i * 3];
#else
                value.token.byte0 = data[i * 3];
#endif
                if(i && (i % HBASE64_WIDTH) ==0){
                    buffer_append_str(buffer, "\n");
                }
                for(n=0;n<4;n++){
                    t[3-n] = hbase64_table[(value.value & 0x003f)];
                    value.value = value.value >> 6;
                    if(n ==0 || n==1){
                        t[3-n] = '=';
                    }
                }
                buffer_append(buffer, t, 4);
            }
            else if(d == 2){
#ifdef BITS_LOW
                value.token.byte2 = data[i * 3];
                value.token.byte1 = data[i * 3 + 1];
#else
                value.token.byte0 = data[i * 3];
                value.token.byte1 = data[i * 3 + 1];
#endif
                if(i && (i % HBASE64_WIDTH) ==0){
                    buffer_append_str(buffer, "\n");
                }
                for(n=0;n<4;n++){
                    t[3-n] = hbase64_table[(value.value & 0x003f)];
                    value.value = value.value >> 6;
                    if(n ==0){
                        t[3-n] = '=';
                    }
                }
                buffer_append(buffer, t, 4);
            }
        }
        return hbool_true;
    }
    return hbool_false;
}

hbool hbase64_decode(hcchar * str,hbuffer_t buffer,InvokeTickDeclare){
    if(str && buffer){
        hchar *p = (hchar *)str;
        hint32 i;
        hbase64_value_t value;
        hint32 last = 0;
        
        while(*p != '\0'){
            if(*p == '\n' || *p == '\r' || *p == ' ' || *p =='\t'){
                p ++;
                continue;
            }
            
            if(p[0] && p[1] && p[2] && p[3]){
                value.value = 0;
                for(i=0;i<4;i++){
                    if( IS_BASE_BIT(*p)){
                        value.value = value.value << 6;
                        if(*p >= 'A' && *p<= 'Z'){
                            value.value = value.value | (*p -'A');
                        }
                        else if(*p >= 'a' && *p <='z'){
                            value.value = value.value | (*p - 'a' + 26);
                        }
                        else if(*p >= '0' && *p <= '9'){
                            value.value = value.value | (*p - '0' + 52);
                        }
                        else if(*p == '+'){
                            value.value = value.value | 62;
                        }
                        else if(*p == '/'){
                            value.value = value.value | 63;
                        }
                    }
                    else if(*p == '='){
                        value.value = value.value << 6;
                        last ++;
                    }
                    else{
                        return hbool_false;
                    } 
                    p ++;
                }
                
                if(last ==0){
#ifdef BITS_LOW
                    buffer_append(buffer,&value.token.byte2,1);
                    buffer_append(buffer,&value.token.byte1,1);
                    buffer_append(buffer,&value.token.byte0,1);
#else
                    buffer_append(buffer,&value.token.byte0,1);
                    buffer_append(buffer,&value.token.byte1,1);
                    buffer_append(buffer,&value.token.byte2,1);
#endif
                }
                else if(last ==1){
#ifdef BITS_LOW
                    buffer_append(buffer,&value.token.byte2,1);
                    buffer_append(buffer,&value.token.byte1,1);
#else
                    buffer_append(buffer,&value.token.byte0,1);
                    buffer_append(buffer,&value.token.byte1,1);
#endif
                }
                else if(last == 2){
#ifdef BITS_LOW
                    buffer_append(buffer,&value.token.byte2,1);
#else
                    buffer_append(buffer,&value.token.byte0,1);
#endif
                }
                else {
                    return hbool_false;
                }
            }
            else{
                return hbool_false;
            }
        }
        return hbool_true;
    }
    return hbool_false;
}

