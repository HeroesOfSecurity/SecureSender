/* 
 * File:   session.h
 * Author: pedro1
 *
 * Created on April 4, 2016, 8:17 AM
 */

#ifndef SESSION_H
#define	SESSION_H

#include "user.h"
#include "crypto.h"

class Session
{
private:
    
    User session_user; 
    unsigned char *key;
    
    Crypto crypto;
    
public:

    Session();
    unsigned char *get_key() {return key;}
};

#endif	/* SESSION_H */

