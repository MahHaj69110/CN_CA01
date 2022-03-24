#include <map>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sstream>
#include <fstream>
#include "json/json.h"
#include "def.hpp"

typedef std::string (*command_func)(std::vector<std::string>);

std::map<std::string,void*> readJsonFile();

int main(int argc, char const *argv[])
{
     //////////////////////////  initialize command list

    std::map<std::string,command_func> command;

    //////////////////////////  parse and read data from Json file
    std::vector<User*> users;
    std::ifstream cofig_file("config.json");
    Json::Reader reader;
    Json::Value json_obj;
    reader.parse(cofig_file, json_obj);
    int command_channel_port= json_obj["commandChannelPort"].asInt();
    int dataChannelPort= json_obj["dataChannelPort"].asInt();
    const Json::Value& files_json= json_obj["files"]; // array of characters
    const Json::Value& users_json= json_obj["users"];
    for (int i = 0; i < files_json.size(); i++){
        AdminUser::files_only_admin_has_permission.push_back(files_json[i].asString());
    }
    for (int i = 0; i < users_json.size(); i++){
        std::string user_name= users_json[i]["user"].asString();
        std::string user_pass= users_json[i]["password"].asString();
        int user_download_size= std::stoi(users_json[i]["size"].asString());

        if (users_json[i]["admin"].asString()== "true")
            users.push_back(new AdminUser(user_name, user_pass, user_download_size));
        else
            users.push_back(new TypicalUser(user_name, user_pass, user_download_size));
    }
    
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

    std::ifstream cofig_file("config.json");
    Json::Reader reader;
    Json::Value obj;
    reader.parse(cofig_file, obj); // reader can also read strings
    int x= obj["commandChannelPort"].asInt();
    int y= obj["dataChannelPort"].asInt();
    //fd.insert(std::pair<std::string,void*>("commandChannelPort",34));

    std::cout<< x<<'\n';
    std::cout << "Year: " << obj["year"].asUInt() << std::endl;
    const Json::Value& characters = obj["characters"]; // array of characters
    for (int i = 0; i < characters.size(); i++){
        std::cout << "    name: " << characters[i]["name"].asString();
        std::cout << " chapter: " << characters[i]["chapter"].asUInt();
        std::cout << std::endl;
    }
    return fd;
}