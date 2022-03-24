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
    // parse and read data from Json file
    //std::map<std::string,void*> config_data= readJsonFile();
    int command_channel_port=  232;
    int data_channel_port= 43;

    std::vector<User> users;
    std::cout<< AdminUser::files_only_admin_has_permission.size()<< '\n';
    // create socket

    int fd, newfd, nbytes, nbytes2;
    char buf[512], response[512];
    struct sockaddr_in srv;
    fd = socket(AF_INET, SOCK_STREAM, 0);
    //The same as server. Now, it needs to connect to server. 
	srv.sin_family = AF_INET;
    srv.sin_port = htons(command_channel_port);
    srv.sin_addr.s_addr = inet_addr("128.2.15.9");
    connect(fd, (struct sockaddr*) &srv, sizeof(srv));
    //Connect is blocking and send a SYN signal and is blocked until receive SYNACK, (three way handshaking)
    //It can start now reading and writing
    sprintf(response,"Here's my request");
    nbytes2 = write(fd, response, strlen(response));
    close(fd);

    return 0;
}