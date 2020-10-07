# work in progress

Tested on 
* Visual Studio C++14
* Visual Studio C++17
* Visual Studio C++20.1705 (experimental version)
* gcc 6
* gcc 7
* gcc 8
* gcc 9
* gcc 10.1 & 10.2

Does not compile on 
* Visual Studio C++11
* gcc 5.5

# How to use in your code
Grab the file integrity.h from github and drop into your includes directory.

Then in your source code, do
```c++
#include "integrity.h"
```
Then you can do, for example,
```c++
#include "integrity.h"

void myExampleFunc(int i, float f) {
	Integrity::check(i > 0, "Expected i to be greater than 0, it was {}", i);
	Integrity::checkIsValidNumber(f);
}
```
# Using in godbolt
You can go to [godbolt / compiler explorer](https://godbolt.org/) to test it out. The first line of your file should be:

```c++
#include <https://raw.githubusercontent.com/RisingTyde/integrity-cpp/master/integrity.h>
```
# Using in wandbox
You can go to wandbox.org to test it out as well. Press the + symbol to make a new file and call it integrity.h. The copy the contents of integrity.h from github into the file. Then in your main file you can do
```c++
#include "integrity.h"
```

## Integrity API

The full set of functions is:

```c++
    Integrity::check(condition, zeroOrUpToThreeMsgArgs);
    Integrity::checkNotNull(p, zeroOrUpToThreeMsgArgs);
    Integrity::checkStringNotNullOrEmpty(s, zeroOrUpToThreeMsgArgs);
    Integrity::checkIsValidNumber(d, zeroOrUpToThreeMsgArgs);

    Integrity::fail(zeroOrUpToThreeMsgArgs);
```
The message args can be primitives and various types of string, so you could have, for example:
```c++
    int a = 1;
    int b = 2;
    Integrity::check(a == b);            // message will default to "Integrity check failed"
    Integrity::check(a == b, "oops");    // "oops"
    Integrity::check(a == b, "ouch a is {}", a); // "ouch a is 1"
    Integrity::check(a == b, a, b);      // "1, 2"
    Integrity::check(a == b, "was ", a == b, a);      // "was, True, 1" (since there is no {} the values are just appended)
```
There are also variants that allow you to build (delayed until needed) the message yourself. These are the same as above but with M added:
```c++
    Integrity::checkM(condition, [=](Integrity::out out) { out << "whatever I like"; });
    Integrity::checkNotNullM(p, [=](Integrity::out out) { out << "whatever I like"; });
    Integrity::checkStringNotNullOrEmptyM(s, [=](Integrity::out out) { out << "whatever I like"; });
    Integrity::checkIsValidNumberM(d, [=](Integrity::out out) { out << "whatever I like"; });

    Integrity::failM([=](Integrity::out out) { out << "whatever I like"; });
```
Note that Integrity::out is just a typedef for std::stringstream& so you can use that if you prefer

The important thing is that the message building is only invoved when needed, i.e. when the check fails. If the check passes then you do not incur the cost of building the message.

### Exceptions

Only one type of exception is ever throw: a std::logic_error
