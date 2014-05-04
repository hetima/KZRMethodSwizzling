#KZRMethodSwizzling

One of a means of Method Swizzling.  
This library replaces method with function or Block. It does not use dummy method.

##Swizzling with Block

```objc
#import "KZRMethodSwizzlingWithBlock.h"

/*
usage:
KZRMETHOD_SWIZZLING_WITHBLOCK(
  const char* className, // Class name
  const char* selectorName,  // SEL name
  BOOL isClassMethod, // method type. class(KZRClassMethod) or instance(KZRInstanceMethod)
  KZRIMPUnion originalIMP, // variable name of original IMP (will be declared by #define macro)
  SEL originalSelector, // variable name of SEL (will be declared by #define macro)
  ^ (id rself, ...){  // SEL is not brought (id self, arg1, arg2...)
    // swizzling code
  }
);
 
*/

+ (void)load {

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
}
```

This macro gathers together code and definition. Easy to communicate with your object using block capture.

If `selectorName` has prefix `'+'`, the macro treats it as ClassMethod regardless of `isClassMethod`.

originalIMP is pointer to original method implementation. This is `IMP` but declared as `union` which clearly defines return value.

```c
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
```
Example:

```objc
NSObject* result = originalIMP.as_id(rself, originalSelector);
NSInteger result = originalIMP.as_int(rself, originalSelector);
NSRect result = originalIMP.as_rect(rself, originalSelector);
originalIMP.as_void(rself, originalSelector);
```
KZRIMPUnion works well with ARC.





##Swizzling with C Function

This is complicated way. I recommend using Block.

```objc
#import "KZRMethodSwizzling.h"

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
```

KZRMethodSwizzlingInfo.`call` is `union KZRIMPUnion`.

KZRMethodSwizzlingInfo.`representedObject` is useful bridge with obj-c world. But it is `__unsafe_unretained`. It must be retained somewhere else.


## Author

http://hetima.com/  
https://twitter.com/hetima

##License
WTFPL 2.0
```
           DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
                   Version 2, December 2004
 
Copyright (C) 2014 hetima

Everyone is permitted to copy and distribute verbatim or modified
copies of this license document, and changing it is allowed as long
as the name is changed.
 
           DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
 
 0. You just DO WHAT THE FUCK YOU WANT TO.
```