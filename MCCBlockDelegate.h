//
//  MCCBlockDelegate.h
//
//  Created by Thierry Passeron on 06/09/12.
//  Copyright (c) 2012 Monte-Carlo Computing. All rights reserved.
//

#import <Foundation/Foundation.h>
#import </usr/include/objc/objc-class.h>

#define MCCBlockDelegateGetBlock(retype, ...)   retype(^block)(id, SEL, ## __VA_ARGS__ ) = objc_getAssociatedObject(self, _cmd);
#define MCCBlockDelegateGetSBlock(retype, ...)  retype(^block)(__VA_ARGS__) = objc_getAssociatedObject(self, _cmd);


#pragma mark INTERNALLY_USED_MACROS_BEGIN
#define MCCBDTS(retype, ...)  (id self, SEL _cmd, ##__VA_ARGS__){ \
                  MCCBlockDelegateGetSBlock(retype, __VA_ARGS__);
#define MCCBDTSV(...)  (id self, SEL _cmd, ##__VA_ARGS__){ \
                  MCCBlockDelegateGetSBlock(void, __VA_ARGS__);
/* TODO default return type here ... nil may not be the one */
#define MCCBDTE(...)  return block ? block(__VA_ARGS__) : nil; }
#define MCCBDTEV(...)  if (block) block(__VA_ARGS__); }
#pragma mark INTERNALLY_USED_MACROS_END


/* Use these 2 macros to create custom trampoline functions */

#define MCCBlockDelegateTrampoline(FN_NAME, RETYPE, TYPES, BLOCK_ARGS )  RETYPE FN_NAME MCCBDTS TYPES ; MCCBDTE BLOCK_ARGS

#define MCCBlockDelegateVoidTrampoline(FN_NAME, TYPES, BLOCK_ARGS )  void FN_NAME MCCBDTSV TYPES ; MCCBDTEV BLOCK_ARGS


/*
 
 Examples of trampoline function creation:
 -----------------------------------------
 
 * for block: void^(id a, int b) { ... }
 
    => MCCBlockDelegateVoidTrampoline(myCallback, (id a, int b), (a, b))
    
 * for block: NSString *^(id a, char *b) { ... }
 
    => MCCBlockDelegateTrampoline(myOtherCallback, NSString*, (NSString*, id a, char* b), (a, b))
 
*/


/* Prebuilt void trampolines */

// void^(void){...}
static void MCCBD_trampolineVoid(id self, SEL _cmd) {
	MCCBlockDelegateGetSBlock(void);
  if (block) block();
}

// void^(id){...}
static void MCCBD_trampolineVoid_id(id self, SEL _cmd, id arg1) {
	MCCBlockDelegateGetSBlock(void, id);
  if (block) block(arg1);
}

// void^(id, id){...}
static void MCCBD_trampolineVoid_id_id(id self, SEL _cmd, id arg1, id arg2) {
	MCCBlockDelegateGetSBlock(void, id, id);
  if (block) block(arg1, arg2);
}



@interface MCCBlockDelegate : NSObject

+ (BOOL)addMethodWithSelector:(SEL)selector inClass:(Class)aClass protocol:(Protocol*)protocol implementation:(IMP)implementation;
+ (void)setObject:(id)object withAssociatedKey:(void*)key copyValue:(id)value;
+ (void)setObject:(id)object selector:(void*)key withBlock:(id)value;

+ (BOOL)delegateSelector:(SEL)selector inObject:(id)object trampoline:(IMP)trampoline_fn withBlock:(id)block;
+ (BOOL)delegateSelector:(SEL)selector inObject:(id)object protocol:(Protocol*)protocol trampoline:(IMP)trampoline_fn withBlock:(id)block;

@end
