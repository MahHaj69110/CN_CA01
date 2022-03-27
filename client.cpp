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

    int command_channel_fd, data_channel_fd;
    char buf[CLIENT_BUF_SIZE], response[CLIENT_BUF_SIZE];
    struct sockaddr_in srv_command_port, srv_data_port;

    command_channel_fd = socket(AF_INET, SOCK_STREAM, 0);
    data_channel_fd = socket(AF_INET, SOCK_STREAM, 0);
    //////////////////////////  connect to server. 

	srv_command_port.sin_family = AF_INET;
    srv_command_port.sin_port = htons(command_channel_port);
    srv_command_port.sin_addr.s_addr = inet_addr("127.0.0.1");
    connect(command_channel_fd, (struct sockaddr*) &srv_command_port, sizeof(srv_command_port));

    srv_data_port.sin_family = AF_INET;
    srv_data_port.sin_port = htons(data_channel_port);
    srv_data_port.sin_addr.s_addr = inet_addr("127.0.0.1");
    connect(data_channel_fd, (struct sockaddr*) &srv_data_port, sizeof(srv_data_port));

    std::cout<<"Connecting ...\n";
    //Connect is blocking and send a SYN signal and is blocked until receive SYNACK, (three way handshaking)
    //It can start now reading and writing

    while (true){
        memset(&response, 0, sizeof(response));
        recv(command_channel_fd, response, CLIENT_BUF_SIZE, 0);
        //read(fd,response,sizeof(response));
        std::cout<<"server says: "<< response<<'\n';
        // if (response== QUIT_RESPONSE)
        //     break;

        memset(&response, 0, sizeof(response));
        std::string input_command;
        std::getline(std::cin, input_command);
        sprintf(response, input_command.c_str());
        send( command_channel_fd, response, CLIENT_BUF_SIZE, 0);
        //write(fd, response, sizeof(response));
    }

    close(command_channel_fd);
    return 0;
}