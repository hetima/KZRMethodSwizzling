# KZRMethodSwizzling

One of a means of Method Swizzling.  
This library replaces method with Block. It does not use dummy method.  
It works with `ENABLE_STRICT_OBJC_MSGSEND`.


## Swizzling Method

```objc
#import "KZRMethodSwizzlingWithBlock.h"

/*
usage:
KZRMETHOD_SWIZZLING_(
  const char* className, // Class name
  const char* selectorName,  // SEL name. add prefix "+" for class method.
  return type, //specify method return type like id, void, NSRect...
  original IMP name, // variable name of original IMP (will be declared by #define macro)
  SEL originalSelector) // variable name of SEL (will be declared by #define macro)
  ^ (id hookedObject, ...){  // SEL is not brought (id slf, arg1, arg2...)
    // swizzling code
  }_WITHBLOCK;
 
*/

//example:
+ (void)load {

    KZRMETHOD_SWIZZLING_("NSView", "initWithFrame:",
        id, originalIMP, sel)
    ^id (id slf, NSRect frame){  // SEL is not brought (id self, arg1, arg2...)
        id result=originalIMP(slf, sel, frame);
            
        return result;
    }_WITHBLOCK;

}
```

This macro gathers together code and definition. Easy to communicate with your object using block capture.

If `selectorName` has prefix `'+'`, the macro treats it as ClassMethod.

```objc
KZRMETHOD_SWIZZLING_("NSNotificationCenter", "+defaultCenter", id, call, sel)
^id (id slf){
    ...
}_WITHBLOCK;
```

If you hook `dealloc`, must use `__unsafe_unretained`.

```objc
KZRMETHOD_SWIZZLING_("NSView", "dealloc", void, call, sel)
^ (__unsafe_unretained id slf){
    call(slf, sel, frame);
    //Don't touch after call original dealloc.
}_WITHBLOCK;
```

This macro returns error value. You can write like this.

```objc
NSInteger error = KZRMETHOD_SWIZZLING_ ... _WITHBLOCK;
if (error!=KZRMethodSwizzlingNoError) {
    //...
}
```


## Adding Method

```objc
#import "KZRMethodSwizzlingWithBlock.h"
/*
 KZRMETHOD_ADDING_(
 const char* className, //Class name.
 const char* superClassName, // Super Class name that has method.
 const char* selectorName,  //SEL name. add prefix "+" for class method.
 return type, //specify method return type like id, void, NSRect...
 superIMP, // variable name of super IMP (will be declared by #define macro)
 SEL originalSelector //variable name of SEL (will be declared by #define macro)
 )
 ^return-type (id slf, id arg1, id arg2...){  // SEL is not brought (id self, arg1, arg2...)

 id result=superIMP(slf, originalSelector, arg1, arg2...); // almost same as objc_msgSendSuper()
 return result;
 }_WITHBLOCK_ADD;
 
 */


KZRMETHOD_ADDING_("SubTableCellView", "NSTableCellView", "setObjectValue:",
 void, call_super, sel)
^void (NSTableCellView* slf, id value){
    call_super(slf, sel, value); //call -[NSTableCellView setObjectValue:]
}_WITHBLOCK_ADD;
```
Super class must have selectorName method. Target class must not have selectorName method.




## Author

http://hetima.com/  
https://twitter.com/hetima

## License

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