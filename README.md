## Description

MCCBlockDelegate lets you dynamically add a delegate block in place of a delegate implementation to an object.

Checkout the full explanation on my blog here: http://orion98mc.blogspot.com/2012/09/objective-c-blocks-for-fun-part-2.html

## Usage

Some UIKit UI elements require a delegate. Instead of having to implement the delegate protocol methods in a delegate object (usually the view controller), you may use MCCBlockDelegate to inject a delegate block directly in the UI object such that it can become the delegate itself for a particular delegate method.


### Example

Install a trampoline function for UIAlertView in the application delegate so that it's patched for the whole app

```objective-c
// Create the delegate trampoline function
static MCCBlockDelegateVoidTrampoline(alertViewDidDismissWithButtonIndex, (UIAlertView *alertView, NSInteger buttonIndex), (alertView, buttonIndex))

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
  
  // Patch UIAlertView class with a delegate trampoline
  [MCCBlockDelegate addMethodWithSelector:@selector(alertView:didDismissWithButtonIndex:) inClass:[UIAlertView class] protocol:@protocol(UIAlertViewDelegate) implementation:(IMP)alertViewDidDismissWithButtonIndex];
  
  ...
}

```

Now let's create a UIAlertView and use a block as delegate method:

```objective-c

- (void)viewDidAppear:(BOOL)animated {
  UIAlertView *av = [[[UIAlertView alloc]initWithTitle:@"Alert" message:@"This is an alert" delegate:nil cancelButtonTitle:@"Cancel" otherButtonTitles:@"Ok", nil]autorelease];
  av.delegate = av;
  [av show];
  
  [MCCBlockDelegate setObject:av selector:@selector(alertView:didDismissWithButtonIndex:) withBlock:^(UIAlertView *alertView, NSInteger index){
    NSLog(@"AlertView dismissed! index: %d", index);
  }];
}

```

## License terms

Copyright (c), 2012 Thierry Passeron

The MIT License

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
