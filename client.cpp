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

    int fd, newfd, nbytes, nbytes2,BUF_SIZE = 512;
    char buf[BUF_SIZE], response[BUF_SIZE];
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

    while (true){
        memset(&response, 0, sizeof(response));
        std::string input_command;
        std::getline(std::cin, input_command);
        sprintf(response, input_command.c_str());
        send( fd, response, BUF_SIZE, 0);
        //write(fd, response, sizeof(response));

        memset(&response, 0, sizeof(response));
        recv(fd, response, BUF_SIZE, 0);
        //read(fd,response,sizeof(response));
        std::cout<<"server says: "<< response<<'\n';
        // if (response== QUIT_RESPONSE)
        //     break;
    }

    close(fd);
    return 0;
}