#include <iostream>
#include <fstream>
#include <string>

int main(){
	std::fstream serialDev("/dev/ttyAMC0");

	if (!myfile.is_open()){
		std::cout<<"Failed to open!"<<std::endl;
	}else{
		
	}
	serialDev.close();
	return 0;
}
