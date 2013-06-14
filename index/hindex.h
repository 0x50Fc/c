//
//  hindex.h
//  hclib
//
//  Created by zhang hailong on 13-5-23.
//  Copyright (c) 2013年 hailong.org. All rights reserved.
//

#ifndef hclib_hindex_h
#define hclib_hindex_h

#ifdef __cplusplus
extern "C" {
#endif

    
    enum{
        IndexPropertyTypeNone,IndexPropertyTypeInt32,IndexPropertyTypeInt64,IndexPropertyTypeFloat,IndexPropertyTypeDouble,IndexPropertyTypeString,IndexPropertyTypeBytes
    };
    
    typedef huint32 IndexPropertyType;
    
    typedef struct _IndexProperty {
        hcchar * READONLY name;
        IndexPropertyType READONLY type;
        huint32 READONLY size;
        huint32 READONLY offset;
    } IndexProperty;
    
    typedef struct _IndexClass {
        huint32 READONLY version;
        IndexProperty * READONLY propertys;
        huint32 READONLY propertyCount;
        huint32 READONLY size;
        hbool READONLY initialized;
    } IndexClass;
    
    void IndexClassInitialize(IndexClass * const clazz);
    
    typedef struct _Index {
        const IndexClass * READONLY clazz;
        void * READONLY data;
        huint32 READONLY size;
    } Index;
    
    typedef void * IndexDataItem;
    
    void IndexCreate(Index * const index,const IndexClass * clazz);
    
    void IndexDelete(Index * const index);
    
    void IndexSetDataSize(Index * const index,huint32 size);
    
    void IndexPrint(Index * const index,huint32 location,huint32 length);
    
    IndexDataItem IndexDataItemAt(Index * const index,huint32 i);
    
    hany IndexDataItemValue(IndexDataItem dataItem,const IndexProperty * property);
    
    hfloat IndexDataItemFloatValue(IndexDataItem dataItem,const IndexProperty * property,hfloat defaultValue);
    
    hdouble IndexDataItemDoubleValue(IndexDataItem dataItem,const IndexProperty * property,hdouble defaultValue);
    
    hint32 IndexDataItemIntValue(IndexDataItem dataItem,const IndexProperty * property,hint32 defaultValue);
    
    hint64 IndexDataItemInt64Value(IndexDataItem dataItem,const IndexProperty * property,hint64 defaultValue);
    
    hcchar * IndexDataItemStringValue(IndexDataItem dataItem,const IndexProperty * property,hcchar * defaultValue);
    
    void IndexDataItemSetValue(IndexDataItem dataItem,const IndexProperty * property,hany value);
    
    void IndexDataItemSetFloatValue(IndexDataItem dataItem,const IndexProperty * property,hfloat value);
    
    void IndexDataItemSetDoubleValue(IndexDataItem dataItem,const IndexProperty * property,hdouble value);
    
    void IndexDataItemSetIntValue(IndexDataItem dataItem,const IndexProperty * property,hint32 value);
    
    void IndexDataItemSetInt64Value(IndexDataItem dataItem,const IndexProperty * property,hint64 value);
    
    void IndexDataItemSetStringValue(IndexDataItem dataItem,const IndexProperty * property,hcchar * value);
    
    
    typedef enum IndexCompareResult {
        IndexCompareResultSame = 0,IndexCompareResultDesc = 1,IndexCompareResultAsc = -1
    } IndexCompareResult;
    
    typedef IndexCompareResult (* IndexDataItemCompare) (Index * const index,IndexDataItem dataItem1,IndexDataItem dataItem2,hany context);
    
    huint32 IndexSort(Index * const index,IndexDataItemCompare compare,huint32 length,hany context);
    
    huint32 IndexSortProperty(Index * const index,const IndexProperty * property,IndexCompareResult mode,huint32 length);
    
    huint32 IndexLoadDataFromFile(Index * const index,hcchar * file,huint32 location,huint32 length);

    huint32 IndexSaveDataToFile(Index * const index,hcchar * file,huint32 length);
    
    huint32 IndexAppendDataToFile(Index * const index,hcchar * file,huint32 length);
    
    
#ifdef __cplusplus
}
#endif

#endif
