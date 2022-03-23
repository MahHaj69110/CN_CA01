#include <map>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sstream>
#include "def.hpp"

typedef std::string (*command_func)(std::vector<std::string>);

std::map<std::string,void*> readJsonFile();

int main(int argc, char const *argv[])
{
     //////////////////////////  initialize command list

    std::map<std::string,command_func> command;

    //////////////////////////  parse and read data from Json file

    std::map<std::string,void*> config_data= readJsonFile();
    int command_channel_port;
    //int command_channel_port= (int)config_data["commandChannelPort"];
    //int data_channel_port= (int)config_data["dataChannelPort"];

    //std::vector<User> users;

    //////////////////////////  create socket

    
    int fd, newfd, nbytes, nbytes2;
    struct sockaddr_in srv;
    fd = socket(AF_INET, SOCK_STREAM, 0);

    //////////////////////////  bind to determined port

    srv.sin_family = AF_INET;
    srv.sin_port = htons(command_channel_port);
    srv.sin_addr.s_addr = inet_addr("128.2.15.9");
    bind(fd, (struct sockaddr*) &srv, sizeof(srv));

     //////////////////////////  listen to the port and accept client

    listen(fd, 5);

    struct sockaddr_in cli;
    socklen_t cli_len;
    memset(&cli, 0, sizeof(cli));
    newfd = accept(fd, (struct sockaddr*) &cli, &cli_len);

    int BUF_SIZE = 1024, bytesrecv = 0;
    char buf[ BUF_SIZE];
   
    //////////////////////////  receive client's requests

    while (bytesrecv= recv(newfd, buf, BUF_SIZE, 0) && buf!= "quit"){
        std::string buf_string= std::string(buf);
        std::stringstream buf_stream(buf_string);
        std::istream_iterator<std::string> begin(buf_stream);
        std::istream_iterator<std::string> end;

    //////////////////////////////  parse client's request

        std::vector<std::string> input_command(begin, end);

        if (command.count(input_command[0])> 0){
            std::vector<std::string> argument_list(input_command.begin()+1, input_command.end());
            std::string result= command[input_command[0]](argument_list);
        }

        memset(&buf, 0, sizeof(buf));
    }



    //bytesrecv = send( newfd, buf, BUF_SIZE, 0);

    close(newfd);
    return 0;
}
std::map<std::string,void*> readJsonFile(){
    std::map<std::string,void*> fd;
    return fd;
}