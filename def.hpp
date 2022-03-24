#include <string>
#include <vector>

class User
{
    protected:
        std::string name;
        std::string password;
        int download_size;

    public:
        User(std::string user_name, std::string user_password, int user_download_size){
            name= user_name;
            password= user_password;
            download_size= user_download_size;
        }
        virtual bool is_admin()= 0; 
};

class AdminUser : public User
{
    public:
        AdminUser(std::string user_name, std::string user_password, int user_download_size): 
            User(user_name,user_password,user_download_size){}
            
        bool is_admin(){
            return false;
        }
        static std::vector<std::string> files_only_admin_has_permission;
};
std::vector<std::string> AdminUser::files_only_admin_has_permission= std::vector<std::string>();

class TypicalUser : public User
{
    public:
        TypicalUser(std::string user_name, std::string user_password, int user_download_size): 
            User(user_name,user_password,user_download_size){}
        bool is_admin(){
            return true;
        }
};

// struct sockaddr_in {
// 	u_char sin_family;   /* Address Family */
// 	u_short sin_port;    /* Port number */
// 	struct in_addr sin_addr;    /* IP address */
// 	char sin_zero[8]; /* unused */
// };
