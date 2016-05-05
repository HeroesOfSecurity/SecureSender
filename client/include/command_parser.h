#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <qglobal.h>
#include <iostream>
#include <QThread>
#include "include/client.h"   //PEKO
#include <string>
#include <vector>
#include <include/client_server_connection.h>
#define  CMD_COUNT 9


class CmdParser : public QThread
{
    Q_OBJECT
    enum cmd_types { EXIT, LOGOUT, SIGN_IN, SIGN_UP, ONLINE_USERS/*, CHAT, CLOSE_CHAT, SEND*/};
    std::string commands[CMD_COUNT] = { "exit", "logout","sign_in", "sign_up", "online_users"/*, "chat", "close_chat", "send"*/ };

public:

    /**
     * @brief CmdParser constructor of CmdParser
     */
    CmdParser(QObject *parent = 0) : QThread(parent){ }

    /**
     * @brief CmdParser destructor of CmdParser
     */
    virtual ~CmdParser() {}

    /**
     * @brief run Function parse command that user enters
     */
    void run()
    {
        Client* c = nullptr;
        ClientServerConnection con(this);
        con.connect_to_server();
        //QObject::connect(&c, SIGNAL(finished()),, SLOT(quit()));

        std::string input_cmd;
        int cmd_idx;
        bool is_running = true;

        std::cout << "supported commands -> [exit | logout | sign_up | sign_in | online_users | chat | close_chat | send]" << std::endl;
        while (is_running) {
            std::cout << "Enter new command:";
            std::cin >> input_cmd;
            cmd_idx = -1;
            for (int i = 0; i < CMD_COUNT; ++i) {
                if (!input_cmd.compare(commands[i])) {
                    cmd_idx = i;
                    break;
                }
            }
            switch (cmd_idx) {
                case EXIT:
                {
                        if (con.is_connected()){
                            if(c != nullptr){
                                con.logout(c->getUsername());
                            }
                            con.disconnect();
                        }
                        delete c;
                        c = nullptr;
                        return;
                        //break;
                }
                case LOGOUT:
                {
                            if (!con.is_connected()){
                                std::cout << "Connection is lost" << std::endl;
                                return;
                            }

                            if (c == nullptr)
                            {
                                std::cout << "you are not sign in !!!" << std::endl;
                                break;
                            }
                            con.logout(c->getUsername());

                            delete c;
                            c = nullptr;
                            break;
                }
                case SIGN_IN:
                {
                            if (!con.is_connected()){
                                std::cout << "Connection is lost" << std::endl;
                                return;
                            }
                            if (c != nullptr)
                            {
                                std::cout << "You are already sign in" << std::endl;
                                break;

                            }
                            std::string name, password;
                            std::cout << "Enter your username: ";
                            std::cin >> name;
                            std::cout << "enter your password: ";
                            std::cin >> password;
                            int res = con.sign_in(name, password);
                            if(res != OK){
                                std::cout << "Wrong user credentials" << std::endl;
                            }
                            else{
                                c = new Client(name);
                            }
                            break;
                }
                case SIGN_UP:
                {
                            if (!con.is_connected()){
                                std::cout << "Connection is lost" << std::endl;
                                return;
                            }
                            if (c != nullptr){
                                std::cout << "You are already signed in" << std::endl;
                                break;
                            }
                            std::string name, password;
                            std::cout << "Enter your new username: ";
                            std::cin >> name;
                            std::cout << "enter your new password: ";
                            std::cin >> password;
                            int res = con.sign_up(name, password);
                            if(res != OK){
                                std::cout << "Registration failed" << std::endl;
                            }
                            break;
                }
                case ONLINE_USERS:
                {
                            if (!con.is_connected()){
                                std::cout << "Connection is lost" << std::endl;
                                return;
                            }

                            if (c == nullptr){
                                std::cout << "You have to sign in at first" << std::endl;
                                break;
                            }
                            std::vector<User> online_users = con.get_online_users();
                            c->setOnlineUsers(online_users);
                            foreach (auto user, online_users) {
                                std::cout << "Username: " << user.getUsername() <<std::endl;
                            }
                            break;
                }
                default:
                {
                            std::cout << "UNKNOWN command entered -> " << input_cmd << std::endl;
                            break;
                }
            }
        }

        return;
    }

};


#endif
