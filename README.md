# CrossGuid

CrossGuid is a minimal, cross platform, C++ GUID library. It uses the best
native GUID/UUID generator on the given platform and has a generic class for
parsing, stringifying, and comparing IDs. The guid generation technique is
determined by spefifying one of the following preprocessor flags:

* `GUID_LIBUUID` - Uses `libuuid` which is normally used on linux but possibly
  usable elsewhere as well.
* `GUID_CFUUID` - Uses `CFCreateUUID` from Apple's `CoreFoundation` framework.
  This works on both Mac OSX and iOS.
* `GUID_WINDOWS` - Uses the built in `CoCreateGuid` function in Windows.
* `GUID_ANDROID` - Uses JNI to invoke Java functions from Android SDK.

## Versions

This is version 0.2 of CrossGuid. If you all already using CrossGuid and your code
uses `GuidGenerator` then you are using version 0.1. Differences in version 0.2:

* Put everything inside the namespace `xg` instead of using the global
namespace.
* Removed `GuidGenerator` class and replaced with the free function
`xg::newGuid`. This is the way I originally wanted it to work but since Android
is a special snowflake requiring state (`JNIEnv *`) I introduced the
`GuidGenerator` class specifically so that there would be somewhere to store
the `JNIEnv *` when running on Android. However, this basically meant
complicating the library for the sake of one platform. In version 0.2 the goal is
to design for the normal platforms and let Android be weird. In Android you just
need to run `xg::initJni(JNIEnv *)` before you create any guids. The `JNIEnv *`
is just stored as a global variable.
* Added CMake build system. Instead of different scripts for each platform you
can just run cmake and it should handle each platform (except Android which
again is special).
* Actual guid bytes are stored in `std::array<unsigned char, 16>` instead of
`std::vector<unsigned char>`.
* More error checking (like if you try to create a guid with invalid number of
bytes).

If you're happily using version 0.1 then there's not really any reason to
change.

## Compiling

Just do the normal cmake thing:

```
mkdir build
cd build
cmake ..
make
```

## Running tests

After compiling as described above you should get two files: `libxg.a` (the
static library) and `xgtest` (the test runner). So to run the tests just do:

```
./xgtest
```

## Basic usage

### Creating guids

Create a new random guid:

```cpp
auto g = xg::newGuid();
```

**NOTE:** On Android you need to call `xg::initJni(JNIEnv *)` first so that it
is possible for `xg::newGuid()` to call back into java libraries. `initJni`
only needs to be called once when the process starts.

Create a new zero guid:

```cpp
xg::Guid g;
```

Create from a string:

```cpp
xg::Guid g("c405c66c-ccbb-4ffd-9b62-c286c0fd7a3b");
```

### Checking validity

If you have some string value and you need to check whether it is a valid guid
then you can simply attempt to construct the guid:

```cpp
xg::Guid g("bad-guid-string");
if (!g.isValid())
{
	// do stuff
}
```

If the guid string is not valid then all bytes are set to zero and `isValid()`
returns `false`.

### Converting guid to string

First of all, there is normally no reason to convert a guid to a string except
for in debugging or when serializing for API calls or whatever. You should
definitely avoid storing guids as strings or using strings for any
computations. If you do need to convert a guid to a string, then you can
utilize strings because the `<<` operator is overloaded. To print a guid to
`std::cout`:

```cpp
void doGuidStuff(GuidGenerator generator)
{
    auto myGuid = generator.newGuid();
    std::cout << "Here is a guid: " << myGuid << std::endl;
}
```

Or to store a guid in a `std::string`:

```cpp
void doGuidStuff(GuidGenerator generator)
{
    auto myGuid = generator.newGuid();
    std::stringstream stream;
    stream << myGuid;
    auto guidString = stream.str();
}
```

There is also a `str()` function that returns a `std::string`:

```cpp
std::string guidStr = xg::newGuid().str();
```

### Creating a guid from raw bytes

It's unlikely that you will need this, but this is done within the library
internally to construct a `Guid` object from the raw data given by the system's
built-in guid generation function. There are two key constructors for this:

```cpp
Guid(std::array<unsigned char, 16> &bytes);
```

and

```cpp
Guid(const unsigned char * bytes);
```

When possible prefer the `std::array` constructor because it is safer. If you
pass in an incorrectly sized C array then bad things will happen.

### Comparing guids

`==` and `!=` are implemented, so the following works as expected:

```cpp
void doGuidStuff(GuidGenerator generator)
{
    auto guid1 = generator.newGuid();
    auto guid2 = generator.newGuid();

    auto guidsAreEqual = guid1 == guid2;
    auto guidsAreNotEqual = guid1 != guid2;
}
```

## Linux

**The Linux version uses the proprocessor flag `GUID_LIBUUID`**

On linux you can use libuuid which is pretty standard. On distros like Ubuntu
it is available by default but to use it you need the header files so you have
to do:

    sudo apt-get install uuid-dev

## Mac/iOS

**The Mac and iOS versions use the preprocessor flag `GUID_CFUUID`**

On Mac or iOS you can use `CFUUIDCreate` from `CoreFoundation`. Since it's a
plain C function you don't even need to compile as Objective-C++.

## Windows

**The Windows version uses the preprocessor flag `GUID_WINDOWS`**

On Windows we just use the the built-in function `CoCreateGuid`. CMake can
generate a Visual Studio project if that's your thing.

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
* You have an Android sdk setup including `ANDROID_HOME` environment variable.

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
