//
//  hindex_io.c
//  hclib
//
//  Created by zhang hailong on 13-5-23.
//  Copyright (c) 2013年 hailong.org. All rights reserved.
//

#include "hconfig.h"
#include "hindex.h"


huint32 IndexLoadDataFromFile(Index * const index,hcchar * file,huint32 location,huint32 length){
    
    huint32 len = MIN(length, index->size);
    int fileno = open(file, O_RDONLY);
    long size = 0;
    huint32 version = 0;
    
    if(fileno == -1){
        return 0;
    }
    
    if(read(fileno, &version, sizeof(version)) != sizeof(version)){
        close(fileno);
        return 0;
    }
    
    if(index->clazz->version != version){
        close(fileno);
        return 0;
    }
    
    lseek(fileno, SEEK_CUR, location * index->clazz->size);
    
    size = read(fileno, index->data, len * index->clazz->size);
    
    close(fileno);
    
    if(size % index->clazz->size){
        return 0;
    }
    
    return (huint32)(size / index->clazz->size);
}

huint32 IndexSaveDataToFile(Index * const index,hcchar * file,huint32 length){
    
    huint32 len = MIN(length, index->size);
    int fileno = open(file, O_WRONLY | O_CREAT);
    long size = len * index->clazz->size;
    
    if(fileno == -1){
        return 0;
    }
    
    if(write(fileno, &index->clazz->version, sizeof(index->clazz->version)) != sizeof(index->clazz->version)){
        close(fileno);
        remove(file);
        return 0;
    }
    
    if(write(fileno, index->data, size) != size){
        close(fileno);
        remove(file);
        return 0;
    }
    
    close(fileno);
    
    return len;
}

huint32 IndexAppendDataToFile(Index * const index,hcchar * file,huint32 length){
    huint32 len = MIN(length, index->size);
    int fileno = open(file, O_RDONLY);
    huint32 version = 0;
    long size = len * index->clazz->size;
    
    if(fileno == -1){
        return 0;
    }
    
    if(read(fileno, &version, sizeof(version)) != sizeof(version)){
        close(fileno);
        return 0;
    }
    
    if(index->clazz->version != version){
        close(fileno);
        return 0;
    }
    
    close(fileno);
    
    fileno = open(file, O_WRONLY | O_APPEND);
    
    if(fileno == -1){
        return 0;
    }
    
    if(write(fileno, index->data, size) != size){
        close(fileno);
        return 0;
    }
    
    close(fileno);
    
    return len;
}


