#!/usr/bin/env bash

./clean.sh

clang++ -c guid.cpp -o guid.o -Wall -std=c++11 -DGUID_CFUUID
clang++ -c test.cpp -o test.o -Wall -std=c++11
clang++ -c testmain.cpp -o testmain.o -Wall -std=c++11
clang++ test.o guid.o testmain.o -o test -framework CoreFoundation
chmod +x test
./test
