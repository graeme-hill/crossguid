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
* `GUID_ANDROID` - Uses JNI to invoke Java functions from Android SDK.

I recommend taking the time to actually look at the `guid.h` and `guid.cpp` so
that you can see how simple they are. It should be pretty trivial to modify
the code to match your naming conventions or drop it into a namespace so that it
fits in nicely with your code base.

## Basic usage

### Tests

The tests are a simple way to figure out how the library works. There is a file
in the root of the repository called `test.cpp` that runs a simple set of tests
and outputs a few guid strings for a sanity check. This file does not have a
`main()` function entry point there, it is intended to be called from somewhere
else, and it takes a `GuidGenerator` as input. All platforms except for Android
use `testmain.cpp` to construct a `GuidGenerator` and run the tests. In Android
there is a special file called `android/jni/jnitest.cpp` which invokes the
tests.

### Creating a guid generator

Creation of a guid generator is not exactly the same in every platform, but
this is an intentional feature. In Android the guid generation function has to
have access to a `JNIEnv` handle, but that handle is not necessarily the same
all the time. Therefore, there is a `GuidGenerator` class whose construction is
different depending on the platform, but client code can pass around a
`GuidGenerator` object and then use it the same on every platform. On every
platform except Android, you can create a guid generator like this:

    GuidGenerator generator;

But on Android you need to pass a `JNIEnv *`:

    GuidGenerator generator(env);

### Creating guids

On every platform guid creation is the same:

    void doGuidStuff(GuidGenerator generator)
    {
        auto myGuid = generator.newGuid();
    }

### Converting guid to string

First of all, there is normally no reason to convert a guid to a string except
for in debugging or when serializing for API calls or whatever. You should
definitely avoid storing guids as strings or using strings for any
computations. If you do need to convert a guid to a string, then you can
utilize strings because the `<<` operator is overloaded. To print a guid to
`std::cout`:

    
    void doGuidStuff(GuidGenerator generator)
    {
        auto myGuid = generator.newGuid();
        std::cout << "Here is a guid: " << myGuid << std::endl;
    }

Or to store a guid in a `std::string`:

    void doGuidStuff(GuidGenerator generator)
    {
        auto myGuid = generator.newGuid();
        std::stringstring stream;
        stream << myGuid;
        auto guidString = stream.str();
    }

### Parsing a string into a guid

There is a constructor that can be used to create a guid from a string without
needing any reference to a `GuidGenerator`:

    void doGuidStuff()
    {
        Guid guid("e63e03a8-f3e5-4e0f-99bb-a3fc402d4fc8");
    }

### Creating a guid from raw bytes

It's unlikely that you will need this, but this is done within the library
internally to construct a `Guid` object from the raw data given by the system's
built-in guid generation function. There are two key constructors for this:

    Guid(const vector<unsigned char> &bytes);

and

    Guid(const unsigned char *bytes);

In both cases the constructor expects to receive exactly 16 bytes.

### Comparing guids

`==` and `!=` are implemented, so the following works as expected:

    void doGuidStuff(GuidGenerator generator)
    {
        auto guid1 = generator.newGuid();
        auto guid2 = generator.newGuid();
        
        auto guidsAreEqual = guid1 == guid2;
        auto guidsAreNotEqual = guid1 != guid2;
    }

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

## Android

**The Android version uses the preprocessor flag `GUID_ANDROID`**

The Android version uses a handle to a `JNIEnv` object to invoke the
`randomUUID()` function on `java.util.UUID` from C++. The Android specific code
is all in the `android/` subdirectory. If you have an emulator already running,
then you can run the `android.sh` script in the root directory. It has the
following requirements:

* Android emulator is already running (or you have physical device connected).
* You're using bash.
* adb is in your path.
* ndk-build and other ndk cross-compile tools are in your path.
* You have a jdk setup including `JAVA_HOME` environment variable.

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
