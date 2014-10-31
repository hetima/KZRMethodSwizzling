//
//  KZRMethodSwizzlingWithBlock.h
//  KZRMethodSwizzling
//
//  Created by hetima on 2014/05/01.
//  WTFPL 2.0
//

#import <objc/message.h>

// KZRMethodSwizzling

/*
 
 KZRMETHOD_SWIZZLING_(
 const char* className, //Class name.
 const char* selectorName,  //SEL name. add prefix "+" for class method.
 return type, //specify method return type like id, void, NSRect...
 originalIMP, // variable name of original IMP (will be declared by #define macro)
 SEL originalSelector //variable name of SEL (will be declared by #define macro)
 )
 ^return-type (id slf, id arg1, id arg2...){  // SEL is not brought (id self, arg1, arg2...)
 //swizzling code
 id result=originalIMP(slf, originalSelector, arg1, arg2...);
 return result;
 }_WITHBLOCK;
 
 This macro returns error value. You can write like this.
 
 NSInteger error = KZRMETHOD_SWIZZLING_(...}_WITHBLOCK;
 if(error==KZRMethodSwizzlingErrorClassNotFound){
 }
 
 
 */

// v3
enum {
    KZRMethodSwizzlingNoError=0,
    KZRMethodSwizzlingErrorClassNotFound=1,
    KZRMethodSwizzlingErrorMethodNotFound=2
};

#define KZRMETHOD_SWIZZLING_(className, selectorName, returnType, originalIMP, originalSelector) ({\
NSInteger _val_err=KZRMethodSwizzlingNoError; \
BOOL _val_isClassMethod=NO; \
const char* _val_selName=selectorName; \
if(*_val_selName=='+'){_val_isClassMethod=YES; _val_selName++;} \
Class _val_cls=objc_getClass(className); \
if (_val_cls){ \
SEL originalSelector=sel_registerName(_val_selName); \
Method _val_originalMethod; \
if (_val_isClassMethod)_val_originalMethod = class_getClassMethod(_val_cls, originalSelector); \
else _val_originalMethod = class_getInstanceMethod(_val_cls, originalSelector); \
returnType(*originalIMP)(id, SEL, ...) = (returnType(*)(id, SEL, ...))method_getImplementation(_val_originalMethod); \
if (originalIMP) { id _val_block=

#define _WITHBLOCK ;IMP _val_newImp = imp_implementationWithBlock(_val_block); \
method_setImplementation(_val_originalMethod, _val_newImp); \
}else{_val_err=KZRMethodSwizzlingErrorMethodNotFound;} \
}else{_val_err=KZRMethodSwizzlingErrorClassNotFound;} \
_val_err; \
})


// example
#if 0

KZRMETHOD_SWIZZLING_(
    "ClassName",
    "method:name:",
    BOOL, originalIMP, sel)
^BOOL (id slf, id arg1, id arg2){
    BOOL result=originalIMP(slf, sel, arg1, arg2);
    if(result){
        //...
    }
    return result;
}_WITHBLOCK;


KZRMETHOD_SWIZZLING_(
    "MYClass",
    "+classmethod",
    id, originalIMP, originalSelector)
^id (id slf){
    NSRect result=originalIMP(rself, originalSelector);
    return result;
}_WITHBLOCK;

#endif
