EXENAME = final
OBJS = main.o graph.o readFromFile.o

CXX = clang++
CXXFLAGS = $(CS225) -std=c++1y -stdlib=libc++ -c -g -O0 -Wall -Wextra -pedantic -Wno-vla-extension
LD = clang++
LDFLAGS = -std=c++1y -stdlib=libc++ -lc++abi -lm

all : $(EXENAME)

$(EXENAME) : dependency $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $(EXENAME)

main.o : main.cpp graph.h readFromFile.h edge.h
	$(CXX) $(CXXFLAGS) main.cpp

graph.o : graph.cpp graph.h edge.h
	$(CXX) $(CXXFLAGS) graph.cpp

readFromFile.o : readFromFile.cpp readFromFile.h
	$(CXX) $(CXXFLAGS) readFromFile.cpp

# Custom Clang version enforcement Makefile rule:
ccred=$(shell echo -e "\033[0;31m")
ccyellow=$(shell echo -e "\033[0;33m")
ccend=$(shell echo -e "\033[0m")

IS_EWS=$(shell hostname | grep "ews.illinois.edu")
IS_CORRECT_CLANG=$(shell clang -v 2>&1 | grep "version 6")
ifneq ($(strip $(IS_EWS)),)
ifeq ($(strip $(IS_CORRECT_CLANG)),)
CLANG_VERSION_MSG = $(error $(ccred) On EWS, please run 'module load llvm/6.0.1' first when running CS225 assignments. $(ccend))
endif
else
CLANG_VERSION_MSG = $(warning $(ccyellow) Looks like you are not on EWS. Be sure to test on EWS before the deadline. $(ccend))
endif

dependency: ; $(CLANG_VERSION_MSG)

test: dependency tests.o graph.o readFromFile.o 
	$(LD) tests.o graph.o readFromFile.o $(LDFLAGS) -o test

tests.o: tests/tests.cpp tests/catch.hpp graph.h readFromFile.h
	$(CXX) $(CXXFLAGS) tests/tests.cpp
    
.PHONY: dependency

clean :
	-rm -f *.o $(EXENAME) test