//#include <fstream>
#include <thread>
#include <string>
#include <deque>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define MAXLENGTH 20
#define DEBUG 0

#if DEBUG>0
#include <iostream>
#endif

template<class T>
std::deque<T>& operator<<(std::deque<T>& stream,T obj){t
	stream.push_back(obj);
	return stream;
}

template<class T,class U>
U& operator<<(U& o, std::deque<T> &stream){
	while(stream.size()){
		o<<stream.front();
		stream.pop_front();
	}
	return o;
}

template<class T,class U>
U& operator +=(U& o,std::deque<T> stream){
	for(T v:stream){
		o<<v;
	}
	return o;
}

class serialBuffer{
private:
	std::deque<std::string> in, out;
	std::thread workerThread1,workerThread2;
	/*std::fstream*/int serialDev;
	bool alive=false;
public:
	void iworker(){
#if DEBUG>1
		std::cout<<"serialBuffer iworker started"<<std::endl;
#endif
		while(alive){
#if DEBUG>1
		std::cout<<"serialBuffer iworker alive"<<std::endl;
#endif
			//read to serialDev from in
			for(;in.size();){
				std::string tmp;
				tmp=in.front();
				in.pop_front();
				char charBuffer[MAXLENGTH];
				strcpy(charBuffer,tmp.c_str());
				charBuffer[tmp.size()]='\n';
				charBuffer[tmp.size()+1]='\0';
				write(serialDev,charBuffer,tmp.size()+2);
				usleep(1000);
			}
		}
#if DEBUG>1
		std::cout<<"serialBuffer iworker dead"<<std::endl;
#endif
	}
	void oworker(){
#if DEBUG>1
		std::cout<<"serialBuffer oworker start"<<std::endl;
#endif
		while(alive){
#if DEBUG>1
		std::cout<<"serialBuffer owerker alive"<<std::endl;
#endif
			//wight from serialDev to out
			char charBuffer[MAXLENGTH];
			read(serialDev,charBuffer,MAXLENGTH);
			if(charBuffer[0]!='\0')
				out<<std::string(charBuffer);
		}
#if DEBUG>1
		std::cout<<"serialBuffer owerker dead"<<std::endl;
#endif
	}
	explicit serialBuffer(std::string loc)
	//:serialDev(loc){
	{
#if DEBUG>0
		std::cout<<"serialBuffer INITing"<<std::endl;
#endif
		serialDev=open(loc.c_str(), O_RDWR | O_NONBLOCK);
		if(serialDev>=0){
#if DEBUG>0
			std::cout<<"serialBuffer THREADing"<<std::endl;
#endif
			alive=true;
			this->workerThread1=std::thread(&serialBuffer::iworker,this);
			this->workerThread2=std::thread(&serialBuffer::oworker,this);
#if DEBUG>0
			std::cout<<"serialBuffer THREADed"<<std::endl;
#endif
		}
#if DEBUG>0
		std::cout<<"serialBuffer INITed"<<std::endl;
#endif
	}
	~serialBuffer(){
#if DEBUG>0
		std::cout<<"serialBuffer distructor"<<std::endl;
#endif
		if(alive)
			close(serialDev);
		workerThread1.join();
		workerThread2.join();
		//serialDev.close();
#if DEBUG>0
		std::cout<<"serialBuffer distructed"<<std::endl;
#endif
	}
	bool isAlive(){
		return alive;//serialDev.is_open();
	}
	std::string readNext(){
		std::string tmp=out.front();
		out.pop_front();
		return tmp;
	}
	bool hasData(){
		return out.size()!=0;
	}
	void wrightData(std::string data){
		in<<data;
	}
};

class moterController{
private:
	serialBuffer ioDev;
public:
	explicit moterController(std::string dev):ioDev(dev){
	}
	bool areConnected(){
		return ioDev.isAlive();
	}
	//implement the protocall here
	void move(int l,int r){
		if(-100>l || l>100 || -100>r || r>100){
			//noop
			return;
		}
		auto convert=[](int v){
			return v*86/100+95;
		}
		ioDev.wrightData("L"+convert(l)+"\n");
		ioDev.wrightData("R"+convert(r)+"\n");
	}
};
