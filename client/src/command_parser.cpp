#include "include/command_parser.h"


/*
CmdParser::CmdParser(QObject *parent) : QThread(parent){ }


void CmdParser::run()
{
    Client c(*this);
    //ClientManager clientMngr;
    //clientMngr.start();
    std::string input_cmd;
    int cmd_idx;
    bool is_running = true;

    qDebug() << "commands: exit | connect | disconnect | sign_up | sign_in | online_users | chat | close_chat | send";
    while (is_running) {
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
                        /*else if (clientMngr.isLoggedIn())
                        {
                            std::cout << "you are still logged in" << std::endl;
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
                        c.get_online_users();
                        break;
            }
            default:
            {
                        std::cout << "UNKNOWN command entered -> " << input_cmd;
                        break;
            }
        }
    }

    return;
}


*/
