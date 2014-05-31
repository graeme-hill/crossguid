#!/usr/bin/env bash

./clean.sh

g++ -c guid.cpp -o guid.o -Wall -std=c++11 -DGUID_LIBUUID
g++ -c test.cpp -o test.o -Wall -std=c++11
g++ -c testmain.cpp -o testmain.o -Wall
g++ test.o guid.o testmain.o -o test -luuid
chmod +x test
./test
