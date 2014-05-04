//
//  KZRMethodSwizzlingWithBlock.h
//  KZRMethodSwizzling
//
//  Created by hetima on 2014/05/01.
//  WTFPL 2.0
//

#import <objc/message.h>

#ifndef KZRIMPUNION
#define KZRIMPUNION

typedef union KZRIMPUnion {
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
} KZRIMPUnion;

#endif
#ifndef KZRClassMethod

#define KZRClassMethod YES
#define KZRInstanceMethod NO
#define KZRMethodInspection NO

#endif

/*
 
KZRMETHOD_SWIZZLING_WITHBLOCK(
    const char* className, //Class name
    const char* selectorName,  //SEL name
    BOOL isClassMethod, //method type class(YES or KZRClassMethod) or instance(NO or KZRInstanceMethod)
    KZRIMPUnion originalIMP, // variable name of original IMP (will be declared by #define macro)
    SEL originalSelector, //variable name of SEL (will be declared by #define macro)
    ^ (id rself, ...){  // SEL is not brought (id self, arg1, arg2...)
        //swizzling code
    }
);
 
 */

// v2
#define KZRMETHOD_SWIZZLING_WITHBLOCK(className, selectorName, isClassMethod, originalIMP, originalSelector, block) {\
BOOL _val_isClassMethod=isClassMethod; \
const char* _val_selName=selectorName; \
if(*_val_selName=='+'){_val_isClassMethod=YES; _val_selName++;} \
Class _val_cls=objc_getClass(className); \
SEL originalSelector=sel_registerName(_val_selName); \
Method _val_originalMethod; \
if (_val_isClassMethod)_val_originalMethod = class_getClassMethod(_val_cls, originalSelector); \
else _val_originalMethod = class_getInstanceMethod(_val_cls, originalSelector); \
KZRIMPUnion originalIMP = (KZRIMPUnion)(IMP)method_getImplementation(_val_originalMethod); \
if (originalIMP.as_id) { id _val_block=block \
;IMP _val_newImp = imp_implementationWithBlock(_val_block); \
method_setImplementation(_val_originalMethod, _val_newImp); \
} \
}

// v1 obsoleted
#define KZRMETHOD_SWIZZLING_(className, selectorName, isClassMethod, originalIMP, originalSelector) {\
Class _val_cls=objc_getClass(className); \
SEL originalSelector=sel_registerName(selectorName); \
Method _val_originalMethod; \
BOOL isClassMethodVal=isClassMethod; \
if (isClassMethodVal)_val_originalMethod = class_getClassMethod(_val_cls, originalSelector); \
else _val_originalMethod = class_getInstanceMethod(_val_cls, originalSelector); \
KZRIMPUnion originalIMP = (KZRIMPUnion)(IMP)method_getImplementation(_val_originalMethod); \
if (originalIMP.as_id) { id block=

#define _WITHBLOCK ;IMP _val_newImp = imp_implementationWithBlock(block); \
method_setImplementation(_val_originalMethod, _val_newImp); \
} \
}


// example
#if 0

KZRMETHOD_SWIZZLING_WITHBLOCK(
    "NSView",
    "frame",
    KZRInstanceMethod, originalIMP, originalSelector,
    ^NSRect (id rself){  // SEL is not brought (id self, arg1, arg2...)
        NSRect result=originalIMP.as_rect(rself, originalSelector);
        static dispatch_once_t onceToken;
        dispatch_once(&onceToken, ^{
            NSLog(@"frame=%@", NSStringFromRect(result));
        });
        return result;
    }
);

#endif
