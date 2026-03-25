# Objective-C constant literals only work for Objective-C++, but not Objective-C

**Author:** googlewalt
**URL:** https://github.com/llvm/llvm-project/issues/180673

## Body

Xcode has supported `-fobjc-constant-literals` since Xcode 13.  Even though open source clang doesn't accept the flag, it seems to support the underlying feature by default.  But surprisingly that only works for Objective-C++ but not Objective-Cc.  See below for reproducer and more details.

`$ cat lit.m`

```objective-c
#import <Foundation/Foundation.h>

static NSDictionary * const myConstantDictionary = @{ @"something_awesome" : @YES };
static NSArray * const myArray = @[ @1, @2, @3, @4 ];
static NSNumber * const answerToLife = @42;

int main(int argc, char *argv[]) {
  @autoreleasepool {
    NSLog(@"%@", myConstantDictionary);
    NSLog(@"%@", myArray);
    NSLog(@"%@", answerToLife);
  }
  return 0;
}
```
```
# With Xcode clang, both of the following commands succeed:
$ clang -target arm64-apple-macosx15.5 -fsyntax-only -x objective-c lit.m
$ clang -target arm64-apple-macosx15.5 -fsyntax-only -x objective-c++ lit.m

# With open source clang, building for Objective-C++ succeeds:
$ clang -isysroot <sysroot> -target arm64-apple-macosx15.5 -fsyntax-only -x objective-c++ lit.m
# But building for Objective-C fails:
$ clang -isysroot <sysroot> -target arm64-apple-macosx15.5 -fsyntax-only -x objective-c lit.m
lit.m:3:52: error: initializer element is not a compile-time constant
    3 | static NSDictionary * const myConstantDictionary = @{ @"something_awesome" : @YES };
      |                                                    ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
lit.m:4:34: error: initializer element is not a compile-time constant
    4 | static NSArray * const myArray = @[ @1, @2, @3, @4 ];
      |                                  ^~~~~~~~~~~~~~~~~~~
lit.m:5:40: error: initializer element is not a compile-time constant
    5 | static NSNumber * const answerToLife = @42;
      |                                        ^~~
3 errors generated.

```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Walter Lee (googlewalt)

<details>
Xcode has supported `-fobjc-constant-literals` since Xcode 13.  Even though open source clang doesn't accept the flag, it seems to support the underlying feature by default.  But surprisingly that only works for Objective-C++ but not Objective-Cc.  See below for reproducer and more details.

```
$ cat lit.m
#import &lt;Foundation/Foundation.h&gt;

static NSDictionary * const myConstantDictionary = @{ @"something_awesome" : @<!-- -->YES };
static NSArray * const myArray = @[ @<!-- -->1, @<!-- -->2, @<!-- -->3, @<!-- -->4 ];
static NSNumber * const answerToLife = @<!-- -->42;

int main(int argc, char *argv[]) {
  @<!-- -->autoreleasepool {
    NSLog(@"%@", myConstantDictionary);
    NSLog(@"%@", myArray);
    NSLog(@"%@", answerToLife);
  }
  return 0;
}

# With Xcode clang, both of the following commands succeed:
$ clang -target arm64-apple-macosx15.5 -fsyntax-only -x objective-c lit.m
$ clang -target arm64-apple-macosx15.5 -fsyntax-only -x objective-c++ lit.m

# With open source clang, building for Objective-C++ succeeds:
$ clang -isysroot &lt;sysroot&gt; -target arm64-apple-macosx15.5 -fsyntax-only -x objective-c++ lit.m
# But building for Objective-C fails:
$ clang -isysroot &lt;sysroot&gt; -target arm64-apple-macosx15.5 -fsyntax-only -x objective-c lit.m
lit.m:3:52: error: initializer element is not a compile-time constant
    3 | static NSDictionary * const myConstantDictionary = @{ @"something_awesome" : @<!-- -->YES };
      |                                                    ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
lit.m:4:34: error: initializer element is not a compile-time constant
    4 | static NSArray * const myArray = @[ @<!-- -->1, @<!-- -->2, @<!-- -->3, @<!-- -->4 ];
      |                                  ^~~~~~~~~~~~~~~~~~~
lit.m:5:40: error: initializer element is not a compile-time constant
    5 | static NSNumber * const answerToLife = @<!-- -->42;
      |                                        ^~~
3 errors generated.

```

</details>


---

### Comment 2 - shafik

CC @Bigcheese @rjmccall 

---

