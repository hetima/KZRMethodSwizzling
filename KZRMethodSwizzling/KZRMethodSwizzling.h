//
//  KZRMethodSwizzling.h
//  KZRMethodSwizzling
//
//  Created by hetima on 2014/04/30.
//  WTFPL 2.0
//

#import <objc/message.h>

union KZRIMPUnion {
    IMP as_id;
    void (*as_void)(id, SEL, ...);
    void* (*as_pointer)(id, SEL, ...);
    
    char (*as_char)(id, SEL, ...);
    int (*as_int)(id, SEL, ...);
    long long (*as_long_long)(id, SEL, ...);
    double (*as_double)(id, SEL, ...);
    
    CGFloat (*as_float)(id, SEL, ...);
    CGRect (*as_rect)(id, SEL, ...);
    CGSize (*as_size)(id, SEL, ...);
    CGPoint (*as_point)(id, SEL, ...);
    NSRange (*as_range)(id, SEL, ...);
};

typedef struct KZRMethodSwizzlingInfo {
    IMP newIMP;
    const char *className;
    const char *selectorName;
    BOOL isClassMethod;
    __unsafe_unretained id representedObject; //be careful with reference count
    union KZRIMPUnion call;
} KZRMethodSwizzlingInfo;

#define KZRClassMethod YES
#define KZRInstanceMethod NO

BOOL KZRMethodSwizzling(KZRMethodSwizzlingInfo* rep);



// example
#if 0

static void repFunc(id self, SEL _cmd, id obj)
{
    repFuncInfo.call.as_void(self, _cmd, obj); //call original method
    id rep = repFuncInfo.representedObject;
    [rep someMethod:obj];
}

static KZRMethodSwizzlingInfo repFuncInfo = {
    (IMP)repFunc,
    "ClassName",
    "selector:name:",
    KZRInstanceMethod,
    nil,
    NULL
};

+ (void)load {
    repFuncInfo.representedObject = someRetainedObject;
    KZRMethodSwizzling(&repFuncInfo);
}

#endif
