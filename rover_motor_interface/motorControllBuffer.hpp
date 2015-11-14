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

template<class T>
std::deque<T>& operator<<(std::deque<T>& stream,T obj){
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
		while(alive){
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
			}
		}
	}
	void oworker(){
		while(alive){
			//wight from serialDev to out
			char charBuffer[MAXLENGTH];
			read(serialDev,charBuffer,MAXLENGTH);
			if(charBuffer[0]!='\0')
				out<<std::string(charBuffer);
		}
	}
	explicit serialBuffer(std::string loc)
	//:serialDev(loc){
	{
		serialDev=open(loc.c_str(), O_RDWR | O_NONBLOCK);
		if(serialDev>=0){
			alive=true;
			this->workerThread1=std::thread(&serialBuffer::iworker,this);
			this->workerThread2=std::thread(&serialBuffer::oworker,this);
		}
	}
	~serialBuffer(){
		if(alive)
			close(serialDev);
		workerThread1.join();
		workerThread2.join();
		//serialDev.close();
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
};
