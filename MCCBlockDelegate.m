//
//  MCCBlockDelegate.m
//
//  Created by Thierry Passeron on 06/09/12.
//  Copyright (c) 2012 Monte-Carlo Computing. All rights reserved.
//

#import "MCCBlockDelegate.h"

//#define DEBUG_MCCBlockDelegate

@implementation MCCBlockDelegate

+ (BOOL)addMethodWithSelector:(SEL)selector inClass:(Class)aClass protocol:(Protocol*)protocol implementation:(IMP)implementation {
  const char *types = NULL;
  
  if (protocol) {
    types = protocol_getMethodDescription(protocol, selector, NO, YES).types;
  } else {
    Method method = class_getInstanceMethod(aClass, selector);
    types = method_getTypeEncoding(method);
  }
  
#ifdef DEBUG_MCCBlockDelegate
  NSLog(@"Selector: %@, Types: %s", NSStringFromSelector(selector), types);
#endif
  
  BOOL ok = class_addMethod(aClass, selector, implementation, types);
  
#ifdef DEBUG_MCCBlockDelegate
  if (ok) NSLog(@"Method Added");
#endif
  
  return ok;
}


+ (void)setObject:(id)object withAssociatedKey:(void*)key copyValue:(id)value {
  objc_setAssociatedObject(object, key, value, OBJC_ASSOCIATION_COPY);
}

+ (void)setObject:(id)object selector:(void*)key withBlock:(id)value {
  objc_setAssociatedObject(object, key, value, OBJC_ASSOCIATION_COPY);
}


+ (BOOL)delegateSelector:(SEL)selector inObject:(id)object trampoline:(IMP)trampoline_fn withBlock:(id)block {
  [self setObject:object withAssociatedKey:selector copyValue:block];
  return [self addMethodWithSelector:selector inClass:[object class] protocol:nil implementation:trampoline_fn];
}

+ (BOOL)delegateSelector:(SEL)selector inObject:(id)object protocol:(Protocol*)protocol trampoline:(IMP)trampoline_fn withBlock:(id)block {
  [self setObject:object withAssociatedKey:selector copyValue:block];
  return [self addMethodWithSelector:selector inClass:[object class] protocol:protocol implementation:trampoline_fn];
}

@end
