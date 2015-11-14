CXX			= g++
CXXFLAGS	= -std=c++11 -Wall -lpthread

all: linefollowbuffer

linefollowbuffer: linefollowbuffer.o
	$(CXX) $(CXXFLAGS) -o linefollowbuffer linefollowbuffer.o

linefollowbuffer.o: linefollowbuffer.cpp
	$(CXX) $(CXXFLAGS) -c linefollowbuffer.cpp

check-syntax:
	$(CXX) $(CXXFLAGS) ${CHK_SOURCES} #-o /dev/null
# -S-fsyntax-only

clean:
	rm *.o
