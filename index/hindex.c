//
//  hindex.c
//  hclib
//
//  Created by zhang hailong on 13-5-23.
//  Copyright (c) 2013年 hailong.org. All rights reserved.
//

#define READONLY

#include "hconfig.h"
#include "hindex.h"


void IndexClassInitialize(IndexClass * const clazz){
    if(!clazz->initialized){
        IndexProperty * prop = clazz->propertys;
        huint32 c = clazz->propertyCount;
        clazz->size = 0;
        while(c >0){
            prop->offset = clazz->size;
            clazz->size += prop->size;
            prop ++;
            c --;
        }
        clazz->initialized = hbool_true;
    }
}

void IndexCreate(Index * const index,const IndexClass * clazz){
    memset(index, 0, sizeof(Index));
    index->clazz = clazz;
}

void IndexDelete(Index * const index){
    if(index->data){
        free(index->data);
    }
    memset(index, 0, sizeof(Index));
}

void IndexSetDataSize(Index * const index,huint32 size){
    if(index->data){
        index->data = realloc(index->data, size * index->clazz->size);
    }
    else {
        index->data = malloc(size * index->clazz->size);
    }
    index->size = size;
}

IndexDataItem IndexDataItemAt(Index * const index,huint32 i){
    if(i < index->size){
        return (char *) index->data + index->clazz->size * i;
    }
    return NULL;
}

hany IndexDataItemValue(IndexDataItem dataItem,const IndexProperty * property){
    if(dataItem && property){
        return ((hbyte *) dataItem + property->offset);
    }
    return NULL;
}

hfloat IndexDataItemFloatValue(IndexDataItem dataItem,const IndexProperty * property,hfloat defaultValue){
    if(dataItem && property){
        switch (property->type) {
            case IndexPropertyTypeInt32:
                return * (hint32 *) ((hbyte *) dataItem + property->offset);
                break;
            case IndexPropertyTypeInt64:
                return * (hint64 *) ((hbyte *) dataItem + property->offset);
                break;
            case IndexPropertyTypeFloat:
                return * (hfloat *) ((hbyte *) dataItem + property->offset);
                break;
            case IndexPropertyTypeDouble:
                return * (hdouble *) ((hbyte *) dataItem + property->offset);
                break;
            case IndexPropertyTypeString:
                return atof((hcchar *) ((hbyte *) dataItem + property->offset));
                break;
            default:
                break;
        }
    }
    return defaultValue;
}

hdouble IndexDataItemDoubleValue(IndexDataItem dataItem,const IndexProperty * property,hdouble defaultValue){
    if(dataItem && property){
        switch (property->type) {
            case IndexPropertyTypeInt32:
                return * (hint32 *) ((hbyte *) dataItem + property->offset);
                break;
            case IndexPropertyTypeInt64:
                return * (hint64 *) ((hbyte *) dataItem + property->offset);
                break;
            case IndexPropertyTypeFloat:
                return * (hfloat *) ((hbyte *) dataItem + property->offset);
                break;
            case IndexPropertyTypeDouble:
                return * (hdouble *) ((hbyte *) dataItem + property->offset);
                break;
            case IndexPropertyTypeString:
                return atof((hcchar *) ((hbyte *) dataItem + property->offset));
                break;
            default:
                break;
        }
    }
    return defaultValue;
}

hint32 IndexDataItemIntValue(IndexDataItem dataItem,const IndexProperty * property,hint32 defaultValue){
    if(dataItem && property){
        switch (property->type) {
            case IndexPropertyTypeInt32:
                return * (hint32 *) ((hbyte *) dataItem + property->offset);
                break;
            case IndexPropertyTypeInt64:
                return (hint32) (* (hint64 *) ((hbyte *) dataItem + property->offset));
                break;
            case IndexPropertyTypeFloat:
                return * (hfloat *) ((hbyte *) dataItem + property->offset);
                break;
            case IndexPropertyTypeDouble:
                return * (hdouble *) ((hbyte *) dataItem + property->offset);
                break;
            case IndexPropertyTypeString:
                return atoi((hcchar *) ((hbyte *) dataItem + property->offset));
                break;
            default:
                break;
        }
    }
    return defaultValue;
}

hint64 IndexDataItemInt64Value(IndexDataItem dataItem,const IndexProperty * property,hint64 defaultValue){
    if(dataItem && property){
        switch (property->type) {
            case IndexPropertyTypeInt32:
                return * (hint32 *) ((hbyte *) dataItem + property->offset);
                break;
            case IndexPropertyTypeInt64:
                return * (hint64 *) ((hbyte *) dataItem + property->offset);
                break;
            case IndexPropertyTypeFloat:
                return * (hfloat *) ((hbyte *) dataItem + property->offset);
                break;
            case IndexPropertyTypeDouble:
                return * (hdouble *) ((hbyte *) dataItem + property->offset);
                break;
            case IndexPropertyTypeString:
                return atoll((hcchar *) ((hbyte *) dataItem + property->offset));
                break;
            default:
                break;
        }
    }
    return defaultValue;
}

hcchar * IndexDataItemStringValue(IndexDataItem dataItem,const IndexProperty * property,hcchar * defaultValue){
    if(dataItem && property){
        switch (property->type) {
            case IndexPropertyTypeString:
                return (hcchar *) ((hbyte *) dataItem + property->offset);
                break;
            default:
                break;
        }
    }
    return defaultValue;
}

void IndexDataItemSetValue(IndexDataItem dataItem,const IndexProperty * property,hany value){
    if(dataItem && property && value){
        memcpy(((hbyte *) dataItem + property->offset), value, property->size);
    }
}

void IndexDataItemSetFloatValue(IndexDataItem dataItem,const IndexProperty * property,hfloat value){
    if(dataItem && property){
        switch (property->type) {
            case IndexPropertyTypeInt32:
                * (hint32 *) ((hbyte *) dataItem + property->offset) = (hint32) value;
                break;
            case IndexPropertyTypeInt64:
                * (hint64 *) ((hbyte *) dataItem + property->offset) = (hint64) value;
                break;
            case IndexPropertyTypeFloat:
                * (hfloat *) ((hbyte *) dataItem + property->offset) = (hfloat) value;
                break;
            case IndexPropertyTypeDouble:
                * (hdouble *) ((hbyte *) dataItem + property->offset) = (hdouble) value;
                break;
            case IndexPropertyTypeString:
                snprintf((hchar *) ((hbyte *) dataItem + property->offset), property->size,"%f",value);
                break;
            default:
                break;
        }
    }
}

void IndexDataItemSetDoubleValue(IndexDataItem dataItem,const IndexProperty * property,hdouble value){
    if(dataItem && property){
        switch (property->type) {
            case IndexPropertyTypeInt32:
                * (hint32 *) ((hbyte *) dataItem + property->offset) = (hint32) value;
                break;
            case IndexPropertyTypeInt64:
                * (hint64 *) ((hbyte *) dataItem + property->offset) = (hint64) value;
                break;
            case IndexPropertyTypeFloat:
                * (hfloat *) ((hbyte *) dataItem + property->offset) = (hfloat) value;
                break;
            case IndexPropertyTypeDouble:
                * (hdouble *) ((hbyte *) dataItem + property->offset) = (hdouble) value;
                break;
            case IndexPropertyTypeString:
                snprintf((hchar *) ((hbyte *) dataItem + property->offset), property->size,"%lf",value);
                break;
            default:
                break;
        }
    }
}

void IndexDataItemSetIntValue(IndexDataItem dataItem,const IndexProperty * property,hint32 value){
    if(dataItem && property){
        switch (property->type) {
            case IndexPropertyTypeInt32:
                * (hint32 *) ((hbyte *) dataItem + property->offset) = (hint32) value;
                break;
            case IndexPropertyTypeInt64:
                * (hint64 *) ((hbyte *) dataItem + property->offset) = (hint64) value;
                break;
            case IndexPropertyTypeFloat:
                * (hfloat *) ((hbyte *) dataItem + property->offset) = (hfloat) value;
                break;
            case IndexPropertyTypeDouble:
                * (hdouble *) ((hbyte *) dataItem + property->offset) = (hdouble) value;
                break;
            case IndexPropertyTypeString:
                snprintf((hchar *) ((hbyte *) dataItem + property->offset), property->size,"%d",value);
                break;
            default:
                break;
        }
    }
}

void IndexDataItemSetInt64Value(IndexDataItem dataItem,const IndexProperty * property,hint64 value){
    if(dataItem && property){
        switch (property->type) {
            case IndexPropertyTypeInt32:
                * (hint32 *) ((hbyte *) dataItem + property->offset) = (hint32) value;
                break;
            case IndexPropertyTypeInt64:
                * (hint64 *) ((hbyte *) dataItem + property->offset) = (hint64) value;
                break;
            case IndexPropertyTypeFloat:
                * (hfloat *) ((hbyte *) dataItem + property->offset) = (hfloat) value;
                break;
            case IndexPropertyTypeDouble:
                * (hdouble *) ((hbyte *) dataItem + property->offset) = (hdouble) value;
                break;
            case IndexPropertyTypeString:
                snprintf((hchar *) ((hbyte *) dataItem + property->offset), property->size,"%lld",value);
                break;
            default:
                break;
        }
    }
}

void IndexDataItemSetStringValue(IndexDataItem dataItem,const IndexProperty * property,hcchar * value){
    if(dataItem && property){
        switch (property->type) {
            case IndexPropertyTypeInt32:
                * (hint32 *) ((hbyte *) dataItem + property->offset) = value ? atoi(value) : 0;
                break;
            case IndexPropertyTypeInt64:
                * (hint64 *) ((hbyte *) dataItem + property->offset) = value ? atoll(value) : 0;
                break;
            case IndexPropertyTypeFloat:
                * (hfloat *) ((hbyte *) dataItem + property->offset) = value ? atof(value) : 0.0f;
                break;
            case IndexPropertyTypeDouble:
                * (hdouble *) ((hbyte *) dataItem + property->offset) = value ? atof(value) : 0.0;
                break;
            case IndexPropertyTypeString:
                strncpy((hchar *) ((hbyte *) dataItem + property->offset), value ? value : "", property->size);
                break;
            default:
                break;
        }
    }
}

static void IndexSortSwap(Index * const index,IndexDataItem item1,IndexDataItem item2,hany tDataItem){
    memcpy(tDataItem, item1, index->clazz->size);
    memcpy(item1, item2, index->clazz->size);
    memcpy(item2, tDataItem, index->clazz->size);
}

static void IndexSortScan(Index * const index,IndexDataItemCompare compare,huint32 bIndex,huint32 eIndex,hany tDataItem,hany context){
    
    hint32 b = bIndex,e = eIndex;
    IndexDataItem dataItem = IndexDataItemAt(index, (bIndex + eIndex) / 2);
    IndexDataItem item,titem;
    IndexCompareResult rs;
    
    
    while (b < e) {
    
        item = IndexDataItemAt(index, b);
        
        rs = (* compare)(index,item,dataItem,context);
        
        if(rs == IndexCompareResultDesc){
            
            while(b < e){
                titem = IndexDataItemAt(index, e);
                rs = (* compare)(index,titem,dataItem,context);
                if(rs != IndexCompareResultDesc){
                    break;
                }
                e --;
            }
            
            if(b == e){
                break;
            }
            
            IndexSortSwap(index,item,titem,tDataItem);
            
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
        
        item = IndexDataItemAt(index, b);
        
        rs = (* compare)(index,item,dataItem,context);
        
        if(rs == IndexCompareResultDesc){
            break;
        }
        
        b ++;
    }
    
    if(b == eIndex){
       
        item = IndexDataItemAt(index, b);
        
        rs = (* compare)(index,item,dataItem,context);
        
        if(rs != IndexCompareResultDesc){
            IndexSortSwap(index,item,dataItem,tDataItem);
        }
        
        if(bIndex < b -1 ){
            IndexSortScan(index,compare,bIndex,b -1,tDataItem,context);
        }
    }
    else{
    
        if(bIndex < b -1 ){
            IndexSortScan(index,compare,bIndex,b -1,tDataItem,context);
        }
        
        if(b < eIndex && bIndex != b){
            IndexSortScan(index,compare,b,eIndex,tDataItem,context);
        }
    }
}


huint32 IndexSort(Index * const index,IndexDataItemCompare compare,huint32 length,hany context){
    
    huint32 len = MIN(index->size,length);
    hany tDataItem;
    
    if(len > 1){
        tDataItem = malloc(index->clazz->size);
        IndexSortScan(index,compare,0,len -1,tDataItem,context);
        free(tDataItem);
    }


    return len;
    
}

typedef struct _IndexSortPropertyCompareContext {
    const IndexProperty * property;
    IndexCompareResult mode;
} IndexSortPropertyCompareContext;

static IndexCompareResult IndexSortPropertyCompare (Index * const index,IndexDataItem dataItem1,IndexDataItem dataItem2,hany context){
    IndexSortPropertyCompareContext * ctx = (IndexSortPropertyCompareContext *) context;
    switch (ctx->property->type) {
        case IndexPropertyTypeInt32:
        case IndexPropertyTypeInt64:
        {
            hint64 rs = IndexDataItemInt64Value(dataItem1, ctx->property, 0) - IndexDataItemInt64Value(dataItem2, ctx->property, 0);
            if(rs == 0){
                return IndexCompareResultSame;
            }
            else if(rs <0){
                return ctx->mode;
            }
            else{
                return - ctx->mode;
            }
        }
            break;
        case IndexPropertyTypeFloat:
        case IndexPropertyTypeDouble:
        {
            hdouble rs = IndexDataItemDoubleValue(dataItem1, ctx->property, 0) - IndexDataItemDoubleValue(dataItem2, ctx->property, 0);
            if(rs == 0){
                return IndexCompareResultSame;
            }
            else if(rs <0){
                return ctx->mode;
            }
            else{
                return - ctx->mode;
            }
        }
            break;
        case IndexPropertyTypeString:
        {
            hint32 rs = strcmp(IndexDataItemStringValue(dataItem1, ctx->property, ""), IndexDataItemStringValue(dataItem1, ctx->property, ""));
            if(rs == 0){
                return IndexCompareResultSame;
            }
            else if(rs <0){
                return ctx->mode;
            }
            else{
                return - ctx->mode;
            }
        }
            break;
        default:
            break;
    }
    return IndexCompareResultSame;
}

huint32 IndexSortProperty(Index * const index,const IndexProperty * property,IndexCompareResult mode,huint32 length){
    IndexSortPropertyCompareContext ctx = {property,mode};
    return IndexSort(index,IndexSortPropertyCompare,length,&ctx);
}

void IndexPrint(Index * const index,huint32 location,huint32 length){
    huint32 len = MIN(index->size,location + length);
    IndexProperty * prop = index->clazz->propertys;
    huint32 c = index->clazz->propertyCount;
    IndexDataItem dataItem;
    
    printf("\n");
    
    printf("\t");
    
    while(c >0){
        
        printf("%s\t",prop->name);
        
        prop ++;
        c --;
    }
    
    printf("\n");
    
    while(location < len){
        
        dataItem =  IndexDataItemAt(index, location);
        
        printf("%d\t",location);
        
        prop = index->clazz->propertys;
        c = index->clazz->propertyCount;

        while(c >0){
            
            switch (prop->type) {
                case IndexPropertyTypeInt32:
                    printf("%d\t",IndexDataItemIntValue(dataItem, prop, 0));
                    break;
                case IndexPropertyTypeInt64:
                    printf("%lld\t",IndexDataItemInt64Value(dataItem, prop, 0));
                    break;
                case IndexPropertyTypeFloat:
                    printf("%f\t",IndexDataItemFloatValue(dataItem, prop, 0));
                    break;
                case IndexPropertyTypeDouble:
                    printf("%lf\t",IndexDataItemDoubleValue(dataItem, prop, 0));
                    break;
                case IndexPropertyTypeString:
                    printf("%s\t",IndexDataItemStringValue(dataItem, prop, ""));
                    break;
                default:
                    printf("\t");
                    break;
            }
            
            prop ++;
            c --;
        }
        
        printf("\n");
        
        location ++;
    }
}
