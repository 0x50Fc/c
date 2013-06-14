//
//  hfdb_index.c
//  vTeam
//
//  Created by zhang hailong on 13-6-14.
//  Copyright (c) 2013年 hailong.org. All rights reserved.
//

#include "hconfig.h"
#include "hfdb.h"
#include "hfdb_index.h"

void FDBIndexInitialize(FDBIndex * const index){
    FDBProperty * prop = & index->rowid;
    huint32 c = index->propertyCount;
    index->itemSize = 0;
    
    while(c > 0){
        
        assert(prop->type > FDBPropertyTypeNone && prop->type < FDBPropertyTypeMaxCount);
        
        switch (prop->type) {
            case FDBPropertyTypeInt32:
                prop->length = sizeof(hint32);
                break;
            case FDBPropertyTypeInt64:
                prop->length = sizeof(hint64);
                break;
            case FDBPropertyTypeDouble:
                prop->length = sizeof(hdouble);
                break;
            case FDBPropertyTypeString:
                if(prop->length == 0){
                    prop->length = 512;
                }
                break;
            default:
                assert(0);
                break;
        }
        
        prop->offset = index->itemSize;
        
        index->itemSize += prop->length;
        
        prop ++;
        c --;
    }

}

huint32 FDBIndexSize(FDBIndex * const index){
    return sizeof(FDBIndex) + (index->propertyCount - 1) * sizeof(FDBProperty);
}

void FDBIndexDataCreate(FDBIndexData * const data,FDBIndex * index){
    memset(data, 0, sizeof(FDBIndexData));
    data->index = index;
}

void FDBIndexDataDelete(FDBIndexData * const data){
    if(data->data){
        free(data->data);
    }
    memset(data, 0, sizeof(FDBIndexData));
}

huint32 FDBIndexDataExpandSize(FDBIndexData * const data,huint32 size){
    if(data->data){
        if(data->size < size){
            data->data = realloc(data->data, size * data->index->itemSize);
            data->size = size;
        }
    }
    else{
        data->data = malloc(size * data->index->itemSize);
        data->size = size;
    }
    return data->size;
}

void FDBIndexDataClear(FDBIndexData * const data){
    if(data->data){
        memset(data->data, 0, data->index->itemSize * data->size);
    }
    data->length = 0;
}

FDBDataItem FDBIndexDataAt(FDBIndexData * const data,huint32 index){
    if(data && index < data->size){
        return (hbyte *)data->data + index * data->index->itemSize;
    }
    return NULL;
}

FDBDataItem FDBIndexDataInsert(FDBIndexData * const data){
    
    FDBDataItem dataItem = NULL;

    FDBIndexDataExpandSize(data, ((data->length + 1) / FDB_INDEX_EXPAND_SIZE + 1) * FDB_INDEX_EXPAND_SIZE);

    dataItem = (hbyte *)data->data + data->length * data->index->itemSize;
    data->length ++;

    return dataItem;
}

static void FDBIndexSortSwap(FDBIndex * const index,FDBDataItem item1,FDBDataItem item2,hany tDataItem){
    memcpy(tDataItem, item1, index->itemSize);
    memcpy(item1, item2, index->itemSize);
    memcpy(item2, tDataItem, index->itemSize);
}

static void FDBIndexSortScan(FDBIndexData * const data,FDBIndexDataItemCompare compare,huint32 bIndex,huint32 eIndex,hany tDataItem,hany context){
    
    hint32 b = bIndex,e = eIndex;
    FDBDataItem dataItem = FDBIndexDataAt(data, (bIndex + eIndex) / 2);
    FDBDataItem item,titem;
    FDBIndexCompareOrder rs;
    
    
    while (b < e) {
        
        item = FDBIndexDataAt(data, b);
        
        rs = (* compare)(data->index,item,dataItem,context);
        
        if(rs == FDBIndexCompareOrderDesc){
            
            while(b < e){
                titem = FDBIndexDataAt(data, e);
                rs = (* compare)(data->index,titem,dataItem,context);
                if(rs != FDBIndexCompareOrderDesc){
                    break;
                }
                e --;
            }
            
            if(b == e){
                break;
            }
            
            FDBIndexSortSwap(data->index,item,titem,tDataItem);
            
            if(item == dataItem){
                dataItem = titem;
            }
            else if(dataItem == titem){
                dataItem = item;
            }
            
            e --;
            continue;
        }
        
        b ++;
    }
    
    assert(b == e);
    
    while(b < eIndex){
        
        item = FDBIndexDataAt(data, b);
        
        rs = (* compare)(data->index,item,dataItem,context);
        
        if(rs == FDBIndexCompareOrderDesc){
            break;
        }
        
        b ++;
    }
    
    if(b == eIndex){
        
        item = FDBIndexDataAt(data, b);
        
        rs = (* compare)(data->index,item,dataItem,context);
        
        if(rs != FDBIndexCompareOrderDesc){
            FDBIndexSortSwap(data->index,item,dataItem,tDataItem);
        }
        
        if(bIndex < b -1 ){
            FDBIndexSortScan(data,compare,bIndex,b -1,tDataItem,context);
        }
    }
    else{
        
        if(bIndex < b -1 ){
            FDBIndexSortScan(data,compare,bIndex,b -1,tDataItem,context);
        }
        
        if(b < eIndex && bIndex != b){
            FDBIndexSortScan(data,compare,b,eIndex,tDataItem,context);
        }
    }
}



huint32 FDBIndexSort(FDBIndexData * const data,FDBIndexDataItemCompare compare,hany context){
    
    hany tDataItem;
    
    if(data->length > 1){
        tDataItem = malloc(data->index->itemSize);
        FDBIndexSortScan(data,compare,0,data->length -1,tDataItem,context);
        free(tDataItem);
    }
    
    
    return data->length;
}

typedef struct _FDBIndexSortPropertyCompareContext {
    FDBIndexSortProperty * propertys;
    huint32 length;
} FDBIndexSortPropertyCompareContext;

static FDBIndexCompareOrder FDBIndexSortPropertyCompare (FDBIndex * index,FDBDataItem dataItem1,FDBDataItem dataItem2,hany context){
    
    FDBIndexSortPropertyCompareContext * ctx = (FDBIndexSortPropertyCompareContext *) context;
    FDBIndexCompareOrder o = FDBIndexCompareOrderSame;
    huint32 c = ctx->length;
    FDBIndexSortProperty * prop = ctx->propertys;
    
    while(o == FDBIndexCompareOrderSame && c >0){
       
        switch (prop->property->type) {
            case FDBPropertyTypeInt32:
            case FDBPropertyTypeInt64:
            {
                hint64 rs = FDBClassGetPropertyInt64Value(dataItem1, prop->property, 0) - FDBClassGetPropertyInt64Value(dataItem2, prop->property, 0);
                if(rs == 0){
                    o = FDBIndexCompareOrderSame;
                }
                else if(rs <0){
                    o = prop->mode;
                }
                else{
                    o = - prop->mode;
                }
            }
                break;
            case FDBPropertyTypeDouble:
            {
                hdouble rs = FDBClassGetPropertyDoubleValue(dataItem1, prop->property, 0) - FDBClassGetPropertyDoubleValue(dataItem2, prop->property, 0);
                if(rs == 0){
                    o = FDBIndexCompareOrderSame;
                }
                else if(rs <0){
                    o = prop->mode;
                }
                else{
                    o = - prop->mode;
                }
            }
                break;
            case FDBPropertyTypeString:
            {
                hint32 rs = strcmp(FDBClassGetPropertyStringValue(dataItem1, prop->property, ""), FDBClassGetPropertyStringValue(dataItem1, prop->property, ""));
                if(rs == 0){
                    o = FDBIndexCompareOrderSame;
                }
                else if(rs <0){
                    o = prop->mode;
                }
                else{
                    o = - prop->mode;
                }
            }
                break;
            default:
                break;
        }

        c --;
        prop ++;
    }
  
    return o;
}

huint32 FDBIndexSortPropertys(FDBIndexData * const data, FDBIndexSortProperty * propertys,huint32 length){
    FDBIndexSortPropertyCompareContext ctx = {propertys,length};
    return FDBIndexSort(data,FDBIndexSortPropertyCompare,&ctx);
}

typedef struct _FDBIndexDBInternal {
    FDBIndexDB base;
    hchar idxPath[PATH_MAX];
} FDBIndexDBInternal;

typedef struct _FDBIndexHead {
    hchar tag[4];
    huint32 version;
    huint32 indexSize;
} FDBIndexHead;

FDBIndexDB * FDBIndexOpen(hcchar * dbPath,hcchar * name){
    
    if(dbPath){
        
        FDBIndexDBInternal * idx = malloc(sizeof(FDBIndexDBInternal));
        int fno;
        FDBIndexHead head;
        
        memset(idx, 0, sizeof(FDBIndexDBInternal));
        
        snprintf(idx->idxPath, sizeof(idx->idxPath),"%s_%s%s",dbPath,name,FDB_FILE_INDEX);
        
        fno = open(idx->idxPath, O_RDONLY);
        
        if(fno == -1){
            free(idx);
            return NULL;
        }
        
        if(sizeof(head) != read(fno, &head, sizeof(head))){
            free(idx);
            close(fno);
            return NULL;
        }
        
        if(strncmp(head.tag, FDB_TAG, sizeof(head.tag))){
            free(idx);
            close(fno);
            return NULL;
        }
        
        idx->base.version = head.version;
        
        idx->base.index = malloc(head.indexSize);
        
        if(head.indexSize != read(fno, idx->base.index, head.indexSize)){
            free(idx->base.index);
            free(idx);
            return NULL;
        }
        
        close(fno);
        
        return (FDBIndexDB *) idx;
    }
    return NULL;
}


huint32 FDBIndexRowCount(FDBIndexDB * dbIndex){
    FDBIndexDBInternal * idx = (FDBIndexDBInternal *) dbIndex;
    struct stat s;
    
    if(stat(idx->idxPath, &s) == -1){
        return 0;
    }
    
    return ( ((huint32)s.st_size - (huint32)sizeof(FDBIndexHead) - (huint32)FDBIndexSize(idx->base.index)) / idx->base.index->itemSize);
}

void FDBIndexClose(FDBIndexDB * dbIndex){
    FDBIndexDBInternal * idx = (FDBIndexDBInternal *) dbIndex;
    free(idx->base.index);
    free(idx);
}

hint32 FDBIndexWrite(hcchar * dbPath,hcchar * name,FDBIndexData * const indexData){
    
    if(dbPath){
        
        int fno;
        FDBIndexHead head = {FDB_INDEX_TAG,FDB_INDEX_VERSION,FDBIndexSize(indexData->index)};
        hchar path[PATH_MAX];
        
        snprintf(path, sizeof(path),"%s_%s%s",dbPath,name,FDB_FILE_INDEX);
        
        fno = open(path, O_WRONLY | O_CREAT | O_TRUNC);
        
        if(fno == -1){
            return FDB_ERROR;
        }
        
        fchmod(fno, S_IRUSR | S_IWUSR);

        if(sizeof(head) != write(fno, &head, sizeof(head))){
            close(fno);
            return FDB_ERROR;
        }
        
        if(head.indexSize != write(fno, &indexData->index, head.indexSize)){
            close(fno);
            return FDB_ERROR;
        }
        
        if(indexData->length){
            if(indexData->length * indexData->index->itemSize != write(fno, indexData->data, indexData->length * indexData->index->itemSize)){
                close(fno);
                return FDB_ERROR;
            }
        }
    
        close(fno);
        
        return FDB_OK;
    }
    
    return FDB_ERROR;
}

