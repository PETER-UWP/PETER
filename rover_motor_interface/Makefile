CXX			= g++
CXXFLAGS	= -std=c++11 -Wall -lpthread -MD -MP

tests: motorControllBufferTest

motorControllBufferTest: motorControllBufferTest.o
	$(CXX) $(CXXFLAGS) -o motorControllTest motorControllBufferTest.o

motorControllBufferTest.o:
	$(CXX) $(CXXFLAGS) -c motorControllBufferTest.cpp

check-syntax:
	$(CXX) $(CXXFLAGS) ${CHK_SOURCES} #-o /dev/null
# -S-fsyntax-only

clean:
	rm *.o
