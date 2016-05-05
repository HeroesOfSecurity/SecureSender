#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <qglobal.h>
#include <iostream>
#include <QThread>
#include "include/client.h"   //PEKO
#include <string>
#include <vector>
#define  CMD_COUNT 9


class CmdParser : public QThread
{
    Q_OBJECT
    enum cmd_types { EXIT, CONNECT, DISCONNECT, SIGN_IN, SIGN_UP, ONLINE_USERS/*, CHAT, CLOSE_CHAT, SEND*/};
    std::string commands[CMD_COUNT] = { "exit", "connect", "disconnect","sign_in", "sign_up", "online_users"/*, "chat", "close_chat", "send"*/ };

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
        Client c(*this);
        //QObject::connect(&c, SIGNAL(finished()),, SLOT(quit()));

        std::string input_cmd;
        int cmd_idx;
        bool is_running = true;

        std::cout << "supported commands -> [exit | connect | disconnect | sign_up | sign_in | online_users | chat | close_chat | send]" << std::endl;
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
                            is_running = false;
                            break;
                }
                case CONNECT:
                {
                            c.connect_to_server();
                            break;
                }
                case DISCONNECT:
                {
                            if (!c.is_connected())
                            {
                                std::cout << "you are not connected to server !!!" << std::endl;
                                break;
                            }
                            c.disconnect();
                            break;
                }
                case SIGN_IN:
                {
                            if (!c.is_connected())
                            {
                                std::cout << "you are not connected to server !!!" << std::endl;
                                break;
                            }
                            std::string name, password;
                            std::cout << "Enter your username: ";
                            std::cin >> name;
                            std::cout << "enter your password: ";
                            std::cin >> password;
                            c.sign_in(name, password);
                            break;
                }
                case SIGN_UP:
                {
                            if (!c.is_connected())
                            {
                                std::cout << "you are not connected to server !!!" << std::endl;
                                break;
                            }
                            std::string name, password;
                            std::cout << "Enter your new username: ";
                            std::cin >> name;
                            std::cout << "enter your new password: ";
                            std::cin >> password;
                            c.sign_up(name, password);
                            break;
                }
                case ONLINE_USERS:
                {
                            if (!c.is_connected())
                            {
                                std::cout << "you are not connected to server !!!" << std::endl;
                                break;
                            }

                            std::vector<User> online_users = c.get_online_users();
                            foreach (auto user, online_users) {
                                std::cout << "Username: " << user.getUsername() << "    " << "IP" << user.getIP() <<std::endl;
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


#endif COMMAND_PARSER_H
