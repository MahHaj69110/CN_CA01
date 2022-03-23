#include <map>
#include <iostream>
#include <def.hpp>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>


using namespace std;
std::map<std::string,void*> readJsonFile();

int main(int argc, char const *argv[])
{
    // parse and read data from Json file
    std::map<std::string,void*> config_data= readJsonFile();
    int command_channel_port= (int)config_data["commandChannelPort"];
    int data_channel_port= (int)config_data["dataChannelPort"];

    std::vector<User> users;

    // create socket

    
    int fd, newfd, nbytes, nbytes2;
    char buf[512], response[512];
    struct sockaddr_in srv;
    fd = socket(AF_INET, SOCK_STREAM, 0);


    srv.sin_family = AF_INET;
    srv.sin_port = htons(command_channel_port);
    srv.sin_addr.s_addr = inet_addr("128.2.15.9");
    bind(fd, (struct sockaddr*) &srv, sizeof(srv));

    listen(fd, 5);

    struct sockaddr_in cli;
    socklen_t cli_len;
    memset(&cli, 0, sizeof(cli)); /* clear it */
    newfd = accept(fd, (struct sockaddr*) &cli, &cli_len);

    //Now it can read from socket, newfd, and write to socket, newfd.
    int BUF_SIZE = 1024, bytesrecv = 0;
    char buf[ BUF_SIZE];
    /* receives up to BUF_ SIZE bytes from sock and stores them in buf. */
    bytesrecv = recv( newfd, buf, BUF_SIZE, 0);
    /* send up BUF_ SIZE bytes  */
    bytesrecv = send( newfd, buf, BUF_SIZE, 0);
    //At the end, we need to close both sockets by close command.
    close( newfd); 	/* closes the socket newfd */



    return 0;
}
std::map<std::string,void*> readJsonFile(){
    std::map<std::string,void*> fd;
    return fd;
}