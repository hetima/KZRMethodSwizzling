#KZRMethodSwizzling

One of a means of Method Swizzling.  
This library replaces method with function. It does not use dummy method.

Example:

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

`KZRMethodSwizzlingInfo.call` is pointer to original method implementation. This is union which clearly defines return value.

Like this:

```objc
NSInteger result = repFuncInfo.call.as_int(self, _cmd);
NSRect result = repFuncInfo.call.as_range(self, _cmd);
NSObject* result = repFuncInfo.call.as_id(self, _cmd);
```


`KZRMethodSwizzlingInfo.representedObject` is useful bridge with obj-c world. But it is `__unsafe_unretained`. It must be retained somewhere else.


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