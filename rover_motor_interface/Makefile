CXX			= g++
CXXFLAGS	= -std=c++11 -Wall -lpthread

test: motorControllBufferTest

motorControllBufferTest: motorControllBufferTest.o motorControllBuffer.o
	$(CXX) $(CXXFLAGS) -o motorControllTest motorControllBufferTest.o motorControllBuffer.o

motorControllBufferTest.o:
	$(CXX) $(CXXFLAGS) -c motorControllBufferTest.cpp

motorControllBuffer.o:
	$(CXX) $(CXXFLAGS) -c motorControllBuffer.hpp

check-syntax:
	$(CXX) $(CXXFLAGS) ${CHK_SOURCES} #-o /dev/null
# -S-fsyntax-only

clean:
	rm *.o
