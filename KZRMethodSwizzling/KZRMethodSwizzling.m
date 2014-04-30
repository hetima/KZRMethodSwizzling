//
//  KZRMethodSwizzling.m
//  KZRMethodSwizzling
//
//  Created by hetima on 2014/04/30.
//  WTFPL 2.0
//

#import "KZRMethodSwizzling.h"


BOOL KZRMethodSwizzling(KZRMethodSwizzlingInfo* rep)
{
    Class aClass = objc_getClass(rep->className);
    SEL aSelector = sel_registerName(rep->selectorName);
    IMP originalIMP;

    if (aClass && aSelector) {
        
        Method origMethod;
        if (rep->isClassMethod) {
            origMethod = class_getClassMethod(aClass, aSelector);
        }else{
            origMethod = class_getInstanceMethod(aClass, aSelector);
        }
        
        if (origMethod) {
            originalIMP = method_setImplementation(origMethod, rep->newIMP);
        }
        
        if (originalIMP) {
            rep->call.as_id = originalIMP;
            return YES;
        }
    }
    
    return NO;
}

