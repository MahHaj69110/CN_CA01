#include <map>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sstream>
#include <fstream>
#include <chrono>
#include <thread>
#include "json/json.h"
#include "def.hpp"

typedef std::string (*command_func)(std::vector<std::string>);

std::vector<User*> users;
User *incoming_user= NULL, *logged_in_user= NULL;

std::string user_command(std::vector<std::string> arg);
std::string quit_command(std::vector<std::string> arg);
std::string pass_command(std::vector<std::string> arg);

int main(int argc, char const *argv[])
{
     //////////////////////////  initialize command list and status code map

    std::map<std::string,command_func> command;
    command.insert(std::pair<std::string,command_func>("user",user_command));
    command.insert(std::pair<std::string,command_func>("quit",quit_command));
    command.insert(std::pair<std::string,command_func>("pass",pass_command));

    status_code_command.insert(std::pair<std::string,std::string>("221: ","Successful Quit."));
    status_code_command.insert(std::pair<std::string,std::string>("230: ","User logged in, proceed. Logged out if appropirate."));
    status_code_command.insert(std::pair<std::string,std::string>("331: ","User name okay, need password."));
    status_code_command.insert(std::pair<std::string,std::string>("332: ","Need account for login."));
    status_code_command.insert(std::pair<std::string,std::string>("430: ","Invalid username or password."));
    status_code_command.insert(std::pair<std::string,std::string>("500: ","Error."));
    status_code_command.insert(std::pair<std::string,std::string>("501: ","Syntax error in parameters or arguments."));        
    status_code_command.insert(std::pair<std::string,std::string>("503: ","Bad sequence of command."));
    
    //////////////////////////  parse and read data from Json file
    
    std::ifstream cofig_file("config.json");
    Json::Reader reader;
    Json::Value json_obj;
    reader.parse(cofig_file, json_obj);
    int command_channel_port= json_obj["commandChannelPort"].asInt();
    int dataChannelPort= json_obj["dataChannelPort"].asInt();
    const Json::Value& files_json= json_obj["files"];
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
    srv.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    bind(fd, (struct sockaddr*) &srv, sizeof(srv));

     //////////////////////////  listen to the port and accept client
    std::cout<<"Server is listening !!\n";
    listen(fd, 5);

    struct sockaddr_in cli;
    socklen_t cli_len;
    memset(&cli, 0, sizeof(cli));
    newfd = accept(fd, (struct sockaddr*) &cli, &cli_len);

    int BUF_SIZE = 1024, bytesrecv = 0;
    char buf[ BUF_SIZE];
   

    //////////////////////////  receive client's requests
    std::cout<<"Server accept !!\n";

    while (bytesrecv= recv(newfd, buf, BUF_SIZE, 0)){
        std::string buf_string= std::string(buf);
        std::stringstream buf_stream(buf_string);
        std::istream_iterator<std::string> begin(buf_stream);
        std::istream_iterator<std::string> end;

        std::cout<< "Buffer: "<< buf_string<<'\n';

    //////////////////////////////  parse client's request

        std::vector<std::string> input_command(begin, end);
        std::string result;
        try{
            std::vector<std::string> argument_list(input_command.begin()+1, input_command.end());
            if ((logged_in_user== NULL) && input_command[0]!="user" && input_command[0]!="quit" && input_command[0]!="pass")
                throw new NeedAccountForLogin();
            if (command.count(input_command[0])== 0)
                throw new SyntaxError();
            result= command[input_command[0]](argument_list);
        }
        catch(std::exception* exep){
            result= exep->what()+ status_code_command[exep->what()];
        }
        std::cout<< result<< '\n';
        memset(&buf, 0, sizeof(buf));
        sprintf(buf, result.c_str());
        send( newfd, buf, BUF_SIZE, 0);

        memset(&buf, 0, sizeof(buf));
    }
    std::this_thread::sleep_for(std::chrono::seconds(3));
    close(newfd);
    return 0;
}

std::string user_command(std::vector<std::string> arg){
    for (auto user: users){
        if (user->get_name()== arg[0]){
            incoming_user= user;
            return "331: " + status_code_command["331: "];
        }
    }
    throw new InvalidUserNameOrPassword();
}

std::string quit_command(std::vector<std::string> arg){
    return "221: " + status_code_command["221: "];
}

std::string pass_command(std::vector<std::string> arg){
    if (incoming_user== NULL)
        throw new BadSequenceOfCommand();
    if (incoming_user->is_pass(arg[0])){
        logged_in_user= incoming_user;
        return "230: " + status_code_command["230: "];
    }
    throw new InvalidUserNameOrPassword();
}