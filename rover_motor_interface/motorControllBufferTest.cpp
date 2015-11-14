#include <iostream>
#include "motorControllBuffer.hpp"
#include <unistd.h>

int main(){
	moterController weels("/dev/serial/by-id/usb-Arduino_LLC_Arduino_Leonardo-if00");

	if (!weels.areConnected()){
		std::cout<<"Failed to open!"<<std::endl;
	}else{
		std::cout<<"Connected!"<<std::endl;
	}
	moterController.move(10,10);
	sleep(3);
	moterController.move(10,-10);
	sleep(3);
	moterController.move(-10,10);
	sleep(3);
	moterController.move(-10,-10);
	return 0;
}
