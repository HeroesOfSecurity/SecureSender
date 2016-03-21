/* 
 * File:   chat.h
 * Author: pedro1
 *
 * Created on March 15, 2016, 10:03 PM
 */

#ifndef CHAT_H
#define	CHAT_H


class Chat
{
private:
    User user;
    //port where listening on user happens
    int port;
    
public:
    
    /**
     * @brief This function sends message to user
     * @param message message we want to send to user
     * @param status
     */
    void sendMessage(std::string message, int *status);
    
    
    /**
     * @brief This function quits chat
     */
    void quit_chat();

};



#endif	/* CHAT_H */


