all: a.out

a.out: server.o jsoncpp.o  
	g++ -std=c++11 server.o jsoncpp.o  

server.o: server.cpp def.hpp json/json.h
	g++ -std=c++11 -c server.cpp

jsoncpp.o: jsoncpp.cpp json/json.h
	g++ -std=c++11 -c jsoncpp.cpp

clean:
	rm *.o
	rm a.out
