PREFIX = /usr
LIBPATH = /lib
CXX = g++
CXXFLAGS = -std=c++11 -Wall -DGUID_LIBUUID -fPIC

INSTALL = install
PREFIX = /usr
BINDIR = $(PREFIX)/bin
LIBDIR = $(PREFIX)/$(LIBPATH)
INCDIR = $(PREFIX)/include

all: libcrossguid.so libcrossguid.a

clean:
	@rm -f *.o *.a *.so test *.dll *.DLL

install: all
	$(INSTALL) -D libcrossguid.so $(DESTDIR)$(LIBDIR)/libcrossguid.so
	$(INSTALL) -D libcrossguid.a $(DESTDIR)$(LIBDIR)/libcrossguid.a
	$(INSTALL) -D guid.h $(DESTDIR)$(INCDIR)/guid.h

test: test.o testmain.o libcrossguid.a
	$(CXX) $(CXXFLAGS) $^ -o $@ -L=$(LIBPATH) -luuid
	./test

libcrossguid.so: guid.o
	$(CXX) -shared -o $@ $<

libcrossguid.a: guid.o
	ar rc $@ $^
	ranlib $@

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
