#include <iostream>
#include "motorControllBuffer.hpp"

int main(){
	moterController weels("/dev/serial/by-id/usb-Arduino_LLC_Arduino_Leonardo-if00");

	if (!weels.areConnected()){
		std::cout<<"Failed to open!"<<std::endl;
	}
	return 0;
}
