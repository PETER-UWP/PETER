CXX			= g++
CXXFLAGS	= -std=c++11 -Wall -lpthread

test: moterControllerTest

moterControllBufferTest: moterControllBufferTest.o moterControllBuffer.o
	$(CXX) $(CXXFLAGS) -o moterControllTest moterControllBufferTest.o moterControllBuffer.o

moterControllBufferTest.o:
	$(CXX) $(CXXFLAGS) -c moterControllBufferTest.cpp

moterControllBuffer.o:
	$(CXX) $(CXXFLAGS) -c moterControllBuffer.hpp

check-syntax:
	$(CXX) $(CXXFLAGS) ${CHK_SOURCES} #-o /dev/null
# -S-fsyntax-only

clean:
	rm *.o
