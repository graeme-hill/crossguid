# CrossGuid

CrossGuid is a minimal, cross platform, C++ GUID library. It uses the best
native GUID/UUID generator on the given platform and had a generic class for
parsing, stringifying, and comparing IDs. The intention is that anyone who
uses this code can simply copy `guid.h` and `guid.cpp` into their project and
define one of the following preprocessor flags to control the implementation:

* `GUID_LIBUUID` - Uses `libuuid` which is normally used on linux but possibly
  usable elsewhere as well.
* `GUID_CFUUID` - Uses `CFCreateUUID` from Apple's `CoreFoundation` framework.
  This works on both Mac OSX and iOS.
* `GUID_WINDOWS` - Uses the built in `CoCreateGuid` function in Windows.

I recommend taking the time to actually look at the `guid.h` and `guid.cpp` so
that you can see how simple they are. It should be pretty trivial to modify
code to match your naming conventions or drop it into a namespace so that it
fits in nicely with your code base.

## Linux

**The Linux version uses the proprocessor flag `GUID_LIBUUID`**

On linux you can use libuuid which is pretty standard. On distros like Ubuntu
it is available by default but to use it you need the header files so you have
to do:

    sudo apt-get install uuid-dev

Then you can compile and run tests with:

    ./linux.sh

## Mac/iOS

**The Mac and iOS versions use the preprocessor flag `GUID_CFUUID`**

On Mac or iOS you can use `CFUUIDCreate` from `CoreFoundation`. Since it's a
plain C function you don't even need to compile as Objective-C++. If you have
the command line tools that come with Xcode installed, then you can compile and
run the tests like this:

    ./mac.sh

## Windows

**The Windows version uses the preprocessor flag `GUID_WINDOWS`**

On Windows we just the the built-in function `CoCreateGuid`. There is a Visual
Studio 2013 solution in the `VisualStudio` directory which you can use to
compile and run tests.

## License

The MIT License (MIT)

Copyright (c) 2014 Graeme Hill (http://graemehill.ca)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
