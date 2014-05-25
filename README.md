###### CrossGuid

CrossGuid is a minimal, cross platform, C++ implementation. It uses the best
native GUID/UUID generator on the given platform and had a generic class for
parsing, stringifying, and comparing IDs.

#### Building

Each platform has its own build script to create object files and run unit
tests, but the source files (the only ones you need are `guid.h` and
`guid.cpp`) are meant to be incorporated into your own build system. You can
look at the platform build script (eg: `linux.sh` as a reference).

#### Linux

On linux you can use libuuid which is pretty standard. On distros like Ubuntu
it is available by default but to use it you need the header files so you have
to do:

    sudo apt-get install uuid-dev

Then you can compile and run tests with:

    ./linux.sh

