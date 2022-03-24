#include <map>
#include <iostream>
#include "def.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>


using namespace std;
std::map<std::string,void*> readJsonFile();

int main(int argc, char const *argv[])
{
    //////////////////////////  set the connection port

    int command_channel_port=  8000;
    int data_channel_port= 8001;

    //////////////////////////  create socket

    int fd, newfd, nbytes, nbytes2;
    char buf[512], response[512];
    struct sockaddr_in srv;
    fd = socket(AF_INET, SOCK_STREAM, 0);

    //////////////////////////  connect to server. 

	srv.sin_family = AF_INET;
    srv.sin_port = htons(command_channel_port);
    srv.sin_addr.s_addr = inet_addr("127.0.0.1");
    connect(fd, (struct sockaddr*) &srv, sizeof(srv));

    std::cout<<"Connecting ...\n";
    //Connect is blocking and send a SYN signal and is blocked until receive SYNACK, (three way handshaking)
    //It can start now reading and writing

    sprintf(response,"Here's my request");
    write(fd, response, strlen(response));
    memset(&response, 0, sizeof(response));
    sprintf(response,"quit");
    write(fd, response, strlen(response));
    close(fd);

    return 0;
}